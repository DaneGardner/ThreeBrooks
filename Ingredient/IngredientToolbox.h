#ifndef INGREDIENTTOOLBOX_H
#define INGREDIENTTOOLBOX_H

#include <QWidget>
#include <QList>
#include <QListView>
#include <QCompleter>
#include <QMessageBox>
#include <QDomDocument>
#include <QFile>
#include <QSortFilterProxyModel>

#include "Ingredient.h"
#include "IngredientModel.h"
#include "IngredientEditDialog.h"

namespace Ui {
    class IngredientToolbox;
}

class IngredientToolbox : public QWidget
{
    Q_OBJECT

public:
    explicit IngredientToolbox(QString filepath = QString("ingredients.xml"), QWidget *parent = 0);
    ~IngredientToolbox();

protected:
    void load(QString filepath = QString());
    void save(QString filepath = QString());
    void insert(Ingredient *ingredient, bool sort = true);

    QString _filepath;
    bool _ingredientsChanged;

    QList<Ingredient *> _ingredients;
    IngredientModel *_grainIngredientModel;
    IngredientModel *_hopsIngredientModel;
    IngredientModel *_yeastIngredientModel;
    IngredientModel *_otherIngredientModel;
    QSortFilterProxyModel *_grainIngredientProxyModel;
    QSortFilterProxyModel *_hopsIngredientProxyModel;
    QSortFilterProxyModel *_yeastIngredientProxyModel;
    QSortFilterProxyModel *_otherIngredientProxyModel;

protected slots:
    void ingredientChanged();
    void editIngredient(QModelIndex index);
    void on_ToolBox_currentChanged(int page);
    void on_btnNew_clicked();
    void on_btnRemove_clicked();
    void on_txtFilter_textChanged(QString);

private:
    Ui::IngredientToolbox *ui;
};

#endif // INGREDIENTTOOLBOX_H
