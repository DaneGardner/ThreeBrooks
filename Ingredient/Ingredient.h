#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QObject>
#include <QUuid>
#include <QDomDocument>
#include <QMetaType>

class GrainIngredient;
class HopsIngredient;
class YeastIngredient;

class Ingredient : public QObject
{
    Q_OBJECT
public:
    explicit Ingredient(QObject *parent = 0);

    virtual QString name() const;
    virtual void setName(QString name);

    virtual QUuid id() const;
    virtual void setId(QUuid id);

    virtual QString type() const;

    virtual QDomElement toXml(QDomDocument document);

    static Ingredient *createIngredient(QDomElement element);

signals:
    void dataChanged();

protected:
    virtual void fromXml(QDomElement element);

    QString _name;
    QUuid _id;


};

Q_DECLARE_METATYPE(Ingredient *);

#endif // INGREDIENT_H
