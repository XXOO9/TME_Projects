#include "CHttpConfigReaderSingeton.h"

QMutex CHttpConfigReaderSingeton::m_locker;
CHttpConfigReaderSingeton *CHttpConfigReaderSingeton::m_pSingleton = nullptr;

CHttpConfigReaderSingeton::CHttpConfigReaderSingeton(QObject *parent) : QObject(parent), m_pSetting( nullptr )
{
    if( nullptr == m_pSetting ){
        m_pSetting = new QSettings( CONFIGFILEPATH, QSettings::IniFormat, this );
    }
    //检查是否存在
    if( !QFileInfo::exists( CONFIGFILEPATH ) ){
        createDefaultConfig();
    }
    qDebug() << "read existed config file...";
}

void CHttpConfigReaderSingeton::createDefaultConfig()
{
    if( nullptr == m_pSetting ){
        m_pSetting = new QSettings( CONFIGFILEPATH, QSettings::IniFormat, this );
    }

    //设置ip 端口
    m_pSetting->setValue( BASEIP_KEY, BASEIP_VALUE );

    //服务器测试接口
    m_pSetting->setValue( INTERFACE_GET_PARAMETER_KEY, INTERFACE_GET_PARAMETER_VALUE );

    m_pSetting->setValue( INTERFACE_SYNCVIDEO_KEY, INTERFACE_SYNCVIDEO_VALUE );
    m_pSetting->setValue( INTERFACE_SYNCSCORE_KEY, INTERFACE_SYNCSCORE_VALUE );
    m_pSetting->setValue( INTERFACE_SYNCUSERINFO_KEY, INTERFACE_SYNCUSERINFO_VALUE );
    m_pSetting->setValue( INTERFACE_USERIDENTIFY_KEY, INTERFACE_USERIDENTIFY_VALUE );
    m_pSetting->setValue( INTERFACE_HEARTBEAT_KEY, INTERFACE_HEARTBEAT_VALUE );
    m_pSetting->setValue( INTERFACE_UPLOADSOCRE_KEY, INTERFACE_UPLOADSOCRE_VALUE );
    m_pSetting->setValue( INTERFACE_SYNCCLASSINFO_KEY, INTERFACE_SYNCCLASSINFO_VALUE );
    m_pSetting->setValue( INTERFACE_QUERYHISTORY_KEY, INTERFACE_QUERYHISTORY_VALUE );
    m_pSetting->setValue( INTERFACE_BODYSCORE_KEY, INTERFACE_BODYSCORE_VALUE );
    m_pSetting->setValue( INTERFACE_TOURISTRECORD_KEY, INTERFACE_TOURISTRECORD_VALUE );
    m_pSetting->setValue( INTERRFACR_UPDATEUSERBINDCARDNUMBER_KEY, INTERFACR_UPDATEUSERBINDCARDNUMBER_VALUE );
    m_pSetting->setValue( INTERRFACR_DISBINDCARDNUMBER_KEY, INTERFACR_DISBINDCARDNUMBER_VALUE );
    m_pSetting->setValue( INTERFACE_USERLOGIN_KEY, INTERFACE_USERLOGIN_VALUE );
    qDebug() << "create default httpConfig.ini";
}

QString CHttpConfigReaderSingeton::getValue( QString key )
{
    //安全检查
    if( nullptr == m_pSetting ){
        m_pSetting = new QSettings( CONFIGFILEPATH, QSettings::IniFormat, this );
    }

    if( !m_pSetting->contains( key ) ){
        return "";
    }

    QString value = m_pSetting->value( key ).toString();
    return value;
}
CHttpConfigReaderSingeton *CHttpConfigReaderSingeton::getInstance()
{
    if( nullptr == m_pSingleton ){
        m_locker.lock();
        if( nullptr == m_pSingleton ){
            m_pSingleton = new CHttpConfigReaderSingeton();
        }
        m_locker.unlock();
    }
    return m_pSingleton;
}


