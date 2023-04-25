#include "COpencvVideoPlayer.h"
#include <QDebug>


COpencvVideoPlayer::COpencvVideoPlayer(QObject *parent) : QThread(parent)
{

}

COpencvVideoPlayer::~COpencvVideoPlayer()
{
    m_bRunThread = false;
    wait();
}


//绘制帧画面
void COpencvVideoPlayer::paintFrame(QPainter *painter, COpencvVideoControl *videoRenderControl)
{
    m_atomicLocker.ref();
    renderFrame(painter, m_lastFrameMat, videoRenderControl->size().toSize());
    m_atomicLocker.deref();
}

//设置视频播放路径
void COpencvVideoPlayer::setVideoUrl(const QUrl &videoUrl)
{
    //路径未更改，不需要重新播放
    if(m_videoUrl == videoUrl)
    {
        return;
    }

    //设置播放路径
    auto videoPath = videoUrl.toLocalFile().toStdString();

    if(videoPath.empty() || videoPath == "/")
    {
        return;
    }

    //通知视频路径更新
    m_videoUrl = videoUrl;
    Q_EMIT sigVideoUrlChanged(m_videoUrl);

    //播放器初始化
    if(m_videoCapture.isOpened())
    {
        m_videoCapture.release();
    }



    if(!m_videoCapture.open(videoPath))
    {
        qDebug()<<"can not open the file";
        return;
    }

    //获取视频帧率
    m_videoFps = m_videoCapture.get(cv::CAP_PROP_FPS);

    //计算每帧画面间隔时间（ms）
    if(m_videoFps != 0.0)
    {
        m_threadSleepms = int(1000 / m_videoFps);
    }

    //获取视频总帧数
    m_videoFrameCount = int(m_videoCapture.get(cv::CAP_PROP_FRAME_COUNT));
    Q_EMIT sigFrameCountChanged(m_videoFrameCount);

    //通知qml更新总时间
    setTotalTime(m_videoFrameCount);

    //获取视频宽高比
    auto frameWidth = int(m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH));
    auto frameHeight = int(m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT));
    m_inputResulation = QSize(frameWidth, frameHeight);
    Q_EMIT sigVideoSizeChanged(m_inputResulation);

    //当前播放帧
    m_videoframeNumber = 0;
    setCurFrameTime(m_videoframeNumber);

    //当前播放状态
    m_playState = StoppedState;

    //抽取视频帧
    extractFrame();
}

void COpencvVideoPlayer::run()
{
    if(!m_bRunThread)
    {
        return;
    }

    //当前没有播放源
    if(!m_videoCapture.isOpened())
    {
        return;
    }

    while (m_playState == PlayingState)
    {
        //抽取处理一帧
        extractFrame();

        //播放结束
        if(m_videoframeNumber > m_videoFrameCount)
        {
            m_playState = StoppedState;
            break;
        }

        msleep(U32(m_threadSleepms));
    }
}

//处理视频一帧
void COpencvVideoPlayer::extractFrame()
{
    //获取当前帧
    m_captureMute.lock();
    m_videoCapture >> m_curFrameMat;
    m_captureMute.unlock();

    if (!m_curFrameMat.empty() && m_curFrameMat.rows > 0 && m_curFrameMat.cols > 0)
    {
        //opencv读取的是bgr格式图片，需转成rgb
        m_videoframeNumber++;
        Q_EMIT sigFrameNumberChanged(m_videoframeNumber);

        //更新当前播放时间
        setCurFrameTime(m_videoframeNumber);

        //处理上一帧
        cv::cvtColor(m_curFrameMat, m_lastFrameMat, cv::COLOR_BGR2RGB);

        //通知控件重新渲染帧
        Q_EMIT sigVideoFrameUpdate();
    }
}

//绘制一帧画面
void COpencvVideoPlayer::renderFrame(QPainter *painter, const cv::Mat &frame, const QSize &size)
{
    if(frame.empty())
    {
        return;
    }

    auto frameImg = QImage((const unsigned char*)frame.data, frame.cols, frame.rows, int(frame.step), QImage::Format_RGB888);

    painter->drawImage({0, 0, size.width(), size.height()}, frameImg, frameImg.rect());
}

QUrl COpencvVideoPlayer::videoUrl() const
{
    return m_videoUrl;
}

QString COpencvVideoPlayer::getTotalTime() const
{
    return totalFrameTime;
}

void COpencvVideoPlayer::setTotalTime(const int &frameCount)
{
    int iMinCount = int (frameCount / m_videoFps / 60);
    int iSecCount = int (frameCount / m_videoFps) % 60;

    totalFrameTime = QString("%1:%2").arg(iMinCount, 2, 10, QLatin1Char('0')).arg(iSecCount, 2, 10, QLatin1Char('0'));
    Q_EMIT sigTotolTimeChanged(totalFrameTime);
}

QString COpencvVideoPlayer::getCurFrameTime() const
{
    return curFrameTime;
}

void COpencvVideoPlayer::setCurFrameTime(const int &frameNumber)
{
    int iMinCount = int (frameNumber / m_videoFps / 60);
    int iSecCount = int (frameNumber / m_videoFps) % 60;

    curFrameTime = QString("%1:%2").arg(iMinCount, 2, 10, QLatin1Char('0')).arg(iSecCount, 2, 10, QLatin1Char('0'));
    Q_EMIT sigTotolTimeChanged(curFrameTime);
}

QSize COpencvVideoPlayer::inputResulation() const
{
    return m_inputResulation;
}

COpencvVideoPlayer::PlaybackState COpencvVideoPlayer::palyState() const
{
    return m_playState;
}

void COpencvVideoPlayer::setPlayState(const PlaybackState &playState)
{
    if(m_playState == playState)
    {
        return;
    }

    switch (playState) {
    case PlayingState:
    {
        if(!m_videoCapture.isOpened())
        {
            return;
        }
        //启动线程
        if(!isRunning())
        {
            start();
        }
    }
        break;
    case StoppedState:
    {
        m_captureMute.lock();
        m_videoCapture.release();
        m_captureMute.unlock();
    }
        break;
    case PausedState:
        break;
    }

    m_playState = playState;
    Q_EMIT sigPlayStateChanged(m_playState);
}

int COpencvVideoPlayer::videoframeNumber() const
{
    return m_videoframeNumber;
}

//设置当前播放帧
void COpencvVideoPlayer::setVideoframeNumber(int videoframeNumber)
{
    if(videoframeNumber == m_videoframeNumber)
    {
        return;
    }

    //当前没有播放源
    if(!m_videoCapture.isOpened())
    {
        return;
    }

    //绘制渲染前一帧，所以当前帧需-1
    videoframeNumber--;
    //越界处理
    if(videoframeNumber > m_videoFrameCount)
    {
        videoframeNumber = m_videoFrameCount - 1;
    }
    if(videoframeNumber < 1)
    {
        videoframeNumber = 0;
    }

    m_captureMute.lock();
    //从视频流获取帧画面
    cv::Mat frame;
    //定位帧数据
    m_videoCapture.set(cv::CAP_PROP_POS_FRAMES, videoframeNumber);
    //获取帧数据
    m_videoCapture >> frame;
    //帧对齐
    int curFrame = int(m_videoCapture.get(cv::CAP_PROP_POS_FRAMES));
    while(curFrame < videoframeNumber)
    {
        m_videoCapture >> frame;
        curFrame = int(m_videoCapture.get(cv::CAP_PROP_POS_FRAMES));
    }
    m_captureMute.unlock();

    m_videoframeNumber = videoframeNumber;
    Q_EMIT sigFrameNumberChanged(m_videoframeNumber);

    setCurFrameTime(m_videoframeNumber);

    //暂停状态也要重新渲染画面
    if(m_playState == PausedState)
    {
        extractFrame();
    }
}

int COpencvVideoPlayer::videoFrameCount() const
{
    return m_videoFrameCount;
}

void COpencvVideoPlayer::setVideoFrameCount(int videoFrameCount)
{
    m_videoFrameCount = videoFrameCount;
}

COpencvVideoControl *COpencvVideoPlayer::videoControl() const
{
    return m_videoControl;
}

void COpencvVideoPlayer::setVideoControl(COpencvVideoControl *videoControl)
{
    if(m_videoControl == videoControl)
    {
        return;
    }
    videoControl->setOpencvPlayer(this);
    m_videoControl = videoControl;
    Q_EMIT sigViewControlChanged(m_videoControl);
}

