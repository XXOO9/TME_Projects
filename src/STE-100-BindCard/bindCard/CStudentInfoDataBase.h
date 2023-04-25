#ifndef CSTUDENTINFODATABASE_H
#define CSTUDENTINFODATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>
#include <QDebug>
#include <QSqlError>
#include "CSyncUserInfoRet.h"
#include "DataBaseConfigInfo.h"

#pragma execution_character_set( "utf-8" ) //解决中文乱码问题
class CStudentInfo;
class CClassInfo;
class CStudentInfoDataBase;

class CStudentInfo
{
public:
    CStudentInfo( int id = -1, QString name = "", QString className = "",
                  QString picUrl = "", int gerder = 1, qlonglong timeStamp = 0 );
public:

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    QString className() const;
    void setClassName(const QString &className);

    qlonglong upDateTime() const;
    void setUpDateTime(const qlonglong &upDateTime);

    QString picUrl() const;
    void setPicUrl(const QString &picUrl);

    int gerder() const;
    void setGerder(int gerder);

private:
    int        m_id;
    QString    m_name;
    QString    m_className;
    qlonglong  m_upDateTime;
    QString    m_picUrl;
    int        m_gerder;
};

class CClassInfo
{
public:
    CClassInfo( int id = -1, QString name = "", qlonglong timeStamp = -1 );

    int id() const;
    void setId(int id);

    QString name() const;
    void setName(const QString &name);

    qlonglong timeStamp() const;
    void setTimeStamp(const qlonglong &timeStamp);

private:
    int          m_id;
    QString      m_name;
    qlonglong    m_timeStamp;
};


class CStudentInfoDataBase : QObject
{
    Q_OBJECT
public:
    bool isTablExisted( QString tableName );
    bool executeSql( QString sqlString );
    bool isStudentExisted( int id );

public:
    //学生信息
    //插入一条字段
    bool insertIntoStudent( int id, QString stuName, QString className, QString picUrl, int gerder, qlonglong timeStamp );

    bool deleteFromStudent( int id );

    bool updateStudent( int id, QString key, QString val );

    QVariant querySpecificVal( int id, QString key );
    CStudentInfo queryStudentInfo( int id );

    qlonglong getMaxStuTimeStamp();

    bool isStuInfoEmpty();


public:
    //班级信息
    bool insertClassInfo( int id, QString className, qlonglong timeStamp );

    bool updateClassInfo( int id, QString key, QString val );

    CClassInfo queryClassInfo( int id );

    QVariant querySpecificClassInfo( int id, QString key );

    qlonglong getMaxClassTimeStamp();

    bool isClassInfoEmpty();

private:
    explicit CStudentInfoDataBase( QObject *parent = nullptr );
    ~CStudentInfoDataBase();

    void initDataBase();


public:
    static QMutex                  m_locker;
    static CStudentInfoDataBase   *m_pDataBase;
    static CStudentInfoDataBase   *getInstance();
};

#endif // CSTUDENTINFODATABASE_H
