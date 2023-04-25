///////////////////////////////////////////////////////////
//  CTestGuideManager.h
//  Implementation of the Class CTestGuideManager
//  Created on:      08-1月-2022 11:19:53
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_C72A3EAD_D923_404a_8955_DE417D69B7FA_INCLUDED_)
#define EA_C72A3EAD_D923_404a_8955_DE417D69B7FA_INCLUDED_

#include "CUserInfo.h"
#include "CServerCommunication.h"
#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevSteBase.h"
#include "ProductRegistrar.h"
#include "../Common/HttpClient/CHttpRequestMgr.h"
#include "../Common/HttpClient/CHttpConfigCommonDefine.h"

#include <set>
using namespace std;

namespace BindValue{
//绑定用户返回值
enum BIND_RETURN{
    SUCESS_BIND, //成功绑定
    SUCESS_BIND_EXTRA,//其他信息绑定成功
    ALREADY_BIND, //已经绑定
    NOT_AVAILABLE, //没有可用绑定设备
    NOT_AVAILABLE_TEST, //无可用测试单元
    START_TEST_FALSE, //开始失败，异步测试才会出现这个错
    STARTING_TEST, //正在开启测试，同步模式正在开始测试不进行绑定
    TEST_HAVE_BEGIN,//已经在进行测试，同步模式已经开始不进行绑定
    BANDING_OUTTIME,//绑定超时
    BANDING_CANCEL,//取消绑定
    BANDING_FALSE, //绑定失败
    EXTRA_INFO_HAS_BIND, //额外信息已经被绑定
    DEV_NOT_READY, //设备未就绪
    UNKNOWN //未知错误
};
}

//在同步时需要使用，同步识别是一个界面，开始测试是另一个界面
//为了保证顺序一致，在这里添加信息
class CRecognitionTableInfo{
public:
    const CLocation *m_pLocation;
    const CUserInfo *m_pUserInfo;
};

//除非类型切换，会清空所有的设备，其他时候都是将测试指导内部的驱动停止，直到设备重连
//在类析构时会才会进行全部删除

/**
 * 测试流程，对应设备的使用流程，包含开始、停止测试，测试状态管理，测试过程管理，测试防作弊管理，测试相机配置等
 */

//异步处理逻辑和同步一样，在测试中仅包含

class CTestGuideManager : public QObject
{
    Q_OBJECT
public:
    //0.构造、析构
    CTestGuideManager();
    virtual ~CTestGuideManager();

    //1.设备连接断开操作(设备的插拔、掉线、重连等将会调用),在断开操作中不对测试指导进行删除，在最后统一进行删除
    bool connectSteDev(CDevHardwareBase* pSteDev);
    void disconnectSteDev(CDevHardwareBase* pSteDev);

    //由于当前设备信号原因，在当前测试指导管理中设备断连时不进行删除测试管理类，由于id值唯一，不存在重复的可能
    //若存在重复需要在日志中，
    void deleteSteDevice(CDevHardwareBase* pSteDev);
    void deleteSteDevice(const unsigned int &uiDevID);
    void deleteAllDevice();

    //2.属性设置(允许不设置，不设置则为默认)
    bool setTestMode(const short &shTestMode);
    inline const short &getTestMode() const {return CTestGuideBase::s_shTestMode;}

    //3.用户信息绑定(注：通过返回值判断错误类型)
    short bindUserInfo(const CUserInfo &userInfo, int nDevType = DEV_TYPE_UNKNOW);
    void unbindUserInfo(const CUserInfo &userInfo, int nDevType = DEV_TYPE_UNKNOW);

    //针对现有功能进行开始测试函数编写,在同步模式下或者直连模式下生效
    //同时函数内部存在限制只能对统一类型的设备进行开启

    //说明 开始测试中首先触发设备的开始命令发送  开始命令中会设置设备就绪标志为true 若存在问题会设置变量为false CTestGuideBase::onErr中设置
    //设备存在问题变更会通知当前类中 onDevErrStateChange 在类中判断所有设备不存在问题之后 设置设备状态就绪标志为true
    //然后在测试指导中会确切的发送开始 触发 当前类中的 onStart...函数 在函数中开启对应的测试单元 ，整个开始流程就绪
    bool startTest();
    void startDevice();
    bool stopTest();

    //重新开始
    void reStartTest();

    //取消测试 第一个表示是否自愿取消 ，若自愿取消不保存对应的测试信息，如不是自愿那么调用强制结束，若存在结果就保存
    void cancelTest(const bool &bVoluntaryCancel = false, const bool &bModeChangeFlag = false, const bool bSpeak = true);

    //处理签字路径，同时处理当前测试模式
    void procSignature(const QString &qsPath);

    //取消测试 分为 点击取消测试和定时器到取消(自主取消和被动取消),取消测试为取消对应的测试流程中的测试
    void cancelTest(const CLocation& location, const bool &bVoluntaryCancel);

    //取消成绩
    void deleteScore(QString qsNumber);

    //上传成绩到服务器，点击提交按钮使用
    void uploadTestRestltToServer();

    //清除所有信息
    void clearAll();

    //删除绑定信息
    void deleteBindInfoByDevID(unsigned int devID);
    void deleteBindInfoByUser(const CUserInfo &userInfo, int nDevType = DEV_TYPE_UNKNOW);
    void deleteBindInfoByLocation(const CLocation &location);
    void deleteAllBindInfo();

    //删除用户识别信息（变更调用方式，在删除绑定信息中进行调用，避免信息不一致导致软件出现问题）
    void deleteRecognitionByLocation(const CLocation &location);

    //删除表格对应信息
    void deleteTableByLocation(const int &iTableType, const CLocation &location);

    //删除测试结果信息(默认不删除数据库)
    void deleteResultByLocation(const CLocation &location, bool bDeleteSQL = false);

    //中长跑设备刷卡,跑步NFC设备触发
    void onRunMidSwipingCard(unsigned int nCardID);

    short getLocationTestState(const CLocation& location);

    //跳绳数据接收延时设置
    void  setRopeSkipDelayTime(const int &iDelayTime);

    //得到当前绑定用户姓名
    QString getCurrentUserID(){ return m_qsUserID; }

    //取消绑定(界面调用，发送用户唯一ID)
    void cancelUserBind(QString qsStudentID);

    static bool uploadTestResult(const vector<vector<CTestResultInfo>> &vecTestResult, CHttpRequestMgr *pReqeuestMgr, DATA_SRC src);

    //设置发令枪存在状态
    void setStartingGunExistState(bool bExist){m_bStartingGunExist = bExist;}

    //直连模式下将当前最大有效值设置到界面，组网模式下将所有测试结果填充到指定表格用于显示
    void dispTestResult();

    //清除断连人员信息
    void clearDisconnectUserInfo(){m_mapUserInfoDisconn.clear();}

    //强制退出所有测试，会将存在的测试结果放到结果中
    void forceExitTest();

    inline const int &getLastBindInfo() const {return m_iLastbindDevNo;}
    inline const short &getLastBindDevType() const {return m_shLastBindDevType;}

    //清空要删除的成绩
    void clearDeleteContent() {m_vecDeleteTestRes.clear();}

    //设置跳绳超时等待时长
    void initRopeSkipDelayTime();

    //处理犯规命令
    void onSignalFoulType(unsigned short foulType);
private:
    //2022/08/17  添加, 为避免遍历两次, 将改变数据库标志的部分封装出来
    void changeDataBseScoreFlag( CMultiUploadRetInfo &info );

    //体测设备是否已添加
    bool checkDeviceAdded(const unsigned int &nDevId);

    //检查用户是否绑定
    bool checkUserBound(const CUserInfo &userinfo, int nDevType = DEV_TYPE_UNKNOW);

    //获取测试人员绑定的测试设备信息，直接指向原地址
    set<CLocation> getBindUserInfo(const CUserInfo &userinfo, int nDevType = DEV_TYPE_UNKNOW);
    bool getBindInfoByLocation(const CLocation &findCondition, const CLocation **pLocation, CUserInfo **pUserInfo, int nDevType = DEV_TYPE_UNKNOW);
    bool getBindInfoByUserInfo(const CUserInfo &findCondition, const CLocation **pLocation, CUserInfo **pUserInfo, int nDevType = DEV_TYPE_UNKNOW);

    //查找可用单元信息
    bool findAvailableElement(CLocation &location, int nDevType = DEV_TYPE_UNKNOW);

    //内部调用，在发现可用测试单元之后将测试人员与开启设备位置绑定
    bool bindUserToElement(const CUserInfo &userInfo, const CLocation &location, unsigned int nExtraInfoID = 0);

    //绑定用户信息到界面,注意函数在两个地方使用，一个是绑定用户界面，根据实际的测试模式调用
    //同步在开始时会调用，测试表格会和异步相同，此时固定为异步模式调用
    bool bindUserToUi(const CUserInfo &userInfo, const CLocation &loacation, short shTestMode);

    //获取测试指导
    CTestGuideBase *getTestGuid(const unsigned int &nDevId);

    //开启测试
    bool startTest(const CLocation &location);

    //绑定用户信息分为两种模式(1.直接绑定可用单元 2.需要外部信息)
    short bindUserInfoForCommon(const CUserInfo &userInfo, const CLocation &location);
    short bindUserInfoForExtra(const CUserInfo &userInfo, const CLocation &location, int nDevType = DEV_TYPE_UNKNOW);

    //重置额外绑定信息(中长跑，跳绳)
    void resetExtraBindInfo();

    //查询卡号是否已被绑定
    bool checkRunCardBindState(const unsigned int& nCardId);

    //查询跳绳是否已被绑定
    bool checkSkipRopeBindState(const unsigned int& nCardId);

    //检测时间是否结束
    void onTimeOut();
    void checkUserTestTimeOut(); //检测用户超时检测
    void checkCastStartTestSucess(); //检测广播开始测试成功

    void startRunShortSyncByStartingGun();
    void startRopeSkipSyncByStartingGun();

    //跳绳设备启动的时间(从标志发生变化时记录)
    void onStartSkipRope(const CLocation &location,const QDateTime &startTime);
    //跳绳的绑定消息
    void onSkipBind(const unsigned int &uiElementID);
    //跳绳的断连检测
    void onCheckSubElement(const unsigned int &iSubID, const bool &bFlag);

    //测试准备,若是同步仅是最后一个进行触发
    void onTestPrepare(const CLocation &location);

    //测试单元开启时会触发这里，主要是进行语音配置等，实际开启设备之后触发，在内部调用时为了确保命令发送成功做了其他一些操作
    void onStartElement(const CLocation &location);
    void onStartAllElement(const unsigned int &iDevId);
    void onStartCast();

    //测试单元测试结束
    void onTestComplete(const CLocation &location);

    //开启时间检测定时器,针对某些项目可能已经超时（中长跑超过时间算为0 分）
    //跳绳一分钟（）
    //其他项目检测超过某个时间自动停止(当前定时器开启关闭应该是由设备管理器中的开启关闭进行处理)
    void startCheackTimeTimer();
    void stopCheackTimeTimer();

    //软件内部数据库中保存的的测试结果类型(正式测试和非正式测试)和网页中对应的（测试模式和练习模式之间的转换）
    static short convertC2STestNature(const short &shClientType);

    //更新界面上绑定人数
    void updateBindUserNum();

    //提交成绩 返回值处理
    void dealUploadRetValue(CHttpUploadInfos &infos);

    //更新数据库测试结果同步标志
    static void updateTestResultSyncFlag(const QString &qsUserID, const int &nTestItem, const QString &qsTimeStamp);

    //更新数据库同步标志
    static void updateTestResultSyncFlag(const vector<CTestResultInfo> vecSingleTest);

    //绑定信号解绑信号
    void connectSignal(CDevHardwareBase *pDev, CTestGuideBase *pTestGuid);
    void disconnectSignal(CDevHardwareBase *pDev, CTestGuideBase *pTestGuid);

    //处理展示值
    void dispTestResultForOto();
    void dispTestRestltForOtm();

    //判断是否所有人存在有效成绩
    void checkAllUserCommitable();

/*******************************HTTP相关****************************************/
private:

    //初始化信号槽
    void initSig();

    //返回值为 HTTP类型中的测试项目编号
    static int convertCommonTestRes2ServerFormat(QMap<QString, QVariant> &retInfo, const vector<CTestResultInfo> &vecTestResult, bool bNew = false);
    static bool convertBodyTestRes2ServerFormat(QMap<QString, QVariant> &retInfo, const vector<CTestResultInfo> &vecTestResult, bool bNew = false);
    static bool convertEyeTestRes2ServerFormat(QMap<QString, QVariant> &retInfo, const vector<CTestResultInfo> &vecTestResult, bool bNew = false);

    static void addCommonServerFormatInfo(QMap<QString, QVariant> &retInfo, const CTestResultInfo &testResult);
    static void addHBCFormatInfo(QMap<QString, QVariant> &retInfo, const CTestResultInfo &testResult, const float &fHeight, const float &fWeight);
    static void addHBCFormatInfo(QMap<int, QString> &retInfo, const CTestResultInfo &testResult, const float &fHeight, const float &fWeight);

    //提交结果到本地（数据库）（需要提前进行数据有效性判断）
    bool saveTestResultToLoacl(vector<CTestResultInfo> &vecTestResult);

    //删除结果从本地（数据库）并且删除对应资源（主动取消测试时 及 重新测试时）
    bool deleteDatabaseTestResult(const vector<CTestResultInfo> &vecTestResult);

    //通用提交成功的处理消息
    void submitCommonSuccess();

    //实际开启测试单元
    bool actualStartElement(CTestGuideBase *pTestGuid, const CLocation &location, bool bSpeak);
    bool actualStartAllElement(CTestGuideBase *pTestGuid, bool bSpeak);
    bool actualStartCast(bool bSpeak);

    //初始化人体阻抗解析值与HTTP对应的字符关系
    static map<short, QString> initImpedanceAndSetverValue();
    static map<short, int> initImpedanceAndSetverValueNew();

    //删除断连人员信息
    void deleteDisconnUserInfo(const CUserInfo &uerInfo);

    //断连人员重新连接并且绑定
    void rebindDisconnUser(const unsigned int &nDevId);

    //获取临时结果（未完成的测试）
    void getTemporaryRes(vector<CCompleteTestResult> &vec);

    //在判断结果是否在当前完成的结果中
    static bool isTestResInCurCompletedTestList(const vector<CTestResultInfo> &vecSingleTest);

    //map 转json
    static QString variantMap2Json( const QVariantMap &srcMap );

    //设置上一次绑定信息,获取函数返回分机编号
    void setLastBindInfo(const CUserInfo &userInfo, const CLocation *pLocation);

    //成绩是否删除（根据m_vecDeleteTestRes删除）
    bool isDelTestRes(const CUserInfo &user);

    //添加完成成绩到完成成绩容器(异步模式需要在用户解绑之后再操作)
    void addCompletTestResToCompletContent(const CLocation &location, short shTestMode, const CCompleteTestResult &completeRes);

    //用于模拟分配中长跑卡号
    unsigned int simulationRunMidCard();
public slots:
    //发令枪开始响应,目前只有同步模式会进行响应
    void onStartingGun();

    //测试项目变更,包含测试项目和其中的测试次数等
    void onTestItemChanged();

protected slots:
    //在这里获取上传一般成绩的结果( 异步 )
    void onUploadCommonScoreFinished(QVariant data);

    //在这里获取上传体成分成绩的结果( 异步 )
    void onUploadBodyScoreFinished(QVariant data);

    //在这里获取上传视力成绩的结果( 异步 )
    void onUploadEyeSightScoreFinished(QVariant data);

    void onUploadMulityScoreFinished(QVector<CMultiUploadRetInfo> totalSyncScoreInfos);

    //底层设备状态变更
    void onDevTestStateChange();

    //提交进度变化
    void onCommitPersentChanged(QString val);

    //绑卡倒计时完成
    void onUiNFCValidTimeout();

    //绑卡过程取消
    void onCancelNFCBind();

    //解除绑定
    void onUnbindUser(QString qsStudentID);

    //设备错误状态变更，主要用于同步开启和直连模式下的开始按钮响应
    void onDevErrStateChange();

    //设备断连触发
    void onDevDisconn(const CLocation &location);
private:
    //0.存储 当前绑定的具体设备类与测试流程 的对应关系 key:对应设备号 CTestGuideBase:对应设备的测试指导流程
    map<unsigned int, CTestGuideBase*> m_mapTestGuide;

    //1.存储 子设备与用户信息 的对应关系,外部不应该访问体测设备的测试单元，仅是对应
    //的测试指导进行访问
    map<CLocation, CUserInfo> m_mapUserInfo;

    //保存表格上的用户识别顺序，在m_mapUserInfo中添加时进行添加
    vector<CRecognitionTableInfo> m_vecRecognitionSequence;

    //存储当前绑定人员姓名，用于外部查询成绩
    QString m_qsUserID{"-999"};

    //3.服务器通讯
    CServerCommunication m_CServerCommunication;

    //表明正在开始测试（表示开始函数动作是否在执行）
    bool m_bStartingTest{false};

    //当前是否在测试状态（是否存在仪器正在测试，表明表态）
    bool m_bTestState{false};

    //额外信息绑定所需变量(中长跑、跳绳)
    bool m_bExtraBindFlag; //绑定后为true
    bool m_bExtraUnBindFlag; //取消绑定过程为true
    unsigned int m_nExtraInfoID; //绑定后重置信息
    const int m_nExtraCountdownMsec{10000}; //时间限制

    //检测用户是否测试时间到达，1s一次进行检测,有进行测试的就开启定时器，等待全部测试完成需要停止
    QTimer m_timer;
    int m_nTimerTimeOutInterval{1000}; //ms

    //完成测测试结果信息,(在测试完成之后添加结果到当前表格需要函数进行添加，主要是异步需要判断存不存在成绩)
    static vector<CCompleteTestResult> m_vecCompleteTestRes;
    //保存正在测试过程中的临时成绩(在dispTestResOtm中会进行赋值，主要用于删除)
    vector<CCompleteTestResult> m_vecTempTestRes;
    //存储被删除的测试结果信息，记录人员信息主要通过id和其中的测试时间进行删除（在点击提交的时候清空）
    static vector<CUserInfo> m_vecDeleteTestRes;

    CHttpRequestMgr                *m_pReqeuestMgr;

    CInterAction &m_ui{CInterAction::Get_Instance()};

    //跳绳计时时间(默认60s)
    int m_iSkipTime{60000};
    //跳绳数据延时时间(默认20s)
    int m_iDelayTime{20};
    //跳绳是否处于准备阶段(此阶段不允许点击按钮)
    bool m_bPrepareStatus{false};

    CSteDeviceManager &m_objSteDevMgr{CSteDeviceManager::Get_Instance()};
    CTextToSpeech &m_objSpeak{CTextToSpeech::GetInstance()};

    //开始测试就绪，中长跑等需要使用发令枪进行统一开始的通过变量进行判断发令枪信号的使用
    bool m_bStartTestReady{false};

    //发令枪是否存在
    bool m_bStartingGunExist{false};

    //是否在取消测试状态,在进行取消测试状态不动态更新人数和绑定信息，解决同步测试点击返回之后，可以在取消测试逻辑中进行按钮点击的问题
    bool m_bInCancelTestState{false};

    //保存身高体重人体阻抗解析值对应的HTTP发送类型
    static map<short, QString> s_mapImpedanceAndSetverValue;
    //保存身高体重人体阻抗解析值对应的HTTP发送类型
    static map<short, int> s_mapImpedanceAndSetverValueNew;

    //这里只会存储在绑定阶段断连信息，外部在进行添加时需要进行处理
    map<CLocation, CUserInfo> m_mapUserInfoDisconn;

    //一对多模式，绑定成功之后，需要语音提示 请选择一号设备 或者 短跑的提示 请进入一号跑道，若是 -1 就仅提示绑定成功
    int m_iLastbindDevNo{-1};
    short m_shLastBindDevType{DEV_TYPE_UNKNOW};

    //发送广播只有在存在多个设备时才会使用，发送广播后存在几种情况：设备全部开启成功、部分设备开启不成功、发送命令的设备断连导致全部开启不成功
    //不管情况如何，都在开始触发发送命令时将当前值设置为true，在检查全部设备开启之后（分别在测试状态变更和定时器中判断）或者取消测试之后再设置为false
    bool m_bCheckCastStart{false};
    qint64 m_llCheckCastStartTime; //在命令开始之后一段时间再进行检测，要不会出现语音异常问题
};
#endif // !defined(EA_C72A3EAD_D923_404a_8955_DE417D69B7FA_INCLUDED_)
