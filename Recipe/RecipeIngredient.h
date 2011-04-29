/*!
   \file RecipeIngredient.h
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

#ifndef RECIPEINGREDIENT_H
#define RECIPEINGREDIENT_H

#include <QObject>
#include <QDomDocument>
#include <QMetaType>
#include "../Ingredient/Ingredient.h"
#include "../Quantity.h"

class RecipeIngredient : public QObject
{
    Q_OBJECT
public:
    explicit RecipeIngredient(Ingredient *ingredient, QObject *parent = 0);
    explicit RecipeIngredient(Ingredient *ingredient, Quantity quantity = Quantity(),
                              QObject *parent = 0);
    explicit RecipeIngredient(Ingredient *ingredient, Quantity quantity = Quantity(),
                              double minutes = 0, QObject *parent = 0);
    explicit RecipeIngredient(QDomElement element, QObject *parent = 0);

    virtual Ingredient *ingredient();

    virtual QString name() const;
    virtual QString type() const;

    virtual Quantity quantity() const;
    virtual void setQuantity(Quantity quantity);

    virtual double minutes() const;
    virtual void setMinutes(double minutes);

    QDomElement toXml(QDomDocument document);
    void fromXml(QDomElement element);

signals:
    void dataChanged();

protected:
    virtual void initialize();
    virtual void setIngredient(Ingredient* ingredient);

    Ingredient *_ingredient;
    Quantity _quantity;
    double _minutes;

protected slots:
    void ingredientChanged();
};

Q_DECLARE_METATYPE(RecipeIngredient *);

#endif // RECIPEINGREDIENT_H
