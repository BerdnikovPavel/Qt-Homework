#include "stopwatch.h"

Stopwatch::Stopwatch(QObject*parent) : QObject{parent}
{
    min = 0;
    sec = 0;
    ms = 0;
    isTimerOn = false;
    timer = new QTimer(this);
    circleCounter = 0;
    circleTime = 0;
    connect(timer, &QTimer::timeout, this, &Stopwatch::SendTimerValue);
}

Stopwatch::~Stopwatch()
{

}

void Stopwatch::SendTimerValue()
{
    ms++;
    if(ms >= 10)
    {
        ms = 0;
        sec++;
        circleTime++;
    }
    if(sec >= 60)
    {
        sec = 0;
        min++;
    }

    emit sig_SendTimerValue(min, sec, ms);
}

void Stopwatch::SendTimerStatus()
{
    isTimerOn = !isTimerOn;
    if(isTimerOn)
    {
        timer->start(100);
    }
    else
    {
        timer->stop();
    }
    emit sig_SendTimerStatus(isTimerOn);
}

void Stopwatch::SendCircleTime()
{
    ++circleCounter;
    emit sig_SendCircleTime(circleCounter, circleTime);
    circleTime = 0;
}

void Stopwatch::SendClear()
{
    circleCounter = 0;
    circleTime = 0;
    min = 0;
    sec = 0;
    ms = 0;
    emit sig_SendClear();
}
