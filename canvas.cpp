#include "canvas.h"
#include <QImage>
#include <QDebug>
#include <QMouseEvent>
Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    buffer = nullptr;
    isPressed = false;
    zoom = 1;
}

void Canvas::setBuffer(uchar *buffer)
{
    this->buffer = buffer;
}

void Canvas::setFormat(QImage::Format format)
{
    this->format = format;
}

void Canvas::setZoom(int zoom)
{
    this->zoom = zoom;
}

void Canvas::paintEvent(QPaintEvent *)
{
    if (buffer == nullptr)
        return;
    const int w = this->width();
    const int h = this->height();
    QPainter painter;
    painter.begin(this);
    QImage i(buffer,w/zoom,h/zoom,format);
    painter.drawImage(0,0, i.scaled(w,h));
    painter.end();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    isPressed = true;
    emit mark(event->x() / zoom, event->y() / zoom);
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (isPressed)
        emit mark(event->x() / zoom, event->y() / zoom);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    isPressed = false;
}

