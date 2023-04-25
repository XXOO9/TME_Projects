#include "CStudentInfoDataBase.h"

#include <QCoreApplication>

CStudentInfoDataBase::CStudentInfoDataBase(QObject *parent) : QObject (parent)
{
    initDataBase();
}

CStudentInfoDataBase::~CStudentInfoDataBase()
{
    if( nullptr != m_pDataBase ){
        delete m_pDataBase;
        m_pDataBase = nullptr;
    }
}

void CStudentInfoDataBase::initDataBase()
{
    QSqlDatabase dataBase = QSqlDatabase::addDatabase( "QSQLITE" );
    if( !dataBase.isValid() ){
        qDebug() << "data base is not accessible...";
        return;
    }

    dataBase.setDatabaseName( DATABASENAME );
    if( !dataBase.open() ){
        qDebug() << "data base open failed...";
        return;
    }

    if( !isTablExisted( KEY_TABLENAME_STUDENT ) ){
        if( !executeSql( "create table student( id int primary key, name text, className text, picUrl text, gerder int, updateTime int )" ) ){
            qDebug() << "create student table failed...";
            QCoreApplication::exit( -1 );
        }
    }

    if( !isTablExisted( KEY_TABLENAME_CLASS ) ){
        if( !executeSql( "create table class( id int primary key, name text, updateTime int )" ) ){
            qDebug() << "create student table failed...";
            QCoreApplication::exit( -1 );
        }
    }
    qDebug() << "open student database suucess...";
}

bool CStudentInfoDataBase::executeSql(QString sqlString)
{
    QSqlQuery sql;
    if( !sql.exec( sqlString ) ){
        qDebug() << "Execute Error:: " << sql.lastError();
        return false;
    }
    return true;
}

bool CStudentInfoDataBase::isStudentExisted(int id)
{
    QSqlQuery sql;
    QString sqlString = QString( "select * from student where id = %1" ).arg( id );
    if( !sql.exec( sqlString ) ){
        qDebug() << "sql failed...";
        return false;
    }

    return sql.next();
}

bool CStudentInfoDataBase::insertIntoStudent(int id, QString stuName, QString className, QString picUrl, int gerder, qlonglong timeStamp )
{
    QSqlQuery sql;
    QString sqlString = \
            QString( "insert into student values( %1, \"%2\", \"%3\", \"%4\", \"%5\", \"%6\" )" )
            .arg( id )  \
            .arg( stuName ) \
            .arg( className )   \
            .arg( picUrl )      \
            .arg( gerder )      \
            .arg( timeStamp );

    bool ok = sql.exec( sqlString );
    if( !ok ){
        qDebug() << "error string = " << sql.lastError();
    }else{
    }
    return true;
}

bool CStudentInfoDataBase::deleteFromStudent(int id)
{
    QSqlQuery sql;
    QString sqlString = QString( "delete from student where id = %1" ).arg( id );
    bool ok = sql.exec( sqlString );
    if( !ok ){
        qDebug() << "error string = " << sql.lastError();
        return false;
    }
    qDebug() << "delete success...";
    return true;
}

bool CStudentInfoDataBase::updateStudent(int id, QString key, QString val )
{
    QSqlQuery sql;
    QString sqlString = QString( "update student set %1 = \"%2\" where id = %3" ).arg( key ).arg( val ).arg( id );
    bool ok = sql.exec( sqlString );
    if( !ok ){
        qDebug() << "update error:: " << sql.lastError();
        return false;
    }
    qDebug() << "update student success...";
    return true;
}

QVariant CStudentInfoDataBase::querySpecificVal(int id, QString key)
{
    QSqlQuery sql;
    QString sqlString = QString( "select %1 from student where id = %2" ).arg( key ).arg( id );
    bool ok = sql.exec( sqlString );
    if( !ok ){
        qDebug() << "query error:: " << sql.lastError();
        return QVariant();
    }
    QVariant ret;
    while( sql.next() ){
        ret = sql.value( key );
    }
    return ret;
}

CStudentInfo CStudentInfoDataBase::queryStudentInfo(int id)
{
    QSqlQuery sql;
    QString sqlString = QString( "select * from student where id = %1" ).arg( id );
    bool ok = sql.exec( sqlString );
    CStudentInfo studentInfo;
    if( !ok ){
        qDebug() << "query error:: " << sql.lastError();
        return studentInfo;
    }

    while( sql.next() ){
        studentInfo.setId( sql.value( KEY_ID ).toInt() );
        studentInfo.setName( sql.value( KEY_NAME ).toString() );
        studentInfo.setClassName( sql.value( KEY_CLASSNAME ).toString() );
        studentInfo.setUpDateTime( sql.value( KEY_UPDATETIME ).toLongLong() );
    }
    return studentInfo;
}

qlonglong CStudentInfoDataBase::getMaxStuTimeStamp()
{
    QSqlQuery sql;
    qlonglong ret = -1;
    QString sqlString = QString( "select Max(updateTime) from " ) + KEY_TABLENAME_STUDENT;
    if( !sql.exec( sqlString ) ){
        qDebug() << "get max timeStamp Error:: " << sql.lastError();
        return ret;
    }

    while( sql.next() ){
         ret = sql.value( FIRSTRESULT ).toLongLong();
    }
    return ret;
}

bool CStudentInfoDataBase::isStuInfoEmpty()
{
    QSqlQuery sql;
    QString sqlString = QString( "select count( * ) from student" );
    if( !sql.exec( sqlString ) ){
        qDebug() << "sql Error:: " << sql.lastError();
        return true;
    }

    while( sql.next() ){
        if( sql.value( FIRSTRESULT ).toInt() <= 0 ){
            return true;
        }
    }
    return false;
}

bool CStudentInfoDataBase::insertClassInfo(int id, QString className, qlonglong timeStamp)
{
    QSqlQuery sql;
    QString sqlString = \
            QString( "insert into class values( %1, \"%2\", \"%3\" )" )
            .arg( id )  \
            .arg( className )   \
            .arg( timeStamp );


    bool ok = sql.exec( sqlString );
    if( !ok ){
        qDebug() << "error string = " << sql.lastError();
    }else{
    }
    return true;
}

bool CStudentInfoDataBase::updateClassInfo(int id, QString key, QString val)
{
    QSqlQuery sql;
    QString sqlString = QString( "update class set %1 = \"%2\" where id = %3" ).arg( key ).arg( val ).arg( id );
    bool ok = sql.exec( sqlString );
    if( !ok ){
        qDebug() << "update error:: " << sql.lastError();
        return false;
    }
    qDebug() << "update student success...";
    return true;
}

CClassInfo CStudentInfoDataBase::queryClassInfo(int id)
{
    QSqlQuery sql;
    QString sqlString = QString( "select * from class where id = %1" ).arg( id );
    bool ok = sql.exec( sqlString );
    CClassInfo classInfo;
    if( !ok ){
        qDebug() << "query error:: " << sql.lastError();
        return classInfo;
    }

    while( sql.next() ){
        classInfo.setId( sql.value( KEY_ID ).toInt() );
        classInfo.setName( sql.value( KEY_NAME ).toString() );
        classInfo.setTimeStamp( sql.value( KEY_UPDATETIME ).toLongLong() );
    }
    return classInfo;
}

QVariant CStudentInfoDataBase::querySpecificClassInfo(int id, QString key)
{
    QSqlQuery sql;
    QString sqlString = QString( "select %1 from class where id = %2" ).arg( key ).arg( id );
    bool ok = sql.exec( sqlString );
    if( !ok ){
        qDebug() << "query error:: " << sql.lastError();
        return QVariant();
    }
    QVariant ret;
    while( sql.next() ){
        ret = sql.value( key );
    }
    return ret;
}

qlonglong CStudentInfoDataBase::getMaxClassTimeStamp()
{
    QSqlQuery sql;
    qlonglong ret = -1;
    QString sqlString = QString( "select Max(updateTime) from " ) + KEY_TABLENAME_CLASS;
    if( !sql.exec( sqlString ) ){
        qDebug() << "get max timeStamp Error:: " << sql.lastError();
        return ret;
    }

    while( sql.next() ){
         ret = sql.value( FIRSTRESULT ).toLongLong();
    }
    return ret;
}

bool CStudentInfoDataBase::isClassInfoEmpty()
{
    QSqlQuery sql;
    QString sqlString = QString( "select count( * ) from class" );
    if( !sql.exec( sqlString ) ){
        qDebug() << "sql Error:: " << sql.lastError();
        return true;
    }

    while( sql.next() ){
        int count = sql.value( FIRSTRESULT ).toInt();
        if( count <= 0 ){
            return true;
        }
    }
    return false;
}

bool CStudentInfoDataBase::isTablExisted(QString tableName)
{
    QSqlQuery sql;
    QString sqlString = QString( "select * from sqlite_master" );
    bool ok = sql.exec( sqlString );
    if( !ok ){
        qDebug() << "sql error:: " << sql.lastError();
        return false;
    }
    while( sql.next() ){
        if( tableName == sql.value( tableName ) ){
            return true;
        }
    }
    return false;
}

CStudentInfoDataBase *CStudentInfoDataBase::getInstance()
{
    m_locker.lock();
    if( nullptr == m_pDataBase ){
        m_pDataBase = new CStudentInfoDataBase();
    }
    m_locker.unlock();
    return m_pDataBase;
}

CStudentInfoDataBase *CStudentInfoDataBase::m_pDataBase = nullptr;
QMutex CStudentInfoDataBase::m_locker;

QString CStudentInfo::name() const
{
    return m_name;
}

void CStudentInfo::setName(const QString &name)
{
    m_name = name;
}

QString CStudentInfo::className() const
{
    return m_className;
}

void CStudentInfo::setClassName(const QString &className)
{
    m_className = className;
}

qlonglong CStudentInfo::upDateTime() const
{
    return m_upDateTime;
}

void CStudentInfo::setUpDateTime(const qlonglong &upDateTime)
{
    m_upDateTime = upDateTime;
}

QString CStudentInfo::picUrl() const
{
    return m_picUrl;
}

void CStudentInfo::setPicUrl(const QString &picUrl)
{
    m_picUrl = picUrl;
}

int CStudentInfo::gerder() const
{
    return m_gerder;
}

void CStudentInfo::setGerder(int gerder)
{
    m_gerder = gerder;
}







CStudentInfo::CStudentInfo(int id, QString name, QString className, QString picUrl, int gerder, qlonglong timeStamp) :
    m_id( id ),
    m_name( name ),
    m_className( className ),
    m_upDateTime( timeStamp ),
    m_picUrl( picUrl ),
    m_gerder( gerder )
{

}

int CStudentInfo::id() const
{
    return m_id;
}

void CStudentInfo::setId(int id)
{
    m_id = id;
}

CClassInfo::CClassInfo(int id, QString name, qlonglong timeStamp)
{

}

int CClassInfo::id() const
{
    return m_id;
}

void CClassInfo::setId(int id)
{
    m_id = id;
}

QString CClassInfo::name() const
{
    return m_name;
}

void CClassInfo::setName(const QString &name)
{
    m_name = name;
}

qlonglong CClassInfo::timeStamp() const
{
    return m_timeStamp;
}

void CClassInfo::setTimeStamp(const qlonglong &timeStamp)
{
    m_timeStamp = timeStamp;
}
