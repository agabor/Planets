#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dustfield.h>
#include <QSharedPointer.h>

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
private:
    Ui::MainWindow *ui;
    QSharedPointer<DustField> dustCanvas;
};

#endif // MAINWINDOW_H
