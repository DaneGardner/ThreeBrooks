#ifndef INGREDIENTMODEL_H
#define INGREDIENTMODEL_H

#include <QAbstractItemModel>
#include <QVariant>
#include <QString>
#include <QIcon>

#include "Ingredient.h"
#include "GrainIngredient.h"
#include "HopsIngredient.h"
#include "YeastIngredient.h"

class IngredientModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit IngredientModel(QObject *parent = 0);

    /* QAbstractItemModel interface */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /* QList operations */
    int row(Ingredient *ingredient) const;
    Ingredient *ingredient(int row) const;
    void prepend(Ingredient *ingredient);
    void append(Ingredient *ingredient);
    void insert(int row, Ingredient *ingredient);
    void remove(Ingredient *ingredient);
    void remove(int row);
    void move(Ingredient *ingredient, int destination);
    void move(int row, int destination);

protected:
    QList<Ingredient *> _ingredients;

protected slots:
    void ingredientChanged();
};

#endif // INGREDIENTMODEL_H
