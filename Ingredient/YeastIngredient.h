/*!
   \file YeastIngredient.h
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

#ifndef YEASTINGREDIENT_H
#define YEASTINGREDIENT_H

#include "Ingredient.h"

class YeastIngredient : public Ingredient
{
    Q_OBJECT
public:
    explicit YeastIngredient(QObject *parent = 0);
    virtual QString type() const;

    virtual double attenuation() const;
    virtual void setAttenuation(double attenuation);

    virtual double minimumTemperature() const;
    virtual void setMinimumTemperature(double minimumTemperature);

    virtual double maximumTemperature() const;
    virtual void setMaximumTemperature(double maximumTemperature);

    virtual QDomElement toXml(QDomDocument);

protected:
    virtual void fromXml(QDomElement);
    double _attenuation;
    double _minimumTemperature;
    double _maximumTemperature;

};

#endif // YEASTINGREDIENT_H
