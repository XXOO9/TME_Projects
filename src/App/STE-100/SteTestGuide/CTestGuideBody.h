#ifndef CTESTGUIDEBODY_H
#define CTESTGUIDEBODY_H
#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevSteBody.h"
#include "Analysis/CAnalysisHBC.h"

class CTestGuideBody : public CTestGuideBase
{
public:
    CTestGuideBody();

    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;

    //获取人体阻抗测试结果 身高单位为cm 体重单位为 kg
    static void getHBCTestRes(const float &fHeight, const float &hWeight, bool bGender, short shAge,const string &strHBCRes, const vector<short> &vecAnalysisItem, vector<CAnalysisHBC::HBCComponentDes> &vecRes);

    //获取人体阻抗解析项目
    static void getHBCItem(vector<short> &vecAnalysisItem);

    //测试项目变更
    virtual void onTestItemChanged() override;

    static short getHBCAge(const CUserInfo *pUserInfo);
protected:
    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;

    virtual void procTestBegin(const unsigned short &shElementIndex) override;
    virtual void procTestEnd(const unsigned short &shElementIndex) override;

    //测试完成处理数据，只需要肺活量等相似测试继承重写
    virtual void saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement) override;

    //处理测试结果,包含界面中结果显示，和外部统一的数据处理
    virtual void procTestResult(CElementInfo &elementInfo, CSubElementBase *pElement, bool bComplete) override;

    CTestResultInfo *getTestRes(CElementInfo &elementInfo, short shTestItem);

    //人体阻抗序列化与反序列化
    static bool getSerializationHBC(string &strTarget, float *pSrcData, int nSize);
    static bool getDeserializationHBC(float *pSrcData, int nSize, const string &strSrc);
    static bool getDeserializationHBC(vector<double> &vecTarget, const string &strSrc);

    //数据正确性
    bool isDataCorrent(const float &fData, const short &shTestItem);

    //初始化测试单元
    virtual bool initElementInfo(CElementInfo &elemetInfo) override;
private:
    //身高体重保存一次，通过统一的变量进行测试结果的记录
    float m_fWeight{0.0f}; //kg
    float m_fHeight{0.0f}; //cm
    bool m_bFirstWeight{false};

    set<short> m_setCompleteItem;
};

#endif // CTESTGUIDEBODY_H
