#ifndef CDEVSTERUNMID_H
#define CDEVSTERUNMID_H

#include "CDevHardwareBase.h"

/*
 * mxq 2022-02-21 中长跑数据包格式发生变化
 * 一个完整的跑道信息共 28字节 包括：6字节的身份码(1字节设备类型+1字节数据点数+4字节身份码)
 * 2字节数据类型 + 2字节数据状态 + 8字节时间戳 + 4字节跑步时间 + 4字节标签ID + 2字节识别线类型
 * 发送逻辑修改：仅发送存在的跑道数据，若通道不存在，则不发送对应跑道信息，即不会存在全空数据
 */

//软件需要对中长跑设备进行判断，具体设备分为途经点和终点，途经点只是为了确保人员进行了对应的
//途径，通过标志进行对应途经点判断

//中长跑需要重写设备的断连，中长跑设备在断连之后要分状态对断连的设备进行处理，设备断连之后还是按照
//基类设备将所有测试单元都设置为断连状态，在设备重连时将所有未绑定的设备都设置为正常，绑定的设备
//区分测试单元是否开启，若没有开启就需要将卡号重新绑定（设备断连可能是设备断电，可能是信号差，设备断电必须重新绑定卡号）
//若开启了不做任何动作，绑定成功之后，若对应测试单元状态为断连，将对应的测试单元设置为上一次正常状态即可


//2022.12.02 中长跑设备以前在测试单元中进行判定的逻辑保留，适应老设备重连，新增查询命令可以判断设备是否是断电之后
//重连还是


//中长跑
class CDevSteRunMid : public CDevHardwareBase
{
    Q_OBJECT
public:
    enum runMidDevType{
        RUN_MID_TYPE_INVALID = 0, //目前最新是起点设备,现在当做途经点
        RUN_MID_TYPE_VIA_PT,      //途经点 不为0不为无效设备
        RUN_MID_TYPE_DESTINATION = 7,//终点
        RUN_MID_TYPE_COUNT //
    };

    CDevSteRunMid(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteRunMid() override;

    //开始某个卡号的开始命令,重发次数
    bool startRun(unsigned int &nCardId, short shAttemptsNum);

    //中长跑设备在打开时设备并未就绪，需要等待数据帧重新设置设备类型
    virtual bool devOpen() override;

    //检查卡号绑定状态
    bool checkRunCardBindState(const unsigned int &nCardId);

    virtual bool setElementBindFlag(const unsigned short &shIndex, bool bBind) override;

    //开启关闭当前类型设备
    virtual bool startWork(bool bCast, bool bWait = true) override;
    virtual bool stopWork(bool bCast, bool bWait = true) override;

    //开启关闭当前设备 下面的确切设备
    virtual bool startWork(unsigned short shElementIndex, bool bWait = true) override;
    virtual bool stopWork(unsigned short shElementIndex, bool bWait = true) override;

    //绑定卡号
    bool bindCard(const unsigned int &nCardId, short shAttemptsNumm, const unsigned short &shIndex);

    inline unsigned short const &getRunMidDevNo() const{return m_shRunMidDevNo;}
    inline unsigned short const &getRunMidDevType() const{return m_shRunMidDevType;}

    //添加途经点删除途经点,主要是在控制设备开启和关闭时需要使用
    bool setViaPtDev(set<CDevSteRunMid *> &setDev);
    inline const set<CDevSteRunMid *> getViaPtDev() const{return m_setDevViaPt;}

    //设置当前绑卡的设备号
    inline void setRunMidCardNo(unsigned int nCardNo){m_nRunMidCardNo = nCardNo;}
signals:
    //途径点检测通知，当前设备时终点设备才会进行当前信号发送
    void signalViaPtDevCheck();
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;

    virtual void onElementTestStateChange() override;

    void checkViaPtDev();

    //设置卡号和测试单元映射关系
    void setCardAndElementMapping(const unsigned int &nCardNo, const unsigned short &shElementIndex);

    //开启成功之后设置测试单元的信息,传入开始时间
    void setStartSucessElementInfo(const long long &llTimeStamp);

    virtual void onUpdateTimeOut() override;

    //途经点函数相关设置
    void simulationViaSendCmd(const unsigned short &shCmd);
    void simulationViaSendBindCard(const unsigned int &nCardNo);
    bool getViaSimulationSendFlag();

    /***************设备中对卡号的管理 ************/
    set<unsigned int> getStopCardID();
    void setStopCardID(const set<unsigned int> &setTemp = set<unsigned int>());
    //停止卡号，中长跑特有停止某一个卡号
    bool stopCardID(const unsigned int &nCardNo, bool bWait = false);
    //添加卡号和删除卡号,只需要在终点设备中进行管理即可
    void updateStopCardContent(const unsigned int &nCardNo, bool bAdd);

    //函数在硬件设备基类中进行调用
    virtual bool timeOutCheckConnect() override;

    void timeOutSendGetParamCmd();
private:
    //身份码与编号的对应信息,保证绑定了卡号的数据才进行处理，其他数据全部过滤
    //避免遍历所有测试单元
    map<unsigned int, unsigned short> m_mapCardAndElementMapping;

    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;

    //中长跑设备类型,初始化为无效设备
    unsigned short m_shRunMidDevType{RUN_MID_TYPE_INVALID};
    //中长跑设备类型号，初始化为无效设备编号
    unsigned short m_shRunMidDevNo{RUN_MID_TYPE_INVALID};

    //保存所有途经点设备，终点设备中才会存在途经点设备，在开始测试 绑卡 结束测试时需要使用
    set<CDevSteRunMid *> m_setDevViaPt;

    //设备中解绑卡号或者停止某个测试单元测试时需要明确的等待底层设备对某个卡号进行解绑，要不会引起某个人的圈数一直无法刷新的问题
    //目前简单设置解绑逻辑为单个解绑（逻辑简单）
    QMutex m_mutexStopCardID;
    set<unsigned int> m_setStopCardID;

    //队列重新绑定卡号
    set<unsigned int> m_setRebindCardID;

    bool m_bCheckReopen{false};

    //获取参数命令，用于获取设备有没有绑定人员（设备断连之后重连需要判断，若绑定了人员修改做出对应的变更）
    //设备断连之后再设置为true，下一次设备重新连接之后发送查询命令
    static QByteArray s_arrCmdGetParam;
    bool m_bSendGetParamCmd{false};
    int m_iSendGetParamCmdIntevalTotal{0};
};

#endif // CDEVSTERUNMID_H
