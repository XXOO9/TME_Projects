#include "CThreadResourceCtrlBase.h"

CThreadResourceCtrlBase::CThreadResourceCtrlBase()
    :QThread (nullptr)
{

}

CThreadResourceCtrlBase::~CThreadResourceCtrlBase()
{

}

void CThreadResourceCtrlBase::startThread()
{
    m_bThreadLoopFlag = true;

    if(m_bThreadRunFlag)
        return;

    start();

}

void CThreadResourceCtrlBase::stopThread()
{
    m_bThreadLoopFlag = false;

    //线程阻塞情况需要添加资源
    if(m_bThreadRunFlag)
    {
        if(!m_bProcessState)
        {
            m_semaphoreThread.release();
            m_bIntoDealProc = false;
        }

        //一直等待线程退出
        while(m_bThreadRunFlag)
        {
            msleep(10);
        }
    }
}

void CThreadResourceCtrlBase::run()
{
    m_bThreadRunFlag = true;

    while(m_bThreadLoopFlag)
    {
        m_bProcessState = false;
        //申请资源，若无资源则阻塞
        m_semaphoreThread.acquire();
        m_bProcessState = true;

        //用户自身详细处理逻辑
        if(m_bIntoDealProc)
        {
            dealThreadLogic();
        }
    }

    m_bThreadRunFlag = false;
}

void CThreadResourceCtrlBase::dealThreadLogic()
{

}
