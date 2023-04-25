///////////////////////////////////////////////////////////
//  CSubElementSitUp.h
//  Implementation of the Class CSubElementSitUp
//  Created on:      14-1月-2022 10:40:11
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_12E51E59_3ACE_4ffe_9D5A_A07636D6E7FB_INCLUDED_)
#define EA_12E51E59_3ACE_4ffe_9D5A_A07636D6E7FB_INCLUDED_

#include "CSubElementBase.h"

class CElementDataSitUp : public CElementDataBase
{
public:
    int m_nData; //仰卧起坐数据 个数

    virtual void reset()
    {
        CElementDataBase::reset();
        m_nData = 0;
    }
};

class CSubElementSitUp : public CSubElementBase
{

public:
    CSubElementSitUp (unsigned short shUnitID, unsigned int iElementID);
    virtual ~CSubElementSitUp() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline int &getDataSitUp(){return m_objData.m_nData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    //创建防作弊资源
    virtual void createAntiCheating() override;
private:
    CElementDataSitUp m_objData;
};
#endif // !defined(EA_12E51E59_3ACE_4ffe_9D5A_A07636D6E7FB_INCLUDED_)
