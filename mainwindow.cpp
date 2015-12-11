#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dustcanvas.h"
#include <math.h>

#include <QDebug>
#include <QTime>



DustCanvas * MainWindow::getRandomDustCanvas()
{
    const int w0 = 10;
    const int h0 = 10;
    QTime now = QTime::currentTime();
    qsrand(now.msec());
    DustCanvas *dc0 = new DustCanvas(w0,h0);
    for (int y = 0; y < h0; ++y){
        for (int x = 0; x < w0; ++x){
            dc0->set(x, y, qrand() % 256);
        }
    }

    DustCanvas *dc1 = dc0->upscale(2, 10);
    DustCanvas *dc2 = dc1->upscale(2, 10);
    DustCanvas *dc3 = dc2->upscale(2, 10);
    DustCanvas *dc4 = dc3->upscale(10, 10);

    delete dc0;
    delete dc1;
    delete dc2;
    delete dc3;

    return dc4;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dustCanvas = QSharedPointer<DustCanvas>(getRandomDustCanvas());
    ui->widget->setBuffer(dustCanvas->buffer);
}

MainWindow::~MainWindow()
{
    delete ui;
}
