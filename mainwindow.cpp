#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dustfield.h"
#include <math.h>

#include <QDebug>
#include <QTime>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QApplication>
#include <QTimer>
#include <colorencode.h>


DustField * MainWindow::getRandomDustCanvas()
{
    const int w0 = 2;
    const int h0 = 2;
    QTime now = QTime::currentTime();
    qsrand(now.msec());
    DustField *dc0 = new DustField(w0,h0);
    for (int y = 0; y < h0; ++y){
        for (int x = 0; x < w0; ++x){
            dc0->set(x, y, qrand() % 256);
        }
    }

    int noise = ui->noiseSb->value();
    DustField *dc = nullptr;
    do {
        dc = dc0->upscale(2, noise);
        delete dc0;
        dc0 = dc;

    } while (dc->width() < size);

    return dc;
}

uchar * MainWindow::colorCode(int rad)
{
    int r2 = rad * rad;
    int w = rad * 2;
    uchar * cc = new uchar[w * w * 3];
    for (int y = 0; y < w; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            int i = (y * w + x) * 3;
            if (x == rad || y == rad)
            {
                cc[i] = 0;
                cc[i+1] = 0;
                cc[i+2] = 0;
                continue;
            }

            int vx = x - rad;
            int vy = y - rad;
            if (vx * vx + vy * vy > r2)
            {
                cc[i] = 255;
                cc[i+1] = 255;
                cc[i+2] = 255;
                continue;
            }
            float r,g,b;
            encode(vx, vy, rad, &r, &g, &b);

            cc[i] = r*255;
            cc[i+1] = g*255;
            cc[i+2] = b*255;
        }
    }

    return cc;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i = 64; i < 2048; i*=2)
        ui->sizeCbx->addItem(QString::number(i), QVariant(i));
    ui->dustCanvas->setFormat(QImage::Format_Grayscale8);
    ui->gravityCanvas->setFormat(QImage::Format_RGB888);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->progressBar->setVisible(false);
    ui->gravityBtn->setEnabled(false);
    connect(ui->dustCanvas, &Canvas::mark, this, &MainWindow::drawSpot);

    int rad = ui->colorCodeCanvas->width()/2;
    uchar* cc = colorCode(rad);
    ui->colorCodeCanvas->setFormat(QImage::Format_RGB888);
    ui->colorCodeCanvas->setBuffer(cc);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resize(Canvas *canvas, int size)
{
    QSize s2(size, size);
    canvas->setBuffer(nullptr);
    canvas->setMinimumSize(s2);
    canvas->setMaximumSize(s2);
}

void MainWindow::setCanvasSize()
{
    size = ui->sizeCbx->currentData().toInt();
    int zoomed_size = size * ui->zoomSb->value();
    resize(ui->dustCanvas, zoomed_size);
    resize(ui->gravityCanvas, zoomed_size);
    ui->gravityBtn->setEnabled(false);
}

void MainWindow::on_sizeCbx_currentIndexChanged(int index)
{
    setCanvasSize();
}

void MainWindow::on_dustBtn_clicked()
{
    dustField = QSharedPointer<DustField>(getRandomDustCanvas());
    ui->dustCanvas->setBuffer(dustField->buffer);
    ui->dustCanvas->repaint();
    ui->gravityBtn->setEnabled(true);
}


void MainWindow::on_gravityBtn_clicked()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    QApplication::processEvents();
    gravityField = QSharedPointer<GravityField>::create(*dustField.data());
    QFuture<void> f = QtConcurrent::run(gravityField.data(), &GravityField::generate);
    auto fw = new QFutureWatcher<void>();
    QTimer *timer = new QTimer;
    timer->setInterval(1000);
    timer->start();
    connect(timer, &QTimer::timeout, [=](){ui->progressBar->setValue(gravityField->progress());});
    connect(fw, &QFutureWatcherBase::finished, [=](){
        ui->progressBar->setVisible(false);
        ui->gravityCanvas->setBuffer(gravityField->encodedField());
        ui->gravityCanvas->repaint();
        timer->stop();
        delete timer;
    });
    fw->setFuture(f);
}

void MainWindow::drawSpot(int x, int y)
{
    int size = ui->penSizeSb->value();
    uchar color = ui->penColorSb->value();
    int w = dustField->width();
    int h = dustField->height();
    int x0 = x - size < 0 ? 0 : x - size;
    int x1 = x + size >= w ? w - 1 : x + size;
    int y0 = y - size < 0 ? 0 : y - size;
    int y1 = y + size >= h ? h - 1 : y + size;

    int s2 = size * size;
    for (int iy = y0; iy <= y1; ++iy){
        for (int ix = x0; ix <= x1; ++ix){
            int dx = ix - x;
            int dy = iy - y;
            if (dx * dx + dy * dy <= s2)
            {
                dustField->set(ix, iy, color);
            }
        }
    }
    ui->dustCanvas->repaint();
}

void MainWindow::on_zoomSb_valueChanged(int v)
{
    ui->dustCanvas->setZoom(v);
    ui->gravityCanvas->setZoom(v);
    setCanvasSize();
}
