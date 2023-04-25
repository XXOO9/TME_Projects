#ifndef HTTPDOWNLOADTHREAD_H
#define HTTPDOWNLOADTHREAD_H

#include "common_global.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QUrl>
#include <QFile>
#include <QFileInfo>

#include "CHttpDownloadFromUrl.h"

class COMMONSHARED_EXPORT HttpDownloadThread : public QObject
{
    Q_OBJECT
//public:
//    explicit HttpDownloadThread(QObject *parent = nullptr);

//    ~HttpDownloadThread();

//    void startDownloadFies( QVector<structSampleVideoSync> vecSampleVideoInfos );

//    void initConnection();

//    void setDownloadDir( QString dir );

//public slots:
//    void onHttpDownloadComplete();

//    void onHttpDownloading( QString downloadPersent );

//signals:
//    void signalStartDownloadFiles( QStringList urls );

//private:
//    QThread *m_pThread;
//    CHttpDownloadFromUrl *m_pDownloader;

};

#endif // HTTPDOWNLOADTHREAD_H
