///////////////////////////////////////////////////////////
//  CSubElementRopeSkip.h
//  Implementation of the Class CSubElementRopeSkip
//  Created on:      14-1月-2022 10:40:14
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_9DDED943_0740_4ef0_A372_D2A87A66F81C_INCLUDED_)
#define EA_9DDED943_0740_4ef0_A372_D2A87A66F81C_INCLUDED_

#include "CSubElementBase.h"
#include <QTime>

namespace  RopeSkipStatus{
    enum enSkip_Data_Status{
        SKIP_NO_TEST = 0,
        SKIP_IN_TEST,
        SKIP_COMPLETE_TEST,
        SKIP_TEST_COUNT
    };
}

class CSubElementRopeSkip : public CSubElementBase
{

public:
    CSubElementRopeSkip (unsigned short shUnitID,  unsigned int iElementID);
    virtual ~CSubElementRopeSkip() override;

    //0.获取当前开始时间
    inline QDateTime getStartTime(){ return m_StartTime; }

    //1.获取当前设备电量
    inline unsigned char getPowerNum(){ return m_ucPowerNum; }

    //2.获取当前跳绳次数
    unsigned int getCurrentRope();

    //4.设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    //5.开始/结束 测试
    virtual void startTest() override;
    void startSkipTest();
    virtual void stopTest() override;
    void stopUpdate();

    //6.得到开始测试成功的标志
    bool getStartSuccessFlag(){ return m_bStartSuccess; }

    //7.设置重发标志(一次开始最多进行一次重发)
    void setRestartFlag(const bool &bFlag){ m_bReStartFlag = bFlag; }
    bool getRestartFlag(){ return m_bReStartFlag; }

private:
    //充电状态
    unsigned char  m_ucChargeStatus{0};
    //电量
    unsigned char  m_ucPowerNum{0};
    //数据状态(由底层发送)
    unsigned char  m_ucDataStatus{RopeSkipStatus::SKIP_NO_TEST};
    //跳绳次数(由底层发送)
    unsigned int   m_uiKeepNum{0};

    //开始次数(根据数据变化，判断是否启动成功)
    int m_iStartNum{-1};

    bool m_bStartSuccess{false};

    bool m_bReStartFlag{false};

    //开始时间
    QDateTime m_StartTime;
};
#endif // !defined(EA_9DDED943_0740_4ef0_A372_D2A87A66F81C_INCLUDED_)
