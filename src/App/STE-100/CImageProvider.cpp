#include "CImageProvider.h"

CImageProvider::CImageProvider() : QQuickImageProvider( QQuickImageProvider::Image )
{
}

QPixmap CImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED( id );
    Q_UNUSED( size );
    Q_UNUSED( requestedSize );
    //return  QPixmap::fromImage( m_objImage);

    m_imageMutex.lock();

    QImage midImage;

    midImage = m_objImage;

    m_imageMutex.unlock();

    return  QPixmap::fromImage( midImage);
}

QImage CImageProvider::requestImage( const QString &id, QSize *size, const QSize &requestedSize )
{
    Q_UNUSED( id );
    Q_UNUSED( size );
    Q_UNUSED( requestedSize );

//    m_imageMutex.lock();

//    QImage midImage;

//    midImage = m_objImage;

//    m_imageMutex.unlock();

    return  m_objImage;
}

void CImageProvider::setimage(const void*  pData, const int& iWidth, const int& iHeight)
{
    if(!m_bSet)
    {
        m_objImage = QImage(iWidth, iHeight, QImage::Format_RGB888);
        m_szDataLen = static_cast<size_t>(iWidth * iHeight * 3);
    }

    memcpy_s(m_objImage.bits(), m_szDataLen, pData, m_szDataLen);
}
