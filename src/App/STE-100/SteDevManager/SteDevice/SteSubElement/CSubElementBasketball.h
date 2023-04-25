///////////////////////////////////////////////////////////
//  CElementDataBasketball.h
//  Implementation of the Class CElementDataBasketball
//  Created on:      8-10月-2022 14:56:08
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined ELEMENT_BASKETBALL_H
#define ELEMENT_BASKETBALL_H

#include "CSubElementBase.h"
#include <QDateTime>

class CElementDataBasketball : public CElementDataBase
{
public:
    float m_fData; //篮球数据 单位时间 s

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fData = 0.0f;
    }
};

class CSubElementBasketball : public CSubElementBase
{
public:
    CSubElementBasketball (unsigned short shUnitID, unsigned int iElementID);
    virtual ~CSubElementBasketball() override;

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
    CElementDataBasketball m_objData;
};
#endif // !defined ELEMENT_BASKETBALL_H
