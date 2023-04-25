///////////////////////////////////////////////////////////
//  CSubElementFootball.h
//  Implementation of the Class CSubElementFootball
//  Created on:      8-10月-2022 14:56:08
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined ELEMENT_FOOTBALL
#define ELEMENT_FOOTBALL

#include "CSubElementBase.h"
#include <QDateTime>

class CElementDataFootball : public CElementDataBase
{
public:
    float m_fData; //足球数据单位s

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fData = 0.0f;
    }
};

class CSubElementFootball : public CSubElementBase
{
public:
    CSubElementFootball (unsigned short shUnitID, unsigned int iElementID);
    virtual ~CSubElementFootball() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline float &getData(){return m_objData.m_fData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    //创建防作弊资源
    virtual void createAntiCheating() override;
private:
    CElementDataFootball m_objData;
};
#endif // !defined(EA_23F5905B_F602_43cf_AB2D_18A4BAD7F250_INCLUDED_)
