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

    connect(msg, &QMessageBox::accepted, this, [&](){
        timer->singleShot(50000, dataBase, SLOT(ConnectToDataBase()));
    });

    connect(dataBase, &DataBase::sig_SendListOfAirports, this, [&](QMap<QString, QString> ap){
        airports = ap;
        QMap<QString, QString>::const_iterator i = airports.constBegin();
        while(i != airports.constEnd())
        {
            ui->cb_listOfAirports->addItem(i.key());
            ++i;
        }
    });

    connect(dataBase, &DataBase::sig_SendDataFromDB_ListOfFlights, this, [this](QSqlQueryModel*model){
        ui->tv_flights->setModel(model);
    });

    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);
    //auto conn = [&]{dataBase->ConnectToDataBase();};
    //QtConcurrent::run(conn);
    dataBase->ConnectToDataBase();
    do
    {
        if(dataBase->get_StatusConnection())
        {
            ui->lb_statusConnect->setText("Подключено к БД");
            ui->lb_statusConnect->setStyleSheet("color:green");
        }
        else
        {
            dataBase->DisconnectFromDataBase(DB_NAME);
            msg->setIcon(QMessageBox::Critical);
            msg->setText(dataBase->GetLastError().text());
            ui->lb_statusConnect->setText("Отключено");
            ui->lb_statusConnect->setStyleSheet("color:red");
            msg->exec();
        }

    } while(dataBase->get_StatusConnection() == false);

    if(dataBase->get_StatusConnection())
    {
        dataBase->RequestListOfAirports(reqAirports);
    }
}

MainWindow::~MainWindow()
{
    dataBase->DisconnectFromDataBase();
    delete ui;
}



void MainWindow::on_pb_getListOfFlights_clicked()
{
    QString airportName = ui->cb_listOfAirports->currentText();
    if(ui->rb_arrival->isChecked())
    {
        int reqType = 1;
        QString reqArrival = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as Name from bookings.flights f "
                             "JOIN bookings.airports_data ad on ad.airport_code = " + QString(airports.value(airportName)) +
                             " where f.arrival_airport  = " + QString(airports.value(airportName));
        dataBase->RequestListOfFlights(reqArrival, reqType);
    }
    else
    {
        int reqType = 2;
        QString reqDeparture = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as Name from bookings.flights f "
                               "JOIN bookings.airports_data ad on ad.airport_code = " + QString(airports.value(airportName)) +
                               " WHERE f.departure_airport  = " + QString(airports.value(airportName));
        dataBase->RequestListOfFlights(reqDeparture, reqType);
    }
}

