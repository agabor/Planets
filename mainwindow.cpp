#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dustcanvas.h"
#include <math.h>

#include <QDebug>



DustCanvas * MainWindow::getRandomDustCanvas()
{
    int scale = 100;
    const int w0 = ui->widget->width() / scale;
    const int h0 = ui->widget->height() / scale;
    DustCanvas *dc0 = new DustCanvas(w0,h0);
    for (int y = 0; y < h0; ++y){
        for (int x = 0; x < w0; ++x){
            dc0->set(x, y, qrand() % 256);
        }
    }

    DustCanvas *dc1 = dc0->upscale(10);
    DustCanvas *dc2 = dc1->upscale(10);

    delete dc0;
    delete dc1;

    return dc2;
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
