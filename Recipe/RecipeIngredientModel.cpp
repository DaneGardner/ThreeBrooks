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
    _recipe = recipe;
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
    return 2;
}

QVariant RecipeIngredientModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    RecipeIngredient *ingredient = static_cast<RecipeIngredient *>(index.internalPointer());

    if(role == Qt::UserRole) {
        return qVariantFromValue(ingredient);
    }

    if(role == Qt::DisplayRole) {
        if(ingredient) {
            switch(index.column()) {
            case 0:
                return QVariant(ingredient->name());
            case 1:
                if(ingredient->minutes() > 0)
                    return QVariant(ingredient->quantity().toString().append(QString(" @ %1 mins").arg(ingredient->minutes())));
                return QVariant(ingredient->quantity().toString());
            default:
                return QVariant();
            }
        }
    }

    if(role == Qt::ToolTipRole) {
        if(index.column() == 0) {
            GrainIngredient *grainIngredient = qobject_cast<GrainIngredient *>(ingredient->ingredient());
            if(grainIngredient) {
                QString toolTip = QString("Potential Gravity: %1; Color: %2 SRM")
                        .arg(grainIngredient->specificGravity(), 3, 'f', 3)
                        .arg(grainIngredient->color(), 0, 'f', 1);

                if(grainIngredient->extract()) {
                    toolTip.prepend("Extract; ");
                }

                return QVariant(toolTip);
            }

            HopsIngredient *hopsIngredient = qobject_cast<HopsIngredient *>(ingredient->ingredient());
            if(hopsIngredient) {
                QString toolTip = QString("Alpha Acid: %1%")
                        .arg(hopsIngredient->alphaAcid() * 100, 1, 'f', 2);
                return QVariant(toolTip);
            }

            YeastIngredient *yeastIngredient = qobject_cast<YeastIngredient *>(ingredient->ingredient());
            if(yeastIngredient) {
                QString toolTip = QString("Attenuation: %1%")
                        .arg(yeastIngredient->attenuation() * 100, 1, 'f', 0);
                return QVariant(toolTip);
            }

        }

    }

    if(role == Qt::DecorationRole) {
        if(index.column() == 0) {
            QString type = ingredient->type().toLower();
            if(type == "grain") {
                return QVariant(QIcon(":/ThreeBrooks/Ingredients/grain.svg"));
            } else if(type == "hops") {
                    return QVariant(QIcon(":/ThreeBrooks/Ingredients/hops.svg"));
            } else if(type == "yeast") {
                    return QVariant(QIcon(":/ThreeBrooks/Ingredients/yeast.svg"));
            } else if(type == "ingredient") {
                    return QVariant(QIcon(":/ThreeBrooks/Ingredients/other.svg"));
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
            return QVariant(tr("Name"));
        else if(section == 1)
            return QVariant(tr("Quantity"));
    }

    return QVariant();
}

bool RecipeIngredientModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index)
    Q_UNUSED(value)

    if(role != Qt::EditRole)
        return false;

//    RecipeIngredient *ingredient = static_cast<RecipeIngredient *>(index.internalPointer());
//
//    if(index.column() == 1) {
//        ingredient->quantity().setValue(value.toDouble());
//        emit dataChanged(index, index);
//        return true;
//    } else if(index.column() == 2) {
//        ingredient->setMinutes(value.toDouble());
//        emit dataChanged(index, index);
//        return true;
//    }

    return false;
}

Qt::ItemFlags RecipeIngredientModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if(index.column() >= 1 && index.column() <= 2)
        flags |= Qt::ItemIsEditable;

    flags |= Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    return flags;
}

bool RecipeIngredientModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    for(int i=0; i < count; i++) {
        remove(row);
    }

    return true;
}

Qt::DropActions RecipeIngredientModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

QStringList RecipeIngredientModel::mimeTypes() const
{
    QStringList types = QAbstractItemModel::mimeTypes();
    types << "application/x-recipeingredientlist";
    return types;
}

QMimeData *RecipeIngredientModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *data = QAbstractItemModel::mimeData(indexes);

    QDomDocument document("RecipeIngredientList");
    document.appendChild(document.createElement("RecipeIngredients"));
    document.firstChildElement().setAttribute("applicationPid", QApplication::applicationPid());
    document.firstChildElement().setAttribute("recipeId", _recipe->id().toString());

    for(int i = indexes.count(); i > 0; i--) {
        QModelIndex modelIndex = indexes.at(i-1);
        if(modelIndex.column())
            continue;

        RecipeIngredient *recipeIngredient =
                this->data(modelIndex, Qt::UserRole).value<RecipeIngredient *>();
        document.firstChildElement().appendChild(recipeIngredient->toXml(document));
    }

    data->setData("application/x-recipeingredientlist", document.toByteArray());

    return data;
}

bool RecipeIngredientModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if(action == Qt::IgnoreAction)
        return true;

    if(row < 0) {
        if(parent.isValid()) {
            row = parent.row();
        } else {
            row = rowCount();
        }
    }

    if((action == Qt::CopyAction || action == Qt::MoveAction) && data->hasFormat("application/x-recipeingredientlist")) {
        QDomDocument document("RecipeIngredientList");
        document.setContent(data->data("application/x-recipeingredientlist"));
        if(document.isNull())
            return false;

        QDomElement parentElement = document.firstChildElement("RecipeIngredients");
        if(parentElement.isNull())
            return false;

        QDomElement element = parentElement.firstChildElement("RecipeIngredient");
        while(!element.isNull()) {
            this->insert(row, new RecipeIngredient(element, _recipe));
            element = element.nextSiblingElement();
        }

        return true;
    }

    if(action == Qt::CopyAction && data->hasFormat("application/x-ingredientlist")) {
        QDomDocument document("IngredientList");
        document.setContent(data->data("application/x-ingredientlist"));
        if(document.isNull())
            return false;

        QDomElement parentElement = document.firstChildElement("Ingredients");
        if(parentElement.isNull())
            return false;

        if(parentElement.attribute("applicationPid").toLongLong() != QApplication::applicationPid())
            return false;

        QDomElement element = parentElement.firstChildElement("Ingredient");
        while(!element.isNull()) {
            this->insert(row, Ingredient::createIngredient(element));
            element = element.nextSiblingElement();
        }

        return true;
    }

    return QAbstractItemModel::dropMimeData(data, action, row, column, parent);
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
    beginInsertRows(QModelIndex(), row, row);
    connect(ingredient, SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _recipe->insert(row, ingredient);
    endInsertRows();
}

void RecipeIngredientModel::remove(RecipeIngredient *ingredient)
{
    remove(row(ingredient));
}

void RecipeIngredientModel::remove(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    disconnect(ingredient(row), SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    RecipeIngredient *recipeIngredient = _recipe->at(row);
    _recipe->removeAt(row);
    delete recipeIngredient;
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

