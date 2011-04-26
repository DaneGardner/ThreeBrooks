/*!
   \file YeastIngredient.cpp
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

#include "YeastIngredient.h"

YeastIngredient::YeastIngredient(QObject *parent) :
    Ingredient(parent)
{
    _attenuation = 0.75;
    _minimumTemperature = 65;
    _maximumTemperature = 75;
}

QString YeastIngredient::type() const
{
    return tr("Yeast");
}

double YeastIngredient::attenuation() const
{
    return _attenuation;
}
void YeastIngredient::setAttenuation(double attenuation)
{
    _attenuation = attenuation;
    emit dataChanged();
}

double YeastIngredient::minimumTemperature() const
{
    return _minimumTemperature;
}
void YeastIngredient::setMinimumTemperature(double minimumTemperature)
{
    _minimumTemperature = minimumTemperature;
    emit dataChanged();
}

double YeastIngredient::maximumTemperature() const
{
    return _maximumTemperature;
}
void YeastIngredient::setMaximumTemperature(double maximumTemperature)
{
    _maximumTemperature = maximumTemperature;
    emit dataChanged();
}


QDomElement YeastIngredient::toXml(QDomDocument document)
{
    QDomElement retval = Ingredient::toXml(document);
    retval.setAttribute("attenuation", attenuation());
    retval.setAttribute("minimumTemperature", minimumTemperature());
    retval.setAttribute("maximumTemperature", maximumTemperature());
    return retval;
}

void YeastIngredient::fromXml(QDomElement element)
{
    Ingredient::fromXml(element);
    setAttenuation(element.attribute("attenuation", QString().setNum(attenuation(), 'f')).toDouble());
    setMinimumTemperature(element.attribute("minimumTemperature", QString().setNum(minimumTemperature(), 'f')).toDouble());
    setMaximumTemperature(element.attribute("maximumTemperature", QString().setNum(maximumTemperature(), 'f')).toDouble());
}
