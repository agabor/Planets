#ifndef GRAVITYFIELD_H
#define GRAVITYFIELD_H


#include "dustfield.h"

class GravityField
{
public:
    GravityField(DustField &f);
    uchar* encodedField();
    void generate();
    int progress();

private:
    void resultantForce(int x, int y, float *rx, float *ry);
    float maxForce;
    int w, h;
    float* field;
    DustField &f;
    int p;
};

#endif // GRAVITYFIELD_H
