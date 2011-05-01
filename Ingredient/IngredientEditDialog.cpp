/*!
   \file IngredientEditDialog.cpp
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

#include "IngredientEditDialog.h"
#include "ui_IngredientEditDialog.h"

#include "MainWindow.h"

IngredientEditDialog::IngredientEditDialog(Ingredient *ingredient, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IngredientEditDialog)
{
    ui->setupUi(this);

    ui->cmbType->addItem("Grain");
    ui->cmbType->addItem("Hops");
    ui->cmbType->addItem("Yeast");
    ui->cmbType->addItem("Other");

    ui->grpGrain->hide();
    ui->grpHops->hide();
    ui->grpYeast->hide();

    if(!(_ingredient = ingredient)) {
        ui->cmbType->setEnabled(true);
        ui->cmbType->setCurrentIndex(3);
        ui->txtId->setVisible(false);
        ui->lblId->setVisible(false);
    } else {
        initialize();
    }

    QSettings settings("settings.ini", QSettings::IniFormat);
    restoreGeometry(settings.value("IngredientEditDialog/geometry", saveGeometry()).toByteArray());
    move(MainWindow::instance()->pos() + settings.value("IngredientEditDialog/position", QPoint(50,50)).toPoint());
}

IngredientEditDialog::~IngredientEditDialog()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("IngredientEditDialog/geometry", saveGeometry());
    settings.setValue("IngredientEditDialog/position", pos() - MainWindow::instance()->pos());

    delete ui;
}

void IngredientEditDialog::initialize()
{
    ui->txtName->setText(_ingredient->name());
    ui->cmbType->setCurrentIndex(ui->cmbType->findText(_ingredient->type()));
    ui->txtId->setText(_ingredient->id().toString());

    GrainIngredient *grainIngredient = qobject_cast<GrainIngredient *>(_ingredient);
    if(grainIngredient) {
        ui->grpGrain->show();
        ui->spnColor->setValue(grainIngredient->color());
        ui->spnSpecificGravity->setValue(grainIngredient->specificGravity());
        ui->chkExtract->setChecked(grainIngredient->extract());
        return;
    }

    HopsIngredient *hopsIngredient = qobject_cast<HopsIngredient *>(_ingredient);
    if(hopsIngredient) {
        ui->grpHops->show();
        ui->spnAlphaAcid->setValue(hopsIngredient->alphaAcid() * 100);
        return;
    }

    YeastIngredient *yeastIngredient = qobject_cast<YeastIngredient *>(_ingredient);
    if(yeastIngredient) {
        ui->grpYeast->show();
        ui->spnAttenuation->setValue(yeastIngredient->attenuation() * 100);
        return;
    }
}

void IngredientEditDialog::finalize()
{
    if(!_ingredient) {
        if(ui->cmbType->currentText() == tr("Grain")) {
            _ingredient = new GrainIngredient();
        } else if(ui->cmbType->currentText() == tr("Hops")) {
            _ingredient = new HopsIngredient();
        } else if(ui->cmbType->currentText() == tr("Yeast")) {
            _ingredient = new YeastIngredient();
        } else {
            _ingredient = new Ingredient();
        }
    }

    _ingredient->setName(ui->txtName->text());

    GrainIngredient *grainIngredient = qobject_cast<GrainIngredient *>(_ingredient);
    if(grainIngredient) {
        grainIngredient->setColor(ui->spnColor->value());
        grainIngredient->setSpecificGravity(ui->spnSpecificGravity->value());
        grainIngredient->setExtract(ui->chkExtract->isChecked());
        return;
    }

    HopsIngredient *hopsIngredient = qobject_cast<HopsIngredient *>(_ingredient);
    if(hopsIngredient) {
        hopsIngredient->setAlphaAcid(ui->spnAlphaAcid->value() / 100);
        return;
    }

    YeastIngredient *yeastIngredient = qobject_cast<YeastIngredient *>(_ingredient);
    if(yeastIngredient) {
        yeastIngredient->setAttenuation(ui->spnAttenuation->value() / 100);
        return;
    }

}

Ingredient *IngredientEditDialog::ingredient()
{
    return _ingredient;
}

void IngredientEditDialog::accept()
{
    finalize();
    QDialog::accept();
}

void IngredientEditDialog::on_cmbType_currentIndexChanged(const QString &text)
{
    ui->grpGrain->hide();
    ui->grpHops->hide();
    ui->grpYeast->hide();

    if(text == tr("Grain")) {
        ui->grpGrain->show();
    } else if(text == tr("Hops")) {
        ui->grpHops->show();
    } else if(text == tr("Yeast")) {
        ui->grpYeast->show();
    }
}


