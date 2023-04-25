///////////////////////////////////////////////////////////
//  CSubElementPushup.h
//  Implementation of the Class CSubElementPushup
//  Created on:      14-1月-2022 10:40:16
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_FD2CB698_D6F4_4897_BAB8_41A27B143869_INCLUDED_)
#define EA_FD2CB698_D6F4_4897_BAB8_41A27B143869_INCLUDED_

#include "CSubElementBase.h"

class CElementDataPushup : public CElementDataBase
{
public:
    int m_nData; //俯卧撑数据 单位个

    virtual void reset()
    {
        CElementDataBase::reset();
        m_nData = 0;
    }
};

class CSubElementPushup : public CSubElementBase
{

public:
    CSubElementPushup (unsigned short shUnitID,  unsigned int iElementID);
    virtual ~CSubElementPushup() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline int& getDataPushup(){return m_objData.m_nData;}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

protected:
    //俯卧撑防作弊逻辑和引体向上相同
    virtual void createAntiCheating() override;
private:
    CElementDataPushup m_objData;
};
#endif // !defined(EA_FD2CB698_D6F4_4897_BAB8_41A27B143869_INCLUDED_)
