#include "workloadwindow.h"
#include "ui_workloadwindow.h"

WorkloadWindow::WorkloadWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkloadWindow)
{
    ui->setupUi(this);
    QVector<QString> months;
    months << "Янаварь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль"
           << "Август" << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь";
    ui->cb_months->addItems(months);

    graphic = new QCPGraph(ui->cp_graphicPerMonth->xAxis, ui->cp_graphicPerMonth->yAxis);
    ui->cp_graphicPerMonth->setInteraction(QCP::iRangeZoom, true);
    ui->cp_graphicPerMonth->setInteraction(QCP::iRangeDrag, true);
}

WorkloadWindow::~WorkloadWindow()
{
    delete ui;
}

void WorkloadWindow::DisplayGraphicPerYear(QVector<double> flightsCount)
{
    //ui->lb_airportName->setText("Выбранный аэропорт: " + QString(airportName));
    ui->cp_graphicPerYear->clearPlottables();

    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->cp_graphicPerYear->setBackground(QBrush(gradient));

    QCPBars *flights = new QCPBars(ui->cp_graphicPerYear->xAxis, ui->cp_graphicPerYear->yAxis);
    //fossil->setAntialiased(false);
    //fossil->setStackingGap(1);
    flights->setName("Общее количество рейсов за месяц");
    flights->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    flights->setBrush(QColor(111, 9, 176));

    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    labels << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь" << "Янаварь" << "Февраль"
           << "Март" << "Апрель" << "Май" << "Июнь" << "Июль" << "Август";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->cp_graphicPerYear->xAxis->setTicker(textTicker);
    ui->cp_graphicPerYear->xAxis->setTickLabelRotation(60);
    ui->cp_graphicPerYear->xAxis->setSubTicks(false);
    ui->cp_graphicPerYear->xAxis->setTickLength(0, 4);
    ui->cp_graphicPerYear->xAxis->setRange(0, 13);
    ui->cp_graphicPerYear->xAxis->setBasePen(QPen(Qt::white));
    ui->cp_graphicPerYear->xAxis->setTickPen(QPen(Qt::white));
    ui->cp_graphicPerYear->xAxis->grid()->setVisible(true);
    ui->cp_graphicPerYear->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->cp_graphicPerYear->xAxis->setTickLabelColor(Qt::white);
    ui->cp_graphicPerYear->xAxis->setLabelColor(Qt::white);

    double maxY = flightsCount[0];
    for(int i = 0; i<flightsCount.size(); ++i)
    {
        if(flightsCount[i] > maxY){maxY = flightsCount[i];}
    }
    maxY += maxY/2;

    ui->cp_graphicPerYear->yAxis->setRange(0, maxY);
    ui->cp_graphicPerYear->yAxis->setPadding(5);
    ui->cp_graphicPerYear->yAxis->setLabel("Количество рейсов");
    ui->cp_graphicPerYear->yAxis->setBasePen(QPen(Qt::white));
    ui->cp_graphicPerYear->yAxis->setTickPen(QPen(Qt::white));
    ui->cp_graphicPerYear->yAxis->setSubTickPen(QPen(Qt::white));
    ui->cp_graphicPerYear->yAxis->grid()->setSubGridVisible(true);
    ui->cp_graphicPerYear->yAxis->setTickLabelColor(Qt::white);
    ui->cp_graphicPerYear->yAxis->setLabelColor(Qt::white);
    ui->cp_graphicPerYear->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->cp_graphicPerYear->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    flights->setData(ticks, flightsCount);

    ui->cp_graphicPerYear->legend->setVisible(true);
    ui->cp_graphicPerYear->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->cp_graphicPerYear->legend->setBrush(QColor(255, 255, 255, 100));
    ui->cp_graphicPerYear->legend->setBorderPen(Qt::NoPen);

    ui->cp_graphicPerYear->replot();
}

void WorkloadWindow::DisplayGraphicPerMonth(QMap<QString, int> fPerDay)
{
    flightsPerDay = fPerDay;
    QVector<double> flightsCount;
    QVector<QString> date;
    QVector<double> x;
    int currentMonth = ui->cb_months->currentIndex();
    QString currentMonthNumber;
    switch(currentMonth)
    {
    case 0:
        currentMonthNumber = "01";
        break;
    case 1:
        currentMonthNumber = "02";
        break;
    case 2:
        currentMonthNumber = "03";
        break;
    case 3:
        currentMonthNumber = "04";
        break;
    case 4:
        currentMonthNumber = "05";
        break;
    case 5:
        currentMonthNumber = "06";
        break;
    case 6:
        currentMonthNumber = "07";
        break;
    case 7:
        currentMonthNumber = "08";
        break;
    case 8:
        currentMonthNumber = "09";
        break;
    case 9:
        currentMonthNumber = "10";
        break;
    case 10:
        currentMonthNumber = "11";
        break;
    case 11:
        currentMonthNumber = "12";
        break;
    }
    QMap<QString, int>::const_iterator i = flightsPerDay.constBegin();
    while(i != flightsPerDay.constEnd())
    {
        if(i.key().mid(5, 2) == currentMonthNumber)
        {
            flightsCount.push_back(i.value());
            date.push_back(i.key());
        }
        ++i;
    }

    for(int i = 0; i < flightsCount.size(); ++i)
    {
        x.push_back(i);
    }

    ui->cp_graphicPerMonth->xAxis->setRange(0, flightsCount.size()+1);

    double minY = flightsCount[0];
    double maxY = flightsCount[0];
    for(int i = 0; i<flightsCount.size(); ++i)
    {
        if(flightsCount[i] < minY){minY = flightsCount[i];}
        if(flightsCount[i] > maxY){maxY = flightsCount[i];}
    }

    minY -= minY/5;
    maxY += maxY/5;

    ui->cp_graphicPerMonth->yAxis->setRange(minY, maxY);

    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->cp_graphicPerMonth->setBackground(QBrush(gradient));

    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(111, 9, 176));
    ui->cp_graphicPerMonth->graph()->setPen(pen);

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(x, date);
    ui->cp_graphicPerMonth->xAxis->setTicker(textTicker);
    ui->cp_graphicPerMonth->xAxis->setTickLabelRotation(90);
    ui->cp_graphicPerMonth->xAxis->setBasePen(QPen(Qt::white));
    ui->cp_graphicPerMonth->xAxis->setTickPen(QPen(Qt::white));
    ui->cp_graphicPerMonth->xAxis->setTickLabelColor(Qt::white);
    ui->cp_graphicPerMonth->xAxis->setLabelColor(Qt::white);

    ui->cp_graphicPerMonth->yAxis->setLabel("Количество рейсов");
    ui->cp_graphicPerMonth->yAxis->setBasePen(QPen(Qt::white));
    ui->cp_graphicPerMonth->yAxis->setTickPen(QPen(Qt::white));
    ui->cp_graphicPerMonth->yAxis->setSubTickPen(QPen(Qt::white));
    ui->cp_graphicPerMonth->yAxis->setTickLabelColor(Qt::white);
    ui->cp_graphicPerMonth->yAxis->setLabelColor(Qt::white);

    ui->cp_graphicPerMonth->graph(0)->setData(x, flightsCount);
    ui->cp_graphicPerMonth->replot();
}

void WorkloadWindow::on_cb_months_currentIndexChanged()
{
    if(!flightsPerDay.empty())
    {
        DisplayGraphicPerMonth(flightsPerDay);
    }
}

void WorkloadWindow::reciveAirportName(QString airportName)
{
    ui->lb_airportName->setText("Выбранный аэропорт: " + QString(airportName));
}

