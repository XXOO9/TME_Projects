#ifndef CHIGHTPERFORMANCETIMECOUNTER_H
#define CHIGHTPERFORMANCETIMECOUNTER_H

#include <QObject>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>

class CHightPerformanceTimeCounter : public QObject
{
    Q_OBJECT
public:
    explicit CHightPerformanceTimeCounter(QObject *parent = nullptr);
    virtual ~CHightPerformanceTimeCounter() override;

    void start();

    QString calculateDiffenenceTillNow();

private:
    QString translate2formatString( const qint64 &val );
private:
    QDateTime    m_start;
    QDateTime    m_end;
};

#endif // CHIGHTPERFORMANCETIMECOUNTER_H
