///////////////////////////////////////////////////////////
//  CSubElementBody.cpp
//  Implementation of the Class CSubElementBody
//  Created on:      14-1月-2022 10:40:06
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementBody.h"
#include "CDevDataCfg.h"


CSubElementBody::CSubElementBody(unsigned short shUnitID,  unsigned int iElementID)
    :CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_BODY))
{
    setCurTestItem(TEST_ITEM_HEIGHT);
    m_shDevType = DEV_TYPE_BODY;

    m_objHeight.reset();
    m_objWeight.reset();
    m_objHBC.reset(); //人体阻抗
}

CSubElementBody::~CSubElementBody(){

}

void CSubElementBody::setElementInfo(void *pData, unsigned short shLength)
{
    if(pData == nullptr || shLength == 0)
        return;

    CElementBody *pElementData = reinterpret_cast<CElementBody *>(pData);

    m_tLastTestState = pElementData->m_tTestState;
    //软件在测试状态，才进行错误码的判定
    if(/*isCheckErrCode()*/m_bStartFlag)
    {
        CSubElementBase::setElementInfo(pData, shLength);
    }
    else
    {
        m_tLastTestState = 0;
    }

    //身高体重测试存在问题暂不进行判断
    m_tLastTestState = 0;

    //只有在当前测试单元开启时，才向外通知，同时变更对应的测试状态
    if(!m_bStartFlag)
        return;

    bool bSignalStateChange = false, bSignalDataChange = false;

    //外部已经进行了设备类型判断，这里根据实际数据帧类型进行解析

    switch (pElementData->m_shTestItem)
    {
    case TEST_ITEM_HEIGHT:
        {
            if(m_objHeight.m_bCompleteInternal)
                break;

            //设置状态,未经历过开始不往下发送数据变更和完成状态变更,内部会将开始标志设置为true
            bSignalStateChange = m_objHeight.setTestState(pElementData->m_cTestState);
            if(!m_objHeight.m_bStart)
            {
                bSignalStateChange = false;
                break;
            }

            CElementHeight *p = reinterpret_cast<CElementHeight *>(pData);
            bSignalDataChange = compareFloating(p->m_fHeight, m_objHeight.m_fHeight) != 0;
            //设置数据
            m_objHeight.m_fHeight = p->m_fHeight;

            pElementData = &m_objHeight;
        }
        break;
    case TEST_ITEM_WEIGHT:
        {
            if(m_objWeight.m_bCompleteInternal)
                break;

            bSignalDataChange = m_objWeight.setTestState(pElementData->m_cTestState);
            if(!m_objWeight.m_bStart)
            {
                bSignalDataChange = false;
                break;
            }

            CElementWeight *p = reinterpret_cast<CElementWeight *>(pData);
            bSignalDataChange = compareFloating(p->m_fWeight, m_objWeight.m_fWeight) != 0;
            m_objWeight.m_fWeight = p->m_fWeight;

            pElementData = &m_objWeight;
        }
        break;
    //人体阻抗需要判断是否完成
    case TEST_ITEM_HBC:
        {
            if((m_objHBC.m_bCompleteInternal || m_shTestItem != TEST_ITEM_HBC))
                break;

            CElementHBC *p = reinterpret_cast<CElementHBC *>(pData);

            //状态变更进行解析
            bSignalStateChange = m_objHBC.setTestState(pElementData->m_cTestState);
            if(!m_objHBC.m_bStart)
            {
                bSignalStateChange = false;
                break;
            }

            //内部标志为完成，进行赋值操作
            if(m_objHBC.m_bCompleteInternal)
            {
                m_objHBC = *p;
            }

            pElementData = &m_objHBC;
        }
        break;
    default:
        break;
    }

    if(m_objHeight.isCompleteFlagDif())
    {
        m_setElement.insert(&m_objHeight);
    }

    if(m_objWeight.isCompleteFlagDif())
    {
        m_setElement.insert(&m_objWeight);
    }

    if(m_objHBC.isCompleteFlagDif())
    {
        m_setElement.insert(&m_objWeight);
    }

    if(bSignalDataChange)
    {
        emit signalDataChange(getIndex());
    }

    if(bSignalStateChange)
    {
        m_setElement.insert(pElementData);
    }

    //身高体重在逻辑上会出现测试完成没有触发的问题
    for(auto &elemet : m_setElement)
    {
        emit signalHardwareTestStateChange(getIndex(), elemet->m_cTestState);
    }

    m_setElement.clear();
}

void CSubElementBody::startTest()
{
    //测试身高体重会进入多次当前函数，若已经开始不需要重复进行开始，重复开始会将当前数据重置
    m_bStartFlag = true;

    if(m_bFirstTestItemFlag)
    {
        setFirstTestFlag(false);
        clearErr();
        m_objHeight.reset();
        m_objWeight.reset();
        m_objHBC.reset();
    }
}

void CSubElementBody::stopTest()
{
    m_bStartFlag = false;
    m_objHeight.reset();
    m_objWeight.reset();
    m_objHBC.reset(); //人体阻抗
}

void CSubElementBody::setComplete(const short &shTestItem)
{
    setFlag(shTestItem, CSubElementBody::flage_Guid_Complete, true);
}

void CSubElementBody::setFlag(const short &shTestItem, CSubElementBody::flagType flagType, bool bFlag)
{
    CElementDataBase *pElementData = getElementData(shTestItem);

    if(pElementData != nullptr)
    {
        switch (flagType) {
        case Flag_Start:
            pElementData->m_bStart = bFlag;
            break;
        case Flag_Complete:
            pElementData->m_bCompleteInternal = bFlag;
            break;
        case flage_Guid_Complete:
            pElementData->m_bCompleteExternal = bFlag;
            break;
        }
    }
    else
    {
        LOGERROR("shTestItem == " + to_string(shTestItem))
    }
}


bool CSubElementBody::isCompleteFlagDif(const short &shTestItem)
{
    bool bRet = false;
    CElementDataBase *pElementData = getElementData(shTestItem);
    if(pElementData != nullptr)
    {
        bRet = pElementData->isCompleteFlagDif();
    }
    else
    {
        LOGERROR("shTestItem == " + to_string(shTestItem))
    }

    return bRet;
}

bool CSubElementBody::isCompleteFlagSame(const short &shTestItem)
{
    bool bRet = false;
    CElementDataBase *pElementData = getElementData(shTestItem);
    if(pElementData != nullptr)
    {
        bRet = pElementData->isCompleteFlagSame();
    }
    else
    {
        LOGERROR("shTestItem == " + to_string(shTestItem))
    }

    return bRet;
}

bool CSubElementBody::checkIfCanStart(const short &shTestItem)
{
    bool bRet = false;
    CElementDataBase *pElementData = getElementData(shTestItem);
    if(pElementData != nullptr)
    {
        bRet = !pElementData->isStart();
    }
    else
    {
        LOGERROR("shTestItem == " + to_string(getCurTestItem()))
    }

    return bRet;
}

void CSubElementBody::setElementStartFlag(const short &shTestItem, bool bStart)
{
    setFlag(shTestItem, Flag_Start, bStart);
}

CElementDataBase *CSubElementBody::getElementData(const short &shTestItem)
{
    CElementDataBase *pElementData = nullptr;
    switch (shTestItem) {
    case TEST_ITEM_HEIGHT:
        pElementData = &m_objHeight;
        break;
    case TEST_ITEM_WEIGHT:
        pElementData = &m_objWeight;
        break;
    case TEST_ITEM_HBC:
        pElementData = &m_objHBC;
        break;
    default:
        break;
    }

    return pElementData;
}
