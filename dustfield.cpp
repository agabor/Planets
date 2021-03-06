#include "dustfield.h"

DustField::DustField(int w, int h) : w(w), h(h)
{
    buffer = new uchar[w*h];
}

DustField::~DustField()
{
    delete [] buffer;
}

void DustField::set(int x, int y, uchar v)
{
    buffer[y * w + x] = v;
}

uchar DustField::get(float x, float y)
{
    int x1 = ((int)x) == w-1 ? x : x + 1;
    int y1 = ((int)y) == h-1 ? y : y + 1;
    uchar v00 = get((int)x, (int)y);
    uchar v10 = get(x1, (int)y);
    uchar v01 = get((int)x, y1);
    uchar v11 = get(x1, y1);
    float rx = x - (int)x;
    float ry = y - (int)y;
    return (v00 * (1-rx) + v10*rx)*(1-ry) + (v01 * (1-rx) + v11*rx)*ry;
}

uchar DustField::get(int x, int y)
{
    return buffer[y * w + x];
}

int DustField::width() const
{
    return w;
}

int DustField::height() const
{
    return h;
}

void upsample(DustField *dc0, DustField *dc1, int x, int y, int scale,  int r)
{
    int v = dc0->get((float)x/scale,(float)y/scale);
    int d = r == 0 ? 0 : qrand() % (2*r) - r;
    dc1->set(x, y, std::max(0, std::min(255, v+d)));
}

DustField * DustField::upscale(int scale, int err)
{
    const int w1 = w*scale;
    const int h1 = h*scale;
    DustField *dc1 = new DustField(w1,h1);
    for (int y = 0; y < h1; ++y){
        for (int x = 0; x < w1; ++x){
            upsample(this, dc1, x, y, scale, err);
        }
    }

    return dc1;
}

void DustField::drawSpot(uchar color, int size, int x, int y)
{
    int x0 = x - size < 0 ? 0 : x - size;
    int x1 = x + size >= w ? w - 1 : x + size;
    int y0 = y - size < 0 ? 0 : y - size;
    int y1 = y + size >= h ? h - 1 : y + size;

    int s2 = size * size;
    int s12 = (size + 1) * (size + 1);
    for (int iy = y0; iy <= y1; ++iy){
        for (int ix = x0; ix <= x1; ++ix){
            int dx = ix - x;
            int dy = iy - y;
            int l2 = dx * dx + dy * dy;
            if (l2 <= s2)
            {
                set(ix, iy, color);
            } else if (l2 < s12)
            {
                float r = (float)(s12 - l2) / (s12 - s2);
                uchar p = get(ix, iy);
                set(ix, iy, color * r + p * (1.f-r));
            }
        }
    }
}
