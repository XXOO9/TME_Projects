#ifndef CTESTPROCANTICHEATINGSITSBJ_H
#define CTESTPROCANTICHEATINGSITSBJ_H
#include "CTestProcAntiCheatingBasic.h"

class CTestProcAntiCheatingSitSBJ : public CTestProcAntiCheatingBasic
{
public:
    CTestProcAntiCheatingSitSBJ();
protected:
    virtual void onStart() override;

    //跳远在结束时进行照片筛选和其他项目不一致，跳远时间比较短
    //选取距离结束4s的图像，在图像中选取三段中的中间图像
    virtual void stopNormal() override;

    virtual void onRecord() override;

    //定时器有两种作用，一种是定时抓拍，一种是视屏录制超出某个时长就重新进行录制
    virtual void onTimeOut() override;

    virtual void onStop(bool bNormalStop) override;
private:
    int m_iRecordVideoTime{0}; //ms;
};

#endif // CTESTPROCANTICHEATINGSITSBJ_H
