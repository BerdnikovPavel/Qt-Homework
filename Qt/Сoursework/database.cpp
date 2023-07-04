#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    db = new QSqlDatabase();
    simpleQuery = new QSqlQuery();
    msg = new QMessageBox();
}

DataBase::~DataBase()
{
    delete db, queryModel, msg;
}

void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *db = QSqlDatabase::addDatabase(driver, nameDB);
}

void DataBase::ConnectToDataBase()
{
    db->setHostName("981757-ca08998.tmweb.ru");
    db->setDatabaseName("demo");
    db->setUserName("netology_usr_cpp");
    db->setPassword("CppNeto3");
    db->setPort(5432);
    db->open();
}

bool DataBase::get_StatusConnection()
{
    return db->isOpen();
}

void DataBase::RequestListOfFlights(QString request, int requestType)
{
    //*simpleQuery = QSqlQuery(*db);
    bool statusRequest = simpleQuery->exec(request);
    if(statusRequest)
    {
        queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(std::move(*simpleQuery));
        queryModel->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));
        queryModel->setHeaderData(1, Qt::Horizontal, tr("Время вылета"));
        switch(requestType)
        {
        case requestArrival:
            queryModel->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));
            break;
        case requestDeparture:
            queryModel->setHeaderData(2, Qt::Horizontal, tr("Аэропорт назначения"));
            break;
        }
        emit sig_SendDataFromDB_ListOfFlights(queryModel);


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

void DataBase::RequestListOfAirports(QString request)
{
    *simpleQuery = QSqlQuery(*db);
    bool statusRequest = simpleQuery->exec(request);
    if(statusRequest)
    {
        QMap<QString, QString> ap;
        while(simpleQuery->next())
        {
            ap.insert(simpleQuery->value(0).toString(), simpleQuery->value(1).toString());
        }
        emit sig_SendListOfAirports(ap);
    }
    else
    {
        msg->setText(simpleQuery->lastError().text());
        msg->show();
    }
}

QSqlError DataBase::GetLastError()
{
    return db->lastError();
}
