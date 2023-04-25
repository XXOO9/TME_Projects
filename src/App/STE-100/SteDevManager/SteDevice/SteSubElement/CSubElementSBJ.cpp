///////////////////////////////////////////////////////////
//  CSubElementSBJ.cpp
//  Implementation of the Class CSubElementSBJ
//  Created on:      14-1月-2022 10:40:09
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementSBJ.h"
#include "TestProcAntiCheating/CTestProcAntiCheatingSitSBJ.h"


CSubElementSBJ::CSubElementSBJ(unsigned short shUnitID, unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_SBJ))
{
    setCurTestItem(TEST_ITEM_SBJ);
    m_shDevType = DEV_TYPE_SBJ;
}

CSubElementSBJ::~CSubElementSBJ()
{

}


void CSubElementSBJ::setElementInfo(void *pData, unsigned short shLength)
{
    //
    if(pData == nullptr || shLength == 0)
    {
        LOGERROR("CSubElementSBJ::setElementInfo err");
        return;
    }

    CElementDataSBJ *pElementData = reinterpret_cast<CElementDataSBJ *>(pData);

    m_tLastTestState = pElementData->m_tTestState;
    //软件在测试状态，才进行错误码的判定(立定跳远需要一直进行检测，和其他设备的不同)
    CSubElementBase::setElementInfo(pData, shLength);

    //只有在当前测试单元开启时，才向外通知，同时变更对应的测试状态
    if(!m_bStartFlag)
    {
        if(s_bNotifyTimeOut)
        {
            m_llTimeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        }

        return;
    }

    bool bSignalStateChange = pElementData->m_cTestState != m_objData.m_cTestState;
    bool bSignalDataChange = compareFloating(pElementData->m_fData, m_objData.m_fData) != 0;
    bool bChange = bSignalDataChange || bSignalStateChange || (pElementData->m_tTestState != m_objData.m_tTestState);

    if(bChange)
    {
        if(s_bNotifyTimeOut)
        {
            m_llTimeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        }

        m_objData = *pElementData;
    }
    else
    {
        //数据未变更
        if(s_bNotifyTimeOut)
        {
            //大于10s未进行测试将数据从内部结束
            if(QDateTime::currentDateTime().toMSecsSinceEpoch() - m_llTimeStamp > 10000 && m_objData.m_cTestState == DEV_TEST_STATE_TESTING)
            {
                m_objData.m_cTestState = DEV_TEST_STATE_TIME_OUT_NOT_TEST;
                bSignalDataChange = true;
            }
        }
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

void CSubElementSBJ::startTest()
{
    //错误码置为无错误
    m_tLastTestState = 0;

    m_bStartFlag = true;
    m_objData.reset();

    clearErr();
}

void CSubElementSBJ::stopTest()
{
    m_bStartFlag = false;

    CSubElementBase::stopTest();
}

void CSubElementSBJ::createAntiCheating()
{
    deleteAntiCheating();

    m_pAntiCheating = new CTestProcAntiCheatingSitSBJ();

    CSubElementBase::createAntiCheating();
}
