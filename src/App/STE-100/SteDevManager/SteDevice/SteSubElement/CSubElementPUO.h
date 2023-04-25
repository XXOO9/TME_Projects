///////////////////////////////////////////////////////////
//  CSubElementPUO.h
//  Implementation of the Class CSubElementPUO
//  Created on:      8-10月-2022 14:56:08
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(ELEMENT_PUO_H)
#define ELEMENT_PUO_H

#include "CSubElementBase.h"
#include <QDateTime>

class CElementDataPUO : public CElementDataBase
{
public:
    int m_nData; //斜体引体向上 单位 次

    virtual void reset()
    {
        CElementDataBase::reset();
        m_nData = 0;
    }
};

class CSubElementPUO : public CSubElementBase
{
public:
    CSubElementPUO (unsigned short shUnitID, unsigned int iElementID);
    virtual ~CSubElementPUO() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline int &getData(){return m_objData.m_nData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    //创建防作弊资源
    virtual void createAntiCheating() override;
private:
    CElementDataPUO m_objData;
};
#endif // !defined(ELEMENT_PUO_H)
