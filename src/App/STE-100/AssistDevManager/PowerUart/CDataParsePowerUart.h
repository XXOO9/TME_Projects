#ifndef CDATAPARSEPOWERUART_H
#define CDATAPARSEPOWERUART_H
#include "CDataParseBase.h"

class CDataParsePowerUart : public CDataParseBase
{
public:
    CDataParsePowerUart();

    virtual  void clearDataCache() override;

    inline bool haveBeenData() {return m_bHaveBeenData;}
protected:
    virtual void dataParse(unsigned int iLen,void *data) override;

private:
    //用于在接收参数中处理底层返回数据，目前来说20个够用
    unsigned short m_arrCmd[20];

    bool m_bHaveBeenData{false};
};

#endif // CDATAPARSEPOWERUART_H
