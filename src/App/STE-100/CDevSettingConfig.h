#ifndef CDEVSETTINGCONFIG_H
#define CDEVSETTINGCONFIG_H

#include <QObject>
#include <QDebug>
#include "CAbstractSettingElement.h"

const QString devNumberConfig  = "devChannelConfig";
const QString devCorrectConfig = "devCorrectConfig";
const QString devNumberField   = "firstColumn";
const QString devChannelField  = "secondColumn";


class CDevSettingConfig : public CAbstractSettingElement
{
public:
    enum DevElementData{
        DevNumberInfos = 0,
        DevCorrectInfos,
        OusAssiatantDevList
    };

    CDevSettingConfig();

    virtual QVariantMap getSpecificDataElement( const int element = -1 );
    virtual void setSpecificElementData( const int &elementIndex, const QVariantMap &newConfigInfo );

    //设置设备的分机编号，顺带也设置了设备的设备号
    QVariantMap devNumberMap() const;
    void setDevNumberMap(const QVariantMap &devNumberMap);

    QVariantMap assistantMaps() const;
    void setAssistantMaps(const QVariantMap &assistantMaps);

private:
    QVariantMap translateArray2VariantMap( const QVariantList &srcList );

    QVariantList generateDevNumberJsonMap() const;

    void setAllConfig( const QVariantMap &srcMap );

private:
    //分机编号设置组
    QVariantMap m_devOrderMap;

    //设备号 --- 设备校准的地方使用
    QStringList m_devNumbers;


    //外接辅助设备的列表
    QVariantMap m_assistantMaps;

};

#endif // CDEVSETTINGCONFIG_H
