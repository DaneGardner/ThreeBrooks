/*!
   \file IngredientEditDialog.h
   \author Dane Gardner <dane.gardner@gmail.com>
   \version 

   \section LICENSE
    This file is part of the ThreeBrooks homebrew recipe application
    Copyright (C) 2011-2013 Dane Gardner

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
