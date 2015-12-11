#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dustfield.h"
#include <math.h>

#include <QDebug>
#include <QTime>
#include "gravityfield.h"



DustField * MainWindow::getRandomDustCanvas()
{
    const int w0 = 10;
    const int h0 = 10;
    QTime now = QTime::currentTime();
    qsrand(now.msec());
    DustField *dc0 = new DustField(w0,h0);
    for (int y = 0; y < h0; ++y){
        for (int x = 0; x < w0; ++x){
            dc0->set(x, y, qrand() % 256);
        }
    }

    //DustField *dc1 = dc0->upscale(2, 10);
    //DustField *dc2 = dc1->upscale(2, 10);
    //DustField *dc3 = dc2->upscale(2, 10);
    DustField *dc4 = dc0->upscale(10, 10);

    delete dc0;
    //delete dc1;
    //delete dc2;
   // delete dc3;

    return dc4;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dustCanvas = QSharedPointer<DustField>(getRandomDustCanvas());
    GravityField *g = new GravityField(*dustCanvas.data());
    ui->widget->setBuffer(g->encodedField());
}

MainWindow::~MainWindow()
{
    delete ui;
}
