/*!
   \file Ingredient.cpp
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

#include "Ingredient.h"
#include "GrainIngredient.h"
#include "HopsIngredient.h"
#include "YeastIngredient.h"

Ingredient::Ingredient(QObject *parent) :
    QObject(parent)
{
    _name = QString();
    _id = QUuid::createUuid();
}

QString Ingredient::name() const
{
    return _name;
}
void Ingredient::setName(QString name)
{
    _name = name;
    emit dataChanged();
}

QUuid Ingredient::id() const
{
    return _id;
}
void Ingredient::setId(QUuid id)
{
    _id = id;
    emit dataChanged();
}

QString Ingredient::type() const
{
    return tr("Ingredient");
}

QDomElement Ingredient::toXml(QDomDocument document)
{
    QDomElement retval = document.createElement("Ingredient");
    retval.setAttribute("name", name());
    retval.setAttribute("type", type());
    retval.setAttribute("id", id().toString());
    return retval;
}
void Ingredient::fromXml(QDomElement element)
{
    setName(element.attribute("name", name()));
    setId(QUuid(element.attribute("id", id().toString())));
}

Ingredient *Ingredient::createIngredient(QDomElement element)
{
    Ingredient *ingredient;
    if(!element.attribute("type").compare("Grain", Qt::CaseInsensitive)) {
        ingredient = new GrainIngredient();
    } else if(!element.attribute("type").compare("Hops", Qt::CaseInsensitive)) {
        ingredient = new HopsIngredient();
    } else if(!element.attribute("type").compare("Yeast", Qt::CaseInsensitive)) {
        ingredient = new YeastIngredient();
    } else if(!element.attribute("type").compare("Ingredient", Qt::CaseInsensitive)) {
        ingredient = new Ingredient();
    }

    ingredient->fromXml(element);
    return ingredient;
}
