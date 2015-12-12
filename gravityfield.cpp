#include "gravityfield.h"
#include <omp.h>
#include <math.h>
#include <QDebug>
#include "colorencode.h"

template<typename T>
inline float len(T x, T y)
{
    return sqrtf(x * x + y * y);
}

template<typename T>
inline float dist(T x0, T y0, T x1, T y1)
{
    return len(x1 - x0, y1 - y0);
}

inline float gravity(float m1, int x1, int y1, float m2, int x2, int y2) {
    float r = dist(x1, y1, x2, y2);
    return m1 * m2 / (r * r);
}

inline void unitVector(int x0, int y0, int x1, int y1, float *vx, float *vy){
    float d = dist(x0, y0, x1, y1);
    *vx = (x1 - x0) / d;
    *vy = (y1 - y0) / d;
}

void GravityField::resultantForce(int x, int y, float *rx, float *ry)
{
    *rx = 0.f;
    *ry = 0.f;
    uchar cm1 = f.get(x, y);
    if (cm1 == 0)
        return;
    float m1 = cm1 / 255.f;
    for (int iy = 0; iy < h; ++iy){
        for (int ix = 0; ix < w; ++ix){
            if (x == ix && y == iy)
                continue;
            uchar cm2 = f.get(ix, iy);
            if (cm2 == 0)
                continue;
            float m2 = cm2 / 255.f;
            float g = gravity(m1, x, y, m2, ix, iy);
            float vx, vy;
            unitVector(x, y, ix, iy, &vx, &vy);
            *rx += vx * g;
            *ry += vy * g;
        }
    }
}

void GravityField::generate()
{
    p = 0;
    int yc = 0;
    maxForce = 0;
    #pragma omp parallel for
    for (int y = 0; y < h; ++y){
        for (int x = 0; x < w; ++x){
            float rx, ry;
            resultantForce(x, y, &rx, &ry);
            const int i = (y * w + x) * 2;
            field[i] = rx;
            field[i+1] = ry;
            const float l = len(rx, ry);
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
