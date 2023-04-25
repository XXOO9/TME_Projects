#include "CSubElementPVV.h"

CSubElementPVV::CSubElementPVV(unsigned short shUnitID, unsigned int iElementID)
    : CSubElementBase(shUnitID, iElementID, createExpectObj(DEV_TYPE_VC))
{
    setCurTestItem(TEST_ITEM_PVV);
    m_shDevType = DEV_TYPE_PVV;
}

CSubElementPVV::~CSubElementPVV()
{

}

void CSubElementPVV::setElementInfo(void *pData, unsigned short shLength)
{
    if(pData == nullptr || shLength == 0)
    {
        LOGERROR("CSubElementPVV::setElementInfo err");
        return;
    }

    CElementDataPVV *pElementData = reinterpret_cast<CElementDataPVV *>(pData);

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
    bool bSignalDataChange = pElementData->m_cValidPointNum != 0;

    auto &arrData = pElementData->m_arrData;
    for(unsigned char i = 0; i < pElementData->m_cValidPointNum; i++)
    {
        m_vecNewPoint.push_back(arrData[i]);
        m_vecAllPoint.push_back(arrData[i]);
    }

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

void CSubElementPVV::startTest()
{
    m_bStartFlag = true;
    m_objData.reset();
    m_vecAllPoint.clear();
    m_vecNewPoint.clear();
    clearErr();
}

void CSubElementPVV::stopTest()
{
    m_bStartFlag = false;
}
