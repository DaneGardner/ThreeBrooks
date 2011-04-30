/*!
   \file IngredientToolbox.cpp
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

#include "IngredientToolbox.h"
#include "ui_IngredientToolbox.h"

#include "MainWindow.h"

IngredientToolbox::IngredientToolbox(QString filepath, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::IngredientToolbox)
{
    _filepath = filepath;
    ui->setupUi(this);

    QSortFilterProxyModel *proxy;

    _grainIngredientModel = new IngredientModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterRole(Qt::EditRole);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy->setSortRole(Qt::EditRole);
    proxy->setSourceModel(_grainIngredientModel);
    proxy->sort(0);
    ui->grainView->setModel(proxy);
    connect(ui->grainView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(editIngredient(QModelIndex)));

    _hopsIngredientModel = new IngredientModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterRole(Qt::EditRole);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy->setSortRole(Qt::EditRole);
    proxy->setSourceModel(_hopsIngredientModel);
    proxy->sort(0);
    ui->hopsView->setModel(proxy);
    connect(ui->hopsView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(editIngredient(QModelIndex)));

    _yeastIngredientModel = new IngredientModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterRole(Qt::EditRole);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy->setSortRole(Qt::EditRole);
    proxy->setSourceModel(_yeastIngredientModel);
    proxy->sort(0);
    ui->yeastView->setModel(proxy);
    connect(ui->yeastView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(editIngredient(QModelIndex)));

    _otherIngredientModel = new IngredientModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterRole(Qt::EditRole);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy->setSortRole(Qt::EditRole);
    proxy->setSourceModel(_otherIngredientModel);
    proxy->sort(0);
    ui->otherView->setModel(proxy);
    connect(ui->otherView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(editIngredient(QModelIndex)));

    load();
    _ingredientsChanged = false;
}

IngredientToolbox::~IngredientToolbox()
{
    if(_ingredientsChanged) {
        save();
    }

    delete ui;
}

void IngredientToolbox::load(QString filepath)
{
    if(filepath.isEmpty())
        filepath = _filepath;

    QDomDocument document("Ingredients");
    QFile file(filepath);

    if(!file.open(QIODevice::ReadOnly))
        throw new QString("Failed to open ingredients file");

    if(!document.setContent(&file)) {
        file.close();
        throw new QString("Failed to read ingredients file");
    }

    file.close();

    QDomElement baseElement = document.documentElement();
    QDomElement element = baseElement.firstChildElement();
    while(!element.isNull()) {
        Ingredient *ingredient = Ingredient::createIngredient(element);
        insert(ingredient);
        element = element.nextSiblingElement();
    }

    on_ToolBox_currentChanged(0);   // Forces a sort of the newly loaded page
}

void IngredientToolbox::save(QString filepath)
{
    if(filepath.isEmpty())
        filepath = _filepath;

    QDomDocument document("Ingredients");
    QDomElement baseElement = document.createElement("Ingredients");
    document.appendChild(baseElement);

    for(int i=0; i<_ingredients.count(); i++) {
        baseElement.appendChild(_ingredients.at(i)->toXml(document));
    }

    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly)) {
        throw new QString("Failed to open ingredients file");
    }

    file.write(document.toString().toLatin1());
    file.flush();
    file.close();
}

void IngredientToolbox::ingredientChanged()
{
    _ingredientsChanged = true;
}

void IngredientToolbox::editIngredient(QModelIndex index)
{
    Ingredient *ingredient = index.model()->data(index, Qt::UserRole).value<Ingredient *>();
    if(ingredient) {
        IngredientEditDialog dlg(ingredient, this);
        dlg.exec();
    }
}

void IngredientToolbox::on_ToolBox_currentChanged(int page)
{
    Q_UNUSED(page)

    QListView *listView = qobject_cast<QListView *>(ui->ToolBox->currentWidget());
    QSortFilterProxyModel *proxy = qobject_cast<QSortFilterProxyModel *>(listView->model());
    proxy->sort(0);
}

void IngredientToolbox::insert(Ingredient *ingredient, bool sort)
{
    connect(ingredient, SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _ingredients.append(ingredient);
    if(qobject_cast<GrainIngredient *>(ingredient)) {
        _grainIngredientModel->append(ingredient);
    } else if(qobject_cast<HopsIngredient *>(ingredient)) {
        _hopsIngredientModel->append(ingredient);
    } else if(qobject_cast<YeastIngredient *>(ingredient)) {
        _yeastIngredientModel->append(ingredient);
    } else {
        _otherIngredientModel->append(ingredient);
    }
    ingredientChanged();
}

void IngredientToolbox::on_btnNew_clicked()
{
    IngredientEditDialog dlg(NULL, this);
    if(dlg.exec() == QDialog::Accepted) {
        Ingredient *ingredient = dlg.ingredient();
        if(ingredient) {
            insert(ingredient);
        }
    }

}

void IngredientToolbox::on_btnRemove_clicked()
{
    QListView *listView = qobject_cast<QListView *>(ui->ToolBox->currentWidget());
    QModelIndexList indices = listView->selectionModel()->selectedIndexes();

    if(indices.count() <= 0)
        return;

    MainWindow::instance()->showNotification(tr("Are you sure you want to permanently delete the selected ingredients?"),
                                             QIcon(), true,
                                             QDialogButtonBox::Yes,
                                             this, SLOT(removeSelected(QDialogButtonBox::StandardButton)));
}

void IngredientToolbox::removeSelected(QDialogButtonBox::StandardButton standardButton)
{
    if(standardButton != QDialogButtonBox::Yes)
        return;

    QListView *listView = qobject_cast<QListView *>(ui->ToolBox->currentWidget());
    QSortFilterProxyModel *model = qobject_cast<QSortFilterProxyModel *>(listView->model());
    IngredientModel *ingredientModel = qobject_cast<IngredientModel *>(model->sourceModel());
    QModelIndexList indices = listView->selectionModel()->selectedIndexes();

    // The indices list changes as items are removed, so we have to cache the pointers locally first
    QList<Ingredient *> ingredients;
    foreach(QModelIndex modelIndex, indices) {
        ingredients.append(modelIndex.model()->data(modelIndex, Qt::UserRole).value<Ingredient *>());
    }

    // Now we can remove them
    for(int i=0; i < ingredients.count(); i++) {
        ingredientModel->remove(ingredients.at(i));
        _ingredients.removeOne(ingredients.at(i));
        ingredientChanged();

        //FIXME: We can't delete the ingredient memory because it might be used elsewhere. Gotta solve this
    }
}


void IngredientToolbox::on_txtFilter_textChanged(QString text)
{
    for(int i=0; i < ui->ToolBox->count(); i++) {
        QListView *listView = qobject_cast<QListView *>(ui->ToolBox->widget(i));
        QSortFilterProxyModel *proxy = qobject_cast<QSortFilterProxyModel *>(listView->model());
        proxy->setFilterRegExp(text);
    }
}


