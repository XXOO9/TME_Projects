///////////////////////////////////////////////////////////
//  CTestGuideBase.h
//  Implementation of the Class CTestGuideBase
//  Created on:      08-1月-2022 11:19:53
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_5A646A60_61CD_46d8_BB6B_44F1D14BF2A3_INCLUDED_)
#define EA_5A646A60_61CD_46d8_BB6B_44F1D14BF2A3_INCLUDED_

#include "SteDevManager/SteDevice/CDevHardwareBase.h"
#include "TestProc/CTestProcManage.h"
#include "CTestResultInfo.h"
#include "CScoreRuleInfo.h"
#include "CTextToSpeech.h"
#include "TestMode.h"
#include "CInterAction.h"
#include "SteDevManager/CSteDeviceManager.h"
#include "AssistDevManager/CAssistDevManage.h"
#include "../Common/HttpClient/CHttpRequestMgr.h"

#include <string>

//测试单元需要对应某些信息，这些信息和测试单元存在一定相关性但是关联性质不是测试单元属性
//特地为测试单元创建信息类进行管理

//保证完成信息的一致性，将结果保存完成结果类中
class CCompleteTestResult
{
public:
    CUserInfo m_userInfo;
    vector<CTestResultInfo> m_vecTestResInfo;

    CCompleteTestResult& operator=(const CCompleteTestResult& res)
    {
        m_userInfo = res.m_userInfo;
        m_vecTestResInfo = res.m_vecTestResInfo;
        return *this;
    }
};

//暂定未含犯规错误为0
#define NOT_FOUL 0
//设备反应的违例
#define CONTROL_FOUL 1
//设备反应的违规(此时成绩无效，足球倒杆，足篮按钮结束)
#define CONTROL_STOP 2

class CElementInfo
{  

public:
    enum{TestResultSize = 3};

    CElementInfo();
    CTestProcManage m_objTestProcManage;
    unsigned short m_shCurTestItemIndex;
    CTestResultInfo m_arrTestResultInfo[TestResultSize];

    //指向外部绑定信息，在当前类中不能对变量中的内容进行变更
    const CUserInfo *m_pUserInfo{nullptr};

    //记录本次数据状态码，默认为测试结束有效
    unsigned char m_cHardwareDataStateCode; // DEV_TEST_STATE_EOT_VALID

    //记录本次犯规码，暂未使用，在测试过程中有效
    unsigned int  m_nFoulCode;

    //添加违例次数，目前是足篮排项目使用，外部设备控制违例和犯规，这里仅记录次数，犯规强制结束同时设置上面的犯规标志
    uint m_nIllegalTimes;

    unsigned int  m_TestExcept;

    //在进行开始测试时设备可能存在异常，此时点击返回需要将变量置为false
    atomic<bool> m_bContinueTest{true};

    //表示对应测试单元是否强制退出（中止测试）（中考体测新的需求需要进行处理）
    atomic<bool> m_bAbortTest{false};

    //测试是否真正开启
    bool m_bActuallyWorking;

    void reset()
    {
        m_shCurTestItemIndex = 0;
        m_cHardwareDataStateCode = DEV_TEST_STATE_EOT_VALID;
        m_nFoulCode = NOT_FOUL;
        m_bActuallyWorking = false;
        m_bContinueTest = true;
        m_nIllegalTimes = 0;
        m_bAbortTest = false;

        for(int i = 0; i < CElementInfo::TestResultSize; i++)
        {
            m_arrTestResultInfo[i].reset();
        }
    }

    CElementInfo& operator=(const CElementInfo& elementInfo)
    {
        m_shCurTestItemIndex     = elementInfo.m_shCurTestItemIndex;
        m_pUserInfo              = elementInfo.m_pUserInfo;
        m_cHardwareDataStateCode = elementInfo.m_cHardwareDataStateCode;
        m_nFoulCode              = elementInfo.m_nFoulCode;
        m_nIllegalTimes          = elementInfo.m_nIllegalTimes;
        m_TestExcept             = elementInfo.m_TestExcept;
        m_bContinueTest          = elementInfo.m_bContinueTest ? true : false;
        m_bActuallyWorking       = elementInfo.m_bActuallyWorking;
        return *this;
    }
};

/**
 * 测试流程是针对软件中的开始测试停止测试
 * 将同步、异步标志只汇总到 Manager 中
 * 分离开始测试、停止测试 函数，判断某个测试是否完成也是在这个类中进行判断
 */
class CTestGuideBase : public QObject
{
    Q_OBJECT
public:
    CTestGuideBase();
    virtual ~CTestGuideBase();

    //设置活动状态，外部在设备连接时设置为true，在设备断开时设置为false
    void setActiveState(bool bActive);
    inline bool getActiveState(){return m_bActive;}

    //进行变更，设备断连时设置null进来
    virtual void setSteDevice(CDevHardwareBase* pSteDev);

    CDevHardwareBase *getSteDev();

    //返回设备指针，从理论上来说是不可用的，该指针不安全
    CDevHardwareBase *getSteDevDisabled(){return m_pSteDev;}

    //开启测试和结束测试的逻辑(只针对当前设备开启工作),分别存在针对特定设备单元和全部设备开始的命令
    //为了进行普适性为函数添加标志 标志为是否为测试起始 (如肺活量中三次测试的第一次)
    //是否由当前类执行开始命令
    //是否是广播执行
    virtual bool startTest(const bool &bExecuteStart, const bool &bCast);
    virtual bool stopTest(const bool &bExecuteStart, const bool &bCast);

    //停止之后默认设置为未测试状态，中长跑强制结束需要设置为完成测试状态
    virtual bool stopTest(unsigned short shElementIndex, bool bSetTestState = true, short shTestState = CTestStateManage::TEST_NOT_START);

    //
    inline short getDevType () const{return m_shDevType;}

    //获取绑定单元是否存在错误
    bool existErrInBindElement();

    //外部调用开始函数之前调用
    static void setStartTestTime();

    //设置测试信息

    //为了方便使用，在进行绑定测试人员信息时需要外部进行设置
    //函数只能在外部进行使用，内部使用会和外部信息不一致
    virtual bool bindUserToElement(const unsigned short &shElementIndex, const CUserInfo &userInfo);
    void unbindUserToElement(const unsigned short &shElementIndex);

    //初始化资源路径
    static string getResPath();

    static CScoreRuleInfo &getScoreInfo();

    //获取当前测试项目
    short getCurTestItem(const unsigned short &shElementIndex);

    //获取测试结果,仅在触发测试完成信号之后调用的函数 bDeleteRes标志才是true
    void getTestResult(vector<CTestResultInfo> &vecResult, const unsigned short &shElementIndex, bool bDeleteRes = false);

    QString getUserID(const CElementInfo &elementInfo);

    void setTestState(short shTestState, unsigned short shElementIndex, bool bSpeak = false, bool bWaitSpeak = false, bool bSendToUi = false);
    //设置有效测试单元测试状态
    void setValidElementState(const short &shTestState, bool bSpeak = false, bool bWaitSpeak = false, bool bSendToUi = false);

    //直连设备才会进行调用，外部点击返回,或者内部点击提交（提前结束，存在测试结果会保存对应的测试结果，不存在就相当于取消）
    void forceExitTest();
    void forceExitTest(const unsigned short &shElementIndex);

    //犯规按钮强制结束
    void changeResultToFoul(const unsigned short &shElementIndex, const short &shIndex);

    //违例的增加与减少
    void changeFoulCnt(const unsigned short &shElementIndex, bool bAddFoulCnt);

    //强制完成测试，和强制退出的区别在于 当前不存在测试结果，写入一个默认结果，存在结果不予处理
    //同时将测试状态设置为测试完成,函数调用在开始测试之后调用
    void forceCompleteTest(const unsigned short &shElementIndex);

    //获取临时成绩,根据外部的调用需求是否删除原始传入
    bool getTemporaryRes(vector<CCompleteTestResult> &ret, bool bClear);
    bool getTemporaryRes(CCompleteTestResult &ret, const unsigned short &shElementIndex);

    //移除指定的命令
    void removeCommand(unsigned short &shElementIndex);

    //将结果转为显示结果
    static QString value2DispValue(const short &shTestItem, const float &fValue);
    static QString value2DispValue(const short &shTestItem, const string &strValue);
    //将结果转为
    static string value2SaveValue(const short &shTestItem, const float &fValue);

    //统一开启设备时会进行设备就绪标志判断，若没有就绪就不允许开启
    static void setDevReadyFlag(const bool &bFlag){s_bDevReady = bFlag;}

    //统一开启设备需要统一的变量进行判断
    static void setContinueTestFlag(const bool &bContinue){s_bContinueTest = bContinue;}
    static bool getContinueTestFlag(){return  s_bContinueTest;}

    //获取所有绑定测试单元索引
    vector<unsigned short> getAllBindElementIndex();

    //处理测试开始结束，中长跑等真实开始结束时通过当前统一触发然后处理
    //在肺活量等处理时，这里代表底层的真实开始和结束,这里针对肺活量等进行统一的数据处理
    virtual void procTestBegin(const unsigned short &shElementIndex);
    //结束表示单次测量结束
    virtual void procTestEnd(const unsigned short &shElementIndex);

    //获取成绩, bMaxValidRes 是否获取最大值
    static QString getTestResult(bool bMaxValidRes, const CTestResultInfo &testResult);

    //获取成绩
    static QString getTestScore(bool bMax, const CTestResultInfo &testResult, const CUserInfo &userInfo);

    //清空测试结果，仅仅是结果
    void clearTestRes(const unsigned short &shElementIndex);

    //测试是否真正开启（目前仅针对通过测试标志判断开启的设备）
    bool isTestStart(const unsigned short &shElementIndex);

    set<unsigned short> getResendCmd();
    void setResendCmd(const set<unsigned short> &setTemp = set<unsigned short>(), bool bEndTimeOut = false);

    set<unsigned short> getDelResendCmd();
    void setDelResendCmd(const set<unsigned short> &setTemp = set<unsigned short>());

    void setResendCmdElement(const unsigned short &shElementIndex);

    bool setDevFoulType(const ushort &shElementIndex, const ushort &shCmd);
public slots:
    virtual bool startTest(const unsigned short &shElementIndex, const bool &bFirstTest, const bool &bExecuteStart);

    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) = 0;

    //测试项目变更,同时也证明测试次数变更
    virtual void onTestItemChanged();
signals:
    bool signaldealTestProc(const unsigned short &shElementIndex, const bool &bFirstTest, const bool &bExecuteStart);

    //测试完成
    bool signalTestComplete(const CLocation &location);

    //测试准备
    void sigTestPrepare(const CLocation &location);

    //错误状态变更(设备异常)
    bool signalErrStateChange();

    //设备断连
    void signalDevDisconn(const CLocation &location);
protected slots:
    ///完成的标志有三种，有两种是下发命令之后由底层设备的状态变更进行判断
    /// 另外一种是认为进行停止，按照当前成绩进行本次成绩的显示

    //测试错误处理
    virtual void onErr(const unsigned short shElementIndex);

    //测试单元底层测试状态变更,由于肺活这些都是相同，在这里进行统一处理，若有不同的表格，需要重载
    virtual void onElementTestStateChange(const unsigned short &shElementIndex, short shTestState);

    //硬件测试状态变更
    void onHardwareStateChange(const unsigned short &shElementIndex, unsigned char cTestState);

    //测试完成保存数据，每个测试流程进行存储和获取数据的方式不一致，每个测试指导流程都需要进行重载
    virtual void saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement);

    //处理测试结果,包含界面中结果显示，和外部统一的数据处理
    virtual void procTestResult(CElementInfo &elementInfo, CSubElementBase *pElement, bool bComplete);

    //防作弊资源的处理
    void dealTestAntiCheating(CTestResultInfo &resultInfo, CSubElementBase *pElement);

    bool getElementInfo(CSubElementBase **pSubElement, CElementInfo **pElementInfom, const unsigned short &shElementIndex);

    //时间结束清除犯规
    void onTimeOutRemoveFoul();
public:
    //仅在加入测试管理的时候或者测试模式变更时进行设置
    static short s_shTestMode;
protected:

    //初始化测试单元
    virtual bool initElementInfo(CElementInfo &elemetInfo);

    //connect
    void connectSignal(); //当前设备赋值之后
    void disconnectSignal(); //当前设备赋值之前

    //设置当前测试项目
    bool setCurTestItem(const unsigned short &shElementIndex, bool bResetTestItem);

    //设置测试状态，此时需要播报语音，设置界面状态，设置测试过程进度
    void setTestState(short shTestState, CElementInfo &elementInfo, bool bSpeak = false, bool bWaitSpeak = false, bool bSendToUi = false);

    //
    void onResendCmdTimeOut();

    //显示测试结果
    void dispTestResult(QString &qsDisp, const CElementInfo &elementInfo);

    //配置防作弊模块
    void cfgAnticheating(CElementInfo &elementInfo, CSubElementBase *pSubElement);

    //获取所有绑定测试单元
    vector<CSubElementBase *> getAllBindElements();

    //设备断连处理,调用之前判断需要是组网才能进行调用
    void devDisconnect(CSubElementBase *pElement, const CElementInfo &elementInfo);

    //获取当前测试所处的的测试次数索引,从0开始
    int getCurTestIndex(const CElementInfo &elementInfo);


    //修改已经存储的结果(足球、篮球、排球、斜身、引体)
    void updateTestResult(const unsigned short &shElementIndex, short shIndex = 0);
protected:
    //设备 0表示无效设备
    unsigned int m_iDevID{0};

    //key代表设备中的某个测试单元索引 针对测试单元索引有测试流程管理，测试 完成状态由当前类进行判断
    //在设备内部如长跑只能判断时间变更而不能判断测试是否完成，如身高体重测量也是，只能在当前类判断
    map<unsigned short, CElementInfo> m_mapElementInfo;

    //存储测试单元可测试项目,每个测试单元 测试项目和顺序一致，设置设备时进行初始化
    vector<short> m_vecTestItem;

    //测试开始时间，需要在调用开始函数前进行，由于同步开始需要同时设置多个，这里的启动时间使用静态变量
    static QDateTime s_datetimeStartTest;

    //测试过程资源文件路径
    static string s_strPathTestProRes;
    static CScoreRuleInfo s_objScoreRuleInfo;

    CSteDeviceManager &m_objSteDevMgr{CSteDeviceManager::Get_Instance()};

    CInterAction &m_ui{CInterAction::Get_Instance()};

    CTextToSpeech &m_speak{CTextToSpeech::GetInstance()};

    //命令统一使用定时器进行发送，设备中存在两个定时器，不使用一个定时器进行实现是因为当前定时器存在阻塞行为
    //这里的定时器时间需要比设备发送的时间长，最好在两倍左右，避免造成软件工作不正常的问题
    QTimer m_timerResendStartCmd;
    set<unsigned short> m_setResendElementIndex;
    set<unsigned short> m_setDeleteElementIndex;

    //操作指令时需要对指令集加锁
    QMutex m_mutexResendStartCmd;

    bool m_bForceComplete{false};

    //错误相关定时器
    QTimer m_timerFoul;
    map<unsigned short, qint64> m_mapFoul;

    //标志表明所有的设备没有错误存在，准备就绪
    static bool s_bDevReady;
    //在开启所有设备时，需要用到的是所有设备的继续测试标记，设备断连需要将变量设置为false（中长跑进行了判断）
    static atomic<bool> s_bContinueTest;

    //表明当前类是否内部主动活动，主要是定时器
    bool m_bActive{false};

    //具体的类型和绑定的类型一致
    short m_shDevType{DEV_TYPE_UNKNOW};

    //保存设备指针，仅仅进行设备指针是否变更的判断，不进行其他的用途，该指针是不安全的
    CDevHardwareBase *m_pSteDev{nullptr};

    //从外部添加了需要发送命令的序号
    bool m_bSendNewSerialID{false};
};
#endif // !defined(EA_5A646A60_61CD_46d8_BB6B_44F1D14BF2A3_INCLUDED_)
