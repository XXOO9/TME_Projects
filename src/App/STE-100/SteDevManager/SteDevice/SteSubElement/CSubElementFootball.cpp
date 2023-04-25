///////////////////////////////////////////////////////////
//  CSubElementFootball.cpp
//  Implementation of the Class CSubElementFootball
//  Created on:      8-10月-2022 14:56:08
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementFootball.h"
#include "TestProcAntiCheating/CTestProcAntiCheatingVC.h"

CSubElementFootball::CSubElementFootball(unsigned short shUnitID, unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_FOOTBALL))
{
    setCurTestItem(TEST_ITEM_FOOTBALL);
    m_shDevType = DEV_TYPE_FOOTBALL;
}

CSubElementFootball::~CSubElementFootball()
{

}

void CSubElementFootball::setElementInfo(void *pData, unsigned short shLength)
{
    if(pData == nullptr || shLength == 0)
    {
        LOGERROR("CSubElementFootball::setElementInfo err");
        return;
    }

    CElementDataFootball *pElementData = reinterpret_cast<CElementDataFootball *>(pData);

    m_tLastTestState = pElementData->m_tTestState;

    //足球设备一直需要更新设备错误，判断终点是否未对齐
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
    //进行错误状态检测，将错误信息保存
    if(m_objTestStateManage.getTestState() == CTestStateManage::TEST_PREPARE)
    {
        return;
    }

    bool bSignalStateChange = pElementData->m_cTestState != m_objData.m_cTestState;
    bool bSignalDataChange = compareFloating(pElementData->m_fData, m_objData.m_fData) != 0;

    qDebug()<<"################"<<bSignalDataChange;
    qDebug()<<"################"<<QDateTime::currentDateTime();

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

void CSubElementFootball::startTest()
{
    m_bStartFlag = true;
    m_objData.reset();

    clearErr();
}

void CSubElementFootball::stopTest()
{
    m_bStartFlag = false;

    CSubElementBase::stopTest();
}

void CSubElementFootball::createAntiCheating()
{
    deleteAntiCheating();

    m_pAntiCheating = new CTestProcAntiCheatingVC();

    CSubElementBase::createAntiCheating();
}
