#ifndef WORKLOADWINDOW_H
#define WORKLOADWINDOW_H

#include <QDialog>
#include "qcustomplot.h"

namespace Ui {
class WorkloadWindow;
}

class WorkloadWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WorkloadWindow(QWidget *parent = nullptr);
    ~WorkloadWindow();
    void DisplayGraphicPerYear(QVector<double> flightsCount);
    void DisplayGraphicPerMonth(QMap<QString, int> fPerDay);

private slots:
    void on_cb_months_currentIndexChanged();

public slots:
    void reciveAirportName(QString airportName);

private:
    Ui::WorkloadWindow *ui;
    QCPGraph* graphic;
    QMap<QString, int> flightsPerDay;
};

#endif // WORKLOADWINDOW_H
