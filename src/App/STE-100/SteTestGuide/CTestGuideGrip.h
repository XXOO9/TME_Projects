#ifndef CTESTGUIDEGRIP_H
#define CTESTGUIDEGRIP_H
#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevSteGrip.h"

class CTestGuideGrip : public CTestGuideBase
{
public:
    CTestGuideGrip();

    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;

protected:

    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;

    //测试完成处理数据，只需要肺活量等相似测试继承重写
    virtual void saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement) override;
private:
    //保证数据获取函数只获取一次，也是为了显示数据和保存的数据一致
    float m_fGrip; //kg
};

#endif // CTESTGUIDEGRIP_H
