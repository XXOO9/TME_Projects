#include "COpencvVideoControl.h"
#include "COpencvVideoPlayer.h"

COpencvVideoControl::COpencvVideoControl(QQuickItem *parent)
{

}

void COpencvVideoControl::paint(QPainter *painter)
{
    if(Q_UNLIKELY(!m_opencvPlayer))
    {
        painter->fillRect(clipRect(), Qt::black);
        return;
    }
    m_opencvPlayer->paintFrame(painter, this);
}

void COpencvVideoControl::setOpencvPlayer(COpencvVideoPlayer *opencvPlayer)
{
    //绑定视频帧更新信号
    connect(opencvPlayer, &COpencvVideoPlayer::sigVideoFrameUpdate, this, &QQuickItem::update);
    m_opencvPlayer = opencvPlayer;
}
