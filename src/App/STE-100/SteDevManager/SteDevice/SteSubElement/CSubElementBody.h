///////////////////////////////////////////////////////////
//  CSubElementBody.h
//  Implementation of the Class CSubElementBody
//  Created on:      14-1月-2022 10:40:06
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_127E9871_E523_48b6_A0F3_83C36498340D_INCLUDED_)
#define EA_127E9871_E523_48b6_A0F3_83C36498340D_INCLUDED_

//身高体重在测试过程中

#include "CSubElementBase.h"
class CElementBody : public CElementDataBase
{
public:
    short m_shTestItem; //当前测试项目
};

class CElementHeight : public CElementBody
{
public:
    float m_fHeight; //cm

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fHeight = 0.0f;
    }
};

class CElementWeight : public CElementBody
{
public:
    float m_fWeight; //kg

    virtual void reset()
    {
        CElementDataBase::reset();
        m_fWeight = 0.0f;
    }
};

class CElementHBC : public CElementBody
{
public:
    enum{HBC_DATA_NUM = 30};
    float m_arrHBC[HBC_DATA_NUM];

    virtual void reset()
    {
        CElementDataBase::reset();
        memset(m_arrHBC, 0, HBC_DATA_NUM * sizeof (float));
    }

    CElementHBC& operator=(const CElementHBC &data)
    {
        m_cTestState = data.m_cTestState;
        m_tTestState = data.m_tTestState;
        m_shTestItem = data.m_shTestItem;
        memcpy(m_arrHBC, data.m_arrHBC, HBC_DATA_NUM * sizeof (float));
        return *this;
    }
};

class CSubElementBody : public CSubElementBase
{
public:
    enum flagType{
        Flag_Start,Flag_Complete, flage_Guid_Complete
    };
    CSubElementBody(unsigned short shUnitID,  unsigned int iElementID = 0);
    virtual ~CSubElementBody() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    //这里原始是开始状态是true返回值，要不返回0，将状态带入数据当中
    inline float getResHeight()
    {
        if(!m_objHeight.m_bStart)
        {
            return 0.0f;
        }

        return m_objHeight.m_fHeight;
    }

    inline float getResWeight()
    {
        if(!m_objWeight.m_bStart)
        {
            return 0.0f;
        }

        return m_objWeight.m_fWeight;
    }

    //返回当前最大返回数量 传入的float数组大小
    inline unsigned int getResHBC(float *pData, unsigned int szDataNum)
    {
        if(pData == nullptr || szDataNum < CElementHBC::HBC_DATA_NUM)
            return 0;

        unsigned int nRetNum = szDataNum > CElementHBC::HBC_DATA_NUM ? CElementHBC::HBC_DATA_NUM : szDataNum;

        memcpy(pData, m_objHBC.m_arrHBC, nRetNum * sizeof (float));
        return nRetNum;
    }

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;

    //外部辅助判断，外部测试完成之后进行设置
    void setComplete(const short &shTestItem);

    //获取标志,后面的标志表示需要获取某个类型的标志
    void setFlag(const short &shTestItem, flagType flagType, bool bFlag);

    //判断是否完成 开始标志为true，内部完成和外部完成不一致同时内部完成为true
    bool isCompleteFlagDif(const short &shTestItem);
    //判断是否完成 开始标志为true，内部完成和外部完成一致同时为true
    bool isCompleteFlagSame(const short &shTestItem);

    virtual bool checkIfCanStart(const short &shTestItem) override;
    virtual void setElementStartFlag(const short &shTestItem, bool bStart) override;
protected:
    CElementDataBase *getElementData(const short &shTestItem);

private:
    CElementHeight m_objHeight; //身高数据
    CElementWeight m_objWeight; //体重数据
    CElementHBC m_objHBC; //人体阻抗数据 这一部分存储的是原始值，需要经过对应的分析处理数据才能使用

    set<CElementDataBase *> m_setElement;
};
#endif // !defined(EA_127E9871_E523_48b6_A0F3_83C36498340D_INCLUDED_)
