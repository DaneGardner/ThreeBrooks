/*!
   \file GrainIngredient.h
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

#ifndef GRAININGREDIENT_H
#define GRAININGREDIENT_H

#include "Ingredient.h"

class GrainIngredient : public Ingredient
{
    Q_OBJECT
public:
    explicit GrainIngredient(QObject *parent = 0);

    virtual QString type() const;
    virtual void setType(QString type);

    virtual double color() const;
    virtual void setColor(double color);

    virtual double specificGravity() const;
    virtual void setSpecificGravity(double specificGravity);

    virtual bool extract() const;
    virtual void setExtract(bool extract);

    virtual QDomElement toXml(QDomDocument);

protected:
    virtual void setExtract(QString extract);
    virtual void fromXml(QDomElement);
    QString _type;
    double _color;
    double _specificGravity;
    bool _extract;
};

#endif // GRAININGREDIENT_H
