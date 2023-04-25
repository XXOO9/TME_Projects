#include "CMainFrame.h"

CMainFrame::
CMainFrame(QObject *parent) : QObject(parent), m_pViewManager( nullptr )
  ,m_pDataManager( nullptr )
  ,m_pNetWorkSyncStuInfo( nullptr )
  ,m_pNetWorkSyncClassInfo( nullptr )
{
    init();
    syncStuInfo();
}

void CMainFrame::startLoadUI()
{
    if( nullptr == m_pViewManager ){
        m_pViewManager = new CViewManager( m_viewFile, this );
    }

    m_pViewManager->loadView( m_viewFile );
}

void CMainFrame::syncStuInfo()
{
    if( nullptr == m_pNetWorkSyncStuInfo ){
        m_pNetWorkSyncStuInfo = new CNetWorkManager( this );
    }

    QString stringTimeStamp = "0";
//    if( m_pDataManager->pStuDataBase()->isStuInfoEmpty() ){
//        stringTimeStamp = "0";
//    }else{
//        //获取student表里面最大的时间戳
//        stringTimeStamp = QString::number( m_pDataManager->pStuDataBase()->getMaxStuTimeStamp() );
//    }

    m_pNetWorkSyncStuInfo->syncStudentInfo( stringTimeStamp );
}

void CMainFrame::syncClassInfo()
{
    if( nullptr == m_pNetWorkSyncClassInfo ){
        m_pNetWorkSyncClassInfo = new CNetWorkManager( this );
    }


    QString stringTimeStamp = "0";
//    if( m_pDataManager->pStuDataBase()->isClassInfoEmpty() ){
//        stringTimeStamp = "0";
//    }else{
//        //获取student表里面最大的时间戳
//        stringTimeStamp = QString::number( m_pDataManager->pStuDataBase()->getMaxClassTimeStamp() );
//    }
    m_pNetWorkSyncClassInfo->syncClassInfo( stringTimeStamp );
}

void CMainFrame::updateLocalStuCardNum(QString stuId, QString cardNum)
{
    bool bFind = false;
    //在MainFrame处理新的id，传给NetWorkManager
    for( int i = 0; i < m_stuInfos.size(); i++ ){
        QMap<QString, QString> stu = m_stuInfos.getRetMapByIndex( i );
        //找到对应stuId的学生后，进行卡号替换
        if( stuId == stu.value( SyncUserInfoKeys::keyId ) ){
            m_stuInfos.replaceVal( stuId, SyncUserInfoKeys::keyCardNo, cardNum );
            break;
        }
    }
}

void CMainFrame::init()
{
    initUI();
    initNetWork();
    initConnection();
}

void CMainFrame::initNetWork()
{
    if( nullptr == m_pNetWorkSyncStuInfo ){
        m_pNetWorkSyncStuInfo = new CNetWorkManager( this );
    }

    if( nullptr == m_pNetWorkSyncClassInfo ){
        m_pNetWorkSyncClassInfo = new CNetWorkManager( this );
    }
}
void CMainFrame::initUI()
{
    if( nullptr == m_pViewManager ){
        m_pViewManager = new CViewManager( m_viewFile, this );
    }
}

void CMainFrame::initConnection()
{
    connect( m_pViewManager, &CViewManager::sigLoadFinished, this, &CMainFrame::onViewManegerLoadFinished );
    connect( m_pViewManager, &CViewManager::sigStuViewDisplayIdChaged, this, &CMainFrame::onUserPageDisplayStuIdChanged );
    connect( m_pViewManager, &CViewManager::sigClassViewDisplayIdChaged, this, &CMainFrame::onClassPageDisplayStuIdChanged );
    connect( m_pViewManager, &CViewManager::sigUpdateLocalStuCardNum, this, &CMainFrame::onUpdateLocalStuCardNum );
    connect( m_pNetWorkSyncStuInfo, &CNetWorkManager::sigNewStuInfoAvailable, this, &CMainFrame::onNewStuInfoAvailable );
    connect( m_pNetWorkSyncClassInfo, &CNetWorkManager::sigNewClassInfoAvailable, this, &CMainFrame::onNewClassInfoAvailable );
}

QString CMainFrame::queryStuInfo(QString id)
{
    bool bFind = false;
    //在MainFrame处理新的id，传给NetWorkManager
    for( int i = 0; i < m_stuInfos.size(); i++ ){
        QMap<QString, QString> stu = m_stuInfos.getRetMapByIndex( i );
        if( id == stu.value( SyncUserInfoKeys::keyId ) ){

            //先获取班级id 以供查询班级名
            QString classId = stu.value( SyncUserInfoKeys::keyClassId );
            QString className = m_classInfos.getClassName( classId );
            QString picUrl = stu.value( SyncUserInfoKeys::keyAvatarUrl );
            QString name = stu.value( SyncUserInfoKeys::keyName );
            QString gender = ( stu.value( SyncUserInfoKeys::keyGender ).toInt() == 1 ? "男" : "女" );
            QString cardNum = stu.value( SyncUserInfoKeys::keyCardNo );

            m_curStuInfo.setClassName( className );
            m_curStuInfo.setName( name );
            m_curStuInfo.setGender( gender );
            m_curStuInfo.setPicUrl( picUrl );
            m_curStuInfo.setCardNum( cardNum );
            bFind = true;

            break;
        }
    }

    QString infoJsonString = "";
    if( bFind ){
        infoJsonString = m_curStuInfo.toJsonString();
    }

    return infoJsonString;
}


void CMainFrame::onViewManegerLoadFinished(bool ok)
{
    if( !ok ){
        qDebug() << "load UI failed...";
    }else {
        qDebug() << "load UI success...";
    }
}

void CMainFrame::onNewStuInfoAvailable()
{
    //由于暂时不用数据库，所以暂时注释掉
/*    CSyncUserInfoRet stuInfo = m_pNetWorkMgr->stuInfo();
//    for( int index = 0; index < stuInfo.size(); index++ ){
//        QMap<QString, QString> retMap = stuInfo.getRetMapByIndex( index );
//        int stuId = retMap.value( KEY_ID ).toInt();
//        if( stuId == SUPER_ADMINISTRATOR_ID || stuId == ADMINISTRATOR_ID ){
//            continue;
//        }
//        int classId = retMap.value( KEY_CLASSID ).toInt();
//        QString stuName        =     retMap.value( KEY_NAME );
//        QString className      =     m_pDataManager->pStuDataBase()->querySpecificClassInfo( classId, KEY_NAME ).toString();
//        QString picUrl         =     retMap.value( SYNCUSERINFO_KEY_AVATARURL );
//        qlonglong updateTime   =     retMap.value( KEY_UPDATETIME ).toLongLong();
//        int gender             =     retMap.value( SYNCUSERINFO_KEY_GENDER ).toInt();

//        //如果该学生已经存在,则删除后重新插入
//        if( m_pDataManager->pStuDataBase()->isStudentExisted( stuId ) ){
//            m_pDataManager->pStuDataBase()->deleteFromStudent( stuId );
//        }
//        bool ok = m_pDataManager->pStuDataBase()->insertIntoStudent(stuId, stuName, className, picUrl, gender, updateTime );
//        if( !ok ){
//            qDebug() << "insert into class table failed...";
//            break;
//        }
//    }
*/

    //到这里所有的学生信息都已经加载完毕
     m_stuInfos = m_pNetWorkSyncStuInfo->stuInfo();
     for (int i = 0; i < m_stuInfos.size(); ++i) {
         QMap<QString, QString> stuInfo = m_stuInfos.getRetMapByIndex( i );
     }
}

void CMainFrame::onNewClassInfoAvailable()
{
    m_classInfos = m_pNetWorkSyncClassInfo->classInfo();
    for (int i = 0; i < m_classInfos.size(); ++i) {
        QMap<QString, QString> classInfo = m_classInfos.getRetMapByIndex( i );
        CClassInfoItem classInfoItem;
        classInfoItem.setClassId( classInfo.value( SyncUserInfoKeys::keyId ) );
        classInfoItem.setClassName( classInfo.value( SyncUserInfoKeys::keyName ) );
        m_pViewManager->classDataModel()->appendRows( classInfoItem );
    }

    startLoadUI();
}

void CMainFrame::onUserPageDisplayStuIdChanged(QString id)
{
    //根据学生ID，在本地临时存储的信息中查找该ID对应的信息,返回json字符串
    QString infoJsonString = queryStuInfo( id );
    if( infoJsonString.isEmpty() ){
        qDebug() << "查无此人...";
        return;
    }

    m_pViewManager->uiStudent()->setBaseStuId( id );
    emit m_pViewManager->uiStudent()->sigNewDisplayStuInfoAvailable(infoJsonString);
}

void CMainFrame::onClassPageDisplayStuIdChanged(QString id)
{
    QString infoJsonString = queryStuInfo(id);
    if( infoJsonString.isEmpty() ){
        return;
    }

    emit m_pViewManager->uiClass()->sigNewDisplayStuInfoAvailable(infoJsonString);
}

void CMainFrame::onUpdateLocalStuCardNum(QString stuId, QString cardNum)
{
    m_stuInfos.replaceVal( stuId, SyncUserInfoKeys::keyCardNo, cardNum );
    m_pViewManager->uiStudent()->setStuCardNum( cardNum );
    m_pViewManager->uiClass()->setStuCardNum( cardNum );
}
