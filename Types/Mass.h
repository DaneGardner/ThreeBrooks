#ifndef MASS_H
#define MASS_H

#include "Base.h"

class Mass : public Base
{
public:
    Mass();

    enum Type {
        Type_Milligram = 2,
        Type_Gram = 3,
        Type_Kilogram = 4,
        Type_Ounce = 5,
        Type_Pound = 6
    };

    virtual QString typeName(int type, bool plural = true, bool abbreviated = true);
    virtual int type(QString typeName);

};

#endif // MASS_H
