#ifndef CDATAPARSERUNNFC_H
#define CDATAPARSERUNNFC_H
#include "CDataParseBase.h"

class CDataParseRunNFC : public CDataParseBase
{
public:
    enum{
        FRAME_HEARTBEAT,
        FRAME_CARD_INFO
    };

    struct PARSE_DATA_RUN_NFC
    {
        int nFrameType;
        unsigned int nCardID;
    };

    CDataParseRunNFC();

    //在使用之前需要设置当前解析的波特率
    void setBoundRate(unsigned long boundRate){m_lBoundRate = boundRate;}
protected:
    virtual void dataParseCustomer(unsigned int iSrcLen, unsigned char *pData, unsigned int &nParsedData) override;

    void parse115200(unsigned int iSrcLen, unsigned char *pData, PARSE_DATA_RUN_NFC &parseData);
    void parse38400(unsigned int iSrcLen, unsigned char *pData, PARSE_DATA_RUN_NFC &parseData);
private:
    unsigned char m_arr[4]{0};
    unsigned int m_nDataLen{sizeof (PARSE_DATA_RUN_NFC)};

    //nfc设备现有两种，一种是38400，一种是115200
    unsigned long m_lBoundRate;
};

#endif // CDATAPARSERUNNFC_H
