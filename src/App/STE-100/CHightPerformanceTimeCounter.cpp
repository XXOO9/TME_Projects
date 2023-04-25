#include "CHightPerformanceTimeCounter.h"

CHightPerformanceTimeCounter::CHightPerformanceTimeCounter(QObject *parent) : QObject(parent)
{
    m_start = QDateTime::currentDateTime();
}

CHightPerformanceTimeCounter::~CHightPerformanceTimeCounter()
{

}

void CHightPerformanceTimeCounter::start()
{
    m_start = QDateTime::currentDateTime();
}

QString CHightPerformanceTimeCounter::calculateDiffenenceTillNow()
{
    m_end = QDateTime::currentDateTime();
    qint64 diffenence = m_start.msecsTo( m_end );
    return translate2formatString( diffenence );
}

QString CHightPerformanceTimeCounter::translate2formatString(const qint64 &val)
{
    static short persentMsecs;
    static short secs;
    static short mins;
    static QString retString;

    persentMsecs = val % 1000 / 100;
    secs = val / 1000;
    mins = secs / 60;


    retString =  QString::number( mins ) + ":" +  QString::number( secs % 60 ) + "." + QString::number( persentMsecs );

    return retString;
}
