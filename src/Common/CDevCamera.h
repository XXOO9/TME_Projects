#ifndef CDevCamera_H
#define CDevCamera_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QMutex>
#include <QApplication>
#include <QReadWriteLock>
#include <set>

#include "common_global.h"
#include "CCamerRegister.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "QSemaphore"

//using namespace cv;
using namespace std;



class COMMONSHARED_EXPORT CDevCamera : public QThread
{
    Q_OBJECT
public:
    explicit CDevCamera(QObject *parent = nullptr);

    //开启 关闭 暂停,外部进行调用是进行参数设置
    //内部表示状态的函数只有其中三个 打开 关闭 暂停
    enum emCameraState
    {CAMERA_OPEN, CAMERA_CLOSE, CAMERA_PAUSE};

    ~CDevCamera();

    void run();

    bool openCamer();
    void closeCamera();

    //外部状态(外部设置摄像机应该打开的状态)和内部状态（opencv 对摄像头的实际打开状态）
    inline bool isOpenExternal() {return m_shCamerState == CAMERA_OPEN;} //外在（比如暂停状态是关闭了线程，但是内部处于打开状态）
    inline bool isOpenInterior() {return m_pCamera != nullptr && m_pCamera->isOpened() && m_nCamerIndex >= 0;} //内部

    //设置状态不影响现有摄像头逻辑，在重连逻辑中进行配置
    inline bool getLastSetState(){return m_bLastSetState;}
    void setState(bool bOpen){m_bLastSetState = bOpen;}

    void setCameraIndex(int iIndex);

    int getCamerIndex();

    cv::Mat  Grab_Image(bool bClocwiseRotation = false);

    //注册摄像头数据分发对象
    void registerNotifyObj(CCamerRegister *p);

    //注销注册对象
    void unRegisterNotifyObj();

    //重置
    void resetCamer();

    //暂停，通过停止线程达到暂停效果，恢复调用open即可
    void pause();

    inline short getCamerState(){return m_shCamerState;}

    //完整的流程是开摄像机
    bool recordVideo(const string &strPath);
    void closeRecordVideo(); //仅仅是关闭录制

    //完整的打开视频
    bool openVideo(const string &strPath);
    int getFrameCount();
    float getFPS();
    void getFrameInVideoFile(vector<cv::Mat> &vec, int nStartFrameIndex = 0, int nStopFrameIndex = 0);
    void closeVideoFile();
public slots:

private:
    //删除变量之前一定要调用关闭函数，等待线程结束
    cv::VideoCapture *m_pCamera{nullptr};

    //线程运行标志
    std::atomic<bool> m_bThreadLoopState{false};

    //线程状态标志
    std::atomic<bool> m_bThreeadRunState{false};

    QMutex m_objMutex;

    QMutex m_mutexPause;

    //摄像头通知对象
    set<CCamerRegister *> m_setNotifyObj;

    //设置opencv使用摄像头
    std::atomic<int> m_nCamerIndex{-1};

    cv::Mat            m_image;

    //用变量表示资源可用上述的mat m_image
    std::atomic<bool> m_bResourcesAvailable;

    bool m_bCamerIndexChange{false};

    std::atomic<short> m_shCamerState{CAMERA_CLOSE};

    //录像相关
    std::atomic<bool> m_bRecordVideo{false};
    cv::VideoWriter *m_pVideoWriter{nullptr};
    cv::Size m_szVideoRange;
    std::atomic<bool> m_bOpenVideo{false};
    cv::VideoCapture m_objRedVideo;

    //设置状态表明用户需要相机是开启状态还是关闭状态(不是开启就是关闭)
    //记录最后一次状态
    std::atomic<bool> m_bLastSetState{false};
};

#endif // CDevCamera_H
