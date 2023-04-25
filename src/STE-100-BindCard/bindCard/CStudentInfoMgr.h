#ifndef CSTUDENTINFOMGR_H
#define CSTUDENTINFOMGR_H

#include <QObject>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

#include "CSyncUserInfoRet.h"

class CStudentInfoMgr : public QObject
{
    Q_OBJECT
public:
    explicit CStudentInfoMgr(QObject *parent = nullptr);

    QString getStuId() const;
    void setStuId(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    QString getClassName() const;
    void setClassName(const QString &value);


    QString getGender() const;
    void setGender(const QString &gender);

    QString toJsonString();

    QString getPicUrl() const;
    void setPicUrl(const QString &picUrl);

    QString getCardNum() const;
    void setCardNum(const QString &cardNum);

signals:

public slots:

private:
    QString m_stuId;
    QString m_name;
    QString m_className;
    QString m_gender;
    QString m_picUrl;
    QString m_cardNum;
};

#endif // CSTUDENTINFOMGR_H
