#ifndef CTESTPROCANTICHEATINGPULLUP_H
#define CTESTPROCANTICHEATINGPULLUP_H
#include "CTestProcAntiCheatingBasic.h"

//外部触发抓取图像进行抓取即可
class CTestProcAntiCheatingPullUp : public CTestProcAntiCheatingBasic
{
public:
    CTestProcAntiCheatingPullUp();

protected:
    //触发记录数据进行抓取即可
    virtual void onRecord() override;
};

#endif // CTESTPROCANTICHEATINGPULLUP_H
