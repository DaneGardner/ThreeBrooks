/*!
   \file Quantity.h
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

#ifndef QUANTITY_H
#define QUANTITY_H

#include <QObject>
#include <QMetaType>
#include <QStringList>
#include <QDomDocument>

class Quantity : public QObject
{
    Q_OBJECT

public:
    enum QuantityTypes {
        QuantityType_unknown,
        QuantityType_milligram,
        QuantityType_gram,
        QuantityType_kilogram,
        QuantityType_ounce,
        QuantityType_pound,
        QuantityType_milliliter,
        QuantityType_liter,
        QuantityType_decaliter,
        QuantityType_fluidOunce,
        QuantityType_quart,
        QuantityType_gallon,
        QuantityType_item
    };

    explicit Quantity(double value = 0, QuantityTypes = QuantityType_item);
    explicit Quantity(QDomElement element);

    Quantity(const Quantity &other);

    double value() { return _value; }
    void setValue(double value) { _value = value; }

    QString toString();

    QuantityTypes type() { return _type; }
    static QuantityTypes type(QString type);
    void setType(QuantityTypes type) { _type = type; }
    QString typeName(bool brevis = true);
    static QStringList typeNames(bool brevis = true);
    static QString typeName(QuantityTypes type, bool brevis = true, bool singular = false);


    Quantity &operator=(const Quantity &other);


    double valueToMilligram();
    double valueToGram();
    double valueToKilogram();
    double valueToOunce();
    double valueToPound();
    double valueToPoundOunce();
    double valueToMilliliter();
    double valueToLiter();
    double valueToDecaliter();
    double valueToFluidOunce();
    double valueToQuart();
    double valueToGallon();

    Quantity toMilligram();
    Quantity toGram();
    Quantity toKilogram();
    Quantity toOunce();
    Quantity toPound();
    Quantity toPoundOunce();
    Quantity toMilliliter();
    Quantity toLiter();
    Quantity toDecaliter();
    Quantity toFluidOunce();
    Quantity toQuart();
    Quantity toGallon();

    QDomElement toXml(QDomDocument document);
    void fromXml(QDomElement element);

protected:
    double _value;
    QuantityTypes _type;
};

Q_DECLARE_METATYPE(Quantity);

#endif // QUANTITY_H
