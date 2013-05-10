/*!
   \file RecipeIngredientDelegate.h
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

#ifndef RECIPEINGREDIENTDELEGATE_H
#define RECIPEINGREDIENTDELEGATE_H

#include <QStyledItemDelegate>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QSignalMapper>

#include "Quantity.h"
#include "RecipeIngredient.h"

class RecipeIngredientDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RecipeIngredientDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // RECIPEINGREDIENTDELEGATE_H
