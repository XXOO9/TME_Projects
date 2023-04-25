#ifndef CTESTPROCMANAGE_H
#define CTESTPROCMANAGE_H
#include "CTestProcBasic.h"

class CTestProcManage
{
public:
    CTestProcManage();
    ~CTestProcManage();

    //设置当前测试项
    void setCurTestItem(short shTestItem);

    //获取当前测试项
    short getCurTestItem();

    //获取当前测试项索引
    short getCurTestItemTimesIndex() const;

    //获取当前测试项描述
    string getcurTestItemDescribe() const;

    //设置当前测试状态
    void setCurTestState(short shTestState);

    //获取当前测试状态
    short getCurTestState() const;

    QString getTestProcContent();

    //获取测试完成等待时长
    int getTestStartWaitTime();

    //由外部设置进来，再进行测试过程状态变更时同时需要设置测试单元的测试状态
    //避免设备对应的测试状态和其测试过程不匹配
    void setTestStateManage(CTestStateManage *pManage);

    //获取当前测试是否完成,根据当前的测试次数进行判断
    bool getTestCompleteFlag();

    //加载测试配置
    void loadTestCfg();
protected:
    static void initProduct();
    static void uninitProduct();
private:
    CTestProcBasic *m_pCurTestProc{nullptr};

    //若是对应的测试未被创建，那么将当前测试状态设置进去即可
    CTestStateManage *m_pTestStateMange{nullptr};

    static bool s_bInitProduct;
};

#endif // CTESTPROCMANAGE_H
