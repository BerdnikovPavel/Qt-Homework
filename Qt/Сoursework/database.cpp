#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    db = new QSqlDatabase();
    simpleQuery = new QSqlQuery();
    queryModel = new QSqlQueryModel(this);
    msg = new QMessageBox();
}

DataBase::~DataBase()
{
    delete db, msg, simpleQuery;
}

void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *db = QSqlDatabase::addDatabase(driver, nameDB);
}

void DataBase::ConnectToDataBase()
{
    db->setHostName("981757-ca08998.tmweb.ru");
    db->setDatabaseName("demo");
    db->setUserName("netology_usr_cp");
    db->setPassword("CppNeto3");
    db->setPort(5432);
    bool statusConnection = db->open();
    if(!statusConnection)
    {
        this->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(this->GetLastError().text());
        msg->exec();
        msg->setModal(true);
        QThread::msleep(5000);
        //QTimer::singleShot(5000, this, &DataBase::ConnectToDataBase);
        this->ConnectToDataBase();
    }
}

bool DataBase::get_StatusConnection()
{
    return db->isOpen();
}

void DataBase::RequestGeneration(QString date, QString airportCode, int requestType)
{
    QString request;
    if(requestType == requestArrival)
    {
        request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' as Name from bookings.flights f "
                  "JOIN bookings.airports_data ad on ad.airport_code = '" + QString(airportCode) +
                  "' where f.arrival_airport  = '" + QString(airportCode) + "' and "
                  "f.scheduled_arrival::date = '" + QString(date) + "'";
    }
    if(requestType == requestDeparture)
    {
        request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' as Name from bookings.flights f "
                  "JOIN bookings.airports_data ad on ad.airport_code = '" + QString(airportCode) +
                  "' WHERE f.departure_airport  = '" + QString(airportCode) + "' and "
                  "f.scheduled_departure::date = '" + QString(date) + "'";
    }
    this->RequestToDB(request, requestType);
}

void DataBase::RequestGeneration(QString airportCode, int requestType)
{
    QString request;
    if(requestType == requestWorkloadPerYear)
    {
        request  = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as Month from bookings.flights f "
                   "WHERE (scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
                   "and ( departure_airport = '" + QString(airportCode) + "' or arrival_airport = '"
                   + QString(airportCode) + "') group by Month";
    }
    if(requestType == requestWorkloadPerMonth)
    {
        request = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as Day from bookings.flights f "
                  "WHERE(scheduled_departure::date > date('2016-08-31') and scheduled_departure::date <= date('2017-08-31')) "
                  "and ( departure_airport = '" + QString(airportCode) + "' or arrival_airport = '"
                  + QString(airportCode) + "') GROUP BY Day";
    }
    this->RequestToDB(request, requestType);
}

void DataBase::RequestToDB(QString request, int requestType)
{
    *simpleQuery = QSqlQuery(*db);
    bool statusRequest = simpleQuery->exec(request);
    if(statusRequest)
    {
        switch(requestType)
        {
        case requestListOfAirports:
        {
            QMap<QString, QString> ap;
            while(simpleQuery->next())
            {
                ap.insert(simpleQuery->value(0).toString(), simpleQuery->value(1).toString());
            }
            emit sig_SendListOfAirports(ap);
            break;
        }
        case requestWorkloadPerYear:
        {
            QVector<double> flightsCount;
            QVector<QString> months;
            while(simpleQuery->next())
            {
                flightsCount.push_back(simpleQuery->value(0).toInt());
                months.push_back(simpleQuery->value(1).toString());
            }
            emit sig_SendWorkloadPerYear(flightsCount);
            break;
        }
        case requestWorkloadPerMonth:
        {
            QMap<QString, int> flightsPerDay;
            while(simpleQuery->next())
            {
                flightsPerDay.insert(simpleQuery->value(1).toString().remove(10, 20), simpleQuery->value(0).toInt());
            }
            emit sig_SendWorkloadPerMonth(flightsPerDay);
            break;
        }
        default:
        {

            queryModel->setQuery(std::move(*simpleQuery));
            queryModel->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
            queryModel->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
            if(requestType == requestArrival)
            {
                queryModel->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));
            }
            else
            {
                queryModel->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));
            }
            emit sig_SendListOfFlights(queryModel);
            break;
        }
        }
    }
    else
    {
        msg->setText(simpleQuery->lastError().text());
        msg->show();
    }
}

void DataBase::DisconnectFromDataBase(QString nameDb)
{
    *db = QSqlDatabase::database(nameDb);
    db->close();
}

QSqlError DataBase::GetLastError()
{
    return db->lastError();
}
