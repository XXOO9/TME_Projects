#ifndef CHTTPDOWNLOADFROMURL_H
#define CHTTPDOWNLOADFROMURL_H

#include "common_global.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QVector>
#include <QDateTime>
#include "CHttpConnection.h"
#include <QMetaType>
#include <QVariant>

//Q_DECLARE_METATYPE( QVector< structSampleVideoSync > );


typedef struct sampleVideoInfo
{
    QString typeId;
    QString url;
    QString updateTime;
    QString type;
    void clean()
    {
        typeId.clear();
        url.clear();
        updateTime.clear();
        type.clear();
    }
}structSampleVideoInfo;

class COMMONSHARED_EXPORT CHttpDownloadFromUrl : public QObject
{
    Q_OBJECT
//public:
//    explicit CHttpDownloadFromUrl( QObject *parent = nullptr );

//    void init();

//    void setDownloadDir( QString dir = "./" );

//    void startDownload( QString strUrl );

//    QFile *getFileWriterSecurity( QString fileName );

//    void destoryFileWriter();

//signals:
//    void signalDownloadFinished();

//    void signalDownloadComplete();

//    void signalDownloadProcess( QString downloadPersent );

//public slots:
//    void downloadMultiFiles( QStringList urls );

//    void onHttpReadyRead();

//    void onHttpDownloadFinished();

//    void onDisplayDownloadPersent( qint64 readBytes, qint64 totalBytes );

//public:
//    QString m_sDownloadDir{ "./" };

//private:
//    QNetworkAccessManager *m_pDownloader;
//    QFile *m_pFileWriter;
//    QNetworkReply *m_pReply;

};

#endif // CHTTPDOWNLOADFROMURL_H
