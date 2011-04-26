/*!
   \file Recipe.h
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

#ifndef BEERRECIPE_H
#define BEERRECIPE_H

#include <math.h>
#include <QObject>
#include <QDomDocument>

#include "../Ingredient/Ingredient.h"
#include "../Ingredient/GrainIngredient.h"
#include "../Ingredient/HopsIngredient.h"
#include "../Ingredient/YeastIngredient.h"

#include "RecipeIngredient.h"

class Recipe : public QObject
{
    Q_OBJECT
public:
    explicit Recipe(QObject *parent = 0);
    explicit Recipe(QDomElement element, QObject *parent = 0);

    void addIngredient(RecipeIngredient *ingredient);
    void addIngredient(Ingredient *ingredient);
    void removeIngredient(RecipeIngredient *ingredient);
    QList<RecipeIngredient *> ingredients() const { return _ingredients; }

    QString name() const { return _name; }
    void setName(QString name) { _name = name; emit dataChanged(); }

    QString style() const { return _style; }
    void setStyle(QString style) { _style = style; emit dataChanged(); }

    Quantity volume() const { return _volume; }
    void setVolume(Quantity volume) { _volume = volume; emit dataChanged(); }
    void setVolume(double volume) { _volume = Quantity(volume, Quantity::QuantityType_gallon, this); emit dataChanged(); }


    double efficiency() const { return _efficiency; }
    void setEfficiency(double efficiency) { _efficiency = efficiency; emit dataChanged(); }

    double originalGravity() const;
    double finalGravity() const;
    double color() const;
    double bitterness() const;
    double alcoholByWeight() const;
    double alcoholByVolume() const;
    double calories() const;

    double boilTime() const;
    Quantity preBoilVolume() const;
    double preBoilGravity() const;
    Quantity boilVolume(double minutes) const;
    double boilGravity(double minutes) const;

    QDomElement toXml(QDomDocument document);
    void fromXml(QDomElement element);

signals:
    void dataChanged();
    void addingIngredient();
    void addedIngredient();
    void removingIngredient(int index);
    void removedIngredient(int index);

protected:
    QString _name;
    QString _style;
    Quantity _volume;
    double _efficiency;

    QList<RecipeIngredient *> _ingredients;

protected slots:
    void ingredientChanged();
};

#endif // BEERRECIPE_H
