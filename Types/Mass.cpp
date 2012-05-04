#include "Mass.h"

Mass::Mass()
{
}

QString Mass::typeName(int type, bool plural, bool abbreviated)
{
    switch((Type)type) {
    case Type_Milligram:
        if(abbreviated) return tr("mg");
        return plural? tr("Milligram"): tr("Milligrams");
    case Type_Gram:
        if(abbreviated) return tr("gm");
        return plural? tr("Gram"): tr("Grams");
    case Type_Kilogram:
        if(abbreviated) return tr("kg");
        return plural? tr("Kilogram"): tr("Kilograms");
    case Type_Ounce:
        if(abbreviated) return tr("oz");
        return plural? tr("Ounce"): tr("Ounces");
    case Type_Pound:
        if(abbreviated) return plural? tr("lb"): tr("lbs");
        return plural? tr("Pound"): tr("Pounds");
    default:
        return Base::typeName(type, plural, abbreviated);
    }
}

int Mass::type(QString name)
{
    if(name.compare(tr("mg"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Milligram"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Milligrams"), Qt::CaseInsensitive) == 0) {
        return Type_Milligram;
    } else if(name.compare(tr("gm"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Gram"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Grams"), Qt::CaseInsensitive) == 0) {
        return Type_Gram;
    } else if(name.compare(tr("km"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Kilogram"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Kilograms"), Qt::CaseInsensitive) == 0) {
        return Type_Kilogram;
    } else if(name.compare(tr("oz"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Ounce"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Ounces"), Qt::CaseInsensitive) == 0) {
        return Type_Ounce;
    } else if(name.compare(tr("lb"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("lbs"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Pound"), Qt::CaseInsensitive) == 0 ||
              name.compare(tr("Pounds"), Qt::CaseInsensitive) == 0) {
        return Type_Pound;
    }

    return Base::type(name);
}
