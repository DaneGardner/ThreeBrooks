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
#include <QUuid>

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

    QList<RecipeIngredient *> ingredients() const;

    QString name() const;
    void setName(QString name);

    QString style() const;
    void setStyle(QString style);

    Quantity volume() const;
    void setVolume(Quantity volume);
    void setVolume(double volume);

    double efficiency() const;
    void setEfficiency(double efficiency);

    virtual QUuid id() const;
    virtual void setId(QUuid id);

    double originalGravity() const;
    double finalGravity() const;
    double color() const;
    double bitterness() const;
    double alcoholByWeight() const;
    double alcoholByVolume() const;
    double calories() const;

    double pitchRate() const;

    double boilTime() const;
    Quantity preBoilVolume() const;
    double preBoilGravity() const;
    Quantity boilVolume(double minutes) const;
    double boilGravity(double minutes) const;

    QDomElement toXml(QDomDocument document);
    void fromXml(QDomElement element);

    /* QList operations */
    int indexOf(RecipeIngredient *ingredient) const;
    RecipeIngredient *at(int row) const;
    int count() const;
    void insert(int row, RecipeIngredient *ingredient);
    bool contains(RecipeIngredient *ingredient);
    void removeAt(int row);
    void move(int row, int destination);


signals:
    void dataChanged();

protected:
    QString _name;
    QString _style;
    Quantity _volume;
    double _efficiency;
    QUuid _id;

    QList<RecipeIngredient *> _ingredients;

protected slots:
    void ingredientChanged();
};

#endif // BEERRECIPE_H
