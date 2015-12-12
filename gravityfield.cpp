#include "gravityfield.h"
#include <omp.h>
#include <math.h>
#include <QDebug>
#include "colorencode.h"

inline float len(int x, int y)
{
    x *= x;
    y *= y;
    return sqrtf(x + y);
}

inline float dist(int x0, int y0, int x1, int y1)
{
    return len(x1 - x0, y1 - y0);
}

inline float gravity(DustField &f, int x0, int y0, int x1, int y1) {
    float m1 = f.get(x0, y0) / 255.f;
    float m2 = f.get(x1, y1) / 255.f;
    float r = dist(x0, y0, x1, y1);
    return m1 * m2 / (r * r);
}

inline void unitVector(int x0, int y0, int x1, int y1, float *vx, float *vy){
    float d = dist(x0, y0, x1, y1);
    *vx = (x1 - x0) / d;
    *vy = (y1 - y0) / d;
}

void GravityField::generate()
{
    p = 0;
    int yc = 0;
    #pragma omp parallel for
    for (int y = 0; y < h; ++y){
        for (int x = 0; x < w; ++x){
            float rx = 0.f;
            float ry = 0.f;
            if (f.get(x,y) != 0){
                for (int iy = 0; iy < h; ++iy){
                    for (int ix = 0; ix < w; ++ix){
                        if (x == ix && y == iy)
                            continue;
                        if (f.get(ix,iy) == 0)
                            continue;
                        float g = gravity(f, x, y, ix, iy);
                        float vx, vy;
                        unitVector(x, y, ix, iy, &vx, &vy);
                        rx += vx * g;
                        ry += vy * g;
                    }
                }
            }
            const int i = (y * w + x)*2;
            field[i] = rx;
            field[i+1] = ry;
            const int l = len(rx, ry);
            if (l > maxForce)
                maxForce = l;
        }
        ++yc;
        p = 100 * yc / h;
    }
}

int GravityField::progress()
{
    return p;
}

GravityField::GravityField(DustField &f) : f(f)
{
    w = f.width();
    h = f.height();
    field = new float[w * h * 2];
}


uchar *GravityField::encodedField()
{
    uchar *result = new uchar[w*h*3];
    for (int i = 0; i < w*h; ++i)
    {
        float r,g,b;
        encode(field[i*2], field[i*2+1], maxForce, &r, &g, &b);
        result[i*3] = (uchar)(255 * r);
        result[i*3+1] = (uchar)(255 * g);
        result[i*3+2] = (uchar)(255 * b);
    }
    return result;
}
