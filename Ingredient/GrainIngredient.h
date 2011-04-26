#ifndef GRAININGREDIENT_H
#define GRAININGREDIENT_H

#include "Ingredient.h"

class GrainIngredient : public Ingredient
{
    Q_OBJECT
public:
    explicit GrainIngredient(QObject *parent = 0);

    virtual QString type() const;
    virtual void setType(QString type);

    virtual double color() const;
    virtual void setColor(double color);

    virtual double specificGravity() const;
    virtual void setSpecificGravity(double specificGravity);

    virtual bool extract() const;
    virtual void setExtract(bool extract);

    virtual QDomElement toXml(QDomDocument);

protected:
    virtual void setExtract(QString extract);
    virtual void fromXml(QDomElement);
    QString _type;
    double _color;
    double _specificGravity;
    bool _extract;
};

#endif // GRAININGREDIENT_H
