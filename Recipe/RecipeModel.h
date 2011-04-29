/*!
   \file RecipeModel.h
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

#ifndef RECIPEMODEL_H
#define RECIPEMODEL_H

#include <QAbstractItemModel>
#include <QColor>
#include "Recipe.h"

class RecipeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RecipeModel(QObject *parent = 0);

    Recipe *recipe() const { return _recipe; }
    void setRecipe(Recipe *recipe) { _recipe = recipe; connect(_recipe, SIGNAL(dataChanged()), this, SLOT(recipeChanged())); }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

protected:
    Recipe *_recipe;

protected slots:
    void recipeChanged();

};

#endif // RECIPEMODEL_H
