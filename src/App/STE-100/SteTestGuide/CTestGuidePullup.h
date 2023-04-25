﻿#ifndef CTESTGUIDEPULLUP_H
#define CTESTGUIDEPULLUP_H
#include "CTestGuideBase.h"
#include "SteDevManager/SteDevice/CDevStePullUp.h"

class CTestGuidePullup : public CTestGuideBase
{
public:
    CTestGuidePullup();

    virtual void setSteDevice(CDevHardwareBase* pSteDev) override;
protected:

    //测试单元数据变更，每个测试存储数据不同需要在具体的流程当中进行处理
    virtual void onDataChange(const unsigned short &shElementIndex, const unsigned char cDataStstus) override;

    //测试完成处理数据，只需要肺活量等相似测试继承重写
    virtual void saveDataToTestResult(CElementInfo &elementInfo, CSubElementBase *pElement) override;
};

#endif // CTESTGUIDEPULLUP_H
