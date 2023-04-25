///////////////////////////////////////////////////////////
//  CTestFlowRunMid.h
//  Implementation of the Class CTestFlowRunMid
//  Created on:      08-1月-2022 11:19:53
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_BD83E2F6_D197_4401_9328_6466C30F635C_INCLUDED_)
#define EA_BD83E2F6_D197_4401_9328_6466C30F635C_INCLUDED_

#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevSteRunMid.h"
#include "SteDevManager/SteDevice/SteSubElement/CSubElementRunMid.h"

class CTestGuideRunMid : public CTestGuideBase
{
    Q_OBJECT
public:
    CTestGuideRunMid();
    virtual ~CTestGuideRunMid() override;

    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;

    bool checkRunCardBindState(const unsigned int &nCardId);

    virtual bool bindUserToElement(const unsigned short &shElementIndex, const CUserInfo &userInfo) override;

    //真实的测试开始和结束可以由外部进行控制
    virtual void procTestBegin(const unsigned short &shElementIndex) override;

    //测试结束，两处地方可以结束，一处是外部判断时间过长自动结束，一种是测试人员完成测试结束
    virtual void procTestEnd(const unsigned short &shElementIndex) override;

    //获取真实开始时间
    bool getActualStartTime(const unsigned short &shElementIndex, qint64 &llTimeStamp);

    //更新同步时间，仅有同步测试模式下调用
    static void upDateSyncActualTime(){s_llSyncTime = QDateTime::currentDateTime().toMSecsSinceEpoch();}

    //计算需要通过的圈数
    void calculateThroughTimes();

    //圈数长度变更
    void onRunMidLenthChanged();

    //中长跑途经点设备数据变更
    void onRunMidViaPtDevDataChange(const unsigned short &shElementIndex);

    //获取当前测试项目
    static void getCurTestRunMidItem(short &shMaleTestItem, short &shFemaleTestItem);
    static int getRunMidItemLen(const short &shTestItem);
signals:
    //异步模式指定真实开始
    void signalAsyncStart(const CLocation &location);
    //同步模式指定真实开启
    void signalSyncStart();
protected:
    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;

    //测试完成处理数据，只需要肺活量等相似测试继承重写
    virtual void saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement) override;

    //获取测试结果
    bool getTestResult(float &fTime, int &nCurThroughTimes, CSubElementBase *pElement);

    //检验本次成绩是否有效，查看所有途经点设备是否存在数据，若存在那么就是有效成绩，若不存在那么无效
    bool checkViaPtDevData(const unsigned short &shElementIndex);

    //处理途经点数据,查找途经点中最大的小于当前数据的数据，然后进行删除，不存在数据返回false
    bool dealViaPtDev(const float &fCurTime, vector<vector<CElementDataRunMid>> &vecViaTime);

    void procTestResult(CElementInfo &elementInfo, CSubElementBase *pElement, bool bComplete) override;

    void onTimeOutCheckViaPtDev();

    virtual void onTestItemChanged() override;
private:
    int m_nMaleThroughTimes{0};  //男生圈数
    int m_nFemaleThroughTimes{0};//女生圈数

    //ms
    static long long s_llSyncTime;

    //定时器用于在完成测试之后检验是否经过了所有途经点
    QTimer m_timerCheckViaPtDev;

    //前面表示测试单元索引，后面表示开始时间，若存在途经点
    map<unsigned short, long long> m_mapCheckViaPtDevIndex;

    //定义途经点设备检测时间,初步定义为 10s
    int m_nCheckViaPtDevCheckTime{10000};

    //无途经点时，需要时间延缓2s出结果
    int m_nDelayTimeStop{2000};
};
#endif // !defined(EA_BD83E2F6_D197_4401_9328_6466C30F635C_INCLUDED_)
