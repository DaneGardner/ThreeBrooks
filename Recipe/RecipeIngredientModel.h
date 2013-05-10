/*!
   \file RecipeIngredientModel.h
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

#ifndef RECIPEINGREDIENTMODEL_H
#define RECIPEINGREDIENTMODEL_H

#include <QApplication>
#include <QAbstractItemModel>
#include <QMimeData>
#include <QIcon>
#include "Recipe/Recipe.h"

class RecipeIngredientModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RecipeIngredientModel(QObject *parent = 0);

    Recipe *recipe() { return _recipe; }
    void setRecipe(Recipe *recipe);

    void addIngredient(RecipeIngredient *ingredient);

    /* QAbstractItemModel operations */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    /* QList operations */
    int row(RecipeIngredient *ingredient) const;
    RecipeIngredient *ingredient(int row) const;
    void prepend(Ingredient *ingredient);
    void append(Ingredient *ingredient);
    void insert(int row, Ingredient *ingredient);
    void prepend(RecipeIngredient *ingredient);
    void append(RecipeIngredient *ingredient);
    void insert(int row, RecipeIngredient *ingredient);
    void remove(RecipeIngredient *ingredient);
    void remove(int row);
    void move(RecipeIngredient *ingredient, int destination);
    void move(int row, int destination);

protected slots:
    void ingredientChanged();

protected:
    Recipe *_recipe;

};

#endif // RECIPEINGREDIENTMODEL_H
