/*!
   \file MainWindow.h
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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSettings>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "Ingredient/IngredientToolbox.h"
#include "Recipe/RecipeWidget.h"
#include "NotificationBar.h"
#include "PrettyTabWidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow *instance();

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void showNotification(const QString text, const QIcon icon = QIcon(), const bool modality = false,
                          const QDialogButtonBox::StandardButtons standardButtons = QDialogButtonBox::NoButton,
                          const QObject *reciever = NULL, const char *member = NULL);

protected slots:
    void on_actionNewRecipe_triggered();
    void on_actionOpenRecipe_triggered();
    void on_actionCloseRecipe_triggered();
    bool on_actionSaveRecipe_triggered();
    bool on_actionSaveAsRecipe_triggered();
    void on_actionDocumentation_triggered();
    void on_actionAboutThreeBrooks_triggered();
    void on_actionAboutQt4_triggered();
    void on_actionPrintRecipe_triggered();
    void on_actionRecipeRight_triggered();
    void on_actionRecipeLeft_triggered();
    void on_actionHideSidebar_triggered(bool checked);
    void on_actionReloadStyleSheet_triggered();
    void on_actionExit_triggered();

    void tabCloseRequested(int);
    void currentChanged(int);

    void recipeChanged(QWidget *sender = NULL);
    void closeEvent(QCloseEvent *);
    void notificationBarClosing();
    void modalNotificationBar(bool modality = false);

protected:
    QBoxLayout *_recipeLayout;
    PrettyTabWidget *_recipeTabWidget;
    IngredientToolbox *_ingredientToolbox;

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
