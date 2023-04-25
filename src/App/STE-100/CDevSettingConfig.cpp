#include "CDevSettingConfig.h"

CDevSettingConfig::CDevSettingConfig()
{
    QVariantMap tmp;
    tmp[ "001" ] = "0";
    tmp[ "002" ] = "0";
    tmp[ "003" ] = "0";

    setDevNumberMap( tmp );
}

QVariantMap CDevSettingConfig::getSpecificDataElement(const int element)
{
    QVariantMap devConfigJson;

    switch (element) {

    case CDevSettingConfig::DevElementData::DevNumberInfos:
        devConfigJson[ devNumberConfig ] = generateDevNumberJsonMap();
        break;

    case CDevSettingConfig::DevElementData::DevCorrectInfos:
        devConfigJson[ devCorrectConfig ] = m_devNumbers;
        break;
    default:
        devConfigJson[ devNumberConfig ] = generateDevNumberJsonMap();
        devConfigJson[ devCorrectConfig ] = m_devNumbers;
        break;
    }

    return devConfigJson;
}

void CDevSettingConfig::setSpecificElementData(const int &elementIndex, const QVariantMap &newConfigInfo)
{
    switch ( elementIndex ) {

    case CDevSettingConfig::DevElementData::DevNumberInfos:
        setDevNumberMap( translateArray2VariantMap( newConfigInfo.value( "channelConfig" ).toList() ) );
        break;
    default:
        setAllConfig( newConfigInfo );
        break;
    }
}

QVariantMap CDevSettingConfig::devNumberMap() const
{
    return m_devOrderMap;
}

void CDevSettingConfig::setDevNumberMap(const QVariantMap &devChannelMap)
{
    m_devNumbers.clear();
    m_devOrderMap = devChannelMap;

    for( auto iter = m_devOrderMap.constBegin(); iter != m_devOrderMap.constEnd(); iter++ ){
        m_devNumbers << iter.key();
    }
}


QVariantMap CDevSettingConfig::assistantMaps() const
{
    return m_assistantMaps;
}

void CDevSettingConfig::setAssistantMaps(const QVariantMap &assistantMaps)
{
    m_assistantMaps = assistantMaps;
}

QVariantMap CDevSettingConfig::translateArray2VariantMap(const QVariantList &srcList)
{
    QVariantMap retMap;
    QVariantMap tmpMap;
    for( auto &ele : srcList ){
        tmpMap.clear();
        tmpMap = ele.toMap();
        retMap.insert( tmpMap.value( "devNumber" ).toString(),
                       tmpMap.value( "devChannel" ).toString() );

    }

    return retMap;
}

QVariantList CDevSettingConfig::generateDevNumberJsonMap() const
{
    QVariantList list;
    QVariantMap tmpDevChannel;
    for( auto iter = m_devOrderMap.constBegin(); iter != m_devOrderMap.constEnd(); iter++ ){
        tmpDevChannel.clear();
        tmpDevChannel[ devNumberField ] = iter.key();
        tmpDevChannel[ devChannelField ] = iter.value().toString();
        list << tmpDevChannel;
    }

    return list;
}

void CDevSettingConfig::setAllConfig(const QVariantMap &srcMap)
{
    resetChangeItem();
    QVariantMap mapTemp = translateArray2VariantMap( srcMap.value( "channelConfig" ).toList() );

    if(m_devOrderMap != mapTemp)
    {
        m_lChangeItem |= changeItem::dev::CHANGE_DEV_EXTENSION_NO;
    }

    setDevNumberMap( mapTemp );
}
