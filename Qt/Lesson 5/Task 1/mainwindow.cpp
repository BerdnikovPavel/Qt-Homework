#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stopwatch = new Stopwatch(this);
    ui->pb_circle->setEnabled(false);
    ui->pb_start->setText("СТАРТ");
    ui->pb_clear->setText("ОЧИСТИТЬ");
    ui->pb_circle->setText("КРУГ");
    ui->tl_time->setText("0:00:0");

    connect(stopwatch, &Stopwatch::sig_SendTimerValue, this, &MainWindow::RcvTimerValue);
    connect(stopwatch, &Stopwatch::sig_SendTimerStatus, this, &MainWindow::RcvTimerStatus);
    connect(stopwatch, &Stopwatch::sig_SendCircleTime, this, &MainWindow::RcvCircle);
    connect(stopwatch, &Stopwatch::sig_SendClear, this, &MainWindow::RcvClear);

    connect(ui->pb_start, &QPushButton::clicked, stopwatch, &Stopwatch::SendTimerStatus);
    connect(ui->pb_circle, &QPushButton::clicked, stopwatch, &Stopwatch::SendCircleTime);
    connect(ui->pb_clear, &QPushButton::clicked, stopwatch, &Stopwatch::SendClear);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RcvTimerValue(int min, int sec, int ms)
{
    if(sec<10)
    {
        ui->tl_time->setText(QString::number(min) + ":0" + QString::number(sec) + ":" + QString::number(ms));
    }
    else
    {
        ui->tl_time->setText(QString::number(min) + ":" + QString::number(sec) + ":" + QString::number(ms));
    }
}

void MainWindow::RcvTimerStatus(bool isTimerOn)
{
    if(isTimerOn)
    {
        ui->pb_start->setText("СТОП");
        ui->pb_circle->setEnabled(true);
    }
    else
    {
        ui->pb_start->setText("СТАРТ");
        ui->pb_circle->setEnabled(false);
    }
}

void MainWindow::RcvCircle(int circleCounter, int circleTime)
{
    ui->tb_circlsTime->append("Круг " + QString::number(circleCounter) + ", время: " + QString::number(circleTime) + " сек.");
}

void MainWindow::RcvClear()
{
    ui->tl_time->setText("0:00:0");
}

