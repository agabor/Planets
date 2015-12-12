#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QImage>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);
    void setBuffer(uchar * buffer);
    void setFormat(QImage::Format format);
    void setZoom(int zoom);
signals:
    void mark(int x, int y);
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent ( QMouseEvent * event ) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
private:
    uchar * buffer;
    QImage::Format format;
    bool isPressed;
    int zoom;
};

#endif // CANVAS_H
