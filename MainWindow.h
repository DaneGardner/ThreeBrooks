/*!
   \file MainWindow.h
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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include "Ingredient/IngredientToolBox.h"
#include "Recipe/RecipeWidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void on_actionNewRecipe_triggered();
    void on_actionOpenRecipe_triggered();
    void on_actionCloseRecipe_triggered();
    bool on_actionSaveRecipe_triggered();
    bool on_actionSaveAsRecipe_triggered();
    void on_tabWidget_tabCloseRequested(int);
    void on_tabWidget_currentChanged(int);
    void recipeChanged(QWidget *sender = NULL);
    void closeEvent(QCloseEvent *);

    void on_actionReloadStyleSheet_triggered();


private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
