///////////////////////////////////////////////////////////
//  CSubElementRunShort.cpp
//  Implementation of the Class CSubElementRunShort
//  Created on:      14-1月-2022 10:40:13
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementRunShort.h"
CSubElementRunShort::CSubElementRunShort(unsigned short shUnitID,  unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_RUN_SHORT))
{
    m_shUnitID = shUnitID;
    m_iElementID = iElementID;

    m_shDevType = DEV_TYPE_RUN_SHORT;

    m_qInitializationTime = QDateTime::currentDateTime();

    LOGINFO("The Runway of " + to_string(m_shUnitID) + " Initialization Time is : " + to_string(m_qInitializationTime.toSecsSinceEpoch()));
}

CSubElementRunShort::~CSubElementRunShort()
{

}

void CSubElementRunShort::startTest()
{
    //初始化所有属性
    initBasicInfo();

    //修改测试状态
    m_bStartFlag = true;

    //获取当前时间
    m_qProtectDate = QDateTime::currentDateTime();

    LOGINFO("The Runway of " + to_string(m_shUnitID) + " Start Time is : " + to_string(m_qProtectDate.toSecsSinceEpoch()));
    clearErr();
}

void CSubElementRunShort::stopTest()
{
    //修改测试状态
    m_bStartFlag = false;
}

bool CSubElementRunShort::getStartFlag()
{
    return m_bStartFlag;
}

void CSubElementRunShort::setUseStartFlag(bool bFlag)
{
    m_bUseStarting = bFlag;
}

void CSubElementRunShort::setStartTime(float fStartTime)
{
    m_queueStartTime.push(fStartTime);
}

bool CSubElementRunShort::getAvailableFlag()
{
    //当跑道没人使用且处于连接状态
    if(!m_bBindFlag && m_shConnect_Status == STE_NORMAL_CONNECT)
    {
        return true;
    }
    else
    {
        return false;
    }
}

unsigned int CSubElementRunShort::getCurrentLaps()
{
    return m_uiCurrentLap;
}

//注：不能使用 结束时间 - 开始时间，部分项目可能在起点结束
float CSubElementRunShort::getCurrentTime()
{
    float fTime = 0.0f;

    fTime = m_fMarkTime - m_fStartTime;

    if(fTime < 0.0f)
    {
        LOGERROR("The Runway of " + to_string(m_shUnitID) + " Run time is " + to_string(fTime));
        fTime = 0.0f;
    }

    return fTime;
}

/*
 * 为了兼容短跑、往返跑等统一使用跑道的测试模式
 * 此处修改逻辑为统一支持方式，兼容以后模式扩展
 *
 * 1.硬件箱子目前为 一个起点 一个终点 所有跑道共用
 *
 * 2.跑道为触发模式，新的触发数据会顶掉旧数据
 *
 * 3.因为接收到的数据可能存在延时，例如已经接收到多
 *   次起点数据，还未收到终点数据，使用队列储存数据
 *
 * 4.缺点：将此逻辑写在此处后变得不通用，因为此处包含了圈数是否增加的逻辑
 *       理论上应该考虑将逻辑判断移植到 流程指导类，此处仅保存每次变化数据
 */
void CSubElementRunShort::setElementInfo(void *pData, unsigned short shLength)
{
    // 0.安全保护
    if(nullptr == pData)
    {
        return;
    }

    //  先调用通用数据处理
    CSubElementBase::setElementInfo(pData, shLength);

    // 1.设置跑道唯一身份码
    PDEV_RUNWAY_INFO DevInfo = static_cast<PDEV_RUNWAY_INFO>(pData);

    if(m_iElementID != DevInfo->uiIDCode)
    {
        m_iElementID = DevInfo->uiIDCode;
        LOGINFO("The Runway of " + to_string(m_shUnitID) + " IDcode is " + to_string(m_iElementID));
    }

    // 构建 QDateTime 时间戳
    QDate qDate(DevInfo->usYear,DevInfo->ucMonth,DevInfo->ucDay);
    QTime qTime(DevInfo->ucHour,DevInfo->ucMinute,DevInfo->ucSecond);
    QDateTime qdTime;
    qdTime.setDate(qDate);
    qdTime.setTime(qTime);

    // 2.数据处理
    if(DevInfo->bEndFlag)
    {
        //不使用起点，默认已经存在
        if(!m_bUseStarting)
        {
            setStartConnectFlag(true);
        }

        //时间同步成功才会认为入网连接
        //if(qdTime.toSecsSinceEpoch() >= m_qInitializationTime.toSecsSinceEpoch())
        {
            setEndConnectFlag(true);
        }

        // 2.1 终点数据处理
        if(compareFloating(DevInfo->fRunTime,m_fUntestEndTime) != 0)
        {
            m_fUntestEndTime = DevInfo->fRunTime;

            if(m_bStartFlag)
            {
                //注：不使用起点则不添加同步保护
                if(!m_bUseStarting || qdTime.toSecsSinceEpoch() >= m_qProtectDate.toSecsSinceEpoch())
                {
                    m_queueEndTime.push(DevInfo->fRunTime);
                    LOGINFO("The Runway of " + to_string(m_shUnitID) + " End Time Queue push time : " + to_string(DevInfo->fRunTime));
                }
                else
                {
                    LOGERROR("The Runway of " + to_string(m_shUnitID) + " End Time Stamp Invalid.");
                    LOGERROR("Protect Time is " + to_string(m_qProtectDate.toSecsSinceEpoch()));
                    LOGERROR("Through Time is " + to_string(qdTime.toSecsSinceEpoch()));
                }
            }
        }
    }
    else
    {
        //不使用起点，不处理起点数据
        if(!m_bUseStarting)
        {
            return;
        }

        //时间同步成功才会认为入网连接
        //if(qdTime.toSecsSinceEpoch() >= m_qInitializationTime.toSecsSinceEpoch())
        {
            setStartConnectFlag(true);
        }

        // 2.2 起点数据处理
        if(compareFloating(DevInfo->fRunTime,m_fUntestStartTime) != 0)
        {
            m_fUntestStartTime = DevInfo->fRunTime;

            if(m_bStartFlag)
            {
                if(qdTime.toSecsSinceEpoch() >= m_qProtectDate.toSecsSinceEpoch())
                {
                    m_queueStartTime.push(DevInfo->fRunTime);
                    LOGINFO("The Runway of " + to_string(m_shUnitID) + " Start Time Queue push time : " + to_string(DevInfo->fRunTime));
                }
                else
                {
                    LOGERROR("The Runway of " + to_string(m_shUnitID) + " Start Time Stamp Invalid");
                    LOGERROR("Protect Time is " + to_string(m_qProtectDate.toSecsSinceEpoch()));
                    LOGERROR("Through Time is " + to_string(qdTime.toSecsSinceEpoch()));
                }
            }
        }
    }

    // 3.逻辑处理
    processData();
}

void CSubElementRunShort::setConnectStatus(unsigned short usStatus)
{
    CSubElementBase::setConnectStatus(usStatus);

    if(usStatus == STE_NO_CONNECT)
    {
        m_bStartConnectFlag = false;

        m_bEndConnectFlag = false;
    }
}

void CSubElementRunShort::setStartConnectFlag(bool bFlag)
{
    m_bStartConnectFlag = bFlag;

    if(m_bStartConnectFlag && m_bEndConnectFlag)
    {
        CSubElementBase::setConnectStatus(STE_NORMAL_CONNECT);
    }
    else
    {
        CSubElementBase::setConnectStatus(STE_NO_CONNECT);
    }
}

void CSubElementRunShort::setEndConnectFlag(bool bFlag)
{
    m_bEndConnectFlag = bFlag;

    if(m_bStartConnectFlag && m_bEndConnectFlag)
    {
        CSubElementBase::setConnectStatus(STE_NORMAL_CONNECT);
    }
    else
    {
        CSubElementBase::setConnectStatus(STE_NO_CONNECT);
    }
}

void CSubElementRunShort::initBasicInfo()
{
    m_bStartFlag = false;

    //0.当前圈数(默认为0)
    m_uiCurrentLap = 0;

    //1.清空时间缓存
    clearTime();

    //2.标志时间
    m_fMarkTime = -1.0f;

    //3.开始时间信息
    m_fStartTime = -1.0f;

    //4.结束时间信息
    m_fEndTime = -1.0f;
}

void CSubElementRunShort::processData()
{
    /// 0. 进入条件(当存在一个时都进入)
    if(m_queueStartTime.empty() && m_queueEndTime.empty())
    {
        return;
    }

    /// 1. 进行数据处理(先进先出原则)
    if(m_fStartTime < 0.0f)
    {
        // 1.0 若开始队列为空，则直接返回
        if(m_queueStartTime.empty())
        {
            return;
        }

        // 1.1 赋值开始时间
        m_fStartTime = m_queueStartTime.front();
        m_queueStartTime.pop();
        LOGINFO("The Runway of " + to_string(m_shUnitID) + " Start Time is : " + to_string(m_fStartTime));

        // 1.2 设置基本信息
        m_fMarkTime = m_fStartTime;
        m_uiCurrentLap = 1;

        // 1.3 递归操作
        processData();
    }
    /// 2.
    else
    {
        // 2.1 应该经过终点
        if(m_uiCurrentLap % 2 == 1)
        {
            // 2.1.0 若结束队列为空，则直接返回
            if(m_queueEndTime.empty())
            {
                return;
            }

            // 2.1.1 得到结束时间
            m_fEndTime = m_queueEndTime.front();
            m_queueEndTime.pop();
            LOGINFO("The Runway of " + to_string(m_shUnitID) + " End Time is : " + to_string(m_fEndTime));

            // 2.1.2 判断时间是否正确
            if(compareFloating(m_fEndTime,m_fMarkTime) > 0)
            {
                m_fMarkTime = m_fEndTime;

                LOGINFO("The Runway of " + to_string(m_shUnitID) + " Mark Time is : " + to_string(m_fEndTime));

                increaseThroughTimes();

                if(!m_bStartFlag)
                {
                   return;
                }
                else
                {
                    processData();
                }
            }
            else
            {
                processData();
            }
        }
        // 2.2 应该经过起点
        else
        {
            // 2.1.0 若开始队列为空，则直接返回
            if(m_queueStartTime.empty())
            {
                return;
            }

            // 2.1.1 得到起点时间
            m_fEndTime = m_queueStartTime.front();
            m_queueStartTime.pop();
            LOGINFO("The Runway of " + to_string(m_shUnitID) + " End Time is : " + to_string(m_fEndTime));

            // 2.1.2 判断时间是否正确
            if(compareFloating(m_fEndTime,m_fMarkTime) > 0)
            {
                m_fMarkTime = m_fEndTime;

                LOGINFO("The Runway of " + to_string(m_shUnitID) + " Mark Time is : " + to_string(m_fEndTime));

                increaseThroughTimes();

                if(!m_bStartFlag)
                {
                   return;
                }
                else
                {
                    processData();
                }
            }
            else
            {
                processData();
            }
        }
    }
}

void CSubElementRunShort::clearTime()
{
    queue<float> emptyStart;
    swap(emptyStart,m_queueStartTime);

    queue<float> emptyEnd;
    swap(emptyEnd,m_queueEndTime);
}

void CSubElementRunShort::increaseThroughTimes()
{
    //0.增加经过次数
    m_uiCurrentLap = m_uiCurrentLap + 1;

    //1.记录日志信息(每一次刷新点的时间)
    LOGINFO("Runway : " + to_string(m_shUnitID) + " Lap : " +
            to_string(m_uiCurrentLap) + " Time : " + to_string(m_fMarkTime));

    //2.通知外部，经过次数发生改变
    if(m_bStartFlag)
    {
        emit signalDataChange(m_shUnitID);
    }
}
