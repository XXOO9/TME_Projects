#ifndef CDEVNFCTHREAD_H
#define CDEVNFCTHREAD_H

#include <QObject>
#include <QThread>
#include "CIdentificDevNFC.h"

class CDevNfcThread : public QObject
{
    Q_OBJECT
public:
    explicit CDevNfcThread(QObject *parent = nullptr);
    ~CDevNfcThread();

signals:
    void sigOpenNfc();
    void sigCloseNfc();

public slots:

private:
    QThread              m_nfcThread;
    CIdentificDevNFC     m_devNfc;
};

#endif // CDEVNFCTHREAD_H
