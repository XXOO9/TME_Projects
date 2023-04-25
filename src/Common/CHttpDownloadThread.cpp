//#include "CHttpDownloadThread.h"

//HttpDownloadThread::HttpDownloadThread( QObject *parent ) : m_pThread( nullptr ),
//    m_pDownloader( nullptr )
//{
//    m_pThread = new QThread();
//    m_pDownloader = new CHttpDownloadFromUrl();
//    initConnection();

//    if(parent == nullptr)
//        return;
//}

//HttpDownloadThread::~HttpDownloadThread()
//{
//    m_pThread->quit();
//    m_pThread->wait();
//}

//void HttpDownloadThread::startDownloadFies( QVector< structSampleVideoSync > vecSampleVideoInfos )
//{
//    m_pDownloader->moveToThread( m_pThread );
//    m_pThread->start();
//    connect( m_pThread, &QThread::finished, m_pThread, &QObject::deleteLater );
//    connect( m_pThread, &QThread::finished, m_pDownloader, &QObject::deleteLater );
//    connect( m_pThread, &QThread::finished, [ = ](){
//        qDebug() << "cust thread finished...";
//    } );

//    QStringList videoUrls;
//    for( auto videoInfo : vecSampleVideoInfos ){
//        videoUrls << videoInfo.url;
//    }
//    emit signalStartDownloadFiles( videoUrls );
//}

//void HttpDownloadThread::initConnection()
//{
//    connect( this, &HttpDownloadThread::signalStartDownloadFiles,
//             m_pDownloader, &CHttpDownloadFromUrl::downloadMultiFiles );

//    connect( m_pDownloader, &CHttpDownloadFromUrl::signalDownloadComplete,
//             this, &HttpDownloadThread::onHttpDownloadComplete );

//    connect( m_pDownloader, &CHttpDownloadFromUrl::signalDownloadProcess,
//             this, &HttpDownloadThread::onHttpDownloading );

//    connect( m_pDownloader, &CHttpDownloadFromUrl::signalDownloadFinished, this, &QObject::deleteLater );
//}

//void HttpDownloadThread::setDownloadDir( QString dirPath )
//{
//    QDir dir( dirPath );
//    if( !dir.exists( ) ){
//        dir.mkdir( dirPath );
//    }
//    m_pDownloader->m_sDownloadDir = dirPath;
//}

//void HttpDownloadThread::onHttpDownloadComplete()
//{
//    qDebug() << "down complete...";
//}

//void HttpDownloadThread::onHttpDownloading( QString downloadPersent )
//{
//    qDebug() << QString( "down load %1 persent" ).arg( downloadPersent );
//}

