#ifndef DUSTCANVAS_H
#define DUSTCANVAS_H

#include <QGlobalStatic>

class DustCanvas
{
public:
    DustCanvas(int w, int h);
    ~DustCanvas();
    void set(int x, int y, uchar v);
    uchar get(float x, float y);
    uchar get(int x, int y);
    uchar *buffer;
    int width() const;
    int height() const;
    DustCanvas * upscale(int scale);
private:
    const int w, h;
};

#endif // DUSTCANVAS_H
