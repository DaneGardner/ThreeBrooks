/*!
   \file RecipeIngredientDelegate.cpp
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

#include "RecipeIngredientDelegate.h"

#include <QDebug>

RecipeIngredientDelegate::RecipeIngredientDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *RecipeIngredientDelegate::createEditor(QWidget *parent,
                                                const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QWidget *widget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    widget->setLayout(layout);
    widget->setAutoFillBackground(true);
    widget->setMinimumWidth(150);

    layout->setMargin(0);
    layout->setSpacing(0);

    QString styleSheet("QLineEdit { border: 1px solid #999999; border-radius: 2px; padding: 0px 1px 1px 1px; min-width: 2em; max-width: 4em; }"

                       "QComboBox { border: 1px solid #999999; border-radius: 2px; padding: 0px 1px 0px 1px; min-width: 2em; max-width: 4em; }"
                       "QComboBox::drop-down { background-color: #cccccc; border: 1px solid #999999; width:2px }"

                       "QAbstractScrollArea { border: 1px solid #999999; border-radius: 2px; padding: 1px; min-width: 3em; }"

                       "QScrollBar { border: none; width: 3px; }"
                       "QScrollBar::handle { border: 1px solid #999999; }"
                       "QScrollBar::add-line { border: none; width: 0px; height: 0px; }"
                       "QScrollBar::sub-line { border: none; width: 0px; height: 0px; }"
                       );
    widget->setStyleSheet(styleSheet);

    QLineEdit *txtQuantity = new QLineEdit(widget);
    txtQuantity->setObjectName("txtQuantity");
    txtQuantity->setFrame(false);
    txtQuantity->setStyleSheet("min-width: 3em;");
    layout->addWidget(txtQuantity);

    QComboBox *cmbQuantityType = new QComboBox(widget);
    cmbQuantityType->setObjectName("cmbQuantityType");
    cmbQuantityType->setMinimumWidth(50);
    cmbQuantityType->setFrame(false);
    cmbQuantityType->addItems(Quantity::typeNames());
    layout->addWidget(cmbQuantityType);

    QLabel *lblAt = new QLabel(widget);
    lblAt->setText(tr("@"));
    lblAt->setStyleSheet("margin-left: 10px");
    layout->addWidget(lblAt);

    QLineEdit *txtMinutes = new QLineEdit(widget);
    txtMinutes->setObjectName("txtMinutes");
    txtMinutes->setFrame(false);
    layout->addWidget(txtMinutes);

    QLabel *lblMinutes = new QLabel(widget);
    lblMinutes->setText(tr("mins"));
    lblMinutes->setStyleSheet("border: 1px solid #999999; border-radius: 2px;");
    layout->addWidget(lblMinutes);

    layout->addStretch();

    return widget;
}

void RecipeIngredientDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *txtQuantity      = editor->findChild<QLineEdit *>("txtQuantity");
    QComboBox *cmbQuantityType  = editor->findChild<QComboBox *>("cmbQuantityType");
    QLineEdit *txtMinutes       = editor->findChild<QLineEdit *>("txtMinutes");

    RecipeIngredient *ingredient = index.model()->data(index, Qt::UserRole).value<RecipeIngredient *>();
    txtQuantity->setText(QString().setNum(ingredient->quantity().value(), 'f', 2));
    int type = cmbQuantityType->findText(ingredient->quantity().typeName());
    cmbQuantityType->setCurrentIndex(type);
    txtMinutes->setText(QString().setNum(ingredient->minutes(), 'f', 1));
}

void RecipeIngredientDelegate::setModelData(QWidget *editor,
                                            QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    Q_UNUSED(model)

    QLineEdit *txtQuantity      = editor->findChild<QLineEdit *>("txtQuantity");
    QComboBox *cmbQuantityType  = editor->findChild<QComboBox *>("cmbQuantityType");
    QLineEdit *txtMinutes       = editor->findChild<QLineEdit *>("txtMinutes");

    RecipeIngredient *ingredient = index.model()->data(index, Qt::UserRole).value<RecipeIngredient *>();
    Quantity quantity(txtQuantity->text().toDouble(), Quantity::type(cmbQuantityType->currentText()));
    ingredient->setQuantityAndMinutes(quantity, txtMinutes->text().toDouble());
}

void RecipeIngredientDelegate::updateEditorGeometry(QWidget *editor,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}
