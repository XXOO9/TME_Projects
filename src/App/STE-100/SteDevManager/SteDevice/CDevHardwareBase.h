#ifndef CDEVHARDWAREBASE_H
#define CDEVHARDWAREBASE_H
#include "CDevSteBase.h"
#include "SteSubElement/CSubElementBase.h"
#include <list>

class CDevHardwareBase : public CDevSteBase
{
    Q_OBJECT
public:
    CDevHardwareBase(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevHardwareBase() override;

    //体测设备不是独立使用通讯，连接相关的信号即可
    virtual void setCommunicate(ICommunicate *pCommunicate) override;

    //开始 停止工作对外只提供单个测试单元开启和全部测试单元开启，开启之后处不处理数据由具体的测试单元进行决定
    //开启关闭当前类型设备
    virtual bool startWork(bool bCast, bool bWait = true);
    virtual bool stopWork(bool bCast, bool bWait = true);

    //开启关闭当前设备 下面的确切设备
    virtual bool startWork(unsigned short shElementIndex, bool bWait = true);
    virtual bool stopWork(unsigned short shElementIndex, bool bWait = true);

    ///测试单元操作函数
    inline unsigned short getElementNum(){return static_cast<unsigned short>(m_vecSubElement.size());}

    inline CSubElementBase *getElement(const unsigned short &shIndex)
    {
        if(shIndex >= m_vecSubElement.size())
            return nullptr;

        return  m_vecSubElement[shIndex];
    }

    //将测试单元绑定函数写在设备中是因为设备需要进行某些绑定响应（目前是中长跑）
    virtual bool setElementBindFlag(const unsigned short &shIndex, bool bBind);

    inline void setTestState(const unsigned short &shIndex, short shTestState)
    {
        if(shIndex >= m_vecSubElement.size())
            return;

        m_vecSubElement[shIndex]->setTestState(shTestState);
    }

    inline void setTestState(short shTestStatus){m_objStateManage.setTestState(shTestStatus);}

    inline vector<CSubElementBase *> getAllElements(){return m_vecSubElement;}

    vector<CSubElementBase *> getAllBindElements();

    //存在错误返回true
    virtual bool existErrInBindElement();
    virtual bool existErr(const unsigned short &shElement);

    virtual CSubElementBase *getFirstAvailableElement();

    //解析单包数据
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;

    //检查单包数据有效性，等于零表示数据有效，大于零直接返回对应的数据长度
    bool checkSignalPackageValid(unsigned char *pData, const int &iDatalen);

    //设置灯光,设备设置灯光命令是一致的，在测试完成时第二个得分参数才会存在作用
    void setLightBelt(short shTestState, int nTestCode = 100);

    //在创建设备之后调用，作用是进行心跳回复的时候判断
    void setStationFlag(bool bStation);

    //返回设备分机编号
    inline const int &getExtensionNo() const {return m_nExtensionNo;}
    bool setExTensionNo(const int &nExtensionNo);

    inline bool isInitExtensionNo() const {return  m_bInitExtensionNo /*&& m_nExtensionNo == m_nExtensionNoTarget*/;}

    //获取测试状态
    inline short getTestState(){return m_objStateManage.getTestState();}

    static unsigned short convertTestState2LightState(const short shTestState);

    //设置上一次发送命令
    void setLastCmd(unsigned short shCmd){m_shLastCommand = shCmd;m_bSendCommandSucess = false;}
    inline bool getSendCmdFlag(){return  m_bSendCommandSucess;}

    //设备使用次数
    inline void setUseNum(const int &iUseNum){m_iUseNum = iUseNum;}
    int getUseNum()const{return m_iUseNum;}

    //433频段设置（足篮 实心球 引体向上 斜身引体向上）
    void enterCfg433();
    void existCfg433();
    void clearCfg433();
    //需要进入配置之后才能进行设置
    void setChannel433(uint nChannel);

    //设置犯规次数，将上层软件犯规次数与底层同步
    void setIllegalTimes(const int &nTimes);
signals:
    //设备状态变更
    void signalTestStateChange(void *pDev, short shTestState);

    //表示单个测试单元启动
    void signalStartElement(const CLocation &localtion);
    //表示当前设备所有有效测试单元启动
    void signalStartAllElement(const unsigned int &iDevId);
    //表示通过广播开启了所有设备
    void signalStartCast();
    //界面在配置中设置设备属性时，界面需要返回值，这里针对某些命令直接返回其对应的值
    void sigSetDevCfgRet(uint nSetDevCfgReturnType, uint DevID);
protected:
    virtual  void dataParse(QByteArray &array) override;

    //目前是在构造函数中初始化，更好的解决方案是在解析数据中判断（但是底层设备存在连接不稳定问题，这里先在构造中进行）
    template<class T>
    void initSubElement(unsigned short ElemenSize)
    {
        uninitSubElement();
        for(unsigned short i = 0; i < ElemenSize; i++)
        {
            m_vecSubElement.push_back(new T(i, 0));

            connect(&m_vecSubElement[i]->getTestStateManage(), &CTestStateManage::signalTestStateChange,\
                    this, &CDevHardwareBase::onElementTestStateChange);
        }
    }

    void uninitSubElement();

    virtual void onElementTestStateChange();

    virtual void onUpdateTimeOut() override;

    static bool chekHardwareCode(unsigned char &cHardwareCode);

    //通用函数,获取状态位（使用条件，在外部已经确认长度够用数据有效）
    static void getTestStateFromPackgeData(CElementDataBase &elementData, unsigned char *pData);

    //通用函数，原始数据转为标准数据
    static void getNormalData(float &fSrcData, const short &shTestItem);

    //接收自身状态变更,进行设备灯光设置
    void onTestStateChange(short shTestState);

    //获取设备测试状态
    short getElementState2devState(const short &shState);

    //获取分机编号
    void timeOutGetExtensionNo();
    //函数调用中若是超时没有数据会判断设备断连，在外部会删除当前设备，后续不能涉及一切对变量的操作
    //连接返回true 断开返回false
    virtual bool timeOutCheckConnect();
    //设置当前测试项目改变对应测试项目单包数据长度
    void updateTestItemToChangePackgeLen(const short &shTestItemLen);
    //更新设备硬件编码，在设备类型设置之后调用
    void updateDevHardwareCode();

    //初始化设备信息函数，在设备类型被确定之后调用
    void initDevInfo();

    //操作命令类函数，函数涉及到不同线程的情景，添加线程保护
    void updateCmd(const cmdInfo &cmdInfo);
    void deleteCmd(const cmdInfo &cmdInfo);
    list<cmdInfo> getCmd();
    void updateCmdTime(const cmdInfo &cmdInfo);
protected:
    //设备可用测试单元，在具体子设备中进行创建，在当前析构函数中进行销毁
    vector<CSubElementBase *> m_vecSubElement;
    
    //记录上次回复时间
    qint64 m_qint64LastRespondTime{0};

    //
    QByteArray m_arrRespondCmd;

    //记录底层设备编码
    unsigned char m_cHardwareCode;

    //身高体重在组网情况下，需要定时发送获取数据指令才能获取数据
    bool m_bGetDataFlag{false};

    unsigned int m_iGetDataInterval{300};
    unsigned int m_iGetDataTotal{0};

    //底层设备的分机编号，本来应该使用无符号类型，由于超不出限制，使用有符号即可
    //设备编号为-1表示设备未准备就绪,设备会存在分机编号不在范围中的情况，使用标志进行判断是否进行了初始化
    int m_nExtensionNo{-1};
    int m_nExtensionNoTarget{-1};
    bool m_bInitExtensionNo{false};

    unsigned int m_iNumIntervalTotal{ 0 };

    //表示当前设备的测试状态，受测试单元的状态影响，对外仅有3种状态
    //未测试 测试中 测试完成
    CTestStateManage m_objStateManage;

    //中长跑设备当前刷卡卡号
    unsigned int m_nRunMidCardNo;

    //跳绳设备当前启动卡号
    unsigned int m_nRopeSkipCardNo{0};

    //短跑获取的开始时间
    float m_fRunShortStartTime{0.0f};

    //设备使用次数
    int m_iUseNum{0};

    //单包数据长度，在设备中进行初始化
    int m_iPackgeLen{-1};
    bool m_bPackgeLenInit{false};

    //变量为true需要将单元id设置为设备id
    bool m_bSubElementIDIsDevID{true};

    //结束命令的参数，目前仅用 unsigned int 表示,目前仅有中长跑存在此变量
    uint m_nStopCmdParam;

    //命令发送队列(并不是真的队列，底层设备偶尔会漏命令，所以需要重新进行发送)，需要经常进行增删操作选用list
    list<cmdInfo> m_listCmd;
    QMutex m_mutexCmd;

    int m_nIllTime{0};
};

#endif // CDEVHARDWAREBASE_H
