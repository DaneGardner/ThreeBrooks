/*!
   \file Quantity.cpp
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

#include "Quantity.h"

Quantity::Quantity(double value, QuantityTypes type, QObject *parent) :
    QObject(parent)
{
    _value = value;
    _type = type;
}

Quantity::Quantity(QDomElement element, QObject *parent) :
    QObject(parent)
{
    _value = 0;
    _type = QuantityType_item;

    fromXml(element);
}


Quantity::Quantity(const Quantity &other)
{
    _value = other._value;
    _type = other._type;
}

Quantity &Quantity::operator=(const Quantity &other)
{
    if(this == &other)
        return *this;

    _value = other._value;
    _type = other._type;
    return *this;
}

QString Quantity::typeName(bool brevis)
{
    return typeName(type(), brevis, (value() == 1));
}

QString Quantity::typeName(QuantityTypes type, bool brevis, bool singular)
{
    switch(type) {
    case QuantityType_milligram:
        if(brevis) return tr("mg");
        if(singular) return tr("Milligram");
        return tr("Milligrams");
    case QuantityType_gram:
        if(brevis) return tr("g");
        if(singular) return tr("Gram");
        return tr("Grams");
    case QuantityType_kilogram:
        if(brevis) return tr("kg");
        if(singular) return tr("Kilogram");
        return tr("Kilograms");
    case QuantityType_ounce:
        if(brevis) return tr("oz");
        if(singular) return tr("Ounce");
        return tr("Ounces");
    case QuantityType_pound:
        if(brevis) return tr("lb");
        if(singular) return tr("Pound");
        return tr("Pounds");
    case QuantityType_milliliter:
        if(brevis) return tr("mL");
        if(singular) return tr("Milliliter");
        return tr("Milliliters");
    case QuantityType_liter:
        if(brevis) return tr("L");
        if(singular) return tr("Liter");
        return tr("Liters");
    case QuantityType_decaliter:
        if(brevis) return tr("daL");
        if(singular) return tr("Decaliter");
        return tr("Decaliters");
    case QuantityType_fluidOunce:
        if(brevis) return tr("fl oz");
        if(singular) return tr("Fluid Ounce");
        return tr("Fluid Ounces");
    case QuantityType_quart:
        if(brevis) return tr("qt");
        if(singular) return tr("Quart");
        return tr("Quarts");
    case QuantityType_gallon:
        if(brevis) return tr("gal");
        if(singular) return tr("Gallon");
        return tr("Gallons");
    case QuantityType_item:
        if(brevis) return tr("ea");
        if(singular) return tr("Item");
        return tr("Items");
    default:
        return QString();
    };
}

QString Quantity::toString()
{
    return QString("%1 %2").arg(value()).arg(typeName());
}


double Quantity::valueToMilligram()
{
    switch(type()) {
    case QuantityType_milligram:
        return value();
    case QuantityType_gram:
        return value() * 1000;
    case QuantityType_kilogram:
        return value() * 1000000;
    case QuantityType_ounce:
        return value() * 28349.5231;
    case QuantityType_pound:
        return value() * 453592.37;
    default:
        return -1;
    }
}

double Quantity::valueToGram()
{
    switch(type()) {
    case QuantityType_milligram:
        return value() * 0.001;
    case QuantityType_gram:
        return value();
    case QuantityType_kilogram:
        return value() * 1000;
    case QuantityType_ounce:
        return value() * 28.3495231;
    case QuantityType_pound:
        return value() * 453.59237;
    default:
        return -1;
    }
}

double Quantity::valueToKilogram()
{
    switch(type()) {
    case QuantityType_milligram:
        return value() * 0.000001;
    case QuantityType_gram:
        return value() * 0.001;
    case QuantityType_kilogram:
        return value();
    case QuantityType_ounce:
        return value() * 0.0283495231;
    case QuantityType_pound:
        return value() * 0.45359237;
    default:
        return -1;
    }
}

double Quantity::valueToOunce()
{
    switch(type()) {
    case QuantityType_milligram:
        return value() * 0.0000352739619;
    case QuantityType_gram:
        return value() * 0.0352739619;
    case QuantityType_kilogram:
        return value() * 35.2739619;
    case QuantityType_ounce:
        return value();
    case QuantityType_pound:
        return value() * 16;
    default:
        return -1;
    }
}

double Quantity::valueToPound()
{
    switch(type()) {
    case QuantityType_milligram:
        return value() * 0.00000220462262;
    case QuantityType_gram:
        return value() * 0.00220462262;
    case QuantityType_kilogram:
        return value() * 2.20462262;
    case QuantityType_ounce:
        return value() * 0.0625;
    case QuantityType_pound:
        return value();
    default:
        return -1;
    }
}

double Quantity::valueToMilliliter()
{
    switch(type()) {
    case QuantityType_milliliter:
        return value();
    case QuantityType_liter:
        return value() * 1000;
    case QuantityType_decaliter:
        return value() * 10000;
    case QuantityType_fluidOunce:
        return value() * 29.5735296;
    case QuantityType_quart:
        return value() * 946.352946;
    case QuantityType_gallon:
        return value() * 3785.41178;
    default:
        return -1;
    }
}

double Quantity::valueToLiter()
{
    switch(type()) {
    case QuantityType_milliliter:
        return value() * .001;
    case QuantityType_liter:
        return value();
    case QuantityType_decaliter:
        return value() * 10;
    case QuantityType_fluidOunce:
        return value() * 0.0295735296;
    case QuantityType_quart:
        return value() * 0.946352946;
    case QuantityType_gallon:
        return value() * 3.78541178;
    default:
        return -1;
    }
}

double Quantity::valueToDecaliter()
{
    switch(type()) {
    case QuantityType_milliliter:
        return value() * .0001;
    case QuantityType_liter:
        return value() * 0.1;
    case QuantityType_decaliter:
        return value();
    case QuantityType_fluidOunce:
        return value() * 0.00295735296;
    case QuantityType_quart:
        return value() * 0.0946352946;
    case QuantityType_gallon:
        return value() * 0.378541178;
    default:
        return -1;
    }
}

double Quantity::valueToFluidOunce()
{
    switch(type()) {
    case QuantityType_milliliter:
        return value() * 0.0338140227;
    case QuantityType_liter:
        return value() * 33.8140227;
    case QuantityType_decaliter:
        return value() * 338.140227;
    case QuantityType_fluidOunce:
        return value();
    case QuantityType_quart:
        return value() * 32;
    case QuantityType_gallon:
        return value() * 128;
    default:
        return -1;
    }
}

double Quantity::valueToQuart()
{
    switch(type()) {
    case QuantityType_milliliter:
        return value() * 0.00105668821;
    case QuantityType_liter:
        return value() * 1.05668821;
    case QuantityType_decaliter:
        return value() * 10.5668821;
    case QuantityType_fluidOunce:
        return value() * 0.03125;
    case QuantityType_quart:
        return value();
    case QuantityType_gallon:
        return value() * 4;
    default:
        return -1;
    }
}

double Quantity::valueToGallon()
{
    switch(type()) {
    case QuantityType_milliliter:
        return value() * 0.000264172052;
    case QuantityType_liter:
        return value() * 0.264172052;
    case QuantityType_decaliter:
        return value() * 2.64172052;
    case QuantityType_fluidOunce:
        return value() * 0.0078125;
    case QuantityType_quart:
        return value() * 0.25;
    case QuantityType_gallon:
        return value();
    default:
        return -1;
    }
}


Quantity Quantity::toMilligram()
{
    return Quantity(valueToMilligram(), QuantityType_milligram, parent());
}

Quantity Quantity::toGram()
{
    return Quantity(valueToGram(), QuantityType_gram, parent());
}

Quantity Quantity::toKilogram()
{
    return Quantity(valueToKilogram(), QuantityType_kilogram, parent());
}

Quantity Quantity::toOunce()
{
    return Quantity(valueToOunce(), QuantityType_ounce, parent());
}

Quantity Quantity::toPound()
{
    return Quantity(valueToPound(), QuantityType_pound, parent());
}

Quantity Quantity::toMilliliter()
{
    return Quantity(valueToMilliliter(), QuantityType_milliliter, parent());
}

Quantity Quantity::toLiter()
{
    return Quantity(valueToLiter(), QuantityType_liter, parent());
}

Quantity Quantity::toDecaliter()
{
    return Quantity(valueToDecaliter(), QuantityType_decaliter, parent());
}

Quantity Quantity::toFluidOunce()
{
    return Quantity(valueToFluidOunce(), QuantityType_fluidOunce, parent());
}

Quantity Quantity::toQuart()
{
    return Quantity(valueToQuart(), QuantityType_quart, parent());
}

Quantity Quantity::toGallon()
{
    return Quantity(valueToGallon(), QuantityType_gallon, parent());
}


QStringList Quantity::typeNames(bool brevis)
{
    QStringList retval;

    for(int i=0; i<32; i++) {
        QString name = typeName((QuantityTypes)i, brevis, true);
        if(!name.isEmpty())
            retval.append(name);
    }

    return retval;
}

Quantity::QuantityTypes Quantity::type(QString type)
{
    for(int i=0; i<32; i++) {
        for(int j=0; j<4; j++) {
            bool brevis = j&1;
            bool singular = j&2;
            QString typeName = Quantity::typeName((QuantityTypes)i, brevis, singular);
            if(!typeName.isEmpty() && !typeName.compare(type, Qt::CaseInsensitive))
                return (QuantityTypes)i;
        }
    }

    return QuantityType_unknown;
}

QDomElement Quantity::toXml(QDomDocument document)
{
    QDomElement element = document.createElement("Quantity");
    element.setAttribute("value", value());
    element.setAttribute("type", typeName(false));
    return element;
}

void Quantity::fromXml(QDomElement element)
{
    setValue(element.attribute("value", QString().setNum(value(), 'f')).toDouble());
    setType(type(element.attribute("type", typeName(false))));
}




