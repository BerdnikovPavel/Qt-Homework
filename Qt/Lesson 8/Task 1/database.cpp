#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase = new QSqlDatabase();
    simpleQuery = new QSqlQuery();
}

DataBase::~DataBase()
{
    delete dataBase, queryModel;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{
    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());


    ///Тут должен быть код ДЗ


    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request)
{
    if(request == "")
    {
        tableModel = new QSqlTableModel(this, *dataBase);
        tableModel->setTable("film");
        bool statusRequest = tableModel->select();
        if(statusRequest)
        {
            QSqlError err;
            emit sig_SendStatusRequest(err);
        }
        else
        {
            emit sig_SendStatusRequest(tableModel->lastError());
        }
    }
    else
    {
        simpleQuery = new QSqlQuery(*dataBase);
        bool statusRequest = simpleQuery->exec(request);
        if(statusRequest)
        {
            QSqlError err;
            emit sig_SendStatusRequest(err);
        }
        else
        {
            emit sig_SendStatusRequest(simpleQuery->lastError());
        }
    }
}

void DataBase::ReadAnswerFromDB(int requestType)
{
    switch(requestType)
    {
    case requestAllFilms:
        tableModel->removeColumn(0);
        tableModel->removeColumns(2, 11);
        tableModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        tableModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        emit sig_SendDataFromDB_TM(tableModel);
        break;
    default:
        queryModel = new QSqlQueryModel(this);
        queryModel->setQuery(std::move(*simpleQuery));

        queryModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
        queryModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
        emit sig_SendDataFromDB_QM(queryModel);
    }
}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
