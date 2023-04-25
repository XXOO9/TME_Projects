///////////////////////////////////////////////////////////
//  CSubElementSolidball.h
//  Implementation of the Class CSubElementSolidball
//  Created on:      8-10月-2022 14:56:08
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(ELEMENT_SOLIDBALL_H)
#define ELEMENT_SOLIDBALL_H

#include "CSubElementBase.h"
#include <QDateTime>

class CElementDataSolidball : public CElementDataBase
{
public:
    float m_fData; //实心球 单位 m

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fData = 0.0f;
    }
};

class CSubElementSolidball : public CSubElementBase
{
public:
    CSubElementSolidball (unsigned short shUnitID, unsigned int iElementID);
    virtual ~CSubElementSolidball() override;

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
    CElementDataSolidball m_objData;
};
#endif // !defined(ELEMENT_SOLIDBALL_H)
