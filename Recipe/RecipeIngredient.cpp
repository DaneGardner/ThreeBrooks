/*!
   \file RecipeIngredient.cpp
   \author Dane Gardner <dane.gardner@gmail.com>
   \version 

   \section LICENSE
    This file is part of the ThreeBrooks homebrew recipe application
    Copyright (C) 2011-2013 Dane Gardner

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

#include "RecipeIngredient.h"

RecipeIngredient::RecipeIngredient(Ingredient *ingredient, QObject *parent) :
    QObject(parent)
{
    setIngredient(ingredient);
    _quantity = Quantity();
    _minutes = 0;
    initialize();
}

RecipeIngredient::RecipeIngredient(Ingredient *ingredient, Quantity quantity, QObject *parent) :
    QObject(parent)
{
    setIngredient(ingredient);
    _quantity = quantity;
    _minutes = 0;
    initialize();
}

RecipeIngredient::RecipeIngredient(Ingredient *ingredient, Quantity quantity, double minutes, QObject *parent) :
    QObject(parent)
{
    setIngredient(ingredient);
    _quantity = quantity;
    _minutes = minutes;
}

RecipeIngredient::RecipeIngredient(QDomElement element, QObject *parent) :
    QObject(parent)
{
    fromXml(element);
}

void RecipeIngredient::initialize()
{
    if(!_ingredient->type().compare("Grain", Qt::CaseInsensitive)) {
        if(_quantity.value() == 0 && _quantity.type() == Quantity::QuantityType_item)
            _quantity = Quantity(1, Quantity::QuantityType_pound);
        if(_minutes == 0)
            _minutes = 0;
    } else if(!_ingredient->type().compare("Hops", Qt::CaseInsensitive)) {
        if(_quantity.value() == 0 && _quantity.type() == Quantity::QuantityType_item)
            _quantity = Quantity(1, Quantity::QuantityType_ounce);
        if(_minutes == 0)
            _minutes = 60;
    } else if(!_ingredient->type().compare("Yeast", Qt::CaseInsensitive)) {
        if(_quantity.value() == 0 && _quantity.type() == Quantity::QuantityType_item)
            _quantity = Quantity(1, Quantity::QuantityType_item);
        if(_minutes == 0)
            _minutes = 0;
    } else {
        if(_quantity.value() == 0 && _quantity.type() == Quantity::QuantityType_item)
            _quantity = Quantity();
        if(_minutes == 0)
            _minutes = 0;
    }

}

QDomElement RecipeIngredient::toXml(QDomDocument document)
{
    QDomElement element = document.createElement("RecipeIngredient");
    element.appendChild(ingredient()->toXml(document));
    element.appendChild(quantity().toXml(document));
    element.setAttribute("minutes", minutes());
    return element;
}

void RecipeIngredient::fromXml(QDomElement element)
{
    setIngredient(Ingredient::createIngredient(element.elementsByTagName("Ingredient").at(0).toElement()));
    setQuantity(Quantity(element.elementsByTagName("Quantity").at(0).toElement()));
    setMinutes(element.attribute("minutes", QString().setNum(minutes())).toDouble());
}


Ingredient *RecipeIngredient::ingredient()
{
    return _ingredient;
}
void RecipeIngredient::setIngredient(Ingredient* ingredient)
{
    connect(ingredient, SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _ingredient = ingredient;
}

void RecipeIngredient::ingredientChanged()
{
    emit dataChanged();
}

QString RecipeIngredient::name() const
{
    return _ingredient->name();
}
QString RecipeIngredient::type() const
{
    return _ingredient->type();
}

Quantity RecipeIngredient::quantity() const
{
    return _quantity;
}
void RecipeIngredient::setQuantity(Quantity quantity)
{
    _quantity = quantity;
    emit dataChanged();
}

double RecipeIngredient::minutes() const
{
    return _minutes;
}
void RecipeIngredient::setMinutes(double minutes)
{
    _minutes = minutes;
    emit dataChanged();
}

/*! \fn RecipeIngredient::setQuantityAndMinutes()
    \brief Just a helper function that allows both quantity and minutes to be set without the model being updated between
           setting either.
    I was having problems with the RecipeIngredientDelegate setting one and being updated before setting the next.  The value
    for minutes would keep being reset as the model was updated!  This allows both to be set at the same time.
 */
void RecipeIngredient::setQuantityAndMinutes(Quantity quantity, double minutes)
{
    _quantity = quantity;
    _minutes = minutes;
    emit dataChanged();
}

