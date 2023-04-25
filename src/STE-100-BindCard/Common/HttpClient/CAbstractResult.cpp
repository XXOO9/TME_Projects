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

bool CAbstractResult::replaceVal(QString stuId, QString key, QString newVal )
{
    QVector< QMap<QString, QString> >::iterator iter = m_vecRets.begin();
    for( ;iter != m_vecRets.end(); iter++ ){
        if( iter->value( "id" ) == stuId ){
            iter->insert( key, newVal );
            qDebug() << "upload local user card success...";
            return true;
        }
    }
    qDebug() << "upload local user card failed...";
    return false;
}

int CAbstractResult::size() const
{
    return m_vecRets.size();
}
