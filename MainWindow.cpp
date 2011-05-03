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

MainWindow *_instance;
MainWindow *MainWindow::instance()
{
    return _instance? _instance: _instance = new MainWindow();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    this->setWindowTitle(QCoreApplication::applicationName());
    on_actionReloadStyleSheet_triggered();

#ifndef QT_DEBUG
    ui->menuBar->removeAction(ui->menuDebug->menuAction());
#endif

    _ingredientToolbox = new IngredientToolbox("ingredients.xml", ui->splitter);

    QWidget *layoutWidget = new QWidget();
    _recipeLayout = new QVBoxLayout();
    _recipeLayout->setSpacing(0);
    _recipeLayout->setMargin(0);
    layoutWidget->setLayout(_recipeLayout);
    ui->splitter->addWidget(layoutWidget);

    _recipeTabWidget = new PrettyTabWidget();
    connect(_recipeTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));
    connect(_recipeTabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
    _recipeLayout->addWidget(_recipeTabWidget);

    /* See issue #9 about incompatibity and user confusion with the menu-based hiding of the ingredient sidebar
       I've disabled the collapsing of the toolbox until this is resolved */
    ui->splitter->setCollapsible(0, false);     // Allow the toolbox to collapse
    ui->splitter->setCollapsible(1, false);     // Don't collapse the recipe tab widget

    QSettings settings("settings.ini", QSettings::IniFormat);
    restoreGeometry(settings.value("MainWindow/geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("MainWindow/state", saveState()).toByteArray());
    ui->splitter->restoreState(settings.value("MainWindow/splitterState", ui->splitter->saveState()).toByteArray());
    resize(settings.value("MainWindow/size", size()).toSize());
    move(settings.value("MainWindow/position", pos()).toPoint());
}

MainWindow::~MainWindow()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/state", saveState());
    settings.setValue("MainWindow/splitterState", ui->splitter->saveState());
    settings.setValue("MainWindow/size", size());
    settings.setValue("MainWindow/position", pos());

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    while(_recipeTabWidget->count())
        this->on_actionCloseRecipe_triggered();

    QMainWindow::closeEvent(event);
}

void MainWindow::showNotification(const QString text, const QIcon icon, const bool modality,
                                  const QDialogButtonBox::StandardButtons standardButtons,
                                  const QObject *reciever, const char *member)
{
    NotificationBar *notificationBar = new NotificationBar(this);

    notificationBar->setText(text);
    notificationBar->setIcon(icon);
    notificationBar->setStandardButtons(standardButtons);

    if(modality) {
        notificationBar->setModality(true);
        modalNotificationBar();
    }

    if(reciever && member) {
        connect(notificationBar, SIGNAL(buttonClicked(QDialogButtonBox::StandardButton)), reciever, member);
    }
    connect(notificationBar, SIGNAL(closing()), this, SLOT(notificationBarClosing()));

    _recipeLayout->insertWidget(0, notificationBar);
    notificationBar->show();
    notificationBar->setFocus();
}

void MainWindow::notificationBarClosing()
{
    NotificationBar *notificationBar = qobject_cast<NotificationBar *>(QObject::sender());
    _recipeLayout->removeWidget(notificationBar);
    if(notificationBar->modality()) {
        modalNotificationBar(true);
    }
}

void MainWindow::modalNotificationBar(bool closing)
{
    static int modalCount = 0;

    if(!closing) {
        modalCount++;
        _recipeTabWidget->setEnabled(false);
        _ingredientToolbox->setEnabled(false);
        ui->menuBar->setEnabled(false);
    } else if(--modalCount <= 0){
        _recipeTabWidget->setEnabled(true);
        _ingredientToolbox->setEnabled(true);
        ui->menuBar->setEnabled(true);
    }
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
    _recipeTabWidget->addTab(recipeWidget, QString());
    _recipeTabWidget->setCurrentWidget(recipeWidget);
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
            for(int i=0; i < _recipeTabWidget->count(); i++) {
                RecipeWidget *recipeWidget = qobject_cast<RecipeWidget *>(_recipeTabWidget->widget(i));
                if(recipeWidget && recipeWidget->windowFilePath() == filepath) {
                    _recipeTabWidget->setCurrentIndex(i);
                    return;
                }
            }

            RecipeWidget *recipeWidget = new RecipeWidget(filepath, this);
            _recipeTabWidget->addTab(recipeWidget, QString());
            _recipeTabWidget->setCurrentWidget(recipeWidget);
            connect(recipeWidget, SIGNAL(changed()), this, SLOT(recipeChanged()));
            recipeChanged(recipeWidget);
        }
    } catch(QString err) {
        showNotification(tr("Could not open recipe: %1").arg(err));
    } catch(...) {
        showNotification(tr("Could not open recipe"));
    }
}

void MainWindow::on_actionCloseRecipe_triggered()
{
    tabCloseRequested(_recipeTabWidget->currentIndex());
}

bool MainWindow::on_actionSaveRecipe_triggered()
{
    try {
        if(_recipeTabWidget->currentWidget()) {
            RecipeWidget *recipeWidget = qobject_cast<RecipeWidget *>(_recipeTabWidget->currentWidget());
            if(recipeWidget) {
                if(!recipeWidget->windowFilePath().isEmpty()) {
                    recipeWidget->save();
                    return true;
                } else {
                    return on_actionSaveAsRecipe_triggered();
                }
            }
        }
    } catch(QString err) {
        showNotification(tr("Could not save recipe: %1").arg(err));
        return false;
    } catch(...) {
        showNotification(tr("Could not save recipe."));
        return false;
    }

    return false;
}

bool MainWindow::on_actionSaveAsRecipe_triggered()
{
    try {
        if(_recipeTabWidget->currentWidget()) {
            RecipeWidget *recipeWidget = qobject_cast<RecipeWidget *>(_recipeTabWidget->currentWidget());
            if(recipeWidget) {
                QString filepath =
                        QFileDialog::getSaveFileName(this, tr("Save Recipe"), recipeWidget->windowFilePath(), tr("Recipe files(*.recipe)"));
                if(!filepath.isEmpty()) {
                    recipeWidget->save(filepath);
                    return true;
                }
            }
        }
    } catch(QString err) {
        showNotification(tr("Could not save recipe: %1").arg(err));
        return false;
    } catch(...) {
        showNotification(tr("Could not save recipe."));
        return false;
    }
    return false;
}

void MainWindow::on_actionDocumentation_triggered()
{
    //TODO:
}

void MainWindow::on_actionAboutThreeBrooks_triggered()
{

    QString title = tr("About %1").arg(QApplication::applicationName());

    QString text;
    text.append(tr("<h2>%1 %2</h2>").arg(QApplication::applicationName(), QApplication::applicationVersion()));
    text.append(tr("Based on Qt %1 (%2-bit)<br/>").arg(QT_VERSION_STR, QString().setNum(QSysInfo::WordSize)));
    text.append(tr("<br/>"));
    text.append(tr("Built on %1 at %2<br/>").arg(__DATE__, __TIME__));
    text.append(tr("<br/><br/>"));
    text.append(QApplication::instance()->property("Copyright").toString());
    text.append(tr("<br/><br/>"));
    text.append(QApplication::instance()->property("License").toString());

    QMessageBox msg;
    msg.about(this, title, text);
}

void MainWindow::on_actionAboutQt4_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionPrintRecipe_triggered()
{
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Recipe"));
    if(dialog->exec() != QDialog::Accepted)
        return;

    RecipeWidget *recipeWidget = qobject_cast<RecipeWidget *>(_recipeTabWidget->currentWidget());
    recipeWidget->print(&printer);
}

void MainWindow::on_actionRecipeRight_triggered()
{
    int index = _recipeTabWidget->currentIndex();
    index++;

    if(index == _recipeTabWidget->count())
        index = 0;

    _recipeTabWidget->setCurrentIndex(index);
}

void MainWindow::on_actionRecipeLeft_triggered()
{
    int index = _recipeTabWidget->currentIndex();
    index--;

    if(index < 0)
        index = _recipeTabWidget->count() -1;

    _recipeTabWidget->setCurrentIndex(index);
}

void MainWindow::on_actionHideSidebar_triggered(bool checked)
{
    //TODO: This isn't really compatible with the collapsing splitter, need to fix this

    ui->splitter->widget(0)->setVisible(checked);
}

void MainWindow::tabCloseRequested(int index)
{
    QWidget *oldWidget = _recipeTabWidget->currentWidget();
    _recipeTabWidget->setCurrentIndex(index);

    QWidget *recipeWidget = _recipeTabWidget->widget(index);
    if(recipeWidget->isWindowModified()) {
        QMessageBox msg(QMessageBox::Warning, tr("Save before closing?"),
                        tr("Would you like to save changes to the recipe before closing it?"),
                        QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, this);

        int standardButton = msg.exec();
        if(standardButton == QMessageBox::Yes) {
            if(!on_actionSaveRecipe_triggered()) {
                return;
            }
        } else if(standardButton == QMessageBox::Cancel) {
            return;
        }
    }

    _recipeTabWidget->removeTab(index);
    if(oldWidget != recipeWidget) {
        _recipeTabWidget->setCurrentWidget(oldWidget);
    }

    recipeWidget->close();
    delete recipeWidget;
}

void MainWindow::currentChanged(int index)
{
    Q_UNUSED(index)

    if(_recipeTabWidget->count() > 0) {
        ui->actionSaveRecipe->setEnabled(true);
        ui->actionSaveAsRecipe->setEnabled(true);
        ui->actionCloseRecipe->setEnabled(true);
        ui->actionPrintRecipe->setEnabled(true);

        QString title = _recipeTabWidget->currentWidget()->windowTitle().replace(" [*]", "*");
        this->setWindowTitle(QCoreApplication::applicationName().append(" - %1").arg(title));

    } else {
        ui->actionSaveRecipe->setEnabled(false);
        ui->actionSaveAsRecipe->setEnabled(false);
        ui->actionCloseRecipe->setEnabled(false);
        ui->actionPrintRecipe->setEnabled(false);
        this->setWindowTitle(QCoreApplication::applicationName());
    }
}

void MainWindow::recipeChanged(QWidget *sender)
{
    if(!sender) {
        sender = qobject_cast<QWidget *>(QObject::sender());
    }

    if(sender) {
        int index = _recipeTabWidget->indexOf(sender);
        QString title = sender->windowTitle().replace(" [*]", "*");
        _recipeTabWidget->setTabText(index, title);
        _recipeTabWidget->setTabToolTip(index, sender->windowFilePath());

        //FIXME: Need a better "recipe" icon for the tabs
        //_recipeTabWidget->setTabIcon(index, sender->windowIcon());

        if(_recipeTabWidget->currentIndex() == index) {
            this->setWindowTitle(QCoreApplication::applicationName().append(" - %1").arg(title));
        }
    }
}



