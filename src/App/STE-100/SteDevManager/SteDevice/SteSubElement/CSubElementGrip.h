///////////////////////////////////////////////////////////
//  CSubElementGrip.h
//  Implementation of the Class CSubElementGrip
//  Created on:      14-1月-2022 10:40:17
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_389F012F_79A7_4e46_A51E_236977DB8391_INCLUDED_)
#define EA_389F012F_79A7_4e46_A51E_236977DB8391_INCLUDED_

#include "CSubElementBase.h"
class CElementDataGrip : public CElementDataBase
{
public:
    float m_fData; //握力数据 单位 kg

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fData = 0.0f;
    }
};

class CSubElementGrip : public CSubElementBase
{

public:
    CSubElementGrip (unsigned short shUnitID,  unsigned int iElementID);

    virtual ~CSubElementGrip() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline float &getDataGrip(){return m_objData.m_fData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    //创建防作弊资源
    virtual void createAntiCheating() override;
private:
    CElementDataGrip m_objData;
};
#endif // !defined(EA_389F012F_79A7_4e46_A51E_236977DB8391_INCLUDED_)
