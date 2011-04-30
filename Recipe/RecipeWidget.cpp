/*!
   \file RecipeWidget.cpp
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

#include "RecipeWidget.h"
#include "ui_RecipeWidget.h"

RecipeWidget::RecipeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecipeWidget)
{
    ui->setupUi(this);

    setRecipe(new Recipe(this));

    QSettings settings;
    restoreGeometry(settings.value("RecipeWidget/geometry", saveGeometry()).toByteArray());
    ui->trvIngredients->header()->restoreState(
                settings.value("RecipeWidget/ingredientsState", ui->trvIngredients->header()->saveState()).toByteArray() );
    ui->tblCalculated->horizontalHeader()->restoreState(
                settings.value("RecipeWidget/calculatedHorizontalState", ui->tblCalculated->horizontalHeader()->saveState()).toByteArray() );
    ui->tblCalculated->verticalHeader()->restoreState(
                settings.value("RecipeWidget/calculatedVerticalState", ui->tblCalculated->verticalHeader()->saveState()).toByteArray() );
}

RecipeWidget::RecipeWidget(QString filepath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecipeWidget)
{
    ui->setupUi(this);

    load(filepath);

    QSettings settings;
    restoreGeometry(settings.value("RecipeWidget/geometry", saveGeometry()).toByteArray());
    ui->trvIngredients->header()->restoreState(
                settings.value("RecipeWidget/ingredientsState", ui->trvIngredients->header()->saveState()).toByteArray() );
    ui->tblCalculated->horizontalHeader()->restoreState(
                settings.value("RecipeWidget/calculatedHorizontalState", ui->tblCalculated->horizontalHeader()->saveState()).toByteArray() );
    ui->tblCalculated->verticalHeader()->restoreState(
                settings.value("RecipeWidget/calculatedVerticalState", ui->tblCalculated->verticalHeader()->saveState()).toByteArray() );
}

RecipeWidget::~RecipeWidget()
{
    QSettings settings;
    settings.setValue("RecipeWidget/geometry", saveGeometry());
    settings.setValue("RecipeWidget/ingredientsState",  ui->trvIngredients->header()->saveState());
    settings.setValue("RecipeWidget/calculatedHorizontalState",  ui->tblCalculated->horizontalHeader()->saveState());
    settings.setValue("RecipeWidget/calculatedVerticalState",  ui->tblCalculated->verticalHeader()->saveState());

    delete ui;
}

void RecipeWidget::setRecipe(Recipe *recipe)
{
    _recipe = recipe;

    RecipeModel *recipeModel = new RecipeModel(this);
    recipeModel->setRecipe(recipe);
    ui->tblCalculated->setModel(recipeModel);

    RecipeIngredientModel *ingredientModel = new RecipeIngredientModel(this);
    ingredientModel->setRecipe(recipe);
    ui->trvIngredients->setModel(ingredientModel);
    ui->trvIngredients->setItemDelegateForColumn(2, new RecipeIngredientDelegate(this));

    connect(_recipe, SIGNAL(dataChanged()), this, SLOT(recipeChanged()));
    refreshText();
}

void RecipeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->proposedAction() != Qt::IgnoreAction && event->mimeData()->hasFormat("application/x-ingredientlist")) {
        QDomDocument document("IngredientList");
        document.setContent(event->mimeData()->data("application/x-ingredientlist"));
        if(document.isNull())
            return;

        QDomElement parentElement = document.firstChildElement("Ingredients");
        if(parentElement.isNull())
            return;

        if(parentElement.attribute("applicationPid").toLongLong() != QApplication::applicationPid())
            return;

        event->setDropAction(Qt::CopyAction);
        event->accept();
        return;
    }

    if(event->proposedAction() != Qt::IgnoreAction && event->mimeData()->hasFormat("application/x-recipeingredientlist")) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
        return;
    }
}

void RecipeWidget::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat("application/x-ingredientlist")) {
        ui->trvIngredients->model()->dropMimeData(event->mimeData(), event->proposedAction(), -1, -1, QModelIndex());
    }

    if(event->mimeData()->hasFormat("application/x-recipeingredientlist")) {
        ui->trvIngredients->model()->dropMimeData(event->mimeData(), event->proposedAction(), -1, -1, QModelIndex());
    }
}

void RecipeWidget::load(QString filepath)
{
    setWindowFilePath(filepath);

    QDomDocument document("Ingredients");
    QFile file(windowFilePath());

    if(!file.open(QIODevice::ReadOnly))
        throw tr("Failed to open recipe file");

    if(!document.setContent(&file)) {
        file.close();
        throw tr("Failed to read recipe file");
    }

    file.close();

    if(document.documentElement().tagName() != "Recipe") {
        throw tr("Not a recipe file");
    }

    setRecipe(new Recipe(document.documentElement()));
}

void RecipeWidget::save(QString filepath)
{

    if(!filepath.isEmpty()) {
        setWindowFilePath(filepath);
    } else if(!isWindowModified()) {
        return;     // Don't do anything if the file would be the same
    }


    QDomDocument document("Recipe");
    document.appendChild(recipe()->toXml(document));

    if(windowFilePath().isEmpty()) {
        throw tr("File path is not valid");
    }
    QFile file(windowFilePath());

    if(!file.open(QIODevice::WriteOnly)) {
        throw tr("Failed to open ingredients file");
    }

    file.write(document.toString().toLatin1());
    file.flush();
    file.close();

    setWindowModified(false);
    refreshText();
    emit changed();
}

void RecipeWidget::print(QPrinter *printer)
{
    QTextDocument document(this);
    QDomDocument xhtml("PrintRecipe");
    xhtml.appendChild(xhtml.createElement("html"));
    xhtml.firstChild().appendChild(xhtml.createElement("head"));

    QDomElement bodyElement = xhtml.createElement("body");
    xhtml.firstChild().appendChild(bodyElement);

    QDomElement element = xhtml.createElement("h1");
    element.appendChild(xhtml.createTextNode(recipe()->name()));
    bodyElement.appendChild(element);

    element = xhtml.createElement("h3");
    element.appendChild(xhtml.createTextNode("Properties"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("p");
    element.appendChild(xhtml.createTextNode(QString("Style: %1").arg(recipe()->style())));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Volume: %1").arg(recipe()->volume().toString())));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Boil time: %1 minutes").arg(recipe()->boilTime())));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Efficiency: %1%").arg(recipe()->efficiency() * 100)));
    element.appendChild(xhtml.createElement("br"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("h3");
    element.appendChild(xhtml.createTextNode("Calculated"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("p");
    element.appendChild(xhtml.createTextNode(QString("Original gravity: %1").arg(recipe()->originalGravity(), 0, 'f', 3)));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Final Gravity: %1").arg(recipe()->finalGravity(), 0, 'f', 3)));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Bitterness: %1 IBU").arg(recipe()->bitterness(), 0, 'f', 0)));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("Color: %1 SRM").arg(recipe()->color(), 0, 'f', 1)));
    element.appendChild(xhtml.createElement("br"));
    element.appendChild(xhtml.createTextNode(QString("ABV: %1%").arg(recipe()->alcoholByVolume() * 100, 0, 'f', 1)));
    element.appendChild(xhtml.createElement("br"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("h3");
    element.appendChild(xhtml.createTextNode("Ingredients"));
    bodyElement.appendChild(element);

    element = xhtml.createElement("p");
    foreach(RecipeIngredient *recipeIngredient, recipe()->ingredients()) {

        QString ingredientText = QString("%1 - %2").arg(recipeIngredient->name()).arg(recipeIngredient->quantity().toString());
        if(recipeIngredient->minutes() > 0)
            ingredientText += QString(" @ %1 minutes").arg(recipeIngredient->minutes(), 0, 'f', 0);

        element.appendChild(xhtml.createTextNode(ingredientText));
        element.appendChild(xhtml.createElement("br"));
    }
    bodyElement.appendChild(element);

    document.setHtml(xhtml.toString());
    document.print(printer);
}

void RecipeWidget::recipeChanged()
{
    refreshText(true);
    emit changed();
}

void RecipeWidget::refreshText(bool modified)
{
    if(isWindowModified() || modified) {
        setWindowTitle(_recipe->name().append(" [*]"));
        setWindowModified(true);
    } else {
        this->setWindowTitle(_recipe->name());
    }

    ui->txtName->setText(_recipe->name());
    ui->cmbStyle->setCurrentIndex(ui->cmbStyle->findText(_recipe->style()));
    ui->spnVolume->setValue(_recipe->volume().valueToGallon());
    ui->spnBoilTime->setValue(_recipe->boilTime());
    ui->spnEfficiency->setValue(_recipe->efficiency() * 100);

    ui->tblCalculated->resizeColumnsToContents();
    ui->tblCalculated->resizeRowsToContents();
}

void RecipeWidget::on_txtName_editingFinished()
{
    QString name = ui->txtName->text();
    if(_recipe->name() != name) {
        _recipe->setName(name);
    } else {
        refreshText();
    }
}

void RecipeWidget::on_cmbStyle_currentIndexChanged(QString style)
{
    if(_recipe->style() != style) {
        _recipe->setStyle(style);
    }
}

void RecipeWidget::on_spnVolume_editingFinished()
{
    double volume = ui->spnVolume->value();
    if(_recipe->volume().valueToGallon() != volume) {
        _recipe->setVolume(volume);
    } else {
        refreshText();
    }
}

void RecipeWidget::on_spnEfficiency_editingFinished()
{
    double efficiency = ui->spnEfficiency->value() / 100;
    if(_recipe->efficiency() != efficiency) {
        _recipe->setEfficiency(efficiency);
    } else {
        refreshText();
    }
}

void RecipeWidget::on_btnRaise_clicked()
{
    if(!ui->trvIngredients->hasFocus())
        return;

    QModelIndexList modelIndexes = ui->trvIngredients->selectionModel()->selectedRows();
    if(modelIndexes.count() <= 0) {
        return;
    }

    RecipeIngredientModel *ingredientModel = qobject_cast<RecipeIngredientModel *>(ui->trvIngredients->model());

    /* The selection changes as we change the model, so we have to cache it, in the order we need to use it
       before actually changing anything in the model */
    QModelIndexList cache;
    foreach(QModelIndex index, modelIndexes)
        cache.push_back(index);

    foreach(QModelIndex modelIndex, cache) {
        int index = modelIndex.row();
        if(index <= 0) break;
        ingredientModel->move(index, index-1);
    }

}

void RecipeWidget::on_btnLower_clicked()
{
    if(!ui->trvIngredients->hasFocus())
        return;

    QModelIndexList modelIndexes = ui->trvIngredients->selectionModel()->selectedRows();
    if(modelIndexes.count() <= 0) {
        return;
    }

    RecipeIngredientModel *ingredientModel = qobject_cast<RecipeIngredientModel *>(ui->trvIngredients->model());

    /* The selection changes as we change the model, so we have to cache it, in the order we need to use it
       before actually changing anything in the model */
    QModelIndexList cache;
    foreach(QModelIndex index, modelIndexes)
        cache.push_front(index);

    foreach(QModelIndex modelIndex, cache) {
        int index = modelIndex.row();
        if(index >= ingredientModel->rowCount()-1) break;
        ingredientModel->move(index, index+1);
    }
}

void RecipeWidget::on_btnRemove_clicked()
{
    if(!ui->trvIngredients->hasFocus())
        return;

    QModelIndexList modelIndexes = ui->trvIngredients->selectionModel()->selectedRows();
    if(modelIndexes.count() <= 0) {
        return;
    }

    MainWindow::instance()->showNotification(
                tr("Are you sure that you want to remove the selected ingredients from the recipe?"),
                QIcon(),
                true,
                QDialogButtonBox::Yes,
                this, SLOT(removeSelectedIngredients(QDialogButtonBox::StandardButton)));
}

void RecipeWidget::removeSelectedIngredients(QDialogButtonBox::StandardButton standardButton)
{
    if(standardButton != QDialogButtonBox::Yes)
        return;

    QModelIndexList modelIndexes = ui->trvIngredients->selectionModel()->selectedRows();
    RecipeIngredientModel *ingredientModel = qobject_cast<RecipeIngredientModel *>(ui->trvIngredients->model());

    /* The selection changes as we change the model, so we have to cache it, in the order we need to use it
       before actually changing anything in the model */
    QModelIndexList cache;
    foreach(QModelIndex index, modelIndexes)
        cache.push_front(index);

    foreach(QModelIndex modelIndex, cache) {
        ingredientModel->remove(modelIndex.row());
    }
}


