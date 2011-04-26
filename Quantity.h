#ifndef QUANTITY_H
#define QUANTITY_H

#include <QObject>
#include <QMetaType>
#include <QStringList>
#include <QDomDocument>

class Quantity : public QObject
{
    Q_OBJECT

public:
    enum QuantityTypes {
        QuantityType_unknown,
        QuantityType_milligram,
        QuantityType_gram,
        QuantityType_kilogram,
        QuantityType_ounce,
        QuantityType_pound,
        QuantityType_milliliter,
        QuantityType_liter,
        QuantityType_decaliter,
        QuantityType_fluidOunce,
        QuantityType_quart,
        QuantityType_gallon,
        QuantityType_item
    };

    explicit Quantity(double value = 0, QuantityTypes = QuantityType_item, QObject *parent = 0);
    explicit Quantity(QDomElement element, QObject *parent = 0);

    Quantity(const Quantity &other);

    double value() { return _value; }
    void setValue(double value) { _value = value; }

    QString toString();

    QuantityTypes type() { return _type; }
    static QuantityTypes type(QString type);
    void setType(QuantityTypes type) { _type = type; }
    QString typeName(bool brevis = true);
    static QStringList typeNames(bool brevis = true);
    static QString typeName(QuantityTypes type, bool brevis = true, bool singular = false);


    Quantity &operator=(const Quantity &other);


    double valueToMilligram();
    double valueToGram();
    double valueToKilogram();
    double valueToOunce();
    double valueToPound();
    double valueToPoundOunce();
    double valueToMilliliter();
    double valueToLiter();
    double valueToDecaliter();
    double valueToFluidOunce();
    double valueToQuart();
    double valueToGallon();

    Quantity toMilligram();
    Quantity toGram();
    Quantity toKilogram();
    Quantity toOunce();
    Quantity toPound();
    Quantity toPoundOunce();
    Quantity toMilliliter();
    Quantity toLiter();
    Quantity toDecaliter();
    Quantity toFluidOunce();
    Quantity toQuart();
    Quantity toGallon();

    QDomElement toXml(QDomDocument document);
    void fromXml(QDomElement element);

protected:
    double _value;
    QuantityTypes _type;
};

Q_DECLARE_METATYPE(Quantity);

#endif // QUANTITY_H
