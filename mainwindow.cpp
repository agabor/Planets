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

    DustField *dc = nullptr;
    do {
        dc = dc0->upscale(2, 10);
        delete dc0;
        dc0 = dc;

    } while (dc->width() < size);

    return dc;
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

void MainWindow::on_sizeCbx_currentIndexChanged(int index)
{
    size = ui->sizeCbx->itemData(index).toInt();
    resize(ui->dustCanvas, size);
    resize(ui->gravityCanvas, size);
}

void MainWindow::on_dustBtn_clicked()
{
    dustField = QSharedPointer<DustField>(getRandomDustCanvas());
    ui->dustCanvas->setBuffer(dustField->buffer);
    ui->dustCanvas->repaint();
}


void MainWindow::on_gravityBtn_clicked()
{
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
