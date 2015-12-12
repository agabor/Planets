#ifndef COLORENCODE
#define COLORENCODE
#define PI 3.14159265
#include <math.h>

inline void hsv2rgb(float h, float s, float v, float *r, float *g, float *b)
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


inline void encode(float x, float y, float maxl, float *r, float *g, float *b)
{
    float h = (atan2(y, x) + PI) / (2* PI) * 360;
    float l = sqrtf(x * x + y * y);
    float s = l / maxl;
    hsv2rgb(h, s, 1.f, r, g, b);
}

#endif // COLORENCODE

