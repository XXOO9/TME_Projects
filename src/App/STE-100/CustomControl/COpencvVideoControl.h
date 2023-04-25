#ifndef COPENCVVIDEOCONTROL_H
#define COPENCVVIDEOCONTROL_H

/*************************************************
  <类    名>    COpencvVideoControl
  <功能描述>     播放器渲染item，将视频帧注册显示至qml

  <作    者>    tcy
  <时    间>    2022-11-11
**************************************************/
#include <QQuickPaintedItem>
#include <QPainter>

class COpencvVideoPlayer;
class COpencvVideoControl : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit COpencvVideoControl(QQuickItem *parent = nullptr);

    void  paint(QPainter *painter) override;

    void setOpencvPlayer(COpencvVideoPlayer *opencvPlayer);

private:
    COpencvVideoPlayer *m_opencvPlayer;
};

#endif // COPENCVVIDEOCONTROL_H
