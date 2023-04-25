#ifndef CTHREADRESOURCECTRLBASE_H
#define CTHREADRESOURCECTRLBASE_H
#include "common_global.h"
#include <QThread>
#include <QSemaphore>

//资源控制线程类，软件项目中有很多需要进行资源控制然后运行线程
//写一个基类进行继承
class COMMONSHARED_EXPORT CThreadResourceCtrlBase : public QThread
{
public:
    CThreadResourceCtrlBase();
    virtual ~CThreadResourceCtrlBase() override;

    //阻塞等待线程结束
    void startThread();
    void stopThread();
protected:
    //展示实际运行
    virtual void run() override;

    //处理线程逻辑，只需重载这个变量即可
    virtual void dealThreadLogic();
protected:
    //线程运行标记，进入run之后就设置为true.线程退出为false
    std::atomic<bool> m_bThreadRunFlag{false};

    //控制线程运行标志
    std::atomic<bool> m_bThreadLoopFlag{false};

    //实际处理状态
    std::atomic<bool> m_bProcessState{false};

    //是否进行处理
    std::atomic<bool> m_bIntoDealProc{true};

    //线程运行控制信号量
    QSemaphore        m_semaphoreThread;
};

#endif // CTHREADRESOURCECTRLBASE_H
