#include "YeastIngredient.h"

YeastIngredient::YeastIngredient(QObject *parent) :
    Ingredient(parent)
{
    _attenuation = 0.75;
    _minimumTemperature = 65;
    _maximumTemperature = 75;
}

QString YeastIngredient::type() const
{
    return tr("Yeast");
}

double YeastIngredient::attenuation() const
{
    return _attenuation;
}
void YeastIngredient::setAttenuation(double attenuation)
{
    _attenuation = attenuation;
    emit dataChanged();
}

double YeastIngredient::minimumTemperature() const
{
    return _minimumTemperature;
}
void YeastIngredient::setMinimumTemperature(double minimumTemperature)
{
    _minimumTemperature = minimumTemperature;
    emit dataChanged();
}

double YeastIngredient::maximumTemperature() const
{
    return _maximumTemperature;
}
void YeastIngredient::setMaximumTemperature(double maximumTemperature)
{
    _maximumTemperature = maximumTemperature;
    emit dataChanged();
}


QDomElement YeastIngredient::toXml(QDomDocument document)
{
    QDomElement retval = Ingredient::toXml(document);
    retval.setAttribute("attenuation", attenuation());
    retval.setAttribute("minimumTemperature", minimumTemperature());
    retval.setAttribute("maximumTemperature", maximumTemperature());
    return retval;
}

void YeastIngredient::fromXml(QDomElement element)
{
    Ingredient::fromXml(element);
    setAttenuation(element.attribute("attenuation", QString().setNum(attenuation(), 'f')).toDouble());
    setMinimumTemperature(element.attribute("minimumTemperature", QString().setNum(minimumTemperature(), 'f')).toDouble());
    setMaximumTemperature(element.attribute("maximumTemperature", QString().setNum(maximumTemperature(), 'f')).toDouble());
}
