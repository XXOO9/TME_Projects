#include "CAbstractSettingElement.h"



CAbstractSettingElement::CAbstractSettingElement()
{

}

void CAbstractSettingElement::saveIntoDatabase(const QVariantMap &srcMap)
{

}

QJsonArray CAbstractSettingElement::translateVariantMap2JsonArray(const QVariantMap &srcMap, const QString &preKey, const QString &suffixKey)
{
    QJsonArray retArray;
    QJsonObject tmpJson;

    for( auto iter = srcMap.constBegin(); iter != srcMap.constEnd(); iter++ ){
        tmpJson[ preKey ]    = iter.key();
        tmpJson[ suffixKey ] = iter.value().toJsonValue();
        retArray << tmpJson;
    }

    return retArray;
}
