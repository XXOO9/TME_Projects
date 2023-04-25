///////////////////////////////////////////////////////////
//  CSubElementPullUp.cpp
//  Implementation of the Class CSubElementPullUp
//  Created on:      14-1月-2022 10:40:12
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementPullUp.h"
#include "TestProcAntiCheating/CTestProcAntiCheatingPullUp.h"

CSubElementPullUp::CSubElementPullUp(unsigned short shUnitID,  unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_PULL_UP))
{
    setCurTestItem(TEST_ITEM_PULL_UP);
    m_shDevType = DEV_TYPE_PULL_UP;
}

CSubElementPullUp::~CSubElementPullUp(){

}

void CSubElementPullUp::setElementInfo(void *pData, unsigned short shLength)
{
    if(pData == nullptr || shLength == 0)
    {
        LOGERROR("CSubElementPullUp::setElementInfo err");
        return;
    }

    CElementDataPullUp *pElementData = reinterpret_cast<CElementDataPullUp *>(pData);

    m_tLastTestState = pElementData->m_tTestState;
    //软件在测试状态，才进行错误码的判定
    if(isCheckErrCode())
    {
        CSubElementBase::setElementInfo(pData, shLength);
    }
    else
    {
        m_tLastTestState = 0;
    }

    //只有在当前测试单元开启时，才向外通知，同时变更对应的测试状态
    if(!m_bStartFlag)
        return;

    bool bSignalStateChange = pElementData->m_cTestState != m_objData.m_cTestState;
    bool bSignalDataChange = pElementData->m_nData != m_objData.m_nData;
    bool bChange = bSignalDataChange || bSignalStateChange || (pElementData->m_tTestState != m_objData.m_tTestState);

    if(bChange)
    {
        m_objData = *pElementData;
    }

    if(bSignalDataChange || bSignalStateChange)
    {
        emit signalDataChange(getIndex());
    }

    if(bSignalStateChange)
    {
        emit signalHardwareTestStateChange(getIndex(), m_objData.m_cTestState);
    }
}

void CSubElementPullUp::startTest()
{
    m_bStartFlag = true;
    m_objData.reset();
    clearErr();
}

void CSubElementPullUp::stopTest()
{
    m_bStartFlag = false;

    CSubElementBase::stopTest();
}

void CSubElementPullUp::createAntiCheating()
{
    deleteAntiCheating();

    m_pAntiCheating = new CTestProcAntiCheatingPullUp();

    CSubElementBase::createAntiCheating();
}
