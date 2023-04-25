#ifndef CHTTPSYNCMULTILOCALSCORES_H
#define CHTTPSYNCMULTILOCALSCORES_H

#include <QObject>
#include "../Logger/CLogger.h"
#include <QObject>
#include "../common_global.h"
#include "CHttpUpLoadMultiBodyScores.h"
#include "CHttpUpLoadMultiCommonScore.h"
#include "CHttpUpLoadMultiEyeSightScores.h"


class COMMONSHARED_EXPORT CHttpSyncMultiLocalScores : public QObject
{
    Q_OBJECT
public:
    explicit CHttpSyncMultiLocalScores(QObject *parent = nullptr);

    void setCommonScoreList(  QVariantList &list );
    void setBodyScoreList(  QVariantList &list );
    void setEyeSightScoreList(  QVariantList &list );

    void startSync( const QString serverTokenString = "" );

    void appendSyncSetp();

    int syncSetp();

    CHttpUpLoadMultiBodyScores *pSyncBodyScores() const;

    CHttpUpLoadMultiCommonScore *pSyncCommonScores() const;

    CHttpUpLoadMultiEyeSightScores *pSyncEyeSightScores() const;

signals:
    void sigSyncFinished( QVector<CMultiUploadRetInfo> &totalSyncScoreInfos );

public  slots:
    void onSyncCommonScoresFinished( bool ok );
    void onSyncBodyScoresFinished( bool ok );
    void onSyncEyeSightScoresFinished( bool ok );
    void onSyncTimerTimeout();

private:
    void init();

    void initTimer();
    void timerSwitch( const bool start );

    void initConnection();


private:
    CHttpUpLoadMultiBodyScores      *m_pSyncBodyScores = nullptr;
    CHttpUpLoadMultiCommonScore     *m_pSyncCommonScores = nullptr;
    CHttpUpLoadMultiEyeSightScores  *m_pSyncEyeSightScores = nullptr;

    //缓存common测试项的成绩
    QList< QVariantList >             m_tmpCommonTestItemScoresList;
    QTimer                           m_commonCommitTimer;
    //同步进度,
    int                              m_syncSetp = 0;
    QMutex                           m_stepLocker;
    QVector<CMultiUploadRetInfo>     m_vecTotalScoreRetInfos;

    QMutex                           m_tmpCommonMutex;
};

#endif // CHTTPSYNCMULTILOCALSCORES_H
