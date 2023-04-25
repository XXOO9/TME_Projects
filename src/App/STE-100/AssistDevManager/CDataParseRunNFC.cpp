#include "CDataParseRunNFC.h"
#include "CSerialPortCom.h"

CDataParseRunNFC::CDataParseRunNFC()
{

}

//30类型
//02 03 04 05 00 21 D1 B6 E3 74 01 2D 00 1E 00 00 00 00 00 00 00 00 00 01 00 06 00 00 1F A5 08 40 6C
//02 03 04 05 数据包头
//00 21 数据包长度
void CDataParseRunNFC::dataParseCustomer(unsigned int iSrcLen, unsigned char *pData, unsigned int &nParsedData)
{
    nParsedData = 0;

    //确认帧
    if(iSrcLen == 0 || pData == nullptr)
        return;

    unsigned int i = 0, nRemainLen = 0;
    unsigned char *pTemp;
    unsigned char cFrameLen;
    bool bParse;

    PARSE_DATA_RUN_NFC parseData;

    for(; i < iSrcLen; i++)
    {
        nRemainLen = iSrcLen - i;
        pTemp = pData + i;

        //数据长度在6所在位置
        if(nRemainLen >= 6
                && pTemp[0] == 0x02
                && pTemp[1] == 0x03
                && pTemp[2] == 0x04
                && pTemp[3] == 0x05 )
        {
            bParse = true;

            cFrameLen = pTemp[5];
            if(nRemainLen >= cFrameLen)
            {
                switch (m_lBoundRate) {
                case QSerialPort::Baud115200:
                    parse115200(nRemainLen, pTemp, parseData);
                    break;
                case QSerialPort::Baud38400:
                    parse38400(nRemainLen, pTemp, parseData);
                    break;
                default:
                    bParse = false;
                    break;
                }

                //分析了一帧完整数据，将数据位移动到当前数据最后一位，完成循环之后i会
                //自增 1,移动到下一个需要解析的数据头
                i += cFrameLen - 1;

                if(bParse && m_pFunc != nullptr)
                {
                    m_pFunc(&parseData, m_nDataLen);
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    nParsedData = i;
}

void CDataParseRunNFC::parse115200(unsigned int iSrcLen, unsigned char *pData, CDataParseRunNFC::PARSE_DATA_RUN_NFC &parseData)
{
    if(iSrcLen < 0x21)
    {
        parseData.nFrameType = FRAME_HEARTBEAT;
    }
    else
    {
        parseData.nFrameType = FRAME_CARD_INFO;

        parseData.nCardID = 0;
        //卡号 在第24 25 26 位三字节，使用 UINT进行存放,最高位用0x00填充
        //大端转小端
        m_arr[0] = pData[26];
        m_arr[1] = pData[25];
        m_arr[2] = pData[24];
        memcpy(&parseData.nCardID, m_arr, 4);
    }
}

void CDataParseRunNFC::parse38400(unsigned int iSrcLen, unsigned char *pData, CDataParseRunNFC::PARSE_DATA_RUN_NFC &parseData)
{
    //暂时按照长度进行
    if(iSrcLen == 0x13)
    {
        parseData.nFrameType = FRAME_CARD_INFO;

        parseData.nCardID = 0;
        //卡号 在第13 12 11 位三字节，使用 UINT进行存放,最高位用0x00填充
        //大端转小端
        m_arr[0] = pData[13];
        m_arr[1] = pData[12];
        m_arr[2] = pData[11];
        memcpy(&parseData.nCardID, m_arr, 4);
    }
    else
    {
        parseData.nFrameType = FRAME_HEARTBEAT;
    }
}
