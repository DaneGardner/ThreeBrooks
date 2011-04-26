#include "RecipeIngredientDelegate.h"


RecipeIngredientDelegate::RecipeIngredientDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *RecipeIngredientDelegate::createEditor(QWidget *parent,
                                                const QStyleOptionViewItem &option,
                                                const QModelIndex &index) const
{
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
    QLineEdit *txtQuantity = editor->findChild<QLineEdit *>("txtQuantity");
    QComboBox *cmbQuantityType = editor->findChild<QComboBox *>("cmbQuantityType");
    QLineEdit *txtMinutes = editor->findChild<QLineEdit *>("txtMinutes");

    RecipeIngredient *ingredient = static_cast<RecipeIngredient *>(index.internalPointer());
    txtQuantity->setText(QString().setNum(ingredient->quantity().value(), 'f', 2));
    int type = cmbQuantityType->findText(ingredient->quantity().typeName());
    cmbQuantityType->setCurrentIndex(type);
    txtMinutes->setText(QString().setNum(ingredient->minutes(), 'f', 1));
}

void RecipeIngredientDelegate::setModelData(QWidget *editor,
                                            QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    QLineEdit *txtQuantity = editor->findChild<QLineEdit *>("txtQuantity");
    QComboBox *cmbQuantityType = editor->findChild<QComboBox *>("cmbQuantityType");
    QLineEdit *txtMinutes = editor->findChild<QLineEdit *>("txtMinutes");

    RecipeIngredient *ingredient = static_cast<RecipeIngredient *>(index.internalPointer());
    Quantity quantity(txtQuantity->text().toDouble(), Quantity::type(cmbQuantityType->currentText()), ingredient);
    ingredient->setQuantity(quantity);
    ingredient->setMinutes(txtMinutes->text().toDouble());

    model->setData(index, QVariant());
}

void RecipeIngredientDelegate::updateEditorGeometry(QWidget *editor,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
