#ifndef YEASTINGREDIENT_H
#define YEASTINGREDIENT_H

#include "Ingredient.h"

class YeastIngredient : public Ingredient
{
    Q_OBJECT
public:
    explicit YeastIngredient(QObject *parent = 0);
    virtual QString type() const;

    virtual double attenuation() const;
    virtual void setAttenuation(double attenuation);

    virtual double minimumTemperature() const;
    virtual void setMinimumTemperature(double minimumTemperature);

    virtual double maximumTemperature() const;
    virtual void setMaximumTemperature(double maximumTemperature);

    virtual QDomElement toXml(QDomDocument);

protected:
    virtual void fromXml(QDomElement);
    double _attenuation;
    double _minimumTemperature;
    double _maximumTemperature;

};

#endif // YEASTINGREDIENT_H
