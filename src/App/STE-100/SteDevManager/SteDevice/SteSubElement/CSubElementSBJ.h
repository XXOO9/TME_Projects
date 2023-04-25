///////////////////////////////////////////////////////////
//  CSubElementSBJ.h
//  Implementation of the Class CSubElementSBJ
//  Created on:      14-1月-2022 10:40:09
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_D597171B_3D38_43a4_9AD3_66A04A65E2C5_INCLUDED_)
#define EA_D597171B_3D38_43a4_9AD3_66A04A65E2C5_INCLUDED_

#include "CSubElementBase.h"
#include <QDateTime>

class CElementDataSBJ : public CElementDataBase
{
public:
    float m_fData; //立定跳远数据 cm

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fData = 0.0f;
    }
};

class CSubElementSBJ : public CSubElementBase
{

public:
    CSubElementSBJ (unsigned short shUnitID,  unsigned int iElementID);
    virtual ~CSubElementSBJ() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline float &getDataSBJ(){return m_objData.m_fData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    virtual void createAntiCheating() override;
private:
    CElementDataSBJ m_objData;
};
#endif // !defined(EA_D597171B_3D38_43a4_9AD3_66A04A65E2C5_INCLUDED_)
