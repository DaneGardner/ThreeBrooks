/*!
   \file 
   \author Dane Gardner <dane.gardner@gmail.com>
   \version 

   \section LICENSE
   This file is part of the Open|SpeedShop Graphical User Interface
   Copyright (C) 2010-2011 Argo Navis Technologies, LLC

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as published by the
   Free Software Foundation; either version 2.1 of the License, or (at your
   option) any later version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
   for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

   \section DESCRIPTION

 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_actionReloadStyleSheet_triggered();

    IngredientToolbox *ingredientToolbox = new IngredientToolbox("ingredients.xml", this);
    ui->centralWidget->insertWidget(0, ingredientToolbox);

    QSettings settings;
    restoreGeometry(settings.value("MainWindow/geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("MainWindow/state", saveState()).toByteArray());
    ui->centralWidget->restoreState(settings.value("MainWindow/splitterState", ui->centralWidget->saveState()).toByteArray());
    resize(settings.value("MainWindow/size", size()).toSize());
    move(settings.value("MainWindow/position", pos()).toPoint());
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/state", saveState());
    settings.setValue("MainWindow/splitterState", ui->centralWidget->saveState());
    settings.setValue("MainWindow/size", size());
    settings.setValue("MainWindow/position", pos());

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    while(ui->tabWidget->count())
        this->on_actionCloseRecipe_triggered();

    QMainWindow::closeEvent(event);
}

void MainWindow::on_actionReloadStyleSheet_triggered()
{
    QFile styleSheet("stylesheet.css");
    if(styleSheet.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(styleSheet.readAll());
        styleSheet.close();
    }
}


void MainWindow::on_actionNewRecipe_triggered()
{
    RecipeWidget *recipeWidget = new RecipeWidget(this);
    ui->tabWidget->addTab(recipeWidget, QString());
    ui->tabWidget->setCurrentWidget(recipeWidget);
    connect(recipeWidget, SIGNAL(changed()), this, SLOT(recipeChanged()));
    recipeChanged(recipeWidget);
}

void MainWindow::on_actionOpenRecipe_triggered()
{
    try {
        QString filepath =
                QFileDialog::getOpenFileName(this, tr("Open Recipe"), QString(), tr("Recipe files (*.recipe)"));

        if(!filepath.isEmpty()) {

            // Make sure it's not already open
            for(int i=0; i < ui->tabWidget->count(); i++) {
                RecipeWidget *recipeWidget = qobject_cast<RecipeWidget *>(ui->tabWidget->widget(i));
                if(recipeWidget && recipeWidget->windowFilePath() == filepath) {
                    ui->tabWidget->setCurrentIndex(i);
                    return;
                }
            }

            RecipeWidget *recipeWidget = new RecipeWidget(filepath, this);
            ui->tabWidget->addTab(recipeWidget, QString());
            ui->tabWidget->setCurrentWidget(recipeWidget);
            connect(recipeWidget, SIGNAL(changed()), this, SLOT(recipeChanged()));
            recipeChanged(recipeWidget);
        }
    } catch(QString err) {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Error!"),
                        tr("Could not open recipe: %1").arg(err),
                        QMessageBox::Ok,
                        this);
        msg.exec();
    } catch(...) {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Error!"),
                        tr("Could not open recipe."),
                        QMessageBox::Ok,
                        this);
        msg.exec();
    }
}

void MainWindow::on_actionCloseRecipe_triggered()
{
    on_tabWidget_tabCloseRequested(ui->tabWidget->currentIndex());
}

bool MainWindow::on_actionSaveRecipe_triggered()
{
    try {
        if(ui->tabWidget->currentWidget()) {
            RecipeWidget *recipeWidget = qobject_cast<RecipeWidget *>(ui->tabWidget->currentWidget());
            if(recipeWidget) {
                if(!recipeWidget->windowFilePath().isEmpty()) {
                    recipeWidget->save();
                } else {
                    on_actionSaveAsRecipe_triggered();
                }
            }
        }
    } catch(QString err) {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Error!"),
                        tr("Could not save recipe: %1").arg(err),
                        QMessageBox::Ok,
                        this);
        msg.exec();
        return false;
    } catch(...) {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Error!"),
                        tr("Could not save recipe."),
                        QMessageBox::Ok,
                        this);
        msg.exec();
        return false;
    }

    return true;
}

bool MainWindow::on_actionSaveAsRecipe_triggered()
{
    try {
        if(ui->tabWidget->currentWidget()) {
            RecipeWidget *recipeWidget = qobject_cast<RecipeWidget *>(ui->tabWidget->currentWidget());
            if(recipeWidget) {
                QString filepath =
                        QFileDialog::getSaveFileName(this, tr("Save Recipe"), recipeWidget->windowFilePath(), tr("Recipe files(*.recipe)"));
                if(!filepath.isEmpty()) {
                    recipeWidget->save(filepath);
                }
            }
        }
    } catch(QString err) {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Error!"),
                        tr("Could not save recipe: %1").arg(err),
                        QMessageBox::Ok,
                        this);
        msg.exec();
        return false;
    } catch(...) {
        QMessageBox msg(QMessageBox::Critical,
                        tr("Error!"),
                        tr("Could not save recipe."),
                        QMessageBox::Ok,
                        this);
        msg.exec();
        return false;
    }
    return true;
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QWidget *oldWidget = ui->tabWidget->currentWidget();
    ui->tabWidget->setCurrentIndex(index);

    QWidget *recipeWidget = ui->tabWidget->widget(index);
    if(recipeWidget->isWindowModified()) {
        QMessageBox msg(QMessageBox::Warning, tr("Save before closing?"),
                        tr("Would you like to save changes to the recipe before closing it?"),
                        QMessageBox::Yes|QMessageBox::No, this);

        if(msg.exec() == QMessageBox::Yes) {
            if(!on_actionSaveRecipe_triggered()) {
                return;
            }
        }
    }

    ui->tabWidget->removeTab(index);
    if(oldWidget != recipeWidget) {
        ui->tabWidget->setCurrentWidget(oldWidget);
    }

    recipeWidget->close();
    delete recipeWidget;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index)

    if(ui->tabWidget->count() > 0) {
        ui->actionSaveRecipe->setEnabled(true);
        ui->actionSaveAsRecipe->setEnabled(true);
        ui->actionCloseRecipe->setEnabled(true);

        QString title = ui->tabWidget->currentWidget()->windowTitle().replace(" [*]", "*");
        this->setWindowTitle(tr("Recipe Creator - %1").arg(title));

    } else {
        ui->actionSaveRecipe->setEnabled(false);
        ui->actionSaveAsRecipe->setEnabled(false);
        ui->actionCloseRecipe->setEnabled(false);
        this->setWindowTitle(tr("Recipe Creator"));
    }

}

void MainWindow::recipeChanged(QWidget *sender)
{
    if(!sender) {
        sender = qobject_cast<QWidget *>(QObject::sender());
    }

    if(sender) {
        int index = ui->tabWidget->indexOf(sender);
        QString title = sender->windowTitle().replace(" [*]", "*");
        ui->tabWidget->setTabText(index, title);
        this->setWindowTitle(tr("Recipe Creator - %1").arg(title));
        ui->tabWidget->setTabToolTip(index, sender->windowFilePath());
    }
}


