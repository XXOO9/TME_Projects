#ifndef CHTTPREQUESTMGR_H
#define CHTTPREQUESTMGR_H

#include <QObject>
#include <QMutex>
#include "../common_global.h"
#include "CHttpGetVideoUrls.h"
#include "CHttpSyncScoreStd.h"
#include "CHttpSyncUserInfo.h"
#include "CHttpSyncClassInfo.h"
#include "CHttpUserIdentify.h"
#include "CHttpHeartBeat.h"
#include "CHttpUpLoadCommonScore.h"
#include "CHttpUpLoadBodyScore.h"
#include "CHttpUpLoadEyeSightScore.h"
#include "CHttpTouristRecord.h"
#include "CHttpSyncLocalScores.h"
#include "CHttpUploadInfos.h"
#include "CHttpQueryHistoryScore.h"
#include "CHttpUpLoadMultiCommonScore.h"
#include "CHttpUpLoadMultiBodyScores.h"
#include "CHttpUpLoadMultiEyeSightScores.h"
#include "CHttpMultiHeartBeat.h"
#include "CHttpSyncMultiLocalScores.h"
#include "CMultiUploadRetInfo.h"
#include "CGetServerToken.h"

#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QMap>
#include <QCoreApplication>
#include "../Logger/CLogger.h"

#include "../../App/STE-100/CProjectEnums.h"

Q_DECLARE_METATYPE(CHttpUploadInfos)
class CHttpSyncLocalScores;


class COMMONSHARED_EXPORT CHttpRequestMgr : public QObject
{
    Q_OBJECT

private:
    class CGarbo
    {
      public:
        ~CGarbo()
        {
            if( nullptr != m_pHttpRequestMgr ){
                delete m_pHttpRequestMgr;
                m_pHttpRequestMgr = nullptr;
            }
        }
    };


public:
    static CHttpRequestMgr* getInstance();
    virtual ~CHttpRequestMgr();
    //2022.12.3 新增， 获取服务器token接口
    void getServerTokenInfo( const QString &userName, const QString &passwd );

    //2022/08/17  新增，批量同步本地数据库成绩
    void syncMultiLocalScores(QVariantList &commonList, QVariantList &bodyList, QVariantList &eyeSightList );


    //2022/07/21 新增批量提交接口
    //批量提交Common成绩
    void uploadMultiCommonScores( const QVariantList &scoreList );
    //批量提交Body成绩
    void uploadMultiBodyScores( const QVariantList &scoreList );
    //批量提交EyeSight成绩
    void uploadMultiEyeSightScores( const QVariantList &scoreList );
    //批量提交心跳
    void sendMultiHeartBeat( const QVariantList &heartBeatList );



    //对外调用接口*********获取指导视频链接
    void getSampleVideoUrls( QString timeStamp = QLatin1String("0") );

    //对外调用接口*********同步成绩标准
    void syncScoreStd();

    //对外调用接口*********同步用户信息
    void syncUserInfo( QString timeStamp = QLatin1String( "0" ), FACE_ENGINE faceEngine = TMEc_Soft );

    //对外调用接口*********同步班级信息
    void syncClassInfo( QString timeStamp = QLatin1String( "0" ) );

    //对外调用接口*********用户识别
    void userIdentify( QString faceFeature, FACE_ENGINE faceEngine = TMEc_Soft, QString cardNo = QLatin1String(""), QString code = QLatin1String(""));

    //对外调用接口*********发送心跳
    void sendHeartBeat(devStatus status, QString battery = "", QString devCode = "", short devType = -1);

    //对外调用接口*********上传一般成绩
    void upLoadCommonScore( const QMap<QString, QVariant> &infos);

    //对外调用接口***********上传一般成绩
    void upLoadCommonScore(const QVector<QMap<QString, QVariant>> &vecInfos);

    //对外调用接口*********上传体成分成绩
    void upLoadBodyScore( const QMap<QString, QVariant> &infos);

    //对外调用接口*********上传视力成绩
    void upLoadEyeSightScore( const QMap<QString, QVariant> &infos);

    //对外调用接口*********游客使用记录
    void touristUsageRecord(const QString &devCode, const short &devType , const QString &timeStamp);

    //心跳验证服务器链接过后,立刻上传本地数据库未同步的成绩
    void upLoadLocalScore();

    //查询历史成绩
    void queryHistoryScore( QVariantMap params );



    /************************获取请求结果**************************/
    //获取视频链接
    QVector<QMap<QString, QString>> getVecSampleVieoUrls() const;

    //获取请求错误信息
    QString getErrorString() const;

    //获取成绩标准结果
    QVector<QMap<QString, QString> > getVecScoreStd() const;

    //获取从服务器同步下来的 用户信息
    QVector<QMap<QString, QString> > getVecUserInfo() const;

    //获取从服务器同步下来的 用户信息 分割时间线
    QString getUserSpliTime() const;

    //获取从服务器同步下来删除的 用户信息
    QVector<QString> getDelUsers() const;

    //获取从服务器同步下来的班级信息
    QVector<QMap<QString, QString> > getVecClassInfo() const;

    //获取从服务器同步下来的 班级信息 时间分割线
    QString getClassSpliTime() const;

    //获取从服务器同步下来的 要删除的 班级信息
    QVector<QString> getDelClass() const;

    //获取用户识别出来的每一项详细信息
    QMap<QString, QString> getMapUserInfo() const;

    //获取服务器状态
    bool getBIsOnline() const;

    void setIsOnline( bool ok );

    //递归上传common成绩
    void autoUpload();

    //递归上传体成分成绩
    void autoUploadBodyScore();

    //递归上传视力成绩
    void autoUploadEyesightScore();

    //单个递归提交
    void uploadScore(const short &shDevType, const QVector<QMap<QString, QVariant> > &vecUploadScore, DATA_SRC dataSrc);

    //批量提交
    void uploadScore(const short &shDevType, const QVariantList &listUploadScore, DATA_SRC dataSrc);

    //设置一般成绩
    void setVecUnUploadCommonScore(QVector<QMap<QString, QVariant> > vecUnUploadCommonScore, bool bSyncSignal = false);

    //设置体重上传成绩
    void setVecUnUploadBodyScore(QVector<QMap<QString, QVariant> > vecUnUploadBodyScore, bool bSyncSignal = false);

    //设置视力上传成绩
    void setVecUnUploadEyesightScore(QVector<QMap<QString, QVariant> > vecUnUploadEyesightScore, bool bSyncSignal = false);

    //开始同步本地数据库内容
    void startSyncLocalScore();
    void setSyncCommonScores( QVector<QMap<QString, QVariant>> vecCommonScores );
    void setSyncBodyScores( QVector<QMap<QString, QVariant>> vecBodyScores );
    void setSyncEyeSightScores( QVector<QMap<QString, QVariant>> vecEyeSightScores );

    static QVariant map2Variant( QMap<QString, QVariant> &tmpMap );
    static QMap<QString, QVariant>  variant2Map( QVariant &data );
    static QVariant upLoadInfos2Variant( CHttpUploadInfos &infos );
    static CHttpUploadInfos Variant2UploadInfos( QVariant &data );

    static testType convertScoreType2QueryScoreType( const short &storageScoreType );

    /*********************获取http实例对象指针*****************************************/
    CHttpQueryHistoryScore *getPQueryHistoryScore() const;

    //身高体重人体成分和视力需要传递多结果的的序列化函数,传入的数据为  HTTP_TEST_ITEM_HEIGHT 176.0f 之类的键值队值，类型全是http中对应的类型
    //传出为 "1=176.0@2=55"
    static QString getSerializationResList(const QMap<int, QString> &tmpMap);


    CHttpSyncUserInfo *getPSyncUserInfo() const;

    CHttpSyncClassInfo *getPSyncClassInfo() const;

    CHttpMultiHeartBeat *getPMultiHeartBeatInfos() const;
    //服务器servertoken操作
    QString getServerTokenString() const;
    void setServerTokenString(const QString &serverTokenString);



public slots:

    //2022.12.3 响应更新服务器token
    void onStartGetServerToken(const QVariantMap loginInfoMap );

    //2022/07/21 新增, 批量提交请求结果对外发送
    void onUploadMultiCommonScores(QVariantList list );
    void onUploadMultiBodyScores( QVariantList list );
    void onUploadMultiEyeSightScores( QVariantList list );
    void onSendMultiHeartBeat( QVariantList list );

    void onSyncMultiLocalScores(QVariantList commonList, QVariantList bodyList, QVariantList eyeSightList );


    //响应获取指导视频的信号, 发起请求
    void onGetSampleVideoUrls( QString timeStamp );

    //响应同步成绩标准的信号, 发起请求
    void onSyncScoreStandard();

    //响应同步用户信息的信号, 发起请求
    void onSyncUserInfo(QString timeStamp = QLatin1String("0"), int faceEngine = static_cast<int>(Arc_Soft) );

    //响应同步班级信息的信号, 发起请求
    void onSyncClassInfo( QString timeStamp = QLatin1String("0") );

    //响应用户识别的信号, 发起请求
    void onUserIdentify( QString faceFeature, int faceEngine, QString cardNo = QLatin1String(""), QString code = QLatin1String("") );

    //响应发送心跳的信号, 发起请求
    void onSendHeartBeat(int status, QString battery, QString devCode, short devType);

    //响应上传一般成绩的的信号, 发起请求
    void onUploadCommonScore( QVariant data );

    //响应上传一般成绩的信号,传入QVecctor 里面存QMap
    void onUploadCommonScoreByVector( QVariant data );

    //响应上传体成分成绩的的信号, 发起请求
    void onUpLoadBodyScore( QVariant data );

    //响应上传视力成绩的的信号, 发起请求
    void onUpLoadEyeSightScore( QVariant data );

    //响应上传游客使用记录的的信号, 发起请求
    void onTouristUsageRecord(QString devCode, short devType , QString timeStamp);

    //响应心跳定时器周期性倒计时信号, 发起心跳请求
    void onTimerTimeout();

    //提交第一个数据
    void onAutoUpload();

    //提交第一个体成分成绩
    void onAutoUploadBodyScore();

    //提交第一个视力成绩
    void onAutoUploadEyesightScore();

    //上传通用成绩
    void onUploadCommon();

    //上传身高体重成绩
    void onUploadBody();

    //上传视力成绩
    void onUploadEyesight();

    //同步本地数据库的数据
    void onSyncLocalScores();

    //响应查询成绩
    void onQueryHistoryScore( QVariantMap params );



    //响应成员变量的信号
public slots:
    void onInitInsideMemberConnection();
    void onSyncSingleScoreResultAvailable( QVariant data, int uploadType );
    void onQueryHistoryScoreResultAvailable( bool ok );



    /**********************对外获取各种请求对象**********************************/
    CHttpQueryHistoryScore*         getHttpQueryRequestObj() const;
    CHttpUpLoadMultiCommonScore*    getUploadMultiCommonScoreObj() const;
    CHttpUpLoadMultiBodyScores*     getUploadMultiBodyScoreObj() const;
    CHttpUpLoadMultiEyeSightScores* getUploadMultiEyeSightScoreObj() const;
    CHttpMultiHeartBeat*            getSendMultiHeartBeatObj() const;
    CHttpSyncMultiLocalScores*      getPSyncMultiLocalScores() const;



    /*******************************接收请求结果***************************************/
public slots:
    //2022.12.3 响应获取服务器token
    void onGetServerTokenFinished( bool ok );

    //获取指导视频链接返回结果
    void onGetSampleVideoUrlsFinished( bool ok );

    //获取成绩标准返回结果
    void onSyncScoreStandardFinished( bool ok );

    //同步用户信息返回结果
    void onSyncUserInfoFinished( bool ok );

    //同步班级信息返回结果
    void onSyncClassInfoFinished( bool ok );

    //人员识别返回结果
    void onUserIdentifyFinished( bool ok );

    //响应心跳返回结果*******************
    void onHeartBeatSendFinished( bool ok );

    //上传普通成绩返回结果
    void onUploadCommonScoreFinished( bool ok );

    //上传身高体重成绩返回结果
    void onUploadBodyScoreFinished( bool ok );

    //上传身高体重成绩返回结果
    void onUploadEyesightScoreFinished( bool ok );

    //游客使用记录返回结果
    void onTouristUsageRecordFinished( bool ok );

    //批量提交Common成绩的返回结果
    void onUploadMultiCommonScoreFinished( bool ok );

    //批量提交Body成绩的返回结果
    void onUploadMultiBodyScoreFinished( bool ok );

    //批量提交EyeSight成绩的返回结果
    void onUploadMultiEyeSightScoreFinished( bool ok );

    //批量提交心跳返回的结果
    void onSendMultiHeartBeatFinished( bool ok );

    //批量同步成绩的结果
    void onSyncMultiLocalScoresFinished(QVector<CMultiUploadRetInfo> &totalSyncScoreInfos );


private:
    explicit CHttpRequestMgr(QObject *parent = nullptr);
    void init();
    void initThread();
    void initConnection();
    void initTimer();

private:
    CHttpUploadInfos getUploadInfos(DATA_SRC dataSrc, UPLOAD_TYPE type, bool ok, int requestRetCode);


//对外传递Http 请求的结果信号
signals:
    void sigGetSampleVideoFinished( bool ok );
    void sigSyncScoreStdFinished( bool ok);
    void sigSyncUserInfoFinished( bool ok );
    void sigSyncClassInfoFinished( bool ok );
    void sigUserIdentifyFinished( bool ok );
    void sigHeartBeatFinished(bool ok );
    void sigUploadCommonScore( QVariant data );
    void sigUploadBodyScore(QVariant data);
    void sigUploadEyeSightScore(QVariant data);
    void sigTouristUsageRecordFinished(bool ok, bool netWorkState, int retCode );
    void sigCommitPrograssChanged(QString commitPersent);

    void sigSyncLocalScore(QString userId, QString timeStmap, short testItem, QString errorCode, bool syncSucess);
    void sigQueryHistoryScoreFinished( bool ok, QVariant data );

    //2022/07/21 新增, 批量提交请求结果对外发送的信号
    void sigUploadMultiCommonScoreFinished( QVector<CMultiUploadRetInfo> vecCommonUploadRetInfos );
    void sigUploadMultiBodyScoreFinished( QVector<CMultiUploadRetInfo> vecBodyUploadRetInfos );
    void sigUploadMultiEyeSightScoreFinished( QVector<CMultiUploadRetInfo> vecEyeSightUploadRetInfos );
    void sigSendMultiHeartBeatFinished( bool ok );

    void sigSyncMultiLocalScoresFinished( QVector<CMultiUploadRetInfo> totalSyncScoreInfos );



    //内部信号, 用于多线程调用函数
signals:
    void insideSigInitMemberConnection();
    void insideSigGetVideoUrls( QString timeStamp );
    void insideSigSendHeartBeat(int status, QString battery, QString devCode, short devType);
    void insideSigSyncScoreStd();
    void insideSigSyncUserInfo( QString timeStamp, int faceEngineType );
    void insideSigSyncClassInfo( QString timeStamp );
    void insideSigUserIdentify( QString faceFeature, int faceEngine, QString cardNo, QString code );
    void insideSigUploadCommonScore( QVariant data );
    void insideSigUploadCommonScoreByVector( QVariant data );
    void insideSigUploadBodyScore( QVariant data );
    void insideSigUploadEyeSightScore( QVariant data );
    void insideSigToutisUsageRecord( QString devCode, short devType, QString timeStamp );
    void insideSigStartAuto();
    void insideSigStartAutoUploadBodyScore();
    void insideSigStartAutoEyesightScore();
    void insideSigSyncLocalScore();
    void insideSigQueryHistoryScore( QVariantMap params );

    //2022/07/21新增批量上传
    void insideSigUploadMultiCommonScores( QVariantList list );
    void insideSigUploadMultiBodyScores( QVariantList list );
    void insideSigUploadMultiEyeSightScores( QVariantList list );
    void insideSigSendMultiHeartBeat( QVariantList list );

    //2022/08/17新增批量同步本地成绩
    void insideSigSyncMultiLcoalScores( QVariantList commonList, QVariantList bodyList, QVariantList eyeSightList );

    //2022.12.3 开始定时获取服务器token
    void insideSigStartGetServerToken( const QVariantMap loginInfoMap );



private:
    static CGarbo Garbo;
    CHttpGetVideoUrls                  *m_pGetVideoUrl;
    CHttpSyncScoreStd                  *m_pSyncScoreStd;
    CHttpSyncUserInfo                  *m_pSyncUserInfo;
    CHttpSyncClassInfo                 *m_pSyncClassInfo;
    CHttpUserIdentify                  *m_pUserIdentify;
    CHttpHeartBeat                     *m_pHeartBeat;
    CHttpUpLoadCommonScore             *m_pUploadCommonScore;
    CHttpUpLoadBodyScore               *m_pUploadBodyScore;
    CHttpUpLoadEyeSightScore           *m_pUploadEyeSightScore;
    CHttpTouristRecord                 *m_pTouristRecord;
    CHttpSyncLocalScores               *m_pSyncLocalScores = nullptr;
    CHttpQueryHistoryScore             *m_pQueryHistoryScore = nullptr;
    CHttpUpLoadMultiCommonScore        *m_pUploadMultiCommonScores = nullptr;
    CHttpUpLoadMultiBodyScores         *m_pUploadMultiBodyScores = nullptr;
    CHttpUpLoadMultiEyeSightScores     *m_pUploadMultiEyeSightScores = nullptr;
    CHttpMultiHeartBeat                *m_pMultiHeartBeatInfos  =  nullptr;
    CHttpSyncMultiLocalScores          *m_pSyncMultiLocalScores = nullptr;

    CGetServerToken                    *m_pGetServerToken = nullptr;

    QThread                            *m_pWorkThread;
    QTimer                             *m_pTimer;

    static CHttpRequestMgr             *m_pHttpRequestMgr;
    static QMutex                       m_locker;

    QString                             m_errorString;

    QVector<QMap<QString, QString>>     m_vecSampleVieoUrls;
    QVector<QMap<QString, QString>>     m_vecScoreStd;
    QVector<QMap<QString, QString>>     m_vecUserInfo;
    QVector<QMap<QString, QString>>     m_vecClassInfo;
    QMap< QString, QString >            m_mapUserInfo;
    bool                                m_bIsOnline = false;

    QMutex                              m_upLoadCommonSocreMutex;
    QMutex                              m_pUploadBodyScoreMutex;
    QMutex                              m_pUploadEyeSightScoreMutex;


    atomic<bool>                        m_isRequestFinished = true;

    //存储 通用成绩，身高体重成绩，视力成绩
    QVector<QMap<QString, QVariant>>    m_vecUnUploadCommonScore;
    QVector<QMap<QString, QVariant>>    m_vecUnUploadBodyScore;
    QVector<QMap<QString, QVariant>>    m_vecUnUploadEyesightScore;

    QMap<QString, QVariant>             m_mapCurCommonScore;
    QMap<QString, QVariant>             m_mapCurBodyScore;
    QMap<QString, QVariant>             m_mapCurEyeSightScore;

    CHttpUploadInfos                    m_curCommonUploadInfos;
    CHttpUploadInfos                    m_curBodyUploadInfos;
    CHttpUploadInfos                    m_curEyeSightUploadInfos;

    bool                                m_bSyncFlag = false;

    int                                 m_uploadCommonTotalCount = 0;
    int                                 m_uploadBodyTotalCount = 0;
    int                                 m_uploadEyeSightTotalCount = 0;

    //使用单挑心跳或者是批量心跳的开关
    const bool                          m_useMultiHeartbeat = true;

    QString                             m_serverTokenString = "";
    bool                                m_tokenRequiredFlag = false;    //是否需要带token的标志位
};

#endif // CHTTPREQUESTMGR_H

