#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->rb_button_1->setText("Какая то кнопка №1");
    ui->rb_button_2->setText("Какая то кнопка №2");

    for(int i = 1; i <=5 ; ++i)
    {
        ui->cb_list->addItem(QString("Пункт № %1 текущего списка").arg(i));
    }

    ui->pb_acceleration->setText("Download acceleration");

    ui->pb_acceleration->setCheckable(true);

    ui->pBar_progress->setRange(0, 10);
    ui->pBar_progress->setValue(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_acceleration_toggled(bool checked)
{

}


void MainWindow::on_pb_acceleration_clicked()
{
    if(ui->pBar_progress->value() == 10)
    {
        ui->pBar_progress->setValue(0);
    }
    else
    {
        ui->pBar_progress->setValue(ui->pBar_progress->value() + 1);
    }
}

