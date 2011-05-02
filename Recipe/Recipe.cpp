/*!
   \file Recipe.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version 

   \section LICENSE
    This file is part of the ThreeBrooks homebrew recipe application
    Copyright (C) 2011 Dane Gardner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   \section DESCRIPTION

 */

#include "Recipe.h"

Recipe::Recipe(QObject *parent) :
    QObject(parent)
{
    _name = tr("New Recipe");
    _volume = Quantity(5.5, Quantity::QuantityType_gallon);
    _efficiency = 0.75;
    _id = QUuid::createUuid();
}

Recipe::Recipe(QDomElement element, QObject *parent) :
    QObject(parent)
{
    fromXml(element);
}

QList<RecipeIngredient *> Recipe::ingredients() const
{
    return _ingredients;
}

QString Recipe::name() const
{
    return _name;
}

void Recipe::setName(QString name)
{
    _name = name;
    emit dataChanged();
}

QString Recipe::style() const
{
    return _style;
}

void Recipe::setStyle(QString style)
{
    _style = style; emit dataChanged();
}

Quantity Recipe::volume() const
{
    return _volume;
}

void Recipe::setVolume(Quantity volume)
{
    _volume = volume; emit dataChanged();
}

void Recipe::setVolume(double volume)
{
    _volume = Quantity(volume, Quantity::QuantityType_gallon, this);
    emit dataChanged();
}


double Recipe::efficiency() const
{
    return _efficiency;
}

void Recipe::setEfficiency(double efficiency)
{
    _efficiency = efficiency;
    emit dataChanged();
}

QUuid Recipe::id() const
{
    return _id;
}
void Recipe::setId(QUuid id)
{
    _id = id;
    emit dataChanged();
}


double Recipe::originalGravity() const
{
    double originalGravity = 0;

    foreach(RecipeIngredient* ingredient, ingredients()) {
        GrainIngredient *grainIngredient =
                qobject_cast<GrainIngredient *>(ingredient->ingredient());
        if(grainIngredient) {
            double specificGravity = (grainIngredient->specificGravity() - 1);
            specificGravity *= ingredient->quantity().valueToPound();

            if(grainIngredient->extract()) {
                originalGravity += specificGravity;
            } else {
                originalGravity += specificGravity * efficiency();
            }

            continue;
        }
    }

    originalGravity = (originalGravity / volume().valueToGallon()) + 1;
    return originalGravity;
}

double Recipe::finalGravity() const
{
    double maximumAttenuation = 0;
    foreach(RecipeIngredient* ingredient, ingredients()) {
        YeastIngredient *yeastIngredient =
                qobject_cast<YeastIngredient *>(ingredient->ingredient());
        if(yeastIngredient && yeastIngredient->attenuation() > maximumAttenuation) {
            maximumAttenuation = yeastIngredient->attenuation();
            continue;
        }
    }

    double finalGravity = ((originalGravity() - 1) * (1 - maximumAttenuation)) + 1;
    return finalGravity;
}

double Recipe::color() const
{
    double maltColorUnits = 0;
    double volumeGallon = this->volume().valueToGallon();

    foreach(RecipeIngredient* ingredient, ingredients()) {
        GrainIngredient *grainIngredient =
                qobject_cast<GrainIngredient *>(ingredient->ingredient());
        if(grainIngredient) {
            maltColorUnits += (ingredient->quantity().valueToPound() * grainIngredient->color()) / volumeGallon;
            continue;
        }
    }

    double standardReferenceMethod = 1.4922 * pow(maltColorUnits, 0.6859);
    return standardReferenceMethod;
}

/*! \fn Recipe::bitterness()
    \brief Returns the bitterness in IBU using Tinseth's formula
 */
double Recipe::bitterness() const
{
    double internationalBitternessUnits = 0;
    double volumeGallon = this->volume().valueToGallon();

    foreach(RecipeIngredient* ingredient, ingredients()) {
        HopsIngredient *hopsIngredient =
                qobject_cast<HopsIngredient *>(ingredient->ingredient());
        if(hopsIngredient) {

            /*! \note Please note that we are taking the average boil gravity for the duration
                that the hops item was actually in the boil.  Not just the pre-boil gravity, as
                most other calculators are using.  This gives a closer approximation of what
                we're really using. */

            double averageGravity = (boilGravity(ingredient->minutes()) + finalGravity()) / 2;

            double utilization = 1.65
                                 * pow(0.000125, averageGravity - 1.0)
                                 * (1.0 - exp( -0.04 * ingredient->minutes() ))
                                 * (1 / 4.15);

            double alphaAcids = hopsIngredient->alphaAcid()
                                * ingredient->quantity().valueToOunce()
                                * 7490.0
                                * (1 / volumeGallon);

            internationalBitternessUnits += utilization * alphaAcids;

            continue;
        }
    }

    return internationalBitternessUnits;
}

double Recipe::alcoholByWeight() const
{
    double alcoholByWeight = 76.08 * (originalGravity() - finalGravity());
    alcoholByWeight /= 1.775 - originalGravity();
    alcoholByWeight /= 100;

    return alcoholByWeight;
}

double Recipe::alcoholByVolume() const
{
    double alcoholByVolume = alcoholByWeight() * (originalGravity() / .794);
    return alcoholByVolume;
}

double Recipe::calories() const
{
    return 851 * (originalGravity() - 1) * (originalGravity() + 3);
}

double Recipe::pitchRate() const
{
    // Points to Brix ([POINTS]*294.01373844122)/([POINTS]+1136.9440774989)
    // Brix to Points ([BRIX]*(-1136.9440774989))/([BRIX]-294.01373844122)

    double points = (originalGravity() - 1) * 1000;
    double brix = (points * 294.01373844122) / (points + 1136.9440774989);
    double pitchRate = 0.75 * volume().valueToLiter() * brix;

    return pitchRate;
}


double Recipe::boilTime() const
{
    double maximumBoilTime = 0;
    foreach(RecipeIngredient* ingredient, ingredients()) {
        if(ingredient->minutes() > maximumBoilTime)
            maximumBoilTime = ingredient->minutes();
    }
    return maximumBoilTime;
}
Quantity Recipe::preBoilVolume() const
{
    return boilVolume(boilTime());
}
double Recipe::preBoilGravity() const
{
    return boilGravity(boilTime());
}
Quantity Recipe::boilVolume(double minutes) const
{
    return Quantity(volume().valueToGallon() / (1.0 - ((minutes / 60.0) * 0.1)), Quantity::QuantityType_gallon);
}
double Recipe::boilGravity(double minutes) const
{
    return ((originalGravity() - 1) * (boilVolume(minutes).valueToGallon() / volume().valueToGallon())) + 1;
}

void Recipe::ingredientChanged()
{
    emit dataChanged();
}

QDomElement Recipe::toXml(QDomDocument document)
{
    QDomElement element = document.createElement("Recipe");

    element.setAttribute("name", name());
    element.setAttribute("efficiency", efficiency());
    element.setAttribute("id", id().toString());

    QDomElement elementVolume = document.createElement("Volume");
    elementVolume.appendChild(volume().toXml(document));
    element.appendChild(elementVolume);

    for(int i=0; i < ingredients().count(); i++) {
        element.appendChild(ingredients().at(i)->toXml(document));
    }

    return element;
}

void Recipe::fromXml(QDomElement element)
{
    setName(element.attribute("name", name()));
    setEfficiency(element.attribute("efficiency", QString().setNum(efficiency())).toDouble());
    setId(QUuid(element.attribute("id", id().toString())));

    QDomNodeList nodes = element.elementsByTagName("RecipeIngredient");
    for(int i=0; i < nodes.count(); i++) {
        QDomElement element = nodes.at(i).toElement();
        if(!element.isNull()) {
            insert(count(), new RecipeIngredient(element, this));
        }
    }

    QDomElement elementVolume = element.elementsByTagName("Volume").at(0).toElement();
    setVolume(Quantity(elementVolume.firstChildElement()));
}


/* QList operations */
int Recipe::indexOf(RecipeIngredient *ingredient) const
{
    return _ingredients.indexOf(ingredient);
}

RecipeIngredient *Recipe::at(int row) const
{
    return _ingredients.at(row);
}

int Recipe::count() const
{
    return _ingredients.count();
}

void Recipe::insert(int row, RecipeIngredient *ingredient)
{
    connect(ingredient, SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _ingredients.insert(row, ingredient);
    emit dataChanged();
}

bool Recipe::contains(RecipeIngredient *ingredient)
{
    return _ingredients.contains(ingredient);
}

void Recipe::removeAt(int row)
{
    disconnect(at(row), SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _ingredients.removeAt(row);
    emit dataChanged();
}

void Recipe::move(int row, int destination)
{
    _ingredients.move(row, destination);
    emit dataChanged();
}
/* END QList operations */

