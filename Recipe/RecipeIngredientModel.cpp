/*!
   \file RecipeIngredientModel.cpp
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

#include "RecipeIngredientModel.h"

RecipeIngredientModel::RecipeIngredientModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    _recipe = NULL;
}

void RecipeIngredientModel::setRecipe(Recipe *recipe)
{
    if(_recipe) {
        disconnect(_recipe, SIGNAL(addingIngredient()), this, SLOT(addingIngredient()));
        disconnect(_recipe, SIGNAL(addedIngredient()), this, SLOT(addedIngredient()));
        disconnect(_recipe, SIGNAL(removingIngredient(int)), this, SLOT(removingIngredient(int)));
        disconnect(_recipe, SIGNAL(removedIngredient(int)), this, SLOT(removedIngredient(int)));
    }

    _recipe = recipe;

    connect(_recipe, SIGNAL(addingIngredient()), this, SLOT(addingIngredient()));
    connect(_recipe, SIGNAL(addedIngredient()), this, SLOT(addedIngredient()));
    connect(_recipe, SIGNAL(removingIngredient(int)), this, SLOT(removingIngredient(int)));
    connect(_recipe, SIGNAL(removedIngredient(int)), this, SLOT(removedIngredient(int)));
}

void RecipeIngredientModel::addingIngredient()
{
    emit beginInsertRows(QModelIndex(), rowCount(), rowCount());
}

void RecipeIngredientModel::addedIngredient()
{
    emit endInsertRows();
}

void RecipeIngredientModel::removingIngredient(int row)
{
    emit beginRemoveRows(QModelIndex(), row, row+1);
}

void RecipeIngredientModel::removedIngredient(int row)
{
    emit endRemoveRows();
}


/* END QAbstractItemModel operators */
QModelIndex RecipeIngredientModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    RecipeIngredient *ingredient;
    ingredient = _recipe->ingredients().at(row);
    if(ingredient)
        return createIndex(row, column, ingredient);

    return QModelIndex();
}

QModelIndex RecipeIngredientModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int RecipeIngredientModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid())
        return _recipe->ingredients().count();
    return 0;
}

int RecipeIngredientModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

QVariant RecipeIngredientModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    RecipeIngredient *ingredient = static_cast<RecipeIngredient *>(index.internalPointer());

    if(role == Qt::DisplayRole) {
        if(ingredient) {
            switch(index.column()) {
            case 0:
                return QVariant(ingredient->type());
            case 1:
                return QVariant(ingredient->name());
            case 2:
                if(ingredient->minutes() > 0)
                    return QVariant(ingredient->quantity().toString().append(QString(" @ %1 mins").arg(ingredient->minutes())));
                return QVariant(ingredient->quantity().toString());
            default:
                return QVariant();
            }
        }
    }

    if(role == Qt::DecorationRole) {
        if(index.column() == 0) {
            QString type = ingredient->type().toLower();
            if(type == "sugar") {
                return QVariant(QIcon(""));
            } else if(type == "extract") {
                    return QVariant(QIcon(""));
            } else if(type == "grain") {
                    return QVariant(QIcon(""));
            } else if(type == "hops") {
                    return QVariant(QIcon(""));
            } else if(type == "yeast") {
                    return QVariant(QIcon(""));
            } else {
                return QVariant(QIcon());
            }
        }
        return QVariant();
    }


    return QVariant();
}

QVariant RecipeIngredientModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section == 0)
            return QVariant(tr("Type"));
        else if(section == 1)
            return QVariant(tr("Name"));
        else if(section == 2)
            return QVariant(tr("Quantity"));
        else if(section == 3)
            return QVariant(tr("Time"));
    }

    return QVariant();
}

bool RecipeIngredientModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
        return false;

    RecipeIngredient *ingredient = static_cast<RecipeIngredient *>(index.internalPointer());

    if(index.column() == 2) {
        ingredient->quantity().setValue(value.toDouble());
        emit dataChanged(index, index);
        return true;
    } else if(index.column() == 3) {
        ingredient->setMinutes(value.toDouble());
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags RecipeIngredientModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if(index.column() >= 2 && index.column() <= 3)
        flags |= Qt::ItemIsEditable;

    flags |= Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    return flags;
}

Qt::DropActions RecipeIngredientModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

bool RecipeIngredientModel::removeRows(int row, int count, const QModelIndex &parent)
{
    for(int i=0; i < count; i++) {
        remove(row);
    }

    return true;
}
/* END QAbstractItemModel operators */


/* QList operators */
int RecipeIngredientModel::row(RecipeIngredient *ingredient) const
{
    return _recipe->indexOf(ingredient);
}

RecipeIngredient *RecipeIngredientModel::ingredient(int row) const
{
    return _recipe->at(row);
}

void RecipeIngredientModel::prepend(Ingredient *ingredient)
{
    insert(0, ingredient);
}

void RecipeIngredientModel::append(Ingredient *ingredient)
{
    insert(_recipe->count(), ingredient);
}

void RecipeIngredientModel::insert(int row, Ingredient *ingredient)
{
    insert(row, new RecipeIngredient(ingredient, _recipe));
}

void RecipeIngredientModel::prepend(RecipeIngredient *ingredient)
{
    insert(0, ingredient);
}

void RecipeIngredientModel::append(RecipeIngredient *ingredient)
{
    insert(_recipe->count(), ingredient);
}

void RecipeIngredientModel::insert(int row, RecipeIngredient *ingredient)
{
    beginInsertRows(QModelIndex(), row, row+1);
    connect(ingredient, SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _recipe->insert(row, ingredient);
    endInsertRows();
}

void RecipeIngredientModel::remove(RecipeIngredient *ingredient)
{
    if(_recipe->contains(ingredient));
        remove(row(ingredient));
}

void RecipeIngredientModel::remove(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    disconnect(ingredient(row), SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _recipe->removeAt(row);
    endRemoveRows();
}

void RecipeIngredientModel::move(RecipeIngredient *ingredient, int destination)
{
    move(row(ingredient), destination);
}

void RecipeIngredientModel::move(int row, int destination)
{
    /* QList::move and QAbstractItemModel::beginMoveRows differ in how they describe the
       destination. We have to do a little translation if it's going up or down. */
    if(row < destination) {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), destination+1);
    } else {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), destination);
    }

    _recipe->move(row, destination);
    endMoveRows();
}
/* END QList operators */

void RecipeIngredientModel::ingredientChanged()
{
    int row = this->row(qobject_cast<RecipeIngredient *>(QObject::sender()));
    dataChanged(index(row, 0), index(row, columnCount()));
}

