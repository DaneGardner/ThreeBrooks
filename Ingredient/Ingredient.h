/*!
   \file Ingredient.h
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

#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QObject>
#include <QUuid>
#include <QDomDocument>
#include <QMetaType>

class GrainIngredient;
class HopsIngredient;
class YeastIngredient;

class Ingredient : public QObject
{
    Q_OBJECT
public:
    explicit Ingredient(QObject *parent = 0);

    virtual QString name() const;
    virtual void setName(QString name);

    virtual QUuid id() const;
    virtual void setId(QUuid id);

    virtual QString type() const;

    virtual QDomElement toXml(QDomDocument document);

    static Ingredient *createIngredient(QDomElement element);

signals:
    void dataChanged();

protected:
    virtual void fromXml(QDomElement element);

    QString _name;
    QUuid _id;


};

Q_DECLARE_METATYPE(Ingredient *);

#endif // INGREDIENT_H
