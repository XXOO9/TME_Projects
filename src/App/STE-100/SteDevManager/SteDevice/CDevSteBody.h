#ifndef CDEVSTEBODY_H
#define CDEVSTEBODY_H
#include "CDevHardwareBase.h"

class CDevSteBody : public CDevHardwareBase
{
public:
    CDevSteBody(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteBody() override;

    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
    virtual bool existErr(const unsigned short &shElement) override;

    //校准身高体重 kg cm
    bool calibrateWeight(const float &fValue);
    bool calibrateHeight(const float &fValue);

    //身高体重方发送开始命令需要进行限制，每次发送对应的开始项目之前需要确定内部开始命令为默认状态
    //会出现重复发送开始身高的情况
    virtual bool startWork(unsigned short shElementIndex, bool bWait = true) override;
protected:

    //解析身高（数据帧正确核对,核对之后不进行长度验证）直接进行使用
    void parseHeight(CSubElementBase *pElement, unsigned char *pData);
    void parseWeight(CSubElementBase *pElement, unsigned char *pData);
    void parseHBC(CSubElementBase *pElement, unsigned char *pData, const unsigned short &shCmd);

    //获取数据类型对应的数据长度
    short getDataTypeLen(const unsigned short &shDataType);

    //进行重写是因为身高体重比较特殊，在组网的情况下需要往下发送获取数据命令
    virtual void onUpdateTimeOut() override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLenHeight;
    const unsigned short m_shDataLenWeight;
    
    //人体阻抗数据存在分包
    const unsigned short m_shDataLenHBC;

    //人体阻抗分包
    set<unsigned short> m_setSubHBC;

    //身高和体重的命令
    set<unsigned short> m_setDataFrameTypeHW;

    //
    QByteArray m_arrGetData;
};

#endif // CDEVSTEBODY_H
