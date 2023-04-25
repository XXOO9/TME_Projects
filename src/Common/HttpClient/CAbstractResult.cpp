#include "CAbstractResult.h"

CAbstractResult::CAbstractResult()
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
    QMap<QString, QString> mapTemp;
    if( index < ( m_vecRets.size() ) && index >= 0 ){
        mapTemp = m_vecRets[ index ];
    }

    return mapTemp;
}

QString CAbstractResult::getRet(int index, QString key)
{
    QString qsTemp("");
    QMap<QString, QString> retMap = getRetMapByIndex( index );

    if( retMap.contains( key ) ){
        qsTemp = retMap.value( key );
    }

    return qsTemp;
}

int CAbstractResult::size() const
{
    return m_vecRets.size();
}
