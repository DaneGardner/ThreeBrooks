/*!
   \file RecipeModel.cpp
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

#include "RecipeModel.h"

RecipeModel::RecipeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QModelIndex RecipeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    if(!parent.isValid()) {
        return createIndex(row, column, quint32(row));
    }

    return QModelIndex();
}

QModelIndex RecipeModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int RecipeModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) {
        return 5;
    }

    return 0;
}

int RecipeModel::columnCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) {
        return 3;
    }

    return 0;
}

QVariant RecipeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole && index.internalId() < 5) {
        if(index.column() == 0) {
            switch(index.row()) {
            case 0:
                return QVariant(QString().setNum(recipe()->originalGravity(), 'f', 3));
            case 1:
                return QVariant(QString().setNum(recipe()->finalGravity(), 'f', 3));
            case 2:
                return QVariant(QString().setNum(recipe()->bitterness(), 'f', 0));
            case 3:
                return QVariant(QString().setNum(recipe()->color(), 'f', 1));
            case 4:
                return QVariant(QString().setNum(recipe()->alcoholByVolume() * 100, 'f', 1));
            }
        } else {
            //TODO: Style high and low
            switch(index.row()) {
            case 0:
                if(index.column() == 1) return QVariant(QString().setNum(1.000, 'f', 3));
                if(index.column() == 2) return QVariant(QString().setNum(1.100, 'f', 3));
            case 1:
                if(index.column() == 1) return QVariant(QString().setNum(1.000, 'f', 3));
                if(index.column() == 2) return QVariant(QString().setNum(1.100, 'f', 3));
            case 2:
                if(index.column() == 1) return QVariant(QString().setNum(0.0, 'f', 0));
                if(index.column() == 2) return QVariant(QString().setNum(200.0, 'f', 0));
            case 3:
                if(index.column() == 1) return QVariant(QString().setNum(1.0, 'f', 1));
                if(index.column() == 2) return QVariant(QString().setNum(100.0, 'f', 1));
            case 4:
                if(index.column() == 1) return QVariant(QString().setNum(0.0, 'f', 1));
                if(index.column() == 2) return QVariant(QString().setNum(15.0, 'f', 1));
            }
        }
    }

    //TODO: SRM Color (or maybe a delegate?)
//    if(role == Qt::DecorationRole) {
//        if(index.column() == 0) {
//        }
//        return QVariant();
//    }

    return QVariant();
}

QVariant RecipeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical && role == Qt::DisplayRole) {
        switch(section) {
        case 0:
            return QVariant(tr("Original Gravity (SG)"));
        case 1:
            return QVariant(tr("Final Gravity (SG)"));
        case 2:
            return QVariant(tr("Bitterness (IBU)"));
        case 3:
            return QVariant(tr("Color (SRM)"));
        case 4:
            return QVariant(tr("Alcohol by Volume"));
        }
    }

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0:
            return QVariant(tr("Calculated"));
        case 1:
            return QVariant(tr("Style Low"));
        case 2:
            return QVariant(tr("Style High"));
        }
    }

    return QVariant();
}

void RecipeModel::recipeChanged()
{
    emit dataChanged(index(0,0), index(rowCount(),columnCount()));
}


