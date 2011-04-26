#include "RecipeWidget.h"
#include "ui_RecipeWidget.h"

RecipeWidget::RecipeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecipeWidget)
{
    ui->setupUi(this);

    setRecipe(new Recipe(this));

    QSettings settings;
    restoreGeometry(settings.value("RecipeWidget/geometry", saveGeometry()).toByteArray());
    ui->trvIngredients->header()->restoreState(
                settings.value("RecipeWidget/ingredientsState", ui->trvIngredients->header()->saveState()).toByteArray() );
    ui->tblCalculated->horizontalHeader()->restoreState(
                settings.value("RecipeWidget/calculatedHorizontalState", ui->tblCalculated->horizontalHeader()->saveState()).toByteArray() );
    ui->tblCalculated->verticalHeader()->restoreState(
                settings.value("RecipeWidget/calculatedVerticalState", ui->tblCalculated->verticalHeader()->saveState()).toByteArray() );
}

RecipeWidget::RecipeWidget(QString filepath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecipeWidget)
{
    ui->setupUi(this);

    load(filepath);

    QSettings settings;
    restoreGeometry(settings.value("RecipeWidget/geometry", saveGeometry()).toByteArray());
    ui->trvIngredients->header()->restoreState(
                settings.value("RecipeWidget/ingredientsState", ui->trvIngredients->header()->saveState()).toByteArray() );
    ui->tblCalculated->horizontalHeader()->restoreState(
                settings.value("RecipeWidget/calculatedHorizontalState", ui->tblCalculated->horizontalHeader()->saveState()).toByteArray() );
    ui->tblCalculated->verticalHeader()->restoreState(
                settings.value("RecipeWidget/calculatedVerticalState", ui->tblCalculated->verticalHeader()->saveState()).toByteArray() );
}

RecipeWidget::~RecipeWidget()
{
    QSettings settings;
    settings.setValue("RecipeWidget/geometry", saveGeometry());
    settings.setValue("RecipeWidget/ingredientsState",  ui->trvIngredients->header()->saveState());
    settings.setValue("RecipeWidget/calculatedHorizontalState",  ui->tblCalculated->horizontalHeader()->saveState());
    settings.setValue("RecipeWidget/calculatedVerticalState",  ui->tblCalculated->verticalHeader()->saveState());

    delete ui;
}

void RecipeWidget::setRecipe(Recipe *recipe)
{
    _recipe = recipe;

    RecipeModel *recipeModel = new RecipeModel(this);
    recipeModel->setRecipe(recipe);
    ui->tblCalculated->setModel(recipeModel);

    RecipeIngredientModel *ingredientModel = new RecipeIngredientModel(this);
    ingredientModel->setRecipe(recipe);
    ui->trvIngredients->setModel(ingredientModel);
    ui->trvIngredients->setItemDelegateForColumn(2, new RecipeIngredientDelegate(this));

    connect(_recipe, SIGNAL(dataChanged()), this, SLOT(recipeChanged()));
    refreshText();
}

void RecipeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    // We're accepting ingredient items only
    QAbstractItemView *view = qobject_cast<QAbstractItemView *>(event->source());
    if(view) {
        QAbstractProxyModel *model = qobject_cast<QAbstractProxyModel *>(view->model());
        if(model) {
            QByteArray encoded = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
            QDataStream stream(&encoded, QIODevice::ReadOnly);

            while (!stream.atEnd())
            {
                int row, col;
                QMap<int,  QVariant> roleDataMap;
                stream >> row >> col >> roleDataMap;

                QModelIndex index = model->sourceModel()->index(row, 0);
                QObject *object = static_cast<QObject *>(index.internalPointer());
                Ingredient *ingredient = qobject_cast<Ingredient *>(object);
                if(ingredient) {
                    event->acceptProposedAction();
                }
            }
        }
    }
}

void RecipeWidget::dropEvent(QDropEvent *event)
{
    QAbstractItemView *view = qobject_cast<QAbstractItemView *>(event->source());
    if(view) {
        QAbstractProxyModel *model = qobject_cast<QAbstractProxyModel *>(view->model());
        if(model) {
            QByteArray encoded = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
            QDataStream stream(&encoded, QIODevice::ReadOnly);

            while (!stream.atEnd())
            {
                int row, col;
                QMap<int,  QVariant> roleDataMap;
                stream >> row >> col >> roleDataMap;

                QModelIndex index = model->sourceModel()->index(row, 0);
                QObject *object = static_cast<QObject *>(index.internalPointer());
                Ingredient *ingredient = qobject_cast<Ingredient *>(object);
                if(ingredient) {
                    recipe()->addIngredient(ingredient);
                    event->acceptProposedAction();
                }
            }
        }
    }
}

void RecipeWidget::load(QString filepath)
{
    setWindowFilePath(filepath);

    QDomDocument document("Ingredients");
    QFile file(windowFilePath());

    if(!file.open(QIODevice::ReadOnly))
        throw tr("Failed to open recipe file");

    if(!document.setContent(&file)) {
        file.close();
        throw tr("Failed to read recipe file");
    }

    file.close();

    if(document.documentElement().tagName() != "Recipe") {
        throw tr("Not a recipe file");
    }

    setRecipe(new Recipe(document.documentElement()));
}

void RecipeWidget::save(QString filepath)
{

    if(!filepath.isEmpty()) {
        setWindowFilePath(filepath);
    } else if(!isWindowModified()) {
        return;     // Don't do anything if the file would be the same
    }


    QDomDocument document("Recipe");
    document.appendChild(recipe()->toXml(document));

    if(windowFilePath().isEmpty()) {
        throw tr("File path is not valid");
    }
    QFile file(windowFilePath());

    if(!file.open(QIODevice::WriteOnly)) {
        throw tr("Failed to open ingredients file");
    }

    file.write(document.toString().toLatin1());
    file.flush();
    file.close();

    setWindowModified(false);
    refreshText();
    emit changed();
}

void RecipeWidget::recipeChanged()
{
    refreshText(true);
    emit changed();
}

void RecipeWidget::refreshText(bool modified)
{
    if(isWindowModified() || modified) {
        setWindowTitle(_recipe->name().append(" [*]"));
        setWindowModified(true);
    } else {
        this->setWindowTitle(_recipe->name());
    }

    ui->txtName->setText(_recipe->name());
    ui->cmbStyle->setCurrentIndex(ui->cmbStyle->findText(_recipe->style()));
    ui->spnVolume->setValue(_recipe->volume().valueToGallon());
    ui->spnBoilTime->setValue(_recipe->boilTime());
    ui->spnEfficiency->setValue(_recipe->efficiency() * 100);
}

void RecipeWidget::on_txtName_editingFinished()
{
    QString name = ui->txtName->text();
    if(_recipe->name() != name) {
        _recipe->setName(name);
    } else {
        refreshText();
    }
}

void RecipeWidget::on_cmbStyle_currentIndexChanged(QString style)
{
    if(_recipe->style() != style) {
        _recipe->setStyle(style);
    }
}

void RecipeWidget::on_spnVolume_editingFinished()
{
    double volume = ui->spnVolume->value();
    if(_recipe->volume().valueToGallon() != volume) {
        _recipe->setVolume(volume);
    } else {
        refreshText();
    }
}

void RecipeWidget::on_spnEfficiency_editingFinished()
{
    double efficiency = ui->spnEfficiency->value() / 100;
    if(_recipe->efficiency() != efficiency) {
        _recipe->setEfficiency(efficiency);
    } else {
        refreshText();
    }
}







