#include "GrainIngredient.h"

GrainIngredient::GrainIngredient(QObject *parent) :
    Ingredient(parent)
{
    _type = tr("Grain");
    _color = 2;
    _specificGravity = 1.038;
    _extract = false;
}

QString GrainIngredient::type() const
{
    return _type;
}
void GrainIngredient::setType(QString type)
{
    _type = type;
    emit dataChanged();
}

double GrainIngredient::color() const
{
    return _color;
}
void GrainIngredient::setColor(double color)
{
    _color = color;
    emit dataChanged();
}

double GrainIngredient::specificGravity() const
{
    return _specificGravity;
}
void GrainIngredient::setSpecificGravity(double specificGravity)
{
    _specificGravity = specificGravity;
    emit dataChanged();
}

bool GrainIngredient::extract() const
{
    return _extract;
}
void GrainIngredient::setExtract(bool extract)
{
    _extract = extract;
    emit dataChanged();
}
void GrainIngredient::setExtract(QString extract)
{
    setExtract(!extract.compare("true", Qt::CaseInsensitive));
}

QDomElement GrainIngredient::toXml(QDomDocument document)
{
    QDomElement retval = Ingredient::toXml(document);
    retval.setAttribute("color", color());
    retval.setAttribute("specificGravity", specificGravity());
    retval.setAttribute("extract", extract()?"true":"false");
    return retval;
}

void GrainIngredient::fromXml(QDomElement element)
{
    Ingredient::fromXml(element);
    setColor(element.attribute("color", QString().setNum(color(), 'f')).toDouble());
    setSpecificGravity(element.attribute("specificGravity", QString().setNum(specificGravity(), 'f')).toDouble());
    setExtract(element.attribute("extract", extract()?"true":"false"));
}

