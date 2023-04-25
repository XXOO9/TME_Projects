#ifndef CDEVSTEBASE_H
#define CDEVSTEBASE_H
#include "CDevice.h"
#include "CGenerationCommand.h"
#include "CSteCommonDefin.h"
#include "IComHelper.h"
#include "commondefin.h"

#include <map>
#include <set>

using namespace SubElementPropertye;
using namespace std;
/*************************************************
  <类    名>    CDevSteBase
  <功能描述>     体测设备基类，抽象体测设备相同功能部分
                该抽象有2种模式：
                1，需要建立独立的通讯和解析过程
                2，不依赖硬件，进行纯虚拟设备的构建

  <作    者>    zhy
  <时    间>    2022-02-22
**************************************************/
class CDevSteBase : public CDevice
{
    Q_OBJECT
public:
    CDevSteBase();

    virtual ~CDevSteBase() override;

    inline const QString &getComName(){return m_strComName;}

    static unsigned char convertDevType2HardwareCode(const short &shDevType);
    static short convertHardwareCode2DevType(const unsigned char &cHardwareCode);

    //是否可以删除对象,只要有一个存在就不能删除
    inline bool isDeleteThis() {return !(isValid() || m_bTimeOutRun || getSendingDataState());}
public:
    virtual bool isValid() override {return m_bValidDev;}
    inline const short& getDevType() const {return m_shDevType;}

    //打开设备，体测设备有特殊处理，只有独立通讯机制的才能打开设备
    //中长跑设备在打开时设备并未就绪，需要等待数据帧重新设置设备类型
    virtual bool devOpen() override;

    //打开设备，体测设备有特殊处理，只有独立通讯机制的才能打开设备
    virtual void devClose() override;
    
    //在设备有效时返回值有效
    inline bool isStaionSte(){return m_bStationSte;}

    //外部调用发送状态函数
    void sendConnectState(bool bConnect){
        if(!bConnect)
        {
            devClose();
        }

        if(!m_bValidDev && bConnect)
            return;

        emit signalDevStatus(bConnect, this);
    }

signals:
    /**
     * 设备出错信号
     */
    void signalDevError(unsigned int shError, QString strComName);

protected:
    //查询设备类型，区分工作站和直连设备以及其他设备
    void queryDevType();

    //解析单点数据
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) = 0;

    // IDataParse interface
    virtual void dataParse(QByteArray &array) override;

    //原始数据命令转为正常指令,原始指令是指从数据中获取的指令值，这个函数所有数据帧会变更为 获取数据指令
    static unsigned short getNormalCmdFromOrigCmd(const unsigned short &shOrigCmd);

    //返回原始指令，数据指令为原始数据指令
    static unsigned short getCmdFromOrigCmd(const unsigned short &shOrigCmd);
protected:
    // 通讯名称
    QString m_strComName;

    //通讯接口助手
    IComHelper *m_pHelper{ nullptr };

    //是否需要构建独立通讯过程,在软件中工厂设备为true，具体设备为false
    bool m_bIndependent{ true };

    //工作站标识符，以此判定该设备是否是合法的工作站
    QString m_strStationIdentify;

    //当前一帧的帧长度，会根据不同的帧发生变化
    unsigned short m_shFrameLength{0};

    //当前一帧的单点数据个数，会根据不同的帧发生变化
    unsigned short m_shDataNumber{0};

    //检测设备是否为可用设备间隔总时长
    unsigned int m_iCheckDevValidTotal{ 0 };

    //设备类型,在设备工厂使用时表示当前设备类型，在具体设备使用就代表具体的设备类型
    short m_shDevType{ DEV_TYPE_UNKNOW };

    bool m_bValidDev{false};

    bool m_bStationSte{false};

    //记录上一帧数据帧帧号
    static unsigned int s_ilastFrameNo;
    static unsigned int s_iCurCmdFrameNo;

    //保存最后一次发送的命令
    unsigned short m_shLastCommand{0};

    //设备统一的命令
    QByteArray m_arrCmdHeartbeat;

    //帧号4字节
    static unsigned int s_iCurFrameNo;

    //在外部进行设备关闭之后立马删除当前对象，会存在还有定时器中的函数未完成的状态
    //导致软件闪退，因当前设备不是在线程中运行，不能通过循环判断变量的方式进行判断是否完全退出
    //判断设备退出的方式为 设备已关闭 同时设备定时器已经完全退出
    bool m_bTimeOutRun{false};
};

#endif // CDEVSTEBASE_H
