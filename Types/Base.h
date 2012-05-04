#ifndef BASETYPE_H
#define BASETYPE_H

#include <QObject>

class Base : public QObject
{
    Q_OBJECT
public:
    enum Type {
        Type_unknown = 0,
        Type_Item = 1
    };

    explicit Base(QObject *parent = 0);

    virtual double value();
    virtual void setValue(double value);

    virtual int type();
    virtual void setType(int type);

    virtual QString typeName(int type, bool plural = true, bool abbreviated = true);
    virtual int type(QString typeName);

    virtual QString toString(int prec = -1);

protected:
    double _value;
    int _type;

};

#endif // BASETYPE_H
