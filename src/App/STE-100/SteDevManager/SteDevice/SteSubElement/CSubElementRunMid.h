///////////////////////////////////////////////////////////
//  CSubElementRunMid.h
//  Implementation of the Class CSubElementRunMid
//  Created on:      14-1月-2022 10:40:15
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_701E35BC_0DB4_4ff8_821E_A3DE12F3A3D2_INCLUDED_)
#define EA_701E35BC_0DB4_4ff8_821E_A3DE12F3A3D2_INCLUDED_

#include "CSubElementBase.h"
/*
 * 中长跑逻辑处理，中长跑设备只对绑定的卡号设备进行处理，其他意外绑定的设备一律不进行处理
 * 在设备中收到对应的信息之后进行删除
 */

class CElementDataRunMid
{
public:
    //每个数据变化值都在设备中进行记录
    long long m_llRecordTime;

    //测试状态,时间单位 s
    float m_fPassTime;
};

class CSubElementRunMid : public CSubElementBase
{

public:
    CSubElementRunMid (unsigned short shUnitID,  unsigned int iElementID);
    virtual ~CSubElementRunMid() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    virtual void startTest() override;
    virtual void stopTest() override;
    //获取当前测试单元时间
    const vector<CElementDataRunMid> &getDataRunMid() const {return m_vecPassTime;}

    inline const bool &getCheckedViaPtDev() const{return m_bCheckedViaPtDev;}
    void setCheckedViaPtDev(const bool &bCheckedFlag){m_bCheckedViaPtDev = bCheckedFlag;}
private:
    //记录经过的时间,记录当前从开始到结束所有的时间点
    vector<CElementDataRunMid> m_vecPassTime;

    //当前测试单元是否检测过途经点，经过了一定延时或者不存在途经点的当前标志才会设置为true，在开始测试时设置为false
    bool m_bCheckedViaPtDev;
};
#endif // !defined(EA_701E35BC_0DB4_4ff8_821E_A3DE12F3A3D2_INCLUDED_)
