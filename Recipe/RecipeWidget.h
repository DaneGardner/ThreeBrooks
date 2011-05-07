/*!
   \file RecipeWidget.h
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

#ifndef RECIPEWIDGET_H
#define RECIPEWIDGET_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDomDocument>
#include <QFile>
#include <QSettings>
#include <QAbstractProxyModel>
#include <QPrinter>
#include <QTextDocument>

#include "Recipe.h"
#include "RecipeModel.h"
#include "RecipeIngredientModel.h"
#include "RecipeIngredientDelegate.h"
#include "MainWindow.h"

namespace Ui {
    class RecipeWidget;
}

class RecipeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecipeWidget(QWidget *parent = 0);
    explicit RecipeWidget(QString filepath, QWidget *parent = 0);
    ~RecipeWidget();

    Recipe *recipe() { return _recipe; }

    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);

    void save(QString filepath = QString());
    void print(QPrinter *printer);

signals:
    void changed();

protected slots:
    void recipeChanged();
    void refreshText(bool modified = false);
    void on_txtName_editingFinished();
    void on_cmbStyle_currentIndexChanged(QString style);
    void on_spnVolume_editingFinished();
    void on_spnEfficiency_editingFinished();
    void on_txtNotes_textChanged();
    void on_btnRaise_clicked();
    void on_btnLower_clicked();
    void on_btnRemove_clicked();
    void removeSelectedIngredients(QDialogButtonBox::StandardButton);

protected:
    void setRecipe(Recipe *recipe);
    void load(QString filepath);
    Recipe *_recipe;

    static bool timeDescending(RecipeIngredient *left, RecipeIngredient *right);

private:
    Ui::RecipeWidget *ui;
};

#endif // RECIPEWIDGET_H
