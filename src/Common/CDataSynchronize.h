#ifndef CDATASYNCHRONIZE_H
#define CDATASYNCHRONIZE_H

#include <QObject>
#include "common_global.h"
#include "Config/CDatabaseManage.h"

//数据同步类：从服务器拉取的数据不确定数量，因此在同步到数据库时单独开启一个线程
class COMMONSHARED_EXPORT CDataSynchronize : public QObject
{
    Q_OBJECT
public:
    CDataSynchronize(QObject *parent = nullptr);
    virtual ~CDataSynchronize();

    enum SYNCSTATUS{
        SYNC_NOT = 0,
        SYNC_PROCESS_USER,
        SYNC_PROCESS_CLASS,
        SYNC_PROCESS_SCORE,
        SYNC_COMPLETE_USER,
        SYNC_COMPLETE_CLASS,
        SYNC_COMPLETE_SCORE,
        SYNC_FAILED_USER,
        SYNC_FAILED_CLASS,
        SYNC_FAILED_SCORE
    };

    static void equalUserInfo(CUserInfo &targetUserInfo, const QMap<QString, QString> &srcUserInfo);
    static void equalClassInfo( CClassInfo &targetClassInfo, const QMap<QString, QString> &srcClassInfo);
    static void equalScoreInfo( CScoreRule &targetScoreInfo, const QMap<QString, QString> &srcScoreInfo);
public slots:
    void onUpdateUserInfo();

    void onUpdateClassInfo();

    void onUpdateScoreInfo();

    void onDevTestStateChange(short shTestStatus);

signals:
    void sigSyncStatusChanged(short shSyncStatus);

    void sigStopWork();
private:
    //等待更新的标志
    bool m_bUserFlag{false};
    bool m_bClassFlag{false};
    bool m_bScoreFlag{false};

    //允许更新的标志
    bool m_bAvailableFlag{true};

    //已经更新的标志(都更新后停止线程)
    bool m_bIsUpdateUser{false};
    bool m_bIsUpdateClass{false};
    bool m_bIsUpdateScore{false};
};

#endif // CDATASYNCHRONIZE_H
