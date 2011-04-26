#include "Ingredient.h"
#include "GrainIngredient.h"
#include "HopsIngredient.h"
#include "YeastIngredient.h"

Ingredient::Ingredient(QObject *parent) :
    QObject(parent)
{
    _name = QString();
    _id = QUuid::createUuid();
}

QString Ingredient::name() const
{
    return _name;
}
void Ingredient::setName(QString name)
{
    _name = name;
    emit dataChanged();
}

QUuid Ingredient::id() const
{
    return _id;
}
void Ingredient::setId(QUuid id)
{
    _id = id;
    emit dataChanged();
}

QString Ingredient::type() const
{
    return tr("Ingredient");
}


QDomElement Ingredient::toXml(QDomDocument document)
{
    QDomElement retval = document.createElement("Ingredient");
    retval.setAttribute("name", name());
    retval.setAttribute("type", type());
    retval.setAttribute("id", id().toString());
    return retval;
}

void Ingredient::fromXml(QDomElement element)
{
    setName(element.attribute("name", name()));
    setId(QUuid(element.attribute("id", id().toString())));
}

Ingredient *Ingredient::createIngredient(QDomElement element)
{
    Ingredient *ingredient;
    if(!element.attribute("type").compare("Grain", Qt::CaseInsensitive)) {
        ingredient = new GrainIngredient();
    } else if(!element.attribute("type").compare("Hops", Qt::CaseInsensitive)) {
        ingredient = new HopsIngredient();
    } else if(!element.attribute("type").compare("Yeast", Qt::CaseInsensitive)) {
        ingredient = new YeastIngredient();
    } else if(!element.attribute("type").compare("Ingredient", Qt::CaseInsensitive)) {
        ingredient = new Ingredient();
    }

    ingredient->fromXml(element);
    return ingredient;
}
