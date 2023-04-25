#include "CNFCThread.h"

#include <QEventLoop>

CNFCThread::CNFCThread()
{
    initConnection();
}

CNFCThread::~CNFCThread()
{
}

void CNFCThread::run()
{
    QEventLoop loop;

    CIdentificDevNFC nfc;

    connect( &nfc, &CIdentificDevNFC::signalFindCard, this, &CNFCThread::onNewCardDeceted, Qt::QueuedConnection );
    connect( &nfc, &CIdentificDevNFC::sigDecetedCard, this, &CNFCThread::onDecedtedCardStateChanged, Qt::QueuedConnection );

    nfc.open( true );

    loop.exec();

}

void CNFCThread::onNewCardDeceted(QString cardNumber)
{
    qDebug() << "new card find, number = " << cardNumber;
    emit sigNewCardDeceted( cardNumber );
}

void CNFCThread::onDecedtedCardStateChanged(bool existed)
{
    if( existed ){
        return;
    }
    QString noCardFlag = "-1";
    emit sigCardExisted( noCardFlag );
}

void CNFCThread::initConnection()
{
    connect( &m_nfc, &CIdentificDevNFC::signalFindCard, this, &CNFCThread::onNewCardDeceted, Qt::QueuedConnection );
}
