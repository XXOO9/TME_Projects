///////////////////////////////////////////////////////////
//  CSubElementRunMid.cpp
//  Implementation of the Class CSubElementRunMid
//  Created on:      14-1月-2022 10:40:15
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementRunMid.h"

CSubElementRunMid::CSubElementRunMid(unsigned short shUnitID,  unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_RUN_MID))
{
    m_shUnitID = shUnitID;
    m_iElementID = iElementID;

    m_shDevType = DEV_TYPE_RUN_MID;
}

CSubElementRunMid::~CSubElementRunMid()
{

}

//数据设置到当前测试单元肯定和卡号进行绑定
void CSubElementRunMid::setElementInfo(void *pData, unsigned short shLength)
{
    //0.安全保护
    if(nullptr == pData || shLength == 0)
    {
        return;
    }

    //  先调用通用数据处理
    CSubElementBase::setElementInfo(pData, shLength);

    if(!m_bStartFlag)
        return;

    //1.设置设备唯一身份码
    PDEV_RUNWAY_INFO DevInfo = reinterpret_cast<PDEV_RUNWAY_INFO>(pData);

    //2.接收数据
    bool bSignalChange = false;

    if(m_vecPassTime.size() == 0)
    {
        bSignalChange = true;
    }
    else
    {
        //当前时间相对于最后一次有变更
        const float &pLastPassTime = (*(m_vecPassTime.rbegin())).m_fPassTime;
        if(compareFloating(DevInfo->fRunTime, pLastPassTime) != 0)
        {
            bSignalChange = true;
        }
    }

    if(bSignalChange)
    {
        CElementDataRunMid elementData;
        m_vecPassTime.push_back(elementData);
        CElementDataRunMid &pushData = m_vecPassTime[m_vecPassTime.size() - 1];

        //目前仅需要通过时间
        pushData.m_llRecordTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        pushData.m_fPassTime = DevInfo->fRunTime;

        emit signalDataChange(m_shUnitID);
    }
}

void CSubElementRunMid::startTest()
{
    CSubElementBase::startTest();
    m_vecPassTime.clear();

    //设置途经点检测标志为false
    setCheckedViaPtDev(false);
    clearErr();
}

void CSubElementRunMid::stopTest()
{

}
