/*!
   \file HopsIngredient.cpp
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

#include "HopsIngredient.h"

HopsIngredient::HopsIngredient(QObject *parent) :
    Ingredient(parent)
{
    _alphaAcid = 0.05;
}

QString HopsIngredient::type() const
{
    return tr("Hops");
}

double HopsIngredient::alphaAcid() const
{
    return _alphaAcid;
}
void HopsIngredient::setAlphaAcid(double alphaAcid)
{
    _alphaAcid = alphaAcid;
    emit dataChanged();
}

QDomElement HopsIngredient::toXml(QDomDocument document)
{
    QDomElement retval = Ingredient::toXml(document);
    retval.setAttribute("alphaAcid", alphaAcid());
    return retval;
}

void HopsIngredient::fromXml(QDomElement element)
{
    Ingredient::fromXml(element);
    setAlphaAcid(element.attribute("alphaAcid", QString().setNum(alphaAcid(), 'f')).toDouble());
}
