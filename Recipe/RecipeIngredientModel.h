#ifndef RECIPEINGREDIENTMODEL_H
#define RECIPEINGREDIENTMODEL_H

#include <QAbstractItemModel>
#include <QIcon>
#include "Recipe/Recipe.h"

class RecipeIngredientModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RecipeIngredientModel(QObject *parent = 0);

    Recipe *recipe() { return _recipe; }
    void setRecipe(Recipe *recipe);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void addIngredient(RecipeIngredient *ingredient);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

protected slots:
    void addingIngredient();
    void addedIngredient();
    void removingIngredient(int);
    void removedIngredient(int);

protected:
    Recipe *_recipe;

};

#endif // RECIPEINGREDIENTMODEL_H
