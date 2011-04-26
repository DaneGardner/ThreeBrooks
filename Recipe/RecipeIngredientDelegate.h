#ifndef RECIPEINGREDIENTDELEGATE_H
#define RECIPEINGREDIENTDELEGATE_H

#include <QStyledItemDelegate>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QSignalMapper>

#include "Quantity.h"
#include "RecipeIngredient.h"

class RecipeIngredientDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RecipeIngredientDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

};

#endif // RECIPEINGREDIENTDELEGATE_H
