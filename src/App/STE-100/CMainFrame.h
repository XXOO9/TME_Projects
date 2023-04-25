///////////////////////////////////////////////////////////
//  CMainFrame.h
//  Implementation of the Class CMainFrame
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_9DF2B846_1D3B_49a5_A1F9_00A8D6C54C20_INCLUDED_)
#define EA_9DF2B846_1D3B_49a5_A1F9_00A8D6C54C20_INCLUDED_
#include <Windows.h>

#include "COpencvVideoControl.h"
#include "COpencvVideoPlayer.h"

#include "SteTestGuide/CTestGuideManager.h"
#include "AssistDevManager/CAssistDevManage.h"
#include "UserIdentification/CIdentifyUser.h"
#include "SteDevManager/CSteDeviceManager.h"
#include "CInterAction.h"
#include "CProjectEnums.h"
#include "../../Common/HttpClient/CHttpUpLoadEyeSightScore.h"
#include "../../Common/HttpClient/CHttpQueryHistoryScore.h"
#include "../../Common/HttpClient/CHttpRequestMgr.h"
#include "CustomControl/CCustomCurveView.h"
#include "./UserIdentification/CFindUserByFaceFeature.h"
#include "../Common/HttpClient/CHttpUserIdentify.h"
#include "../Common/CDataSynchronize.h"


class CMainFrame : public QObject
{
    Q_OBJECT
public:
    explicit CMainFrame( QObject *parent = nullptr );
    virtual ~CMainFrame();


    //2022.12.10, 对外获取设备详细信息（设备号，分机编号）
    QVariantMap getDevDetailInfos();

    void close();

    //根据服务器结果+本地得分标准获取成绩
    QString getScoreByLocalRules(QString testResult);

    short convertLocalNatureTypeToServer(const int curNatureType);

    //签字板相关初始化
    void initSignPaint();

    //加载签字板
    void initPaint();

    //注册画板对象
    void initPaintServer();

    //关闭签字板进程
    void killProcess(const QString &exeName);

    //处理绑定用户返回值
    void procBindUserReturnValue(const short &shRet);

    //获取多设备状态设置到服务器，用于服务器心跳，在体测设备变更时调用
    void getMultiDevStateSetToServer();

    //重置上一次识别信息，影响正常流程，添加的限制
    void resetLastIdentifyInfo();
    void updateIdentifyInfo(const long long &llUserID);

    //设置立定跳远设备起跳线位置
    bool setSBJStartPos(const int &nPos);

    //体测设备变更
    void onDevConnectChangeSteFactory();

    //开启识别
    void enableUserIdentify();

    //配置变更
    void onCfgChange();
protected:
    //测试模式变更 同步测试 异步测试，界面点击触发
    void onSystemModeChange(short shTestMode);

    //设备连接断连信息（某一个具体设备连接断连），需要保证传入的指针是可用的
    void onDevConnectChangeSteDev(bool bConnec, void *pSteDev);

    //身份识别nfc
    void onDevConnectChangeIdentify(bool bConnect);
    void onFindCardByIdentifyDev(const string &strCard);

    //中长跑刷卡设备
    void onDevConnectChangeRunMidNFC();

    //发令枪设备
    void onDevConnectChangeStartingGun(bool bConnect);

    //电源设备
    void onDevConnecChangePower();
    //电源管理设备电量变更
    void onBatteryChange(short shRemindPower);
    //电源管理设备供电电池连接220v电源
    void onPowerUnitConnect220VPower(bool bConnect);
    //电源管理设备供电电池电量变更
    void onPowerUnitBatteryChange(short shRemindPower);

    //识别用户成功后，识别卡号和识别人脸都会触发
    void onIdentifySuccess( CUserInfo userInfo );
    void onIdentifyFail();

    //响应界面页面变更
    void onPageChanged();

    //响应开始测试
    void onClickBtnStartTest(bool bTouristMode);

    //返回首页 现在只在一对一中出现
    void onClickBtnBackHome();

    //测试页面返回 现在只在一对一中出现
    void onClickBtnTestPageBack();
    //信息界面返回
    void onClickBtnInfoPageBack();
    //提交按钮（只是点击提交按钮）
    void onClickBtnSubmitResult();
    //确认提交结果 (确认提交结果)
    void onClickBtnVerifySubmitResult();

    //响应重新测试
    void onReTest();

    //响应取消测试
    void onCancelTest();

    //取消成绩
    void onDelScore(QString qsNumber);

    //设备配置变更
    void onDevConfigInfoChanged(QVariantMap vMapInfo);

    //查询一般成绩
    void onQueryHistoryScore( int testItem, int  timeInterval, int type );

    void onQueryScoreFinished(bool ok, QVariant data);

    //开始识别录入学生id
    void onStartProcAddUserID(bool bStart, QString id);

    //扫码枪录入学生id
    void onProcScanUserID(QString id);

    //延时开启人脸识别
    void onTimeOutStartIdentify();

    //界面获取设备分机编号，在进行分机编号设置时触发
    void onGetAllDevExtensionNum();

    //界面设置某个设备分机编号
    void onSetDevExtensionNum(QString devID, int devNum);

    //设置校准步骤
    bool onSetCorrectProgress(const int &nStep, const unsigned int &nDevID, const QVariant &val);

    //获取当前设备信道号
    void onGetCurDevChannel();

    //获取当前设备最大连接数量
    void onGetCurDevConnectNum();

    //获取指定页面的设置信息, 默认读取所有信息( 设备数量变更时，也要调用一次 )
    void onGetAllSettingViewInfo( int viewIndex = -1 );

    //用于 显示和影藏签字版
    void onShowWidgetPainterFlagChanged(QString qsShowFlag);

    //清除widget签字版
    void onClearWidgetPainter();

    void onChangedSignPainter( QString cmd );

    //保存widget签字版为png图片
    void onSaveWidgetToPng(QString qsShowFlag);

    //测试模式切换
    void onTestModelChanged(short shModel);

    //修改信道配置
    void onChangeChannelConfig(int channel, int devMaxCount);
protected:
    void initQmlEngine();

    void initConnection();
    void initNetWorkConnection();

    //测试显示代码(在没有设备时，用于显示界面)
    void testDisplayCode(short shDevType, short shTestMode = TEST_SYNC, bool bIsStation = false);

    //发送拉取命令(用户、班级、成绩)
    void sendSyncUserCommand();
    void sendSyncClassCommand();
    void sendSyncScoreCommand();

    //初始化同步对象
    void initDataSync();

    //获取当前设备连接数
    void onTimerOutGetDevConnectNum();

    //向服务器发送同步请求
    void onTimerSyncServerInfo();

    //在进行语音时限制按钮的使用
    void onSpeakState(bool bSpeak);

    //每一次启动软件删除无用文件夹,空的资源路径，或者已经上传的资源路径（一周之前之类的逻辑）认为是无用文件夹
    void deleteUnnecessaryFolder();

    /************************* 校准步骤 ****************************************************/
    bool setCorrectProgressBody(CDevHardwareBase *pDev, const int &nStep, const float &fValue);
    bool setCorrectProgressGrip(CDevHardwareBase *pDev, const int &nStep, const float &fValue);
    bool setCorrectProgressSFB(CDevHardwareBase *pDev, const int &nStep, const float &fValue);
    bool setCorrectProgressPushUp(CDevHardwareBase *pDev, const int &nStep, const float &fValue);

    /*************************    HTTP 相关        *******************************************/
public:
    void getSampleVideoUrls( QString timeStamp = QLatin1String("0") );
    void syncScoreStd();
    void syncUserInfo( QString timeStamp = QLatin1String("0") );
    void syncClassInfo( QString timeStamp = QLatin1String("0") );
    void userIdentify( QString faceFeature, QString cardNo = QLatin1String("0") );
    void sendHeartBeat( devStatus status, QString battery, QString devCode, short devType );
    void upLoadCommonScore( QMap< QString, QVariant > infos );
    void upLoadBodyScore( QMap< QString, QVariant > infos );
    void upLoadEyeSightScore( QMap< QString, QVariant > infos );
    void touristUsageRecord( QString devCode, DEV_TYPE devType );

    void sendMultiHeartBeat();

    //新设备测试结果已组装测试结果-违规次数
    bool procHttpQueryRets(QVector<QStringList> &displayVec, const QVector<QStringList> &vecStrList);

signals:
    void sigGetSampleVideoUrls( QString timeStamp );
    void sigSyncScoreStd();
    void sigSyncUserInfo( QString timeStamp );
    void sigSyncClassInfo( QString timeStamp );
    void sigUserIdentify( QString faceFeature, QString cardNo );
    void sigSendHeartBeat( int devStatus, QString battery, QString devCode, short devType );
//    void sigUploadCommonScore( QVariant data );
    void sigUploadBodyScore( QVariant data );
    void sigUploadEyeSightScore( QVariant data );
    void sigTouristUsageRecord( QString devCode, int devType );

    void sigUpdateUserInfo();
    void sigUpdateClassInfo();
    void sigUpdateScoreInfo();
    
    void sigUpdateDevConfig(bool bSuccess);

    void sigUpdateDevChannel(int channel);
    void sigUpdateDevMaxConnectNum(int num);
    void sigQuite();
public slots:
    //用于获取HTTP请求的结果
    void onSampleVideoUrlsFinished( bool ok );
    void onSyncScoreStdFinished( bool ok );
    void onSyncUserInfoFinished( bool ok );
    void onSyncClassInfoFinished( bool ok );
    void onUserIdentifyFinished( bool ok );
    void onSendHeartBeatFinished( bool ok );
    void onUploadCommonScoreFinished(bool ok);
    void onUploadBodyScoreFinished(bool ok);
    void onUploadEyeSightScoreFinished(bool ok);
    void onTouristUsageRecordFinished(bool ok);

    void onSyncStatusChanged(short shSyncStatus);
    void onStopSync();

    //2022.12.13 响应批量心跳结果
    void onSendMultiHeartBeatFinished( bool ok );

 /***********************************************************************************************/
private:
    //查询本地数据库身高体重成绩
    void queryLocalHumanCompositionTestResult(QVariantList &srcTestRetList, const QVariantMap &queryConditions );

    //去掉没有成对出现的身条体重测试记录
    void removeSingleRecord( vector<CTestResultInfo> &vecHeightRecords, vector<CTestResultInfo> &vecWeightRecords );

    //获取主机设置页面的信息
    void getHostPageInfo();

    //获取设备设置界面信息
    void getDevPageInfo();

    //获取测试设置界面信息
    void getUsingPageInfo();

public slots:
    //调试模式查找学生
    void queryStudentByUniqueID(QString stuUniqueID );

 /*************************************签字版相关***************************************************/
public slots:
    void onSignPainterServerNewConnection();
    void onSignPainterSocketReadyRead();

private:

    //2022.12.21 计时类测试项的测试结果转换为 分·秒形式(忽略毫秒)
    QString convertSecs2FmtContent( const double &seconds );

    //2022.12.21 判断是否是长时间(超过1分钟，需要进行分秒转换的)计时类的测试项目
    bool isLongTimeKeepingTestItem(const TEST_ITEM &srcTestItem );


    //软件启动时，发送一个空心跳包
    void sendEmptyHeartbeatPkt();

    bool startIdentify();
    void stopIdentify();

    CTestGuideManager m_objTestGuideManager;

    //设备管理(协助设备)
    CAssistDevManage m_objAssistDevMgr;

    //身份识别(包含人脸 nfc刷卡 手动输入)
    CIdentifyUser m_objIdentifyUser;

    //电源设备(固定只会存在一个指针)
    bool m_bFirstConnectPowerUart{true};

    CDevRunNFC    *m_pDevRunNFC{nullptr};

    //使用单例
    CSteDeviceManager &m_objSteDevMgr{CSteDeviceManager::Get_Instance()};

    //对于ui的变更只有两个地方可以影响，一个是mainframe一个是测试指导模块
    CInterAction      &m_ui{CInterAction::Get_Instance()};

    //签字板相关
    QLocalSocket *m_pLocalPainterSocket{ nullptr };


    bool          m_bPainetrRunning{ false };
    double        m_dFactor;
public:
    //QML 引擎
    QQmlApplicationEngine    m_objQmlEngine;

private:
    CHttpRequestMgr         *m_pRequestMgr{nullptr};

    CDataSynchronize        *m_pDataSync{ nullptr };
    QThread                 *m_pSyncThread{ nullptr };

    short m_shQuertTestItem{TEST_ITEM_UNKNOW};

    //将信息同步分离拉取：用户、班级、成绩
    bool m_bFirstSyncFlag{true};

    bool m_bSyncUserFlag{true};
    bool m_bSyncClassFlag{true};
    bool m_bSyncScoreFlag{true};

    bool m_bCompleteUserFlag{false};
    bool m_bCompleteClassFlag{false};
    bool m_bCompleteScoreFlag{false};

    bool m_bAvailable{true};

    bool m_bFirstInChange{false};

    bool m_bPowerConnect{false};

    //更新设备计时器
    int m_iIntervalGetDevInfo{1000};
    QTimer m_timerGetDevConnectCount;

    //同步数据库信息计时器
    QTimer m_timerSyncServerInfo;

    int m_iSyncServerInfoTime{10000};

    //服务器通讯标志
    bool m_bNetworkFlag{false};
    CUserInfo       m_userInfo;

    //保存当前所在界面，除开弹窗和视频播放界面
    short m_shCurPage;

    //与签字版通信的进程服务器
    QLocalServer       *m_pLocalServer = nullptr;

    //用于表示是否需要提示，最新需求不进行繁杂的识别语音提示，
    //设备数量变量，设备变更，界面变更，识别人员变更，识别失败，测试状态变更（粗略定为确认提交成绩）， 超出一定时间进行识别时 重置为默认值进行正常流程
    long long m_llLastIdentify{-1};
    qint64 m_llLastIdentifyTime{0};
    int m_iLastIdentifyResetTime{5000}; //初定5s

    //是否仅开启ID识别
    bool m_bIDRecgOnlyFlag{false};

    QProcess *m_painterProcess;

    //同步人员信息状态
    bool     m_syncUserInfoRunning = false;
};
#endif // !defined(EA_9DF2B846_1D3B_49a5_A1F9_00A8D6C54C20_INCLUDED_)
