#ifndef CSYSTEMSETTINGMGR_H
#define CSYSTEMSETTINGMGR_H

#include "CHostSettingConfig.h"
#include "CDevSettingConfig.h"
#include "CUsingSettingConfig.h"
#include "CProjectEnums.h"
#include <QObject>

class CSystemSettingMgr  : public QObject
{
    Q_OBJECT
public:
    CSystemSettingMgr();
    virtual ~CSystemSettingMgr(){}

    CHostSettingConfig& hostConfiger();

    CDevSettingConfig& devConfiger();

    CUsingSettingConfig& userConfiger();

    QVariantMap getSpecificElementData( const int &viewPage, const int elementIndex = -1 );

    void setSpecificElementData( const int &viewPage, const int &elementIndex, const QVariantMap &newConfigInfo );

    bool isExistChangeItem(){return m_setChangePage.size() != 0;}

    QSet<int> getChangedPage(){return m_setChangePage;}

    CAbstractSettingElement *getConfig(const int &viewPage);

private:
    CHostSettingConfig  m_hostConfiger;
    CDevSettingConfig   m_devConfiger;
    CUsingSettingConfig m_usingConfiger;

    //保存设备变更选项
    QSet<int> m_setChangePage;
    QMap<int, CAbstractSettingElement *> m_mapMapping;
};

#endif // CSYSTEMSETTINGMGR_H
