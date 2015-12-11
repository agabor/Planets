#ifndef GRAVITYFIELD_H
#define GRAVITYFIELD_H

#include "dustfield.h"

class GravityField
{
public:
    GravityField(DustField &f);
    uchar* encodedField();

private:
    float maxForce;
    int w, h;
    float* field;
};

#endif // GRAVITYFIELD_H
