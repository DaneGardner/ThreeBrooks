#include "IngredientToolbox.h"
#include "ui_IngredientToolbox.h"

IngredientToolbox::IngredientToolbox(QString filepath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IngredientToolbox)
{
    _filepath = filepath;
    ui->setupUi(this);

    QSortFilterProxyModel *proxy;

    _grainIngredientModel = new IngredientModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterRole(Qt::EditRole);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy->setSortRole(Qt::EditRole);
    proxy->setSourceModel(_grainIngredientModel);
    proxy->sort(0);
    ui->grainView->setModel(proxy);
    connect(ui->grainView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(editIngredient(QModelIndex)));

    _hopsIngredientModel = new IngredientModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterRole(Qt::EditRole);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy->setSortRole(Qt::EditRole);
    proxy->setSourceModel(_hopsIngredientModel);
    proxy->sort(0);
    ui->hopsView->setModel(proxy);
    connect(ui->hopsView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(editIngredient(QModelIndex)));

    _yeastIngredientModel = new IngredientModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterRole(Qt::EditRole);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy->setSortRole(Qt::EditRole);
    proxy->setSourceModel(_yeastIngredientModel);
    proxy->sort(0);
    ui->yeastView->setModel(proxy);
    connect(ui->yeastView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(editIngredient(QModelIndex)));

    _otherIngredientModel = new IngredientModel(this);
    proxy = new QSortFilterProxyModel(this);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxy->setFilterRole(Qt::EditRole);
    proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    proxy->setSortRole(Qt::EditRole);
    proxy->setSourceModel(_otherIngredientModel);
    proxy->sort(0);
    ui->otherView->setModel(proxy);
    connect(ui->otherView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(editIngredient(QModelIndex)));

    load();
    _ingredientsChanged = false;
}

IngredientToolbox::~IngredientToolbox()
{
    if(_ingredientsChanged) {
        save();
    }

    delete ui;
}

void IngredientToolbox::load(QString filepath)
{
    if(filepath.isEmpty())
        filepath = _filepath;

    QDomDocument document("Ingredients");
    QFile file(filepath);

    if(!file.open(QIODevice::ReadOnly))
        throw new QString("Failed to open ingredients file");

    if(!document.setContent(&file)) {
        file.close();
        throw new QString("Failed to read ingredients file");
    }

    file.close();

    QDomElement baseElement = document.documentElement();
    QDomElement element = baseElement.firstChildElement();
    while(!element.isNull()) {
        Ingredient *ingredient = Ingredient::createIngredient(element);
        insert(ingredient);
        element = element.nextSiblingElement();
    }

    on_ToolBox_currentChanged(0);   // Forces a sort of the newly loaded page
}

void IngredientToolbox::save(QString filepath)
{
    if(filepath.isEmpty())
        filepath = _filepath;

    QDomDocument document("Ingredients");
    QDomElement baseElement = document.createElement("Ingredients");
    document.appendChild(baseElement);

    for(int i=0; i<_ingredients.count(); i++) {
        baseElement.appendChild(_ingredients.at(i)->toXml(document));
    }

    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly)) {
        throw new QString("Failed to open ingredients file");
    }

    file.write(document.toString().toLatin1());
    file.flush();
    file.close();
}

void IngredientToolbox::ingredientChanged()
{
    _ingredientsChanged = true;
}

void IngredientToolbox::editIngredient(QModelIndex index)
{
    Ingredient *ingredient = index.model()->data(index, Qt::UserRole).value<Ingredient *>();
    if(ingredient) {
        IngredientEditDialog dlg(ingredient, this);
        dlg.exec();
    }
}

void IngredientToolbox::on_ToolBox_currentChanged(int page)
{
    Q_UNUSED(page)

    QListView *listView = qobject_cast<QListView *>(ui->ToolBox->currentWidget());
    QSortFilterProxyModel *proxy = qobject_cast<QSortFilterProxyModel *>(listView->model());
    proxy->sort(0);

    /*! \bug This is a workaround for a Qt4.7 (and others?) bug in the QSortFilterProxyModel
             where it creates blank entries for every insert! */
    proxy->setSourceModel(proxy->sourceModel());
}

void IngredientToolbox::insert(Ingredient *ingredient, bool sort)
{
    connect(ingredient, SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _ingredients.append(ingredient);
    if(qobject_cast<GrainIngredient *>(ingredient)) {
        _grainIngredientModel->append(ingredient);
    } else if(qobject_cast<HopsIngredient *>(ingredient)) {
        _hopsIngredientModel->append(ingredient);
    } else if(qobject_cast<YeastIngredient *>(ingredient)) {
        _yeastIngredientModel->append(ingredient);
    } else {
        _otherIngredientModel->append(ingredient);
    }
    ingredientChanged();
}

void IngredientToolbox::on_btnNew_clicked()
{
    IngredientEditDialog dlg(NULL, this);
    if(dlg.exec() == QDialog::Accepted) {
        Ingredient *ingredient = dlg.ingredient();
        if(ingredient) {
            insert(ingredient);

            /*! \bug This is a workaround for a Qt4.7 (and others?) bug in the
                     QSortFilterProxyModel where it creates blank entries for every
                     insert! We only need to worry about the current list, if we switch to
                     another, it'll be sorted automatically. */
            QListView *listView = qobject_cast<QListView *>(ui->ToolBox->currentWidget());
            QSortFilterProxyModel *proxy = qobject_cast<QSortFilterProxyModel *>(listView->model());
            proxy->setSourceModel(proxy->sourceModel());
        }
    }

}

void IngredientToolbox::on_btnRemove_clicked()
{
    QMessageBox dlg( QMessageBox::Warning,
                     tr("Delete Ingredient"),
                     tr("Are you sure that you want to permanently remove the selected ingredients?"),
                     QMessageBox::Yes|QMessageBox::No,
                     this);

    if(dlg.exec() == QMessageBox::Yes) {
        QListView *listView = qobject_cast<QListView *>(ui->ToolBox->currentWidget());
        QSortFilterProxyModel *model = qobject_cast<QSortFilterProxyModel *>(listView->model());
        IngredientModel *ingredientModel = qobject_cast<IngredientModel *>(model->sourceModel());
        QModelIndexList indices = listView->selectionModel()->selectedIndexes();
        for(int i=0; i < indices.count(); i++) {
            Ingredient *ingredient =
                    indices.at(i).model()->data(indices.at(i), Qt::UserRole).value<Ingredient *>();

            ingredientModel->remove(ingredient);
            _ingredients.removeOne(ingredient);
            ingredientChanged();
        }
    }
}

void IngredientToolbox::on_txtFilter_textChanged(QString text)
{
    for(int i=0; i < ui->ToolBox->count(); i++) {
        QListView *listView = qobject_cast<QListView *>(ui->ToolBox->widget(i));
        QSortFilterProxyModel *proxy = qobject_cast<QSortFilterProxyModel *>(listView->model());
        proxy->setFilterRegExp(text);
    }
}


