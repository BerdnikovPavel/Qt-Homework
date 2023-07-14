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

    connect(msg, &QMessageBox::accepted, this, [&](){
        timer->singleShot(5000, dataBase, &DataBase::ConnectToDataBase);
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

    connect(dataBase, &DataBase::sig_SendListOfFlights, this, [this](QSqlQueryModel*model){
        QHeaderView* header = ui->tv_flights->horizontalHeader();
        header->setSectionResizeMode(QHeaderView::Stretch);
        ui->tv_flights->setModel(model);
        ui->tv_flights->show();
    });

    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);
    dataBase->ConnectToDataBase();
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

    if(dataBase->get_StatusConnection())
    {
        int reqType = requestListOfAirports;
        dataBase->RequestToDB(reqAirports, reqType);
        ui->cb_listOfAirports->setCurrentIndex(-1);
    }

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

void MainWindow::on_pb_getListOfFlights_clicked()
{
    QString date = ui->de_date->date().toString();
    QString airportName = ui->cb_listOfAirports->currentText();
    if(ui->rb_arrival->isChecked())
    {
        int reqType = requestArrival;
        QString reqArrival = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as Name from bookings.flights f "
                             "JOIN bookings.airports_data ad on ad.airport_code = '" + QString(airports.value(airportName)) +
                             "' where f.arrival_airport  = '" + QString(airports.value(airportName)) + "' and "
                             "f.scheduled_arrival::date = '" + QString(date) + "'";
        dataBase->RequestToDB(reqArrival, reqType);
    }
    else
    {
        int reqType = requestDeparture;
        QString reqDeparture = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as Name from bookings.flights f "
                               "JOIN bookings.airports_data ad on ad.airport_code = '" + QString(airports.value(airportName)) +
                               "' WHERE f.departure_airport  = '" + QString(airports.value(airportName)) + "' and "
                               "f.scheduled_departure::date = '" + QString(date) + "'";
        dataBase->RequestToDB(reqDeparture, reqType);
    }
}


void MainWindow::on_pb_getWorkload_clicked()
{
    QString airportName = ui->cb_listOfAirports->currentText();
    QString reqWorkloadPerYear = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as Month from bookings.flights f "
            "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
            "and ( departure_airport = '" + QString(airports.value(airportName)) + "' or arrival_airport = '"
            + QString(airports.value(airportName)) + "') group by Month";

    int reqType = requestWorkloadPerYear;
    dataBase->RequestToDB(reqWorkloadPerYear, reqType);

    QString reqWorkloadPerMonth = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as Day from bookings.flights f "
            "WHERE(scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
            "and ( departure_airport = '" + QString(airports.value(airportName)) + "' or arrival_airport = '"
            + QString(airports.value(airportName)) + "') GROUP BY Day";

    reqType = requestWorkloadPerMonth;
    dataBase->RequestToDB(reqWorkloadPerMonth, reqType);
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

