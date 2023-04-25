#include "CDataParseStartingGun.h"

CDataParseStartingGun::CDataParseStartingGun()
{

}

void CDataParseStartingGun::dataParseCustomer(unsigned int iSrcLen, unsigned char *pData, unsigned int &nParsedData)
{
    nParsedData = 0;
    if(iSrcLen == 0 || pData == nullptr)
        return;

    unsigned int i = 0, nRemainLen = 0;
    unsigned char *pTemp;
    unsigned char cFrameLen;

    for(; i < iSrcLen; i++)
    {
        nRemainLen = iSrcLen - i;
        pTemp = pData + i;

        //发令枪数据不存在其他数据
        if(nRemainLen >= 4)
        {
            if(pTemp[0] != 0x63 && pTemp[1] != 0xE1 && pTemp[2] != 0xE2 && pTemp[3] != 0xE3)
            {
                continue;
            }

            cFrameLen = 4;


            //分析了一帧完整数据，将数据位移动到当前数据最后一位，完成循环之后i会
            //自增 1,移动到下一个需要解析的数据头
            i += cFrameLen - 1;

            //通知外部解析成功
            m_pFunc(nullptr, 0);
        }
        else
        {
            break;
        }
    }

    nParsedData = i;
}
