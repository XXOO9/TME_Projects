#ifndef CIMAGEPROVIDER_H
#define CIMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QDebug>
#include <QMutex>

class CImageProvider : public QQuickImageProvider
{
public:
    CImageProvider();

    QPixmap requestPixmap( const QString &id, QSize *size, const QSize &requestedSize );

    QImage requestImage( const QString &id, QSize *size, const QSize &requestedSize );

    void setimage(const void*  pData, const int& iWidth, const int& iHeight);
private:
    QImage m_objImage;

    QMutex m_imageMutex;

    bool m_bSet{false};
    size_t m_szDataLen{0};
};

#endif // CIMAGEPROVIDER_H
