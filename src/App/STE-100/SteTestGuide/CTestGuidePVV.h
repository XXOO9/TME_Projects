#ifndef CTestGuidePVV_H
#define CTestGuidePVV_H
#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevStePVV.h"
#include "CEFV.h"

class CTestGuidePVV : public CTestGuideBase
{
public:
    CTestGuidePVV();
    virtual ~CTestGuidePVV() override;

    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;

    void setLungAnlsItem(short shAnlsItem);

    //测试项目变更
    virtual void onTestItemChanged() override;
protected:
    //结束表示单次测量结束
    virtual void procTestEnd(const unsigned short &shElementIndex) override;

    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;

    //测试完成处理数据，只需要肺活量等相似测试继承重写
    virtual void saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement) override;

    //将结果数据展示在界面上
    void dispTestResToUi(CElementInfo &elementInfo, CSubElementBase *pElement);

    //设置结果到界面
    void dispResToUi(const CAnlsResultVC &result);
    void dispResToUi(const CAnlsResultFVC &result);
    void dispResToUi(const CAnlsResultMPV &result);

    void appendData(const QStringList &list);
private:
    //当前分析的项目，默认为潮气量,分为（潮气量 用力肺活量 最大肺通气量）
    //对于数据类型来说
    short m_shCurAnlsLungItem{TEST_ITEM_PVV_VC};

    //创建一个类进行呼吸容积曲线的绘制（由于实时性要求在线程中进行运算）
    CEFV m_objEFV;

    //分析肺功能相关参数
    CAnalysisLungFunc m_objAnlsLung;

    float *m_pDataFlowOrVelocity{nullptr};
    ulong m_lDataLen{0};

    float m_fLastFlow{0.0f};
    float m_fLastVelocity{0.0f};
};

#endif // CTestGuidePVV_H
