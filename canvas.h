#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = 0);
    void setBuffer(uchar * buffer);
protected:
    void paintEvent(QPaintEvent *) override;
private:
    uchar * buffer;
};

#endif // CANVAS_H
