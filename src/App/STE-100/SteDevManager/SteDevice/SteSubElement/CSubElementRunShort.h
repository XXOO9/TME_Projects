///////////////////////////////////////////////////////////
//  CSubElementRunShort.h
//  Implementation of the Class CSubElementRunShort
//  Created on:      14-1月-2022 10:40:13
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_B6F78D9F_7436_411b_AB70_4B69ECE9FB93_INCLUDED_)
#define EA_B6F78D9F_7436_411b_AB70_4B69ECE9FB93_INCLUDED_

#include "CSubElementBase.h"
#include <QDate>
#include <queue>

using namespace std;

/*
 * 跑道逻辑处理
 */
class CSubElementRunShort : public CSubElementBase
{

public:
    CSubElementRunShort (unsigned short shUnitID,  unsigned int iElementID);
    virtual ~CSubElementRunShort() override;

    //0.开始/结束 测试
    virtual void startTest() override;
    virtual void stopTest() override;

    bool getStartFlag();

    //设置是否使用起点
    void setUseStartFlag(bool bFlag);

    //比赛模式专用：外部设置开始时间
    void setStartTime(float fStartTime);

    //1.获取是否可用的标志
    virtual bool getAvailableFlag() override;

    //2.获取当前经过的圈数
    unsigned int getCurrentLaps();

    //3.获取当前的时间
    float getCurrentTime();

    //4.设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    virtual void setConnectStatus(unsigned short usStatus) override;
private:
    void setStartConnectFlag(bool bFlag);

    void setEndConnectFlag(bool bFlag);

    void initBasicInfo();

    void processData();

    void clearTime();

    void increaseThroughTimes();

private:
    //接收数据的变量标志，用于判断是否连接成功
    bool  m_bStartConnectFlag{false};
    bool  m_bEndConnectFlag{false};

    //初始化时间，用于判断设备时间是否完成同步
    QDateTime m_qInitializationTime;

    //0.当前圈数
    unsigned int m_uiCurrentLap{0};

    //1.时间缓存(先进先出)
    queue<float> m_queueStartTime;
    queue<float> m_queueEndTime;

    //2.起点时间
    float m_fUntestStartTime{-1.0f};
    float m_fStartTime{-1.0f};
    QDateTime m_qStartDate;

    //3.终点时间
    float m_fUntestEndTime{-1.0f};
    float m_fEndTime{-1.0f};
    QDateTime m_qEndTime;

    //4.标志时间
    float m_fMarkTime{-1.0f};

    //5.保护时间(避免信号延时导致上一次结果影响下一次)
    QDateTime m_qProtectDate;

    //6.是否使用起点的标志，默认需要使用
    bool  m_bUseStarting{true};
};
#endif // !defined(EA_B6F78D9F_7436_411b_AB70_4B69ECE9FB93_INCLUDED_)
