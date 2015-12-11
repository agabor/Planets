#ifndef DUSTCANVAS_H
#define DUSTCANVAS_H

#include <QGlobalStatic>

class DustField
{
public:
    DustField(int w, int h);
    ~DustField();
    void set(int x, int y, uchar v);
    uchar get(float x, float y);
    uchar get(int x, int y);
    uchar *buffer;
    int width() const;
    int height() const;
    DustField * upscale(int scale, int err = 0);
private:
    const int w, h;
};

#endif // DUSTCANVAS_H
