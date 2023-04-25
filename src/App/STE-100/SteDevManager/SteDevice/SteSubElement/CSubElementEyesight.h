///////////////////////////////////////////////////////////
//  CSubElementEyesight .h
//  Implementation of the Class CSubElementEyesight 
//  Created on:      14-1月-2022 10:40:17
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_E7D3D3CB_FCBD_4d01_B37B_D92AD371B309_INCLUDED_)
#define EA_E7D3D3CB_FCBD_4d01_B37B_D92AD371B309_INCLUDED_

#include "CSubElementBase.h"

namespace  EyeSightStatus{
    enum enEyeSight_Data_Status{
        EYE_LEFT_DATA = 0,
        EYE_RIGHT_DATA = 1,
        EYE_DATA_COUNT
    };
}

class CElementDataEyesight : public CElementDataBase
{
public:
    float m_fREyesight;
    float m_fLEyesight;

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fREyesight = 0.0f;
        m_fLEyesight = 0.0f;
    }
};

class CSubElementEyesight  : public CSubElementBase
{

public:
    CSubElementEyesight (unsigned short shUnitID,  unsigned int iElementID);

    virtual ~CSubElementEyesight () override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline float &getDataREyesight(){return m_objData.m_fREyesight;}
    inline float &getDataLEyesight(){return m_objData.m_fLEyesight;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;
private:
    CElementDataEyesight m_objData;
};
#endif // !defined(EA_E7D3D3CB_FCBD_4d01_B37B_D92AD371B309_INCLUDED_)
