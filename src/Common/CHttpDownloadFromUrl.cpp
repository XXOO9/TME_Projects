//#include "CHttpDownloadFromUrl.h"

//CHttpDownloadFromUrl::CHttpDownloadFromUrl( QObject *parent ) : QObject( parent ),
//    m_pDownloader( nullptr ),
//    m_pFileWriter( nullptr ),
//    m_pReply( nullptr )
//{

//    init();
//}

//void CHttpDownloadFromUrl::init()
//{
//    m_pDownloader = new QNetworkAccessManager( this );
//}

//void CHttpDownloadFromUrl::setDownloadDir( QString dir )
//{
//    m_sDownloadDir = dir + "/";
//}

//void CHttpDownloadFromUrl::startDownload( QString strUrl )
//{
//    QUrl url( strUrl );
//    QFileInfo fileInfo( url.path() );
//    QString fileName = fileInfo.fileName();

//    if( fileName.isEmpty() )
//    {
//        fileName = "index.html";
//    }

//    m_pFileWriter = getFileWriterSecurity( m_sDownloadDir + fileName );

//    if( !m_pFileWriter->open( QIODevice::WriteOnly ) )
//    {
//        qDebug() << "file open faield...";
//        destoryFileWriter();
//        return;
//    }

//    QNetworkRequest request;
//    request.setUrl( url );
//    m_pDownloader->get( request );

//    connect( m_pReply, &QNetworkReply::readyRead, this, &CHttpDownloadFromUrl::onHttpReadyRead );
//    connect( m_pReply, &QNetworkReply::downloadProgress, this, &CHttpDownloadFromUrl::onDisplayDownloadPersent );
//    connect( m_pReply, &QNetworkReply::finished, this, &CHttpDownloadFromUrl::onHttpDownloadFinished );
//}

//void CHttpDownloadFromUrl::downloadMultiFiles( QStringList urls )
//{
//    qDebug() << "downloadMultiFiles,,," << __FUNCTION__ << " " << __LINE__;
//    if( urls.isEmpty() ){
//        qDebug() << "download file list is empty...";
//        return;
//    }

//    int i = 0;
//    for( auto sampleVideoUrl : urls ){
//        QUrl url( sampleVideoUrl );
//        QFileInfo fileInfo( url.path() );
//        qDebug() << "file Name = " << fileInfo.fileName();
//        m_pFileWriter = getFileWriterSecurity( m_sDownloadDir + fileInfo.fileName() );
//        if( !m_pFileWriter->open( QIODevice::WriteOnly | QIODevice::Truncate ) ){
//            qDebug() << "file open failed...";
//            destoryFileWriter();
//            return;
//        }

//        QNetworkRequest request;
//        request.setUrl( url );
//        m_pReply = m_pDownloader->get( request );

//        connect( m_pReply, &QNetworkReply::readyRead, this, &CHttpDownloadFromUrl::onHttpReadyRead );
//        connect( m_pReply, &QNetworkReply::downloadProgress, this, &CHttpDownloadFromUrl::onDisplayDownloadPersent );
//        connect( m_pReply, &QNetworkReply::finished, this, &CHttpDownloadFromUrl::onHttpDownloadFinished );

//        QEventLoop loop;
//        connect( m_pReply, &QNetworkReply::finished, &loop, &QEventLoop::quit );
//        loop.exec();
//        qDebug() << QString( "down load %1 file comllete..." ).arg( i );
//    }
//    emit signalDownloadFinished();
//}

//QFile *CHttpDownloadFromUrl::getFileWriterSecurity( QString fileName )
//{
//    if( m_pFileWriter == nullptr )
//    {
//        m_pFileWriter = new QFile( fileName );
//    }else
//    {
//        m_pFileWriter->setFileName( fileName );
//    }

//    return m_pFileWriter;
//}

//void CHttpDownloadFromUrl::destoryFileWriter()
//{
//    if( m_pFileWriter != nullptr )
//    {
//        delete m_pFileWriter;
//        m_pFileWriter = nullptr;
//    }
//}

//void CHttpDownloadFromUrl::onHttpReadyRead()
//{
//    if( m_pFileWriter == nullptr )
//    {
//        qDebug() << "file writer is nullptr";
//        return;
//    }
//    m_pFileWriter->write( m_pReply->readAll() );
//}

//void CHttpDownloadFromUrl::onHttpDownloadFinished()
//{
//    //刷新缓冲区,, 有啥用？
//    m_pFileWriter->flush();

//    m_pFileWriter->close();
//    destoryFileWriter();
//    emit signalDownloadComplete();
//}

//void CHttpDownloadFromUrl::onDisplayDownloadPersent( qint64 readBytes, qint64 totalBytes )
//{
//    float download = ( float )readBytes / ( float )totalBytes;
//    QString downloadPersent = QString::number( download * 100, 'f', 0 );
//    qDebug() << QString( "download %1%" ).arg( downloadPersent );
//    emit signalDownloadProcess( downloadPersent );
//}
