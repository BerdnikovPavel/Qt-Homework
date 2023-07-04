#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QComboBox>
#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QMessageBox>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "AirportsDB"
#define NUM_DATA_FOR_CONNECT_TO_DB 5

enum requestType{
    requestArrival = 1,
    requestDeparture   = 2
};


class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestListOfAirports(QString request);
    void ReadAnswerFromDB(int answerType);
    QSqlError GetLastError(void);
    void ConnectToDataBase();
    bool get_StatusConnection();
    void RequestListOfFlights(QString request, int requestType);

signals:
    void sig_SendDataFromDB_ListOfFlights(QSqlQueryModel *model);
    //void sig_SendStatusConnection(bool);
    //void sig_SendStatusRequest(QSqlError err);
    void sig_SendListOfAirports(QMap<QString, QString> airports);

private:
    QSqlDatabase* db;
    QSqlQueryModel* queryModel;
    QSqlQuery* simpleQuery;
    QMessageBox *msg;

};

#endif // DATABASE_H
