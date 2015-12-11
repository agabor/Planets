#include "dustcanvas.h"

DustCanvas::DustCanvas(int w, int h) : w(w), h(h)
{
    buffer = new uchar[w*h];
}

DustCanvas::~DustCanvas()
{
    delete [] buffer;
}

void DustCanvas::set(int x, int y, uchar v)
{
    buffer[y * w + x] = v;
}

uchar DustCanvas::get(float x, float y)
{
    uchar v00 = get((int)x, (int)y);
    uchar v10 = get((int)x+1, (int)y);
    uchar v01 = get((int)x, (int)y+1);
    uchar v11 = get((int)x+1, (int)y+1);
    float rx = x - (int)x;
    float ry = y - (int)y;
    return (v00 * (1-rx) + v10*rx)*(1-ry) + (v01 * (1-rx) + v11*rx)*ry;
}

uchar DustCanvas::get(int x, int y)
{
    return buffer[y * w + x];
}

int DustCanvas::width() const
{
    return w;
}

int DustCanvas::height() const
{
    return h;
}

void upsample(DustCanvas *dc0, DustCanvas *dc1, int x, int y, int scale,  int r)
{
    int v = dc0->get((float)x/scale,(float)y/scale);
    int d = qrand() % (2*r) - r;
    dc1->set(x, y, std::max(0, std::min(255, v+d)));
}

DustCanvas * DustCanvas::upscale(int scale, int err)
{
    const int w1 = w*scale;
    const int h1 = h*scale;
    DustCanvas *dc1 = new DustCanvas(w1,h1);
    for (int y = 0; y < h1; ++y){
        for (int x = 0; x < w1; ++x){
            upsample(this, dc1, x, y, scale, err);
        }
    }

    return dc1;
}

