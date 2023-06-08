#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QObject>
#include <QTimer>

class Stopwatch : public QObject
{
    Q_OBJECT
public:
    explicit Stopwatch(QObject* parent = nullptr);
    ~Stopwatch();

private:
    QTimer* timer;
    int min, sec, ms;
    bool isTimerOn;
    int circleCounter;
    int circleTime;

signals:
    void sig_SendTimerValue(int min, int sec, int ms);
    void sig_SendTimerStatus(bool isTimerOn);
    void sig_SendCircleTime(int circleCounter, int circleTime);
    void sig_SendClear();

private slots:
    void SendTimerValue();
public slots:
    void SendTimerStatus();
    void SendCircleTime();
    void SendClear();
};

#endif // STOPWATCH_H
