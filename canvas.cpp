#include "canvas.h"
#include <QImage>
#include <QDebug>
Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    buffer = nullptr;
}

void Canvas::setBuffer(uchar *buffer)
{
    this->buffer = buffer;
}

void Canvas::setFormat(QImage::Format format)
{
    this->format = format;
}

void Canvas::paintEvent(QPaintEvent *)
{
    if (buffer == nullptr)
        return;
    const int w = this->width();
    const int h = this->height();
    QPainter painter;
    painter.begin(this);
    painter.drawImage(0,0,QImage(buffer,w,h,format));
    painter.end();
}

