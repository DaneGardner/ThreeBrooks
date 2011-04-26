/*!
   \file GrainIngredient.cpp
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

#include "GrainIngredient.h"

GrainIngredient::GrainIngredient(QObject *parent) :
    Ingredient(parent)
{
    _type = tr("Grain");
    _color = 2;
    _specificGravity = 1.038;
    _extract = false;
}

QString GrainIngredient::type() const
{
    return _type;
}
void GrainIngredient::setType(QString type)
{
    _type = type;
    emit dataChanged();
}

double GrainIngredient::color() const
{
    return _color;
}
void GrainIngredient::setColor(double color)
{
    _color = color;
    emit dataChanged();
}

double GrainIngredient::specificGravity() const
{
    return _specificGravity;
}
void GrainIngredient::setSpecificGravity(double specificGravity)
{
    _specificGravity = specificGravity;
    emit dataChanged();
}

bool GrainIngredient::extract() const
{
    return _extract;
}
void GrainIngredient::setExtract(bool extract)
{
    _extract = extract;
    emit dataChanged();
}
void GrainIngredient::setExtract(QString extract)
{
    setExtract(!extract.compare("true", Qt::CaseInsensitive));
}

QDomElement GrainIngredient::toXml(QDomDocument document)
{
    QDomElement retval = Ingredient::toXml(document);
    retval.setAttribute("color", color());
    retval.setAttribute("specificGravity", specificGravity());
    retval.setAttribute("extract", extract()?"true":"false");
    return retval;
}

void GrainIngredient::fromXml(QDomElement element)
{
    Ingredient::fromXml(element);
    setColor(element.attribute("color", QString().setNum(color(), 'f')).toDouble());
    setSpecificGravity(element.attribute("specificGravity", QString().setNum(specificGravity(), 'f')).toDouble());
    setExtract(element.attribute("extract", extract()?"true":"false"));
}

