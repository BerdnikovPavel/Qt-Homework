#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Stopwatch* stopwatch;

private slots:
    void RcvTimerValue(int min, int sec, int ms);
    void RcvTimerStatus(bool isTimerOn);
    void RcvCircle(int circleCounter, int circleTime);
    void RcvClear();
};
#endif // MAINWINDOW_H
