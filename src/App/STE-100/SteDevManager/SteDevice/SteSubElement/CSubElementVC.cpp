﻿///////////////////////////////////////////////////////////
//  CSubElementVC.cpp
//  Implementation of the Class CSubElementVC
//  Created on:      14-1月-2022 10:40:08
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementVC.h"
#include "TestProcAntiCheating/CTestProcAntiCheatingVC.h"

CSubElementVC::CSubElementVC(unsigned short shUnitID, unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_VC))
{
    setCurTestItem(TEST_ITEM_VC);
    m_shDevType = DEV_TYPE_VC;
}

CSubElementVC::~CSubElementVC()
{

}

void CSubElementVC::setElementInfo(void *pData, unsigned short shLength)
{
    if(pData == nullptr || shLength == 0)
    {
        LOGERROR("CSubElementVC::setElementInfo err");
        return;
    }

    CElementDataVC *pElementData = reinterpret_cast<CElementDataVC *>(pData);

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
    {
        if(s_bNotifyTimeOut)
        {
            m_llTimeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        }

        return;
    }
    //进行错误状态检测，将错误信息保存
    if(m_objTestStateManage.getTestState() == CTestStateManage::TEST_PREPARE)
    {
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

void CSubElementVC::startTest()
{
    m_bStartFlag = true;
    m_objData.reset();
    clearErr();
}

void CSubElementVC::stopTest()
{
    m_bStartFlag = false;

    CSubElementBase::stopTest();
}

void CSubElementVC::createAntiCheating()
{
    deleteAntiCheating();

    m_pAntiCheating = new CTestProcAntiCheatingVC();

    CSubElementBase::createAntiCheating();
}
