#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <dustcanvas.h>
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
    DustCanvas * getRandomDustCanvas();
private:
    Ui::MainWindow *ui;
    QSharedPointer<DustCanvas> dustCanvas;
};

#endif // MAINWINDOW_H
