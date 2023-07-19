#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QComboBox>
#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "AirportsDB"
#define NUM_DATA_FOR_CONNECT_TO_DB 5

enum requestType{
    requestListOfAirports = 1,
    requestArrival = 2,
    requestDeparture   = 3,
    requestWorkloadPerYear = 4,
    requestWorkloadPerMonth = 5
};

class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void ConnectToDataBase();
    void DisconnectFromDataBase(QString nameDb = "");
    QSqlError GetLastError(void);
    void RequestToDB(QString request, int requestType);

signals:
    void sig_SendStatusConnection(bool statusConnection);
    void sig_SendListOfFlights(QSqlQueryModel *model);
    void sig_SendListOfAirports(QMap<QString, QString> airports);
    void sig_SendWorkloadPerYear(QVector<double>flightsCount);
    void sig_SendWorkloadPerMonth(QMap<QString, int> flightsPerDay);

public slots:
    void RequestGeneration(QString date, QString airportCode, int requestType);
    void RequestGeneration(QString airportCode, int requestType);

private:
    QSqlDatabase* db;
    QSqlQueryModel* queryModel;
    QSqlQuery* simpleQuery;
    QMessageBox *msg;
};

#endif // DATABASE_H
