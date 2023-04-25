#include "CHttpDownLoadFile.h"

CHttpDownLoadFile::CHttpDownLoadFile(QObject *parent) : QObject(parent),
    m_pNetWorkMgr( nullptr ),
    m_pFileWriter( nullptr ),
    m_pReply( nullptr )
{
    m_pNetWorkMgr = new QNetworkAccessManager( this );
}

CHttpDownLoadFile::~CHttpDownLoadFile()
{

}

void CHttpDownLoadFile::startDownLoad(QString urlString)
{
    if( nullptr == m_pNetWorkMgr ){
        return;
    }
    QUrl url = QUrl( urlString );
    QFileInfo fileInfo( url.path() );
    QString fileName = fileInfo.fileName();

    if( fileName.isEmpty() ){
        fileName = "index.html";
    }

    QString holeFileName = m_downloadDir + fileName;
    if( nullptr == m_pFileWriter ){
        m_pFileWriter = new QFile( holeFileName, this );
    }

    m_pFileWriter = getFileWriterSecurity( holeFileName );
    if( !m_pFileWriter->open( QIODevice::WriteOnly | QIODevice::Truncate) ){
        destoryFileWriter();
        return;
    }

    QNetworkRequest request;
    request.setUrl( url );
    m_pReply = m_pNetWorkMgr->get( request );
    connect( m_pReply, &QNetworkReply::readyRead, this, &CHttpDownLoadFile::onHttpDownLoadReadyRead );
    connect( m_pReply, &QNetworkReply::downloadProgress, this, &CHttpDownLoadFile::onDisplayDownloadProgress );
    connect( m_pReply, &QNetworkReply::finished, this, &CHttpDownLoadFile::onHttpDownLoadFinished );
    return;
}


QString CHttpDownLoadFile::downloadDir() const
{
    return m_downloadDir;
}

void CHttpDownLoadFile::setDownloadDir(const QString &downloadDir)
{
    m_downloadDir.clear();
    m_downloadDir = downloadDir;
}

void CHttpDownLoadFile::destoryFileWriter()
{
    if( nullptr != m_pFileWriter ){
        delete m_pFileWriter;
        m_pFileWriter = nullptr;
    }
}

QFile *CHttpDownLoadFile::getFileWriterSecurity(QString holeName)
{
    if( m_pFileWriter == nullptr ){
        m_pFileWriter = new QFile( holeName );
    }else{
        m_pFileWriter->setFileName( holeName );
    }

    return m_pFileWriter;
}

void CHttpDownLoadFile::onHttpDownLoadReadyRead()
{
    if( nullptr == m_pFileWriter || nullptr == m_pReply ){
        return;
    }

    m_pFileWriter->write( m_pReply->readAll() );
}

void CHttpDownLoadFile::onDisplayDownloadProgress( qint64 readBytes, qint64 totalBytes )
{
    double read = static_cast<double>(readBytes);
    double total = static_cast<double>(totalBytes);
    double retPersent = read / total;
    QString strDownLoadPercent = QString::number( retPersent, 'g', 0 );
    qDebug() << "download persent = " << strDownLoadPercent;
}

void CHttpDownLoadFile::onHttpDownLoadFinished()
{
    if( nullptr == m_pFileWriter ){
        return;
    }

    m_pFileWriter->flush();

    m_pFileWriter->close();
    destoryFileWriter();
    qDebug() << "downLoad finished...";
}
