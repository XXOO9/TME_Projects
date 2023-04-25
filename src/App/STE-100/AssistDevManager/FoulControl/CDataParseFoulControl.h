#ifndef CDataParseFoulControl_H
#define CDataParseFoulControl_H
#include "CDataParseBase.h"

class CDataParseFoulControl : public CDataParseBase
{
public:
    CDataParseFoulControl();

    virtual  void clearDataCache() override;

    inline bool haveBeenData() {return m_bHaveBeenData;}

protected:
    virtual void dataParseCustomer(unsigned int iSrcLen, unsigned char *pData, unsigned int &nParsedData) override;

private:
    //用于在接收参数中处理底层返回数据，目前来说20个够用
    unsigned short m_arrCmd[20];

    bool m_bHaveBeenData{false};
};

#endif // CDataParseFoulControl_H
