#ifndef INGREDIENTEDITDIALOG_H
#define INGREDIENTEDITDIALOG_H

#include <QDialog>
#include <QSettings>

#include "Ingredient.h"
#include "GrainIngredient.h"
#include "HopsIngredient.h"
#include "YeastIngredient.h"

namespace Ui {
    class IngredientEditDialog;
}

class IngredientEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IngredientEditDialog(Ingredient *ingredient = NULL, QWidget *parent = 0);
    ~IngredientEditDialog();

    Ingredient *ingredient();

protected:
    void initialize();
    void finalize();
    Ingredient *_ingredient;

protected slots:
    virtual void accept();
    void on_cmbType_currentIndexChanged(const QString &text);

private:
    Ui::IngredientEditDialog *ui;

};

#endif // INGREDIENTEDITDIALOG_H
