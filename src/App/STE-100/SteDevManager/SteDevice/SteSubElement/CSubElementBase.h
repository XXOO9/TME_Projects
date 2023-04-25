///////////////////////////////////////////////////////////
//  CSteDevTestUnitBaisc.h
//  Implementation of the Class CSteDevTestUnitBaisc
//  Created on:      08-1月-2022 11:19:51
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_D829AEF9_DD7A_42fc_B748_EE539A8A4763_INCLUDED_)
#define EA_D829AEF9_DD7A_42fc_B748_EE539A8A4763_INCLUDED_

#include "CElementStruct.h"
#include "CCycleQueue.h"
#include "Error/DevExcept.h"
#include "CLogger.h"
#include "CTestStateManage.h"
#include "TestProcAntiCheating/CTestProcAntiCheatingBasic.h"
#include "SteDevManager/SteDevice/CSteCommonDefin.h"
#include "../../SteDevice/CSteCommonDefin.h"

using namespace DataFrameInfo;

#include <QDateTime>
#include <QThread>
#include <QMutexLocker>

class CElementDataBase
{
public:
    CElementDataBase(){}
    virtual ~CElementDataBase(){}

    //在各个继承的测试单元的start函数中进行调用
    virtual void reset()
    {
        m_cTestState = DEV_TEST_STATE_INVALID;
        m_bStart = m_bCompleteInternal = m_bCompleteExternal = false;
    }

    //返回状态是否变更
    bool setTestState(const unsigned char &cState)
    {
        bool bRet = false;
        if( m_cTestState != cState)
        {
            bRet = true;
            m_cTestState = cState;
        }

        switch (cState) {
        case DEV_TEST_STATE_TESTING:
            m_bStart = true;
            break;
        case DEV_TEST_STATE_EOT_VALID:
        case DEV_TEST_STATE_EOT_INVALID:
            m_bStart = true;
            m_bCompleteInternal = true;
            break;
        default:
            break;
        }

        return bRet;
    }

    //当前项目开始测试并且内外标志不同，只有内部完成同时开始状态为true才表示不同
    //是否判断开始标志
    bool isCompleteFlagDif()
    {
        return m_bStart && (m_bCompleteInternal != m_bCompleteExternal && m_bCompleteInternal);
    }

    //判断是否完成 开始标志为true，内部完成和外部完成一致同时为true
    bool isCompleteFlagSame()
    {
        return m_bStart && (m_bCompleteInternal == m_bCompleteExternal && m_bCompleteInternal);
    }

    bool isStart(){return m_bStart;}

    //点数(为了减少数据量在具体的测试单元数据中填写)
    //unsigned short m_shPointNum;

    //测试状态
    unsigned char m_cTestState;

    //完整版的测试状态包含错误信息和测试状态,在测试结束之后可以判断到底是哪一种错误
    uint32_t m_tTestState;

    //------------ 用于确认某个具体项目有没有测试完成(双向保险确保测试项目完成) --------
    //表示设备底层状态，底层标志为 DEV_TEST_STATE_TESTING
    //开始标志有两种，一种是发送给底层开始命令成功，一种是底层标志为开始状态（不稳定，可能刚开始测量就结束了，没有经历过开始）
    bool m_bStart{false};
    //表示内部测试完成，完成标志为 DEV_TEST_STATE_EOT_VALID || DEV_TEST_STATE_EOT_INVALID
    bool m_bCompleteInternal{false};
    //外部完成标志
    bool m_bCompleteExternal{false};
};

/**
 * 测试单元：针对体测设备中的一对一和一对多设备进行统一的测试逻辑，将设备中可用于用户进行测试的模块进行分离
 *
 * 测试单元中的ID可以临时进行分配，分配好了之后证明可以进行测试，或者通过状态管理，针对某一个特殊单元的测试状态进行，
 * 如当前肺活量固定1个测试单元，中长跑为20个，若无id，将对应单元设置为未测状态，通过ID管理进行测试的顺利推荐
 */
#define INVALID_ELEMENT_ID 0

class CSubElementBase : public QObject
{
    Q_OBJECT
public:
    //构造、析构
    CSubElementBase(unsigned short shUnitID, unsigned int iElementID = 0, IDevExcept* const pDevExp = nullptr);
    virtual ~CSubElementBase();

    //设置、获取 测试状态
    inline void setTestState(const short &shState){m_objTestStateManage.setTestState(shState);}
    inline short getTestState(){return m_objTestStateManage.getTestState();}

    //设置、获取 连接状态
    virtual void setConnectStatus(unsigned short usStatus);
    unsigned short getConnectStatus();

    //设置、获取 绑定标志（限制在设备中进行使用）
    inline void setBindFlag(bool bFlag){m_bBindFlag = bFlag;}
    //可在设备外使用
    inline bool getBindFlag(){return m_bBindFlag;}

    //获取子元素是否可用,根据类型进行判段，如中长跑，没有绑定卡号就说明可用
    //其他的如vc未绑定状态和未在测试视为可用
    virtual bool getAvailableFlag();

    //判断子元素连接状态
    bool getConnectFlag();

    // 异常处理接口
    uint32_t checkErr();
    uint32_t getErrCode() const;
    uint32_t getErrType() const;
    uint32_t getLastErrCode() const;
    uint32_t getLastErrType() const;
    string   getErrDesc() const;
    void     clearErr();

    //设置解析出来的 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength);

    //
    inline const unsigned short &getIndex() const{return m_shUnitID;}

    //获取测试状态
    CTestStateManage &getTestStateManage();
    short getCurTestState() const;

    //开启测试单元测试，在统一的地方进行调用，测试状态变更为测试中时进行调用，其他地方不允许调用
    virtual void startTest();

    //测试单元开启测试，有两个地方调用，一个是单个开启设备，一个是外部通过组网开启设备调用，其他地方限制使用
    virtual void stopTest();

    //在设备中设置设备是否是之间设备时调用
    void setAntiCheatingAvailable(bool bAvailable);

    //仿作弊 2.外部获取对象设置所需信息 后续都通过对象进行获取
    //3.当前类内部销毁
    CTestProcAntiCheatingBasic *getAntiCheating();
    inline bool existAntiCheating() {return m_pAntiCheating != nullptr;}
    void startAntiCheating();
    void stopAntiCheating(bool bNormalStop);

    //设置当前测试项目
    void setCurTestItem(short shTestItem);
    inline const short &getCurTestItem() const{return m_shTestItem;}
    inline const unsigned short &getFrameType() const{return m_shDevFrameType;}

    inline const short& getDevType() const {return m_shDevType;}

    //在开始测试时需要对应的测试单元id
    //除开跳绳 中长跑 短跑 其他设备的id和测试单元相同
    //跳绳的id指的是绳id 中长跑id指的是手环id 短跑id指跑道id
    //设置函数通常在设备中使用, 目前中长跑在 测试指导当中使用，慎用
    virtual void setElementID(const unsigned int &nID);
    inline const unsigned int&getElementID() const{return m_iElementID;}

    //开启测试之后数据一直未变更是否通知外部
    static bool s_bNotifyTimeOut;

    //是否检测错误码
    bool isCheckErrCode();

    ///主要作用是记录软件中的命令发送时间和测试单元的正式开启时间
    //设置获取命令发送时间，底层从命令开启开始计时
    inline void setStartCmdSendTime(const long long &llTimestamp){m_llTimeStartCmdSend = llTimestamp;}
    inline void updateStartCmdSendTime(){m_llTimeStartCmdSend = QDateTime::currentDateTime().toMSecsSinceEpoch();}
    inline long long const &getStartCmdSendTime() const {return m_llTimeStartCmdSend;}

    //软件中显示的时间为 底层时间 -（ 真实开启时间 - 底层时间）
    inline void setStartActualTime(const long long &llTimestamp){m_llTimeActualStart = llTimestamp;}
    inline void updateStartActualTime(){m_llTimeActualStart = QDateTime::currentDateTime().toMSecsSinceEpoch();}
    inline const long long &getStartActualTime() const {return m_llTimeActualStart;}

    //设备在使用过程中，相同的异常不会重复通知，调用函数通知异常
    void notifyAbnormal();

    //恢复上次正常状态
    void restoreLastNormalState(){m_objTestStateManage.setTestState(m_shLastNormalTestState);}

    //是否是游客使用
    inline bool isVisitorMode() const{return m_objTestStateManage.isVisitorMode();}

    //设置当前为多项目测试中的第一个项目，需要在设置开始之前使用（主要解决身高体重存在多个项目是调用开始测试时进行对应的处理）
    inline void setFirstTestFlag(bool bFirstFlag) {m_bFirstTestItemFlag = bFirstFlag;}

    inline bool getStartFlag(){return m_bStartFlag;}

    //检测是否可以开始,主要是身高体重仪器在开始之后就不能再次发送对应测试项的开始（避免多次启动）
    virtual bool checkIfCanStart(const short &shTestItem) {return true;}
    //设置测试单元的开始标志，身高体重可能会开始之后直接收到结束数据，在收到开始的返回值之后就设置开始
    virtual void setElementStartFlag(const short &shTestItem, bool bStart) {}

    //外部辅助判断对应的测试项是否已经完成，传测试项目是因为身高体重多项测试,在对应的设备中进行处理
    virtual bool isCompleteFlagDif(const short &shTestItem = TEST_ITEM_VC);
    virtual bool isCompleteFlagSame(const short &shTestItem = TEST_ITEM_VC);
    virtual void setComplete(const short &shTestItem = TEST_ITEM_VC);

    //是否存在新数据
    bool getDataFrameUpdateFlag(){return m_bDataFrameUpdate;}
protected:
    //1.createAntiCheating 仅在内部进行创建和销毁，外部只使用，不进行重复创建
    virtual void createAntiCheating();
    void deleteAntiCheating();
signals:
    //数据变更(第二个参数使用 char,是为了在负数时不识别)
    void signalDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus = 0);
    //测试异常
    void signalErr(const unsigned short &shElementIndex);
    //硬件测试状态变更
    void signalHardwareTestStateChange(const unsigned short &shElementIndex, unsigned char cTestState);

    //软件测试状态变更，这里往外发送的测试状态只存在4种  未测试 测试中 测试完成 测试异常，需要将状态通知到测试流程
    void signalTestStateChange(const unsigned short &shElementIndex, short shTestState);
protected slots:
    //m_objTestStateManage测试状态变更
    virtual void onTestStateChange(short shTestStatus);

protected:
    //单元编号、设备唯一值(例：短跑设备中为跑道)
    unsigned short m_shUnitID{0};
    unsigned int   m_iElementID{INVALID_ELEMENT_ID};

    enum enConnect_Status
    {
        STE_NO_CONNECT = 0,
        STE_NORMAL_CONNECT,
        STE_UNSTABLE_CONNECT,
        STE_CONNECT_COUNT
    };
    unsigned short m_shConnect_Status{STE_NO_CONNECT};

    //绑定标志
    bool m_bBindFlag{false};
    //外部设置当前单元是否开始,外部设置开始才进行某些信号的发送和运行内部数据处理
    bool m_bStartFlag{false};
    //表明底层设备实际开始工作，如坐位体前屈，发送开始命令之后，游标会有

    IDevExcept* const m_pDevExcept;

    //测试状态管理
    CTestStateManage m_objTestStateManage;

    //防作弊类,根据每个测试项目实际进行配置，由当前测试内部
    //自己进行控制
    bool bTurnOnAntiCheating{false};
    CTestProcAntiCheatingBasic *m_pAntiCheating{nullptr};
    QThread *m_pThreadAntiCheating{nullptr};

    //保存当前测试项目,再具体单元当中设置，也可由外部设置，如身高体重可测三个项目，从外部进行设置
    //身高体重同时测试身高和体重
    short m_shTestItem;
    //存储当前项目对应的数据帧中的获取数据类型，在当前测试项目被设置时进行同步变更
    unsigned short m_shDevFrameType;

    //当前对应的设备类型，再具体使用时为了使用方便添加变量,需要在具体设备当中进行赋值
    short m_shDevType{DEV_TYPE_UNKNOW};

    //获取测试数据时对当前值进行赋值
    uint32_t m_tLastTestState{0};

    //资源锁
    QMutex m_mutexResourse;

    //记录当前指令发出成功之后的开始时间，异步模式使用 msec
    long long m_llTimeStartCmdSend;
    long long m_llTimeActualStart;

    //记录上一次正常的状态,用于设备重连之后的状态恢复
    short m_shLastNormalTestState{CTestStateManage::TEST_NOT_START};

    //当前可能存在多个测试项，若当前是第一个，需要根据这个标志重置某些变量,目前身高体重会用到
    bool m_bFirstTestItemFlag{false};

    //防作弊是否可用，目前是通过外部设置，组网不创建，直连创建
    bool m_bAntiCheatingAvailable{false};

    //数据超时时间戳
    qint64 m_llTimeStamp;

    CElementDataBase *m_pElementDataBase{nullptr};

    //每次发送stop,将 数据帧设置为，需要判断是否存在新的数据帧，避免发送命令造成的数据帧阻塞造成
    //的错误标志未更新造成的实心球连续测试完成
    bool m_bDataFrameUpdate{false};
};
#endif // !defined(EA_D829AEF9_DD7A_42fc_B748_EE539A8A4763_INCLUDED_)
