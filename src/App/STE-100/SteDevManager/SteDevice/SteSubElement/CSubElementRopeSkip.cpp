///////////////////////////////////////////////////////////
//  CSubElementRopeSkip.cpp
//  Implementation of the Class CSubElementRopeSkip
//  Created on:      14-1月-2022 10:40:14
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementRopeSkip.h"

CSubElementRopeSkip::CSubElementRopeSkip(unsigned short shUnitID, unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_ROPE_SKIP))
{

    m_shUnitID = shUnitID;
    m_shDevType = DEV_TYPE_ROPE_SKIP;
}

CSubElementRopeSkip::~CSubElementRopeSkip()
{

}

unsigned int CSubElementRopeSkip::getCurrentRope()
{
    //直接返回跳绳次数
    return m_uiKeepNum;
}

void CSubElementRopeSkip::setElementInfo(void *pData, unsigned short shLength)
{
    // 0.安全保护
    if(nullptr == pData && shLength == 0)
    {
        return;
    }

    // 1.设置绳子唯一身份码
    PDEV_ROPE_INFO RopeInfo = static_cast<PDEV_ROPE_INFO>(pData);

//    m_tLastTestState = pElementData->m_tTestState;
//    //软件在测试状态，才进行错误码的判定
//    if(isCheckErrCode())
//    {
//        CSubElementBase::setElementInfo(pData, shLength);
//    }
//    else
    {
        m_tLastTestState = 0;
    }

    if(m_iElementID != RopeInfo->uiIDCode)
    {
        m_iElementID = RopeInfo->uiIDCode;
        LOGINFO("The Rope of " + to_string(m_shUnitID) + " IDcode is " + to_string(m_iElementID));
    }

    //若未开始测试，则不进行任何界面更新,但需要保存开始次数
    if(!m_bStartFlag)
    {
        m_iStartNum = RopeInfo->usStartNum;
        return ;
    }

    // 2.赋值
    m_ucPowerNum = RopeInfo->ucPowerNum;

    m_ucChargeStatus = RopeInfo->ucChargeStatus;

    // 3.根据数据状态分类处理(硬件只存在 1 或 2 两种情况)
    unsigned char ucStatus = RopeInfo->ucDataStatus;

    // 3.1 测试中
    if(ucStatus == RopeSkipStatus::SKIP_IN_TEST)
    {
        m_uiKeepNum = RopeInfo->usSkipNum;

        //接收到测试中数据时，判断开始测试是否发生变化
        if(m_iStartNum != RopeInfo->usStartNum)
        {
            LOGINFO("The Rope of " + to_string(m_shUnitID) + " Start Success " + to_string(m_iStartNum) + " change to " + to_string(RopeInfo->usStartNum));
            m_iStartNum = RopeInfo->usStartNum;
            m_bStartSuccess = true;
        }

        //开始成功才往下走
        if(!m_bStartSuccess)
        {
            m_uiKeepNum = 0;
            emit signalDataChange(m_shUnitID);
            return ;
        }

        //原本处于未测状态,则证明为测试开始，发送 测试开始
        if(m_ucDataStatus == RopeSkipStatus::SKIP_NO_TEST)
        {
            LOGINFO("The Rope of " + to_string(m_shUnitID) + " enter the test");
            m_ucDataStatus = ucStatus;
            emit signalDataChange(m_shUnitID,m_ucDataStatus);
        }
        //已经处于测试中
        else if(m_ucDataStatus == RopeSkipStatus::SKIP_IN_TEST)
        {
            emit signalDataChange(m_shUnitID);
        }
        //其他情况，不做处理
        else
        {
        }
    }
    // 3.2 测试完成
    else if(ucStatus == RopeSkipStatus::SKIP_COMPLETE_TEST)
    {
        //处于测试中，则记录改变瞬间的值，发送 测试完成
        if(m_ucDataStatus == RopeSkipStatus::SKIP_IN_TEST)
        {
            LOGINFO("The Rope of " + to_string(m_shUnitID) + " complete the test");
            m_ucDataStatus = ucStatus;
            m_uiKeepNum = RopeInfo->usSkipNum;
            emit signalDataChange(m_shUnitID,m_ucDataStatus);
        }
        //其他情况，不做处理
        else
        {
            if(!m_bStartSuccess)
            {
                m_uiKeepNum = 0;
                emit signalDataChange(m_shUnitID);
                return ;
            }
        }
    }
    // 3.3 其他状态
    else
    {
        LOGERROR("The Rope of " + to_string(m_shUnitID) + " error status is " + to_string(ucStatus));
    }
}

void CSubElementRopeSkip::startTest()
{
    /*
    //重置 数据状态 跳绳次数
    m_ucDataStatus = 0;
    m_uiKeepNum    = 0;

    //设置测试标志
    m_bStartFlag = true;

    //得到开始时间
    m_StartTime = QDateTime::currentDateTime();

    LOGINFO("######### The Rope of " + to_string(m_shUnitID) + " Test Start Time is : " + to_string(m_StartTime.toMSecsSinceEpoch()));
    */
    clearErr();
}

void CSubElementRopeSkip::startSkipTest()
{
    //重置 数据状态 跳绳次数
    m_ucDataStatus = 0;
    m_uiKeepNum    = 0;

    //设置测试标志
    m_bStartFlag = true;

    m_bStartSuccess = false;

    m_bReStartFlag = false;

    //得到开始时间
    m_StartTime = QDateTime::currentDateTime();

    LOGINFO("######### The Rope of " + to_string(m_shUnitID) + " Test Start Time is : " + to_string(m_StartTime.toMSecsSinceEpoch()));
}

void CSubElementRopeSkip::stopTest()
{
    //设置测试标志
    m_bStartFlag = false;

    //得到结束时间
    QDateTime qEndTime = QDateTime::currentDateTime();

    long long llTime = qEndTime.toMSecsSinceEpoch() - m_StartTime.toMSecsSinceEpoch();

    LOGINFO("######### The Rope of " + to_string(m_shUnitID) + " Test End Time is : " + to_string(qEndTime.toMSecsSinceEpoch()));
    LOGINFO("######### The Rope of " + to_string(m_shUnitID) + " Test Through Time is : " + to_string(llTime));
    LOGINFO("######### The Rope of " + to_string(m_shUnitID) + " End skip num is : " + to_string(m_uiKeepNum));

    CSubElementBase::stopTest();
}

//因为设备断连，暂时停止更新对应数据
void CSubElementRopeSkip::stopUpdate()
{
    //设置测试标志
    m_bStartFlag = false;
}
