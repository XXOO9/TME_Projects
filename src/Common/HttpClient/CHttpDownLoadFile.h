#ifndef CHTTPDOWNLOADFILE_H
#define CHTTPDOWNLOADFILE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include "../common_global.h"

/*
 * 下载建议放到子线程中执行
*/

class COMMONSHARED_EXPORT CHttpDownLoadFile : public QObject
{
    Q_OBJECT
public:
    explicit CHttpDownLoadFile(QObject *parent = nullptr);
    ~CHttpDownLoadFile();

    void startDownLoad( QString urlString );

    QString downloadDir() const;
    void setDownloadDir(const QString &downloadDir);

private:
    void destoryFileWriter();
    QFile *getFileWriterSecurity( QString holeName );

signals:
    void sigDownLoadFinished();

private slots:
    void onHttpDownLoadReadyRead();
    void onDisplayDownloadProgress( qint64 readBytes, qint64 totalBytes );
    void onHttpDownLoadFinished();

private:
    QNetworkAccessManager    *m_pNetWorkMgr;
    QFile                    *m_pFileWriter;
    QNetworkReply            *m_pReply;

    QString                   m_downloadDir{ "./" };
};

#endif // CHTTPDOWNLOADFILE_H
