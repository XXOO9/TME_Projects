///////////////////////////////////////////////////////////
//  CSubElementSFB.h
//  Implementation of the Class CSubElementSFB
//  Created on:      14-1月-2022 10:40:10
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_27B44618_E294_43b5_8469_7C51C5C26535_INCLUDED_)
#define EA_27B44618_E294_43b5_8469_7C51C5C26535_INCLUDED_

#include "CSubElementBase.h"
#include <QDateTime>

class CElementDataSFB : public CElementDataBase
{
public:
    float m_fData; //作为体前屈数据 cm

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fData = 0.0f;
    }
};

class CSubElementSFB : public CSubElementBase
{

public:
    CSubElementSFB (unsigned short shUnitID,  unsigned int iElementID);
    virtual ~CSubElementSFB() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline float &getDataSFB(){return  m_objData.m_fData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    virtual void createAntiCheating() override;
private:
    CElementDataSFB m_objData;
};
#endif // !defined(EA_27B44618_E294_43b5_8469_7C51C5C26535_INCLUDED_)
