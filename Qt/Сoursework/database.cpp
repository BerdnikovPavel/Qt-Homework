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
    db->open();
}

bool DataBase::get_StatusConnection()
{
    return db->isOpen();
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
