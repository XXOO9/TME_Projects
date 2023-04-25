#ifndef COPENCVVIDEOPLAYER_H
#define COPENCVVIDEOPLAYER_H
/*************************************************
  <类    名>    COpencvVideoPlayer
  <功能描述>     opencv视频播放器

  <作    者>    tcy
  <时    间>    2022-11-11
**************************************************/

#include <opencv2/opencv.hpp>
#include <QUrl>
#include <QSize>
#include <QThread>
#include <QMutex>
#include "COpencvVideoControl.h"

typedef  unsigned long   U32;

class COpencvVideoPlayer : public QThread
{

    Q_OBJECT

    Q_PROPERTY(QUrl videoURL READ videoUrl WRITE setVideoUrl NOTIFY sigVideoUrlChanged)
    Q_PROPERTY(COpencvVideoControl* videoControl READ videoControl WRITE setVideoControl NOTIFY sigViewControlChanged)
    Q_PROPERTY(int frameCount READ videoFrameCount WRITE setVideoFrameCount NOTIFY sigFrameCountChanged)
    Q_PROPERTY(int frameNumber READ videoframeNumber WRITE setVideoframeNumber NOTIFY sigFrameNumberChanged)
    Q_PROPERTY(PlaybackState playState READ palyState WRITE setPlayState NOTIFY sigPlayStateChanged)
    Q_PROPERTY(QSize inputResulation READ inputResulation NOTIFY sigVideoSizeChanged)
    Q_PROPERTY(QString curTime READ getCurFrameTime NOTIFY sigCurTimeChanged)
    Q_PROPERTY(QString totalTime READ getTotalTime NOTIFY sigTotolTimeChanged)

public:
    //播放状态
    enum PlaybackState {
        StoppedState,
        PlayingState,
        PausedState
    };
    Q_ENUM(PlaybackState)

    explicit COpencvVideoPlayer(QObject *parent = nullptr);
    ~COpencvVideoPlayer();

    //向外提供绘制视频帧接口
    void paintFrame(QPainter *painter, COpencvVideoControl *videoRenderControl);

    COpencvVideoControl* videoControl() const;

    QUrl videoUrl() const;

    int videoFrameCount() const;

    int videoframeNumber() const;

    PlaybackState palyState() const;

    QSize inputResulation() const;

    double videoFps() const;

    QString getCurFrameTime() const;
    void setCurFrameTime(const int &frameNumber);

    QString getTotalTime() const;
    void setTotalTime(const int &frameCount);

public Q_SLOTS:
    void setVideoUrl(const QUrl &videoUrl);
    void setVideoControl(COpencvVideoControl *videoControl);
    void setVideoFrameCount(int videoFrameCount);
    void setVideoframeNumber(int videoframeNumber);
    void setPlayState(const PlaybackState &playState);

protected:
    void run();

private:
    void extractFrame();
    void renderFrame(QPainter *painter, const cv::Mat &frame, const QSize &size);

signals:
    void sigVideoUrlChanged(QUrl videoUrl);
    void sigVideoSizeChanged(QSize frameSize);
    void sigVideoFrameUpdate();
    void sigViewControlChanged(COpencvVideoControl *viewControl);
    void sigFrameCountChanged(int count);
    void sigFrameNumberChanged(int curFrame);
    void sigPlayStateChanged(PlaybackState curFrame);
    void sigTotolTimeChanged(QString time);
    void sigCurTimeChanged(QString time);

private:
    QUrl m_videoUrl;            //视频播放路径
    int m_videoFrameCount{-1};  //视频总帧数
    int m_videoframeNumber{-1}; //视频当前帧数
    QSize m_inputResulation;    //视频宽高大小
    cv::VideoCapture m_videoCapture; //opencv视频操作对象
    double m_videoFps{-1};      //视频帧率
    PlaybackState m_playState{StoppedState};
    QMutex m_captureMute;
    QAtomicInt m_atomicLocker;

    int m_threadSleepms{0};

    cv::Mat m_curFrameMat;     //当前视频帧图像
    cv::Mat m_lastFrameMat;    //上一帧视频图像

    QString curFrameTime {"00:00"};
    QString totalFrameTime{"00:00"};

    COpencvVideoControl *m_videoControl;

     bool m_bRunThread{true};
};

#endif // COPENCVVIDEOPLAYER_H
