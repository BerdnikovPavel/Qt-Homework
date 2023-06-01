#include <QCoreApplication>
#include <QtNetwork>
#include <QtSql>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpSocket s;

    QSqlDatabase db;

    return a.exec();
}
