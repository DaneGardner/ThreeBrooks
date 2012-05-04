#include "Base.h"

Base::Base(QObject *parent) :
    QObject(parent)
{
}


double Base::value()
{
    return _value;
}

void Base::setValue(double value)
{
    _value = value;
}

int Base::type()
{
    return _type;
}

void Base::setType(int type)
{
    _type = type;
}

QString Base::typeName(int type, bool plural, bool abbreviated)
{
    switch(type) {
    case Type_Item:
        if(abbreviated) return tr("item");
        return plural? tr("Item"): tr("Items");
    case Type_unknown:
    default:
        return tr("Unknown");
    }
}

int Base::type(QString typeName)
{
    if(typeName.compare(tr("Item"), Qt::CaseInsensitive) == 0 ||
              typeName.compare(tr("Items"), Qt::CaseInsensitive) == 0) {
        return Type_Item;
    } else if(typeName.compare(tr("Unknown"), Qt::CaseInsensitive) == 0) {
        return Type_unknown;
    }

    return -1;
}

QString Base::toString(int prec)
{
    if(value() == 1) {
        return tr("%1 %2").arg(value(), 0, 'f', prec)
                          .arg(typeName(type(), false));
    }

    return tr("%1 %2").arg(value(), 0, 'f', prec)
                      .arg(typeName(type()));

}



