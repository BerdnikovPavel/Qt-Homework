#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();


    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);
    connect(udpWorker, &UDPworker::sig_sendMessageToGUI, this, &MainWindow::DisplayMessage);


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
    ui->pb_start->setEnabled(false);
    ui->pb_stop->setEnabled(true);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}

void MainWindow::DisplayMessage(QNetworkDatagram datagram)
{
    QByteArray msg;
    msg = datagram.data();


    QDataStream inStr(&msg, QIODevice::ReadOnly);
    QString message;
    inStr >> message;

    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Принято сообщение от " + datagram.senderAddress().toString()
                          + ", размер сообщения(байт) " + QString::number(sizeof(message)));

}


void MainWindow::on_pb_stop_clicked()
{
    ui->pb_start->setEnabled(true);
    timer->stop();
}


void MainWindow::on_pb_sendDatagram_clicked()
{
    QString message = ui->le_message->text();
    QByteArray dataToSend;
    QDataStream streamOut(&dataToSend, QIODevice::WriteOnly);
    streamOut << message;
    udpWorker->SendDatagram_2(dataToSend);
}

