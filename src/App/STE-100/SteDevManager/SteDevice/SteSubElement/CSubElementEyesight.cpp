///////////////////////////////////////////////////////////
//  CSubElementEyesight .cpp
//  Implementation of the Class CSubElementEyesight 
//  Created on:      14-1月-2022 10:40:18
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementEyesight.h"

CSubElementEyesight::CSubElementEyesight(unsigned short shUnitID,  unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_GRIP))
{
    setCurTestItem(TEST_ITEM_EYESIGHT);
    m_shDevType = DEV_TYPE_EYESIGHT;
}

CSubElementEyesight ::~CSubElementEyesight ()
{

}

void CSubElementEyesight::setElementInfo(void *pData, unsigned short shLength)
{
    if(pData == nullptr || shLength == 0)
    {
        LOGERROR("CSubElementEyesight::setElementInfo err");
        return;
    }

    CElementDataEyesight *pElementData = reinterpret_cast<CElementDataEyesight *>(pData);

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
        return;
    }
    //进行错误状态检测，将错误信息保存
    if(m_objTestStateManage.getTestState() == CTestStateManage::TEST_PREPARE)
    {
        return;
    }

    //判断 测试状态，左眼数据，右眼数据 是否发生变化
    bool bSignalStateChange  = pElementData->m_cTestState != m_objData.m_cTestState;
    bool bSignalLeftChange   = compareFloating(pElementData->m_fLEyesight, m_objData.m_fLEyesight) != 0;
    bool bSignalRightChange  = compareFloating(pElementData->m_fREyesight, m_objData.m_fREyesight) != 0;
    bool bChange = bSignalStateChange || bSignalLeftChange || bSignalRightChange || (pElementData->m_tTestState != m_objData.m_tTestState);

    if(bChange)
    {
        m_objData = *pElementData;
    }

    //左眼数据改变
    if(bSignalLeftChange)
    {
        emit signalDataChange(getIndex(),EyeSightStatus::EYE_LEFT_DATA);
    }

    //右眼数据改变
    if(bSignalRightChange)
    {
        emit signalDataChange(getIndex(),EyeSightStatus::EYE_RIGHT_DATA);
    }

    //测试状态改变
    if(bSignalStateChange)
    {
        emit signalHardwareTestStateChange(getIndex(), m_objData.m_cTestState);
    }
}

void CSubElementEyesight::startTest()
{
    m_bStartFlag = true;
    m_objData.reset();

    clearErr();
}

void CSubElementEyesight::stopTest()
{
    m_bStartFlag = false;
}
