#include "HopsIngredient.h"

HopsIngredient::HopsIngredient(QObject *parent) :
    Ingredient(parent)
{
    _alphaAcid = 0.05;
}

QString HopsIngredient::type() const
{
    return tr("Hops");
}

double HopsIngredient::alphaAcid() const
{
    return _alphaAcid;
}
void HopsIngredient::setAlphaAcid(double alphaAcid)
{
    _alphaAcid = alphaAcid;
    emit dataChanged();
}

QDomElement HopsIngredient::toXml(QDomDocument document)
{
    QDomElement retval = Ingredient::toXml(document);
    retval.setAttribute("alphaAcid", alphaAcid());
    return retval;
}

void HopsIngredient::fromXml(QDomElement element)
{
    Ingredient::fromXml(element);
    setAlphaAcid(element.attribute("alphaAcid", QString().setNum(alphaAcid(), 'f')).toDouble());
}
