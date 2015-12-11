#include "canvas.h"
#include <QImage>
Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    buffer = nullptr;
}

void Canvas::setBuffer(uchar *buffer)
{
    this->buffer = buffer;
}

void Canvas::paintEvent(QPaintEvent *)
{
    if (buffer == nullptr)
        return;
    const int w = this->width();
    const int h = this->height();
    QPainter painter;
    painter.begin(this);
    painter.drawImage(0,0,QImage(buffer,w,h,QImage::Format_Grayscale8));
    painter.end();
}

