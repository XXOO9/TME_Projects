#include "CDevCamera.h"
#include "CFilePath.h"
#include "Logger//CLogger.h"
#include <QDateTime>
using namespace cv;

CDevCamera::CDevCamera(QObject *parent)
    : QThread(parent)
{

}

CDevCamera::~CDevCamera()
{
    closeCamera();

    if( m_pCamera != nullptr )
    {
        delete m_pCamera;
        m_pCamera = nullptr;
    }

    if(m_pVideoWriter != nullptr)
    {
        delete m_pVideoWriter;
    }
}

void CDevCamera::run()
{
    m_bThreeadRunState = true;
    qint64 int64CurrTime, int64SleepTime;

    if(m_pCamera == nullptr)
        return;

    while( m_bThreadLoopState )
    {
        int64CurrTime = QDateTime::currentMSecsSinceEpoch();

        //处于暂停状态
        if(m_shCamerState != CAMERA_OPEN)
        {
            QThread::msleep(static_cast<unsigned long>(500));
            continue;
        }

        m_bResourcesAvailable = false;
        //新的一帧数据
        if(!m_pCamera->read(m_image))
            break;

        if(m_image.empty())
            continue;

        if(!m_bRecordVideo)
        {
            for(auto &regObj : m_setNotifyObj)
            {
                if(regObj != nullptr)
                {
                    regObj->setCamerData(m_image);
                }
            }

            m_bResourcesAvailable = true;

            int64SleepTime = 10 - (QDateTime::currentMSecsSinceEpoch() - int64CurrTime);
            if(int64SleepTime > 0)
            {
                QThread::msleep(static_cast<unsigned long>(int64SleepTime));
            }
        }
        else
        {
            m_bResourcesAvailable = true;

            if(m_pVideoWriter != nullptr)
                m_pVideoWriter->write(m_image);
        }
    }

    m_bThreeadRunState = false;
}

bool CDevCamera::openCamer()
{
    if(m_nCamerIndex < 0)
        return false;

    if(m_pCamera == nullptr)
    {
        m_pCamera = new cv::VideoCapture();
    }

    bool bRet = true;
    if(!(m_bThreadLoopState && m_bThreeadRunState && m_pCamera->isOpened()))
    {
        if(m_bCamerIndexChange)
        {
            closeCamera();

           //先关闭以前的
           m_pCamera->release();
        }

        if(m_pCamera->open(m_nCamerIndex, cv::CAP_DSHOW))
        {
            m_bThreadLoopState = true;
            start();
            bRet = true;
            m_bCamerIndexChange = false;
        }
        else
        {
            bRet = false;
            LOGERROR("Failed to open camera");
        }
    }

    if(bRet)
    {
        if(m_shCamerState == CAMERA_PAUSE)
        {
            //m_mutexPause.unlock();
        }

        m_shCamerState = CAMERA_OPEN;
    }

    return bRet;
}

void CDevCamera::closeCamera()
{
    //关闭打开的文件
    closeVideoFile();

    if(m_shCamerState == CAMERA_CLOSE)
        return;

    if(m_shCamerState == CAMERA_PAUSE)
    {
        m_shCamerState = CAMERA_OPEN;
    }

    if(m_bThreeadRunState)
    {
        m_bThreadLoopState = false;
    }

    //一直等待线程运行结束
    while (m_bThreeadRunState)
    {
        msleep(10);
    }

    m_shCamerState = CAMERA_CLOSE;

    //若在录像，停止即可
    if(m_bRecordVideo)
    {
        m_bRecordVideo = false;
        m_bOpenVideo = false;

        if(m_pVideoWriter != nullptr)
            m_pVideoWriter->release();
    }

    qDebug() << "closeCamera";
}

void CDevCamera::setCameraIndex(int iIndex)
{
    if(m_nCamerIndex != iIndex)
    {
        m_bCamerIndexChange = true;
        m_nCamerIndex = iIndex;
    }
}

int CDevCamera::getCamerIndex()
{
    return m_nCamerIndex;
}

cv::Mat CDevCamera::Grab_Image(bool bClocwiseRotation/* = false*/)
{
    cv::Mat grabImage;

    if(!(isOpenExternal() && isOpenInterior()))
    {
        return grabImage;
    }

    //等待资源可用
    while (!m_bResourcesAvailable)
    {
    }



    grabImage = m_image;

    if(grabImage.empty())
    {
        return grabImage;
    }

    if(bClocwiseRotation)
    {
        transpose(grabImage, grabImage);
    }

    //压缩图片尺寸为200k以内
    int width = grabImage.cols;
    int height = grabImage.rows;
    int shorter = width > height ? height : width;
    double ratio = 0.6;
    if (shorter * ratio <= 64)
    {
        ratio = 64.0 / shorter;
        cv::resize(grabImage, grabImage, cv::Size(int(width * ratio), int(height * ratio)));
    }
    else
    {
        cv::resize(grabImage, grabImage, cv::Size(int(width * ratio), int(height * ratio)));
    }

    return grabImage;
}

void CDevCamera::registerNotifyObj(CCamerRegister *p)
{
    m_objMutex.lock();
    if(p != nullptr)
        m_setNotifyObj.insert(p);
    m_objMutex.unlock();
}

void CDevCamera::unRegisterNotifyObj()
{
    m_objMutex.lock();
    m_setNotifyObj.clear();
    m_objMutex.unlock();
}

void CDevCamera::resetCamer()
{
    closeCamera();
    if(m_pCamera != nullptr)
    {
        delete m_pCamera;
        m_pCamera = nullptr;
    }

    m_nCamerIndex = -1;

    m_bRecordVideo = false;
}

void CDevCamera::pause()
{
    //摄像机状态不是打开状态不处理
    if(m_shCamerState != CAMERA_OPEN)
        return;

    //直到暂停成功
    m_shCamerState = CAMERA_PAUSE;
}

bool CDevCamera::recordVideo(const string &strPath)
{
    if(!openCamer())
    {
        return false;
    }

    if(m_pVideoWriter == nullptr)
    {
        m_pVideoWriter = new VideoWriter();
    }

    int a(static_cast<int>(m_pCamera->get(CAP_PROP_FRAME_WIDTH)));
    int b(static_cast<int>(m_pCamera->get(CAP_PROP_FRAME_HEIGHT)));
    m_szVideoRange = Size(a, b);

    int fourcc = m_pVideoWriter->fourcc('m', 'p', '4', 'v');
    if(!m_pVideoWriter->open(strPath, fourcc, 30, m_szVideoRange))
        return false;

    m_bOpenVideo = true;
    m_bRecordVideo = true;

    return true;
}

void CDevCamera::closeRecordVideo()
{
    pause();
    m_bRecordVideo = false;

    if(m_pVideoWriter != nullptr)
        m_pVideoWriter->release();
}

bool CDevCamera::openVideo(const string &strPath)
{
    m_objRedVideo.open(strPath);
    if(!m_objRedVideo.isOpened())
        return false;

    return true;
}

int CDevCamera::getFrameCount()
{
    int ret = 0;
    if(m_objRedVideo.isOpened())
    {
        ret = static_cast<int>(m_objRedVideo.get(CAP_PROP_FRAME_COUNT));
    }

    return ret;
}

float CDevCamera::getFPS()
{
    float ret = 0.0f;
    if(m_objRedVideo.isOpened())
    {
        ret = static_cast<int>(m_objRedVideo.get(CAP_PROP_FPS));
    }

    return ret;
}

void CDevCamera::getFrameInVideoFile(vector<Mat> &vec, int nStartFrameIndex, int nStopFrameIndex)
{
    vec.clear();
    if(!m_objRedVideo.isOpened())
        return;

    if(nStartFrameIndex >= 0)
    {
        m_objRedVideo.set(CAP_PROP_POS_FRAMES, static_cast<double>(nStartFrameIndex));
    }
    else
    {
        m_objRedVideo.set(CAP_PROP_POS_FRAMES, static_cast<double>(0));
    }

    int ret = static_cast<int>(m_objRedVideo.get(CAP_PROP_FRAME_COUNT));
    if(nStopFrameIndex < ret)
    {
        ret = nStopFrameIndex + 1;
    }

    Mat matTemp;
    int i = nStartFrameIndex;
    while(m_objRedVideo.read(matTemp) && i++ < ret)
    {
        vec.push_back(matTemp);
        vec[vec.size() - 1] = matTemp.clone();
//        imshow("111", matTemp);
//        waitKey(5);
    }
}

void CDevCamera::closeVideoFile()
{
    if(m_objRedVideo.isOpened())
    {
        m_objRedVideo.release();
    }
}



