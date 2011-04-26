/*!
   \file IngredientToolbox.h
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

#ifndef INGREDIENTTOOLBOX_H
#define INGREDIENTTOOLBOX_H

#include <QWidget>
#include <QList>
#include <QListView>
#include <QCompleter>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QSortFilterProxyModel>

#include "Ingredient.h"
#include "IngredientModel.h"
#include "IngredientEditDialog.h"

namespace Ui {
    class IngredientToolbox;
}

class IngredientToolbox : public QWidget
{
    Q_OBJECT

public:
    explicit IngredientToolbox(QString filepath = QString("ingredients.xml"), QWidget *parent = 0);
    ~IngredientToolbox();

protected:
    void load(QString filepath = QString());
    void save(QString filepath = QString());
    void insert(Ingredient *ingredient, bool sort = true);

    QString _filepath;
    bool _ingredientsChanged;

    QList<Ingredient *> _ingredients;
    IngredientModel *_grainIngredientModel;
    IngredientModel *_hopsIngredientModel;
    IngredientModel *_yeastIngredientModel;
    IngredientModel *_otherIngredientModel;
    QSortFilterProxyModel *_grainIngredientProxyModel;
    QSortFilterProxyModel *_hopsIngredientProxyModel;
    QSortFilterProxyModel *_yeastIngredientProxyModel;
    QSortFilterProxyModel *_otherIngredientProxyModel;

protected slots:
    void ingredientChanged();
    void editIngredient(QModelIndex index);
    void on_ToolBox_currentChanged(int page);
    void on_btnNew_clicked();
    void on_btnRemove_clicked();
    void on_txtFilter_textChanged(QString);

private:
    Ui::IngredientToolbox *ui;
};

#endif // INGREDIENTTOOLBOX_H
