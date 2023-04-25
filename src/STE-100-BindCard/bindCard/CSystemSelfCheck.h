#ifndef CSYSTEMSELFCHECK_H
#define CSYSTEMSELFCHECK_H

#include <QObject>
#include <QMutex>

class CSystemSelfCheck : public QObject
{
    Q_OBJECT
public:
    explicit CSystemSelfCheck(QObject *parent = nullptr);
};

#endif // CSYSTEMSELFCHECK_H
