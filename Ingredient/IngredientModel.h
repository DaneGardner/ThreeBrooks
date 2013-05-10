/*!
   \file IngredientModel.h
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

#ifndef INGREDIENTMODEL_H
#define INGREDIENTMODEL_H

#include <QApplication>
#include <QAbstractItemModel>
#include <QMimeData>
#include <QVariant>
#include <QString>
#include <QIcon>

#include "Ingredient.h"
#include "GrainIngredient.h"
#include "HopsIngredient.h"
#include "YeastIngredient.h"

class IngredientModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit IngredientModel(QObject *parent = 0);

    /* QAbstractItemModel interface */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;

    /* QList operations */
    int row(Ingredient *ingredient) const;
    Ingredient *ingredient(int row) const;
    void prepend(Ingredient *ingredient);
    void append(Ingredient *ingredient);
    void insert(int row, Ingredient *ingredient);
    void remove(Ingredient *ingredient);
    void remove(int row);
    void move(Ingredient *ingredient, int destination);
    void move(int row, int destination);

protected:
    QList<Ingredient *> _ingredients;

protected slots:
    void ingredientChanged();
};

#endif // INGREDIENTMODEL_H
