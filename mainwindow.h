#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dustfield.h>
#include <QSharedPointer.h>
#include "canvas.h"
#include "gravityfield.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    DustField * getRandomDustCanvas();
    void Resize(int size);
    void resize(Canvas *canvas, int size);
private slots:
    void on_sizeCbx_currentIndexChanged(int index);

    void on_dustBtn_clicked();

    void on_gravityBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSharedPointer<DustField> dustField;
    QSharedPointer<GravityField> gravityField;
    int size;
};

#endif // MAINWINDOW_H
