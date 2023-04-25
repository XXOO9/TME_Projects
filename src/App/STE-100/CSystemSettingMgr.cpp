#include "CSystemSettingMgr.h"

CSystemSettingMgr::CSystemSettingMgr()
{
    m_mapMapping[CProjectEnums::SystemSettingPage::PAGE_HOST]  = &m_hostConfiger;
    m_mapMapping[CProjectEnums::SystemSettingPage::PAGE_DEV]   = &m_devConfiger;
    m_mapMapping[CProjectEnums::SystemSettingPage::PAGE_USING] = &m_usingConfiger;
}

CHostSettingConfig& CSystemSettingMgr::hostConfiger()
{
    return m_hostConfiger;
}

CDevSettingConfig& CSystemSettingMgr::devConfiger()
{
    return m_devConfiger;
}

CUsingSettingConfig& CSystemSettingMgr::userConfiger()
{
    return m_usingConfiger;
}

QVariantMap CSystemSettingMgr::getSpecificElementData(const int &viewPage, const int elementIndex)
{
    auto itFind = m_mapMapping.find(viewPage);

    QVariantMap mapRet;

    if(itFind != m_mapMapping.end())
    {
        mapRet = itFind.value()->getSpecificDataElement(elementIndex);
    }

    return mapRet;
}


void CSystemSettingMgr::setSpecificElementData(const int &viewPage, const int &elementIndex, const QVariantMap &newConfigInfo)
{
    auto *pCfgPage = getConfig(viewPage);

    QVariantMap mapRet;

    if(pCfgPage != nullptr)
    {
        pCfgPage->setSpecificElementData( elementIndex, newConfigInfo );

        m_setChangePage.clear();

        for(auto itFind = m_mapMapping.begin(); itFind != m_mapMapping.end(); itFind++)
        {
            if(itFind.value()->getChangeItem() != changeItem::CHANGE_NO)
            {
                m_setChangePage.insert(itFind.key());
            }
        }
    }
}

CAbstractSettingElement *CSystemSettingMgr::getConfig(const int &viewPage)
{
    CAbstractSettingElement *pRet = nullptr;

    auto itFind = m_mapMapping.find(viewPage);

    if(itFind != m_mapMapping.end())
    {
        pRet = itFind.value();
    }

    return pRet;
}
