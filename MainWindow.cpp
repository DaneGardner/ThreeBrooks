/*!
   \file MainWindow.cpp
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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QCoreApplication::applicationName());
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

void MainWindow::on_actionDocumentation_triggered()
{
    //TODO:
}

void MainWindow::on_actionAboutThreeBrooks_triggered()
{
    //TODO:
}

void MainWindow::on_actionAboutQt4_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionPrint_triggered()
{

    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Recipe"));
    if(dialog->exec() != QDialog::Accepted)
        return;

    Recipe *recipe = qobject_cast<RecipeWidget *>(ui->tabWidget->currentWidget())->recipe();

    QTextDocument document(this);
    QDomDocument xhtml("PrintRecipe");
    xhtml.appendChild(xhtml.createElement("html"));
    xhtml.firstChild().appendChild(xhtml.createElement("head"));

    QDomElement bodyElement = xhtml.createElement("body");
    xhtml.firstChild().appendChild(bodyElement);

    QDomElement element = xhtml.createElement("h1");
    element.appendChild(xhtml.createTextNode(recipe->name()));
    bodyElement.appendChild(element);

    element = xhtml.createElement("h3");
    element.appendChild(xhtml.createTextNode("Properties"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("p");
    element.appendChild(xhtml.createTextNode(QString("Style: %1").arg(recipe->style())));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Volume: %1").arg(recipe->volume().toString())));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Boil time: %1 minutes").arg(recipe->boilTime())));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Efficiency: %1%").arg(recipe->efficiency() * 100)));
    element.appendChild(xhtml.createElement("br"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("h3");
    element.appendChild(xhtml.createTextNode("Calculated"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("p");
    element.appendChild(xhtml.createTextNode(QString("Original gravity: %1").arg(recipe->originalGravity(), 0, 'f', 3)));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Final Gravity: %1").arg(recipe->finalGravity(), 0, 'f', 3)));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Bitterness: %1 IBU").arg(recipe->bitterness(), 0, 'f', 0)));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Color: %1 SRM").arg(recipe->color(), 0, 'f', 1)));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("ABV: %1%").arg(recipe->alcoholByVolume() * 100, 0, 'f', 1)));
    element.appendChild(xhtml.createElement("br"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("h3");
    element.appendChild(xhtml.createTextNode("Ingredients"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("p");
    foreach(RecipeIngredient *recipeIngredient, recipe->ingredients()) {

        QString ingredientText = QString("%1 - %2").arg(recipeIngredient->name()).arg(recipeIngredient->quantity().toString());
        if(recipeIngredient->minutes() > 0)
            ingredientText += QString(" @ %1 minutes").arg(recipeIngredient->minutes(), 0, 'f', 0);

        element.appendChild(xhtml.createTextNode(ingredientText));
        element.appendChild(xhtml.createElement("br"));
    }
    bodyElement.appendChild(element);

    document.setHtml(xhtml.toString());
    document.print(&printer);
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
        this->setWindowTitle(QCoreApplication::applicationName().append(" - %1").arg(title));

    } else {
        ui->actionSaveRecipe->setEnabled(false);
        ui->actionSaveAsRecipe->setEnabled(false);
        ui->actionCloseRecipe->setEnabled(false);
        this->setWindowTitle(QCoreApplication::applicationName());
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
        this->setWindowTitle(QCoreApplication::applicationName().append(" - %1").arg(title));
        ui->tabWidget->setTabToolTip(index, sender->windowFilePath());
    }
}


