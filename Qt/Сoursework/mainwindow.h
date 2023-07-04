#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QTimer>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

signals:
    void sig_connectToDB();
private slots:
    void on_pb_getListOfFlights_clicked();

private:
    Ui::MainWindow *ui;
    DataBase *dataBase;
    QMessageBox *msg;
    QTimer *timer;
    QString reqAirports = "SELECT airport_name->>'ru' as airportName, airport_code FROM bookings.airports_data";
    QMap<QString, QString> airports;
};
#endif // MAINWINDOW_H
