#include "RecipeIngredient.h"

RecipeIngredient::RecipeIngredient(Ingredient *ingredient, QObject *parent) :
    QObject(parent)
{
    setIngredient(ingredient);
    _quantity = Quantity();
    _minutes = 0;
    initialize();
}

RecipeIngredient::RecipeIngredient(Ingredient *ingredient, Quantity quantity, QObject *parent) :
    QObject(parent)
{
    setIngredient(ingredient);
    _quantity = quantity;
    _minutes = 0;
    initialize();
}

RecipeIngredient::RecipeIngredient(Ingredient *ingredient, Quantity quantity, double minutes, QObject *parent) :
    QObject(parent)
{
    setIngredient(ingredient);
    _quantity = quantity;
    _minutes = minutes;
}

RecipeIngredient::RecipeIngredient(QDomElement element, QObject *parent) :
    QObject(parent)
{
    fromXml(element);
}

void RecipeIngredient::initialize()
{
    if(!_ingredient->type().compare("Grain", Qt::CaseInsensitive)) {
        if(_quantity.value() == 0 && _quantity.type() == Quantity::QuantityType_item)
            _quantity = Quantity(1, Quantity::QuantityType_pound);
        if(_minutes == 0)
            _minutes = 0;
    } else if(!_ingredient->type().compare("Hops", Qt::CaseInsensitive)) {
        if(_quantity.value() == 0 && _quantity.type() == Quantity::QuantityType_item)
            _quantity = Quantity(1, Quantity::QuantityType_ounce);
        if(_minutes == 0)
            _minutes = 60;
    } else if(!_ingredient->type().compare("Yeast", Qt::CaseInsensitive)) {
        if(_quantity.value() == 0 && _quantity.type() == Quantity::QuantityType_item)
            _quantity = Quantity(1, Quantity::QuantityType_item);
        if(_minutes == 0)
            _minutes = 0;
    } else {
        if(_quantity.value() == 0 && _quantity.type() == Quantity::QuantityType_item)
            _quantity = Quantity();
        if(_minutes == 0)
            _minutes = 0;
    }

}

QDomElement RecipeIngredient::toXml(QDomDocument document)
{
    QDomElement element = document.createElement("RecipeIngredient");
    element.appendChild(ingredient()->toXml(document));
    element.appendChild(quantity().toXml(document));
    element.setAttribute("minutes", minutes());
    return element;
}

void RecipeIngredient::fromXml(QDomElement element)
{
    setIngredient(Ingredient::createIngredient(element.elementsByTagName("Ingredient").at(0).toElement()));
    setQuantity(Quantity(element.elementsByTagName("Quantity").at(0).toElement()));
    setMinutes(element.attribute("minutes", QString().setNum(minutes())).toDouble());
}


Ingredient *RecipeIngredient::ingredient()
{
    return _ingredient;
}
void RecipeIngredient::setIngredient(Ingredient* ingredient)
{
    connect(ingredient, SIGNAL(dataChanged()), this, SLOT(ingredientChanged()));
    _ingredient = ingredient;
}

void RecipeIngredient::ingredientChanged()
{
    emit dataChanged();
}

QString RecipeIngredient::name() const
{
    return _ingredient->name();
}
QString RecipeIngredient::type() const
{
    return _ingredient->type();
}

Quantity RecipeIngredient::quantity() const
{
    return _quantity;
}
void RecipeIngredient::setQuantity(Quantity quantity)
{
    _quantity = quantity;
    emit dataChanged();
}

double RecipeIngredient::minutes() const
{
    return _minutes;
}
void RecipeIngredient::setMinutes(double minutes)
{
    _minutes = minutes;
    emit dataChanged();
}
