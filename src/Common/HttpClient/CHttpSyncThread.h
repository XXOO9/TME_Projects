#ifndef CHTTPSYNCTHREAD_H
#define CHTTPSYNCTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include "CHttpSyncClassInfo.h"
#include "CHttpSyncScoreStd.h"
#include "CHttpSyncUserInfo.h"

class CHttpSyncThread : public QObject
{
    Q_OBJECT
public:
    static CHttpSyncThread *getInstance();

    //发起同步用户信息请求
    void startSyncUserInfo( QString timeStamp = QLatin1String("0"), FACE_ENGINE engine  = TMEc_Soft);

    //发起同步班级信息请求
    void startSyncClassInfo( QString timeStamp = QLatin1String("0") );

    //发起成绩同步请求
    void startSyncScoreStd();

    //获取成绩标准信息
    QVector<QMap<QString, QString> > getVecScoreStd() const;

    //获取同步下来的用户信息
    QVector<QMap<QString, QString> > getVecUserInfo() const;

    //获取同步下来的班级信息
    QVector<QMap<QString, QString> > getVecClassInfo() const;

private:

    explicit CHttpSyncThread(QObject *parent = nullptr);

    void init();

    //初始化线程
    void initThread();

    //初始化信号槽
    void initConnection();

signals:
    void sigSyncUserInfoFinished(bool ok);
    void sigSyncClassInfoFinished(bool ok);
    void sigSyncScoreStdFinished(bool ok);

public slots:
    //响应同步用户信息
    void onSyncUserInfoFinished(bool ok);

    //响应同步班级信息
    void onSyncClassInfoFinished(bool ok );

    //响应同步成绩标注
    void onSyncScoreStd( bool ok );

private:
    //请求对象

    CHttpSyncClassInfo      *m_pSyncClassInfo{nullptr};
    CHttpSyncUserInfo       *m_pSyncUserInfo{nullptr};
    CHttpSyncScoreStd       *m_pSyncScoreStd{nullptr};

    //存同步信息的容器
    QVector<QMap< QString, QString >>   m_vecScoreStd;
    QVector<QMap< QString, QString >>   m_vecUserInfo;
    QVector<QMap< QString, QString >>   m_vecClassInfo;

    QThread                 *m_pWorkThread{nullptr};
    QMutex                   m_reqstMuetx;
    QString                  m_errorString;

    static CHttpSyncThread  *m_pSyncThread;
    static QMutex            m_locker;
};

#endif // CHTTPSYNCTHREAD_H
