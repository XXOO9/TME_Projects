#ifndef CSubElementPVV_H
#define CSubElementPVV_H
#include "CSubElementBase.h"

#define DATA_SIZE_MAX_PVV 40

class CElementDataPVV : public CElementDataBase
{
public:
    float m_arrData[DATA_SIZE_MAX_PVV]; //肺通气量数据 单位 ml/s
    unsigned char m_cValidPointNum; //本次数据中的有效数据点数

    virtual void reset()
    {
        CElementDataBase::reset();
        memset(m_arrData, 0, DATA_SIZE_MAX_PVV * 4);
        m_cValidPointNum = 0;
    }
};

class CSubElementPVV : public CSubElementBase
{
public:
    CSubElementPVV (unsigned short shUnitID, unsigned int iElementID);
    virtual ~CSubElementPVV() override;

    //设置 子元素信息 (根据类型不同，包括结果、违规标志、电量等)
    virtual void setElementInfo(void* pData, unsigned short shLength) override;

    inline vector<float> &getDataAll(){return m_vecAllPoint;}

    //获取新数据之后应该调用清除函数
    inline vector<float> &getDataNew(){return m_vecNewPoint;}

    inline void clearNewData(){m_vecNewPoint.clear();}

    //测试单元开启测试,外部在开始测试之前进行设置
    virtual void startTest() override;
    virtual void stopTest() override;
private:
    //暂时用vector来存储
    vector<float> m_vecAllPoint;
    vector<float> m_vecNewPoint;

    //主要保存测试状态
    CElementDataPVV m_objData;
};

#endif // CSubElementPVV_H
