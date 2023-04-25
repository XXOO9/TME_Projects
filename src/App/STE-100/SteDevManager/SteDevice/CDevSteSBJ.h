#ifndef CDEVSTESBJ_H
#define CDEVSTESBJ_H
#include "CDevHardwareBase.h"
#include <QByteArray>
class CDevSteSBJ : public CDevHardwareBase
{
    //参数值为 0 ——表示起跳线在 0（cm）处
    //参数值为 1 ——表示起跳线在 80（cm）处
    enum Start_Pos{
        Start_Pos_0 = 0, Start_Pos_80 = 1
    };

public:
    CDevSteSBJ(unsigned int iDevID, ICommunicate &communicate);
    virtual ~CDevSteSBJ() override;

    //设置起跳点位置 Start_Pos
    void setStartPos(const int &pos);

    //是否校准了起跳点
    bool getCalibrationSPosFlag();
protected:
    virtual int parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen) override;
private:
    //保存数据长度，避免每一次都sizeof
    const unsigned short m_shDataLen;
};

#endif // CDEVSTESBJ_H
