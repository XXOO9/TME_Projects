#ifndef CTESTPROCBASIC_H
#define CTESTPROCBASIC_H
#include <QString>
#include <map>
#include "CTestStateManage.h"
#include "commondefin.h"
using namespace std;

//这里的测试过程是指测试状态

class CTestProcBasic
{
public:
    //单位ms
    static const int TEST_START_WAIT_TIME{1500};

public:
    CTestProcBasic();
    virtual ~CTestProcBasic();

    //返回当前测试完成状态
    virtual bool setTestState(short shTestState);

    short getCurTestState() const;

    //获取当前测试过程显示内容
    QString getTestProcDispContent();

    //获取当前测试项目
    short getTestItem();

    //测试项目描述
    string getTestItemDescribe();

    //获取当前测试索引，在进行截图时需要使用到
    short getCurTestTimesIndex();

    //获取测试完成等待时长
    int getTestStartWaitTime();

    inline void setTestStateManage(CTestStateManage *pManage){m_pTestStateManage = pManage;}

    //获取测试是否结束根据 测试人员 和 测试次数进行判断
    bool getTestCompleteFlag();

    //加载测试项配置，需要确认测试项目之后再调用函数
    void loadTestCfg();

    //重置，主要是重置测试状态和测试次数
    void reset();
protected:
    virtual void initTestProcess();

protected:
    //总测试次数
    short m_shTestTimesTotal{1};

    //当前测试索引(从零开始)
    short m_shTestTimesCur{0};

    //当前测试状态
    short m_shTestState;

    //当前测试项目
    short m_shTestItem;

    //当前测试状态存在4种，准备测试 设备检查之后：开始进行测试/开始进行第n次测试 测试中 完成测试
    //仅是设备检测完成需要根据测试次数显示不同的内容其他的都是相同
    map<short,QString> m_mapCommonProcContent;

    //在开始测试时会有一个等待时长，单位 ms
    int m_nTestStartWaitTime{TEST_START_WAIT_TIME};

    //由外部设置进来的测试状态，对应设备的测试单元的测试状态
    CTestStateManage *m_pTestStateManage{nullptr};
};

#endif // CTESTPROCBASIC_H
