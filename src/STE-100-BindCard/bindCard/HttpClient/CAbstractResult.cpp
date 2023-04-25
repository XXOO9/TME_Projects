#include "CAbstractResult.h"

CAbstractResult::CAbstractResult(QObject *parent) : QObject(parent)
{

}

CAbstractResult::~CAbstractResult()
{

}

void CAbstractResult::clear()
{
    m_vecRets.clear();
}

void CAbstractResult::appendRet(QMap<QString, QString> retMap)
{
    if( retMap.size() < 0 ){
        return;
    }
    m_vecRets << retMap;
}

QMap<QString, QString> CAbstractResult::getRetMapByIndex(int index)
{
    if( index > m_vecRets.size() || index < 0 || m_vecRets.isEmpty() ){
        return QMap<QString, QString>();
    }
    return m_vecRets[ index ];
}

QString CAbstractResult::getRet(int index, QString key)
{
    if( index > m_vecRets.size() || index < 0 || m_vecRets.isEmpty() ){
        return "";
    }
    QMap<QString, QString> retMap = getRetMapByIndex( index );
    if( retMap.contains( key ) ){
        return retMap.value( key );
    }
    return "";
}

int CAbstractResult::size() const
{
    return m_vecRets.size();
}
