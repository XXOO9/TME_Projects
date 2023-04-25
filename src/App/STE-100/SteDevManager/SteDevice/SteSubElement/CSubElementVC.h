///////////////////////////////////////////////////////////
//  CSubElementVC.h
//  Implementation of the Class CSubElementVC
//  Created on:      14-1月-2022 10:40:08
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_23F5905B_F602_43cf_AB2D_18A4BAD7F250_INCLUDED_)
#define EA_23F5905B_F602_43cf_AB2D_18A4BAD7F250_INCLUDED_

#include "CSubElementBase.h"
#include <QDateTime>

class CElementDataVC : public CElementDataBase
{
public:
    float m_fData; //肺活量数据 单位 ml

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fData = 0.0f;
    }
};

class CSubElementVC : public CSubElementBase
{
public:
    CSubElementVC (unsigned short shUnitID, unsigned int iElementID);
    virtual ~CSubElementVC() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline float &getDataVC(){return m_objData.m_fData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    //创建防作弊资源
    virtual void createAntiCheating() override;
private:
    CElementDataVC m_objData;
};
#endif // !defined(EA_23F5905B_F602_43cf_AB2D_18A4BAD7F250_INCLUDED_)
