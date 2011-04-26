#ifndef RECIPEMODEL_H
#define RECIPEMODEL_H

#include <QAbstractItemModel>
#include "Recipe.h"

class RecipeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RecipeModel(QObject *parent = 0);

    Recipe *recipe() const { return _recipe; }
    void setRecipe(Recipe *recipe) { _recipe = recipe; connect(_recipe, SIGNAL(dataChanged()), this, SLOT(recipeChanged())); }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

protected:
    Recipe *_recipe;

protected slots:
    void recipeChanged();

};

#endif // RECIPEMODEL_H
