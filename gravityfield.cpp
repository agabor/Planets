#include "gravityfield.h"
#include <math.h>
float gravity(DustField &f, int x0, in y0, int x1, in y1) {
    int m1 = f.get(x0, y0);
    int m2 = f.get(x1, y1);
    int x2 = x1 - x0;
    x2 *= x2;
    int y2 = y1 - y0;
    y2 *= y2;
    return (float)m1 * m2 / sqrtf(x2 + y2);
}

GravityField::GravityField(DustField &f)
{
    field = new int[f.width() * f.height() * 2];
    for (int y = 0; y < f.height(); ++y){
        for (int x = 0; x < f.width(); ++x){

            for (int iy = 0; iy < f.height(); ++iy){
                for (int ix = 0; ix < f.width(); ++ix){
                    float g = gravity(f,x,y,ix,iy);
                }
            }

        }
    }
}

uchar *GravityField::encodedField()
{

}
