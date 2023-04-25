#include "CDataParseFoulControl.h"
#include <QDebug>
CDataParseFoulControl::CDataParseFoulControl()
{

}

void CDataParseFoulControl::clearDataCache()
{
    CDataParseBase::clearDataCache();
    m_bHaveBeenData = false;
}

void CDataParseFoulControl::dataParseCustomer(unsigned int iSrcLen, unsigned char *pData, unsigned int &nParsedData)
{
    nParsedData = 0;

    if(iSrcLen < 6 || pData == nullptr)
        return;

    unsigned int i = 0, nRemainLen = 0;
    unsigned char *pTemp;

    static unsigned int nDataLen = 4;

    unsigned short shCmd;
    for(; i < iSrcLen; i++)
    {
        nRemainLen = iSrcLen - i;

        //剩余数据小于帧头返回
        if(nRemainLen <= 6)
        {
            break;
        }

        pTemp = pData + i;

        if(nRemainLen >= nDataLen && pTemp[0] == 0xFF && pTemp[1] == 0xFF && pTemp[4] == 0xFF && pTemp[5] == 0xFF)
        {
            memcpy(&shCmd, pTemp + 2, 2);
            if(m_pFunc != nullptr)
            {
                m_pFunc(&shCmd, 2);
            }
            i += nDataLen - 1;
        }
    }

    nParsedData = i;
}
