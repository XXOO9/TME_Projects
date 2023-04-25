///////////////////////////////////////////////////////////
//  CSubElementPullUp.h
//  Implementation of the Class CSubElementPullUp
//  Created on:      14-1月-2022 10:40:12
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_C59179DB_5C44_45a6_9093_E458F072D911_INCLUDED_)
#define EA_C59179DB_5C44_45a6_9093_E458F072D911_INCLUDED_

#include "CSubElementBase.h"
class CElementDataPullUp : public CElementDataBase
{
public:
    int m_nData; //引体向上数据 单位个

    virtual void reset()
    {
        CElementDataBase::reset();
        m_nData = 0;
    }
};

class CSubElementPullUp : public CSubElementBase
{

public:
    CSubElementPullUp (unsigned short shUnitID,  unsigned int iElementID);
    virtual ~CSubElementPullUp() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline int& getDataPullup(){return m_objData.m_nData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    virtual void createAntiCheating() override;
private:
    CElementDataPullUp m_objData;
};
#endif // !defined(EA_C59179DB_5C44_45a6_9093_E458F072D911_INCLUDED_)
