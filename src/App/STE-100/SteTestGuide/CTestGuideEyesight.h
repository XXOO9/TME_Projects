#ifndef CTESTGUIDEEYESIGHT_H
#define CTESTGUIDEEYESIGHT_H
#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevSteEyesight.h"

class CTestGuideEyesight : public CTestGuideBase
{
public:
    CTestGuideEyesight();

    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;

    //左右视力存储的序列化与反序列化
    static void getSerializationEyeightData(string &strTarget, const float &fLEyeightData, const float &fREyeightData);
    static bool getDeserializationEyeightData(float &fLEyeightData, float &fREyeightData, const string &strSrc);
    static bool getDeserializationEyeightData(QString &qsLEyeightData, QString &qsREyeightData, const string &strSrc);
protected:
    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;

    //测试完成处理数据，只需要肺活量等相似测试继承重写
    virtual void saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement) override;

    virtual void procTestResult(CElementInfo &elementInfo, CSubElementBase *pElement, bool bComplete) override;

    void updateTestResult(QString &qsDisp, const CElementInfo &elementInfo, bool bRightData, float fLeftData);
private:
    //身高体重为了保证数据一致，通过统一的变量进行测试结果的记录
    float m_fLEyesight;
    float m_fREyesight;
};

#endif // CTESTGUIDEEYESIGHT_H
