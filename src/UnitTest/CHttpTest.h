#ifndef CHTTPTEST_H
#define CHTTPTEST_H

#include <QObject>
#include <QDebug>
#include <QSignalSpy>
#include <QTest>
#include <QtTest>
#include <QDateTime>
#include <QVariant>
#include <QElapsedTimer>
#include "../Common/HttpClient/CHttpRequestMgr.h"
#include "../Common/HttpClient/CHttpAbstractNetWork.h"
#include "../common/HttpClient/CMITDataBatchCommit.h"
#include "../Common/HttpClient/CVRBDataBatchCommit.h"
#include "../Common/HttpClient/CEPSDataBatchCommit.h"
#include "../Common/HttpClient/CQuerySandReport.h"
#include "../Common/HttpClient/CGetServerToken.h"

class CHttpTest : public QObject
{
    Q_OBJECT
public:
    explicit CHttpTest(QObject *parent = nullptr);

    //已测试完成的
public:

    void case_heartBeat();
    void case_userIdentity();
    void case_syncClassInfo();
    void case_uploadCommonScore();
    void case_uploadBodyScore();
    void case_uploadEyeSightScore();
    void case_touristUsageRecord();
    void case_queryHistoryScore();
    //    void case_syncUserInfo();


    void case_translateBodyComposition();

    void case_syncScoreRules();



    /**************基准测试*******************/
    void case_benchmark_case_userIdentity();

    void case_benchmark_case_uploadMultiCommon();



    /***************批量提交测试******************/

    void case_uploadMultiBodyScores();
    void case_uploadMultiEyeSightScores();
    void case_uploadMultiCommonScores();
    void case_multiHeartBeatInfo();


    /***************ESP 测试*****************/

    void case_VRB_commit();

    void case_MIT_commit();

    void case_queryReport();

    void case_Batch_EPS_commit();





    void case_directUploadMultiCommonScore();

    void case_syncMultiLocalScores();

private slots:
    void initTestCase();
    void init();

    void case_getServerToken();

    void case_EPS_commit();



    void cleanup();
    void cleanupTestCase();

private:
    QVariantList generateCommonScores(int maxCount = 256, bool isRepeatTimeStamp = false);
    QVariantList generateBodyScores( int maxCount = 256, bool isRepeatTimeStamp = false );
    QVariantList generateEyeSightScores( int maxCount = 256, bool isRepeatTimeStamp = false );

private:
    CHttpRequestMgr *m_pNetWorkMgr = nullptr;
};

#endif // CHTTPTEST_H
