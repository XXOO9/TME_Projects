#ifndef CNFCTHREAD_H
#define CNFCTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "CIdentificDevNFC.h"

class CNFCThread : public QThread
{
    Q_OBJECT
public:
    CNFCThread();
    virtual ~CNFCThread() override;


    virtual void run() override;

signals:
    void sigNewCardDeceted( QString &newCardNumber );
    void sigCardExisted( QString &newCardNumber );


private slots:
    void onNewCardDeceted( QString cardNumber );
    void onDecedtedCardStateChanged( bool existed );
private:
    void initConnection();

private:
    CIdentificDevNFC    m_nfc;
};

#endif // CNFCTHREAD_H
