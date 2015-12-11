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
signals:
    void mousePress(int x, int y);
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent * event) override;
private:
    uchar * buffer;
    QImage::Format format;
};

#endif // CANVAS_H
