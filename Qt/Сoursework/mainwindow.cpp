#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lb_statusConnect->setText("Отключено");
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->de_date->setDateRange(QDate(2016, 8, 15), QDate(2017, 9, 14));
    ui->rb_arrival->setChecked(true);

    dataBase = new DataBase(this);
    msg = new QMessageBox(this);
    timer = new QTimer(this);
    wlWindow = new WorkloadWindow(this);
    wlWindow->setWindowTitle("Загруженность аэропорта");

    connect(timer, &QTimer::timeout, this, [&](){
        dataBase->ConnectToDataBase();
    });

    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnection);

    connect(dataBase, &DataBase::sig_SendListOfAirports, this, [&](QMap<QString, QString> ap){
        airports = ap;
        QMap<QString, QString>::const_iterator i = airports.constBegin();
        while(i != airports.constEnd())
        {
            ui->cb_listOfAirports->addItem(i.key());
            ++i;
        }
    });

    connect(dataBase, &DataBase::sig_SendListOfFlights, this, [this](QSqlQueryModel*model){
        QHeaderView* header = ui->tv_flights->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::Stretch);
        ui->tv_flights->setModel(model);
        ui->tv_flights->show();
    });

    connect(this, &MainWindow::sig_sendReqListOfFlights, dataBase, qOverload<QString, QString, int>(&DataBase::RequestGeneration));
    connect(this, &MainWindow::sig_sendReqWorkload, dataBase, qOverload<QString, int>(&DataBase::RequestGeneration));

    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);
    dataBase->ConnectToDataBase();

    ui->pb_getListOfFlights->setEnabled(false);
    ui->pb_getWorkload->setEnabled(false);

    connect(dataBase, &DataBase::sig_SendWorkloadPerYear, this, &MainWindow::DisplayWorkloadPerYear);
    connect(dataBase, &DataBase::sig_SendWorkloadPerMonth, this, &MainWindow::DisplayWorkloadPerMonth);
    connect(this, &MainWindow::sig_SendAirportName, wlWindow, &WorkloadWindow::reciveAirportName);
}

MainWindow::~MainWindow()
{
    dataBase->DisconnectFromDataBase();
    delete ui;
}

void MainWindow::ReceiveStatusConnection(bool statusConnection)
{
    if(statusConnection)
    {
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");

        int reqType = requestListOfAirports;
        dataBase->RequestToDB(reqAirports, reqType);
        ui->cb_listOfAirports->setCurrentIndex(-1);
    }
    else
    {
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
        timer->start(5000);
    }
}

void MainWindow::on_pb_getListOfFlights_clicked()
{
    QString date = ui->de_date->date().toString();
    QString airportCode = airports.value(ui->cb_listOfAirports->currentText());
    if(ui->rb_arrival->isChecked())
    {
        int reqType = requestArrival;
        emit sig_sendReqListOfFlights(date, airportCode, reqType);
    }
    else
    {
        int reqType = requestDeparture;
        emit sig_sendReqListOfFlights(date, airportCode, reqType);
    }
}

void MainWindow::on_pb_getWorkload_clicked()
{
    QString airportCode = airports.value(ui->cb_listOfAirports->currentText());
    int reqType = requestWorkloadPerYear;
    emit sig_sendReqWorkload(airportCode, reqType);
    reqType = requestWorkloadPerMonth;
    emit sig_sendReqWorkload(airportCode, reqType);
}

void MainWindow::DisplayWorkloadPerYear(QVector<double>flightsCount)
{
    wlWindow->setModal(true);
    wlWindow->show();
    wlWindow->DisplayGraphicPerYear(flightsCount);
}

void MainWindow::DisplayWorkloadPerMonth(QMap<QString, int> fPerDay)
{
    wlWindow->DisplayGraphicPerMonth(fPerDay);
}

void MainWindow::on_cb_listOfAirports_currentIndexChanged()
{
    QString airportName = ui->cb_listOfAirports->currentText();
    emit sig_SendAirportName(airportName);
    ui->pb_getListOfFlights->setEnabled(true);
    ui->pb_getWorkload->setEnabled(true);
}
