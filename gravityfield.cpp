#include "gravityfield.h"
#include <math.h>
#include <QDebug>

#define PI 3.14159265

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
    return m1 * m2 / dist(x0, y0, x1, y1);
}

inline void unitVector(int x0, int y0, int x1, int y1, float *vx, float *vy){
    *vx = x1 - x0;
    *vy = y1 - y0;
    float d = dist(x0, y0, x1, y1);
    *vx /= d;
    *vy /= d;
}

void GravityField::generate()
{
    p = 0;
    for (int y = 0; y < h; ++y){
        for (int x = 0; x < w; ++x){
            float rx = 0.f;
            float ry = 0.f;
            for (int iy = 0; iy < h; ++iy){
                for (int ix = 0; ix < w; ++ix){
                    if (x == ix && y == iy)
                        continue;
                    float g = gravity(f, x, y, ix, iy);
                    float vx, vy;
                    unitVector(x, y, ix, iy, &vx, &vy);
                    rx += vx * g;
                    ry += vy * g;
                }
            }
            const int i = y * w + x;
            field[i*2] = rx;
            field[i*2+1] = ry;
            const int l = len(rx, ry);
            if (l > maxForce)
                maxForce = l;
        }
        p = 100 * y / h;
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

void hsv2rgb(float h, float s, float v, float *r, float *g, float *b);

uchar *GravityField::encodedField()
{
    uchar *result = new uchar[w*h*3];
    for (int i = 0; i < w*h; ++i)
    {
        float x = field[i*2];
        float y = field[i*2+1];
        float h = (atan2(y, x) + PI) / (2* PI) * 360;
        float s = len(x,y) / maxForce;
        float r,g,b;
        hsv2rgb(h, s, 1.f, &r, &g, &b);
        result[i*3] = (uchar)(255 * r);
        result[i*3+1] = (uchar)(255 * g);
        result[i*3+2] = (uchar)(255 * b);
    }
    return result;
}



void hsv2rgb(float h, float s, float v, float *r, float *g, float *b)
{
    double      hh, p, q, t, ff;
    long        i;

    if(s <= 0.0) {       // < is bogus, just shuts up warnings
        *r = v;
        *g = v;
        *b = v;
        return;
    }
    hh = h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch(i) {
    case 0:
        *r = v;
        *g = t;
        *b = p;
        break;
    case 1:
        *r = q;
        *g = v;
        *b = p;
        break;
    case 2:
        *r = p;
        *g = v;
        *b = t;
        break;

    case 3:
        *r = p;
        *g = q;
        *b = v;
        break;
    case 4:
        *r = t;
        *g = p;
        *b = v;
        break;
    case 5:
    default:
        *r = v;
        *g = p;
        *b = q;
        break;
    }
}
