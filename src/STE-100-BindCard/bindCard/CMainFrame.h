#ifndef CMAINFRAME_H
#define CMAINFRAME_H

#include <QObject>
#include <QThread>
#include <QDebug>

#include "CSystemSelfCheck.h"
#include "CViewManager.h"
#include "CDataBaseManager.h"
#include "CNetWorkManager.h"

#include "CInteractionStudent.h"
#include "CSyncUserInfoRet.h"
#include "CStudentInfoMgr.h"
#include "CIdentificDevNFC.h"

class CMainFrame : public QObject
{
    Q_OBJECT
public:
    explicit CMainFrame(QObject *parent = nullptr);

    void startLoadUI();

public:
    //同步信息
    void syncStuInfo();
    void syncClassInfo();

    //更新本地临时学生信息
    void updateLocalStuCardNum( QString stuId, QString cardNum );

private:
    void init();
    void initNetWork();
    void initUI();
    void initConnection();

    QString queryStuInfo(QString id);

signals:


    //学生信息相关
public slots:
    void onNewStuInfoAvailable();

    void onUserPageDisplayStuIdChanged(QString id);
    void onClassPageDisplayStuIdChanged(QString id);
    void onUpdateLocalStuCardNum( QString stuId, QString cardNum );

public slots:
    void onNewClassInfoAvailable();

public slots:
    void onViewManegerLoadFinished(bool ok);



private:
    CSystemSelfCheck       m_selfCheck;
    CViewManager          *m_pViewManager;
    CDataBaseManager      *m_pDataManager;
    CNetWorkManager       *m_pNetWorkSyncStuInfo;
    CNetWorkManager       *m_pNetWorkSyncClassInfo;
    CInteractionStudent    m_stuInteraction;
    CSyncUserInfoRet       m_stuInfos;
    CSyncClassInfoRet      m_classInfos;

    CStudentInfoMgr        m_curStuInfo;


private:
    const QUrl          m_viewFile{ "qrc:/main.qml" };
};

#endif // CMAINFRAME_H
