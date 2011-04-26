#include "IngredientModel.h"

IngredientModel::IngredientModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}

/* QAbstractItemModel interface */
QModelIndex IngredientModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    Ingredient *ingredient = NULL;
    ingredient = _ingredients.at(row);

    if(!ingredient) {
        return QModelIndex();
    }

    return createIndex(row, column, ingredient);
}

QModelIndex IngredientModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int IngredientModel::rowCount(const QModelIndex &parent) const
{
    if(!parent.isValid()) {
        return _ingredients.count();
    }

    return 0;
}

int IngredientModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 10;
}

QVariant IngredientModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    Ingredient *ingredient = static_cast<Ingredient *>(index.internalPointer());

    if(role == Qt::UserRole) {
        return qVariantFromValue(ingredient);
    }

    if(role == Qt::DisplayRole) {
        if(index.column() == 0) {
            GrainIngredient *grainIngredient = qobject_cast<GrainIngredient *>(ingredient);
            if(grainIngredient) {
                return QVariant(QString("%1 [%2][%3 SRM]").arg(grainIngredient->name())
                                         .arg(grainIngredient->specificGravity())
                                         .arg(grainIngredient->color()));
            }
            HopsIngredient *hopsIngredient = qobject_cast<HopsIngredient *>(ingredient);
            if(hopsIngredient) {
                return QVariant(QString("%1 [%2%]").arg(hopsIngredient->name())
                                         .arg(hopsIngredient->alphaAcid()*100));
            }
            return QVariant(ingredient->name());
        } else if(index.column() == 1) {
            return QVariant(ingredient->type());
        } else {
            return QVariant();
        }
    }

    // For auto-completer
    if(role == Qt::EditRole) {
        if(index.column() == 0) {
            return QVariant(ingredient->name());
        }
    }

    if(role == Qt::DecorationRole) {
        if(index.column() == 0) {
            QString type = ingredient->type().toLower();
            if(type == "grain") {
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

Qt::ItemFlags IngredientModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsDragEnabled;

    return flags;
}


/* QList operators */
int IngredientModel::row(Ingredient *ingredient) const
{
    return _ingredients.indexOf(ingredient);
}

Ingredient *IngredientModel::ingredient(int row) const
{
    return _ingredients.at(row);
}

void IngredientModel::prepend(Ingredient *ingredient)
{
    insert(0, ingredient);
}

void IngredientModel::append(Ingredient *ingredient)
{
    insert(_ingredients.count(), ingredient);
}

void IngredientModel::insert(int row, Ingredient *ingredient)
{
    beginInsertRows(QModelIndex(), row, row+1);
    connect(ingredient, SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _ingredients.insert(row, ingredient);
    endInsertRows();
}

void IngredientModel::remove(Ingredient *ingredient)
{
    if(_ingredients.contains(ingredient));
    remove(row(ingredient));
}

void IngredientModel::remove(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    disconnect(ingredient(row), SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _ingredients.removeAt(row);
    endRemoveRows();
}

void IngredientModel::move(Ingredient *ingredient, int destination)
{
    move(row(ingredient), destination);
}

void IngredientModel::move(int row, int destination)
{
    beginMoveRows(QModelIndex(), row, row+1, QModelIndex(), destination);
    _ingredients.move(row, destination);
    endMoveRows();
}

void IngredientModel::ingredientChanged()
{
    int row = this->row(qobject_cast<Ingredient *>(QObject::sender()));
    dataChanged(index(row, 0), index(row, columnCount()));
}


