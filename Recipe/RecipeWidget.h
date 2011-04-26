#ifndef RECIPEWIDGET_H
#define RECIPEWIDGET_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDomDocument>
#include <QFile>
#include <QSettings>
#include <QAbstractProxyModel>

#include "Recipe.h"
#include "RecipeModel.h"
#include "RecipeIngredientModel.h"
#include "RecipeIngredientDelegate.h"

namespace Ui {
    class RecipeWidget;
}

class RecipeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecipeWidget(QWidget *parent = 0);
    explicit RecipeWidget(QString filepath, QWidget *parent = 0);
    ~RecipeWidget();

    Recipe *recipe() { return _recipe; }

    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent *);

    void save(QString filepath = QString());

signals:
    void changed();

protected slots:
    void recipeChanged();
    void refreshText(bool modified = false);
    void on_txtName_editingFinished();
    void on_cmbStyle_currentIndexChanged(QString style);
    void on_spnVolume_editingFinished();
    void on_spnEfficiency_editingFinished();

protected:
    void setRecipe(Recipe *recipe);
    void load(QString filepath);
    Recipe *_recipe;

private:
    Ui::RecipeWidget *ui;
};

#endif // RECIPEWIDGET_H
