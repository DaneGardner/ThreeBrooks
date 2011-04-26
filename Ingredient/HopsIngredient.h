#ifndef HOPSINGREDIENT_H
#define HOPSINGREDIENT_H

#include "Ingredient.h"

class HopsIngredient : public Ingredient
{
    Q_OBJECT
public:
    explicit HopsIngredient(QObject *parent = 0);
    virtual QString type() const;

    virtual double alphaAcid() const;
    virtual void setAlphaAcid(double alphaAcid);

    virtual QDomElement toXml(QDomDocument);

protected:
    virtual void fromXml(QDomElement);
    double _alphaAcid;
};

#endif // HOPSINGREDIENT_H
