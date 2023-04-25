#include "CDataParseBase.h"
#include "CLogger.h"

CDataParseBase::CDataParseBase()
{

}

CDataParseBase::~CDataParseBase()
{
    if(m_pCacheData != nullptr)
    {
        delete []m_pCacheData;
        m_pCacheData = nullptr;
    }
}

void CDataParseBase::clearDataCache()
{
    m_lExistLenth = 0;
}

void CDataParseBase::dataParse(unsigned int iLen, void *data)
{
    if(iLen == 0 || data == nullptr)
        return ;

    //将数据放入缓存
    appendData(data, iLen);

    //解析了多少数据就需要删除多少数据
    unsigned int nParsedNum = 0;

    //详细解析
    unsigned int nDataLen = getExistDataSize();
    unsigned char *pDataParse = getCacheData();

    dataParseCustomer(nDataLen, pDataParse, nParsedNum);

    //删除已经解析的数据长度
    delExistData(nParsedNum);
}

void CDataParseBase::dataParseCustomer(unsigned int iSrcLen, unsigned char *pData, unsigned int &nParsedData)
{
    nParsedData = 0;
    if(iSrcLen == 0 || pData == nullptr)
        return;
}

void CDataParseBase::appendData(void *src, unsigned long lSrcLen)
{
    if(lSrcLen == 0 || src == nullptr)
        return;

    //将数据放到指定缓存当中,若空间不足内部申请
    if(m_lExistLenth + lSrcLen > m_lCacheDataSize || m_pCacheData == nullptr)
    {
        auto pTemp = m_pCacheData;
        m_lCacheDataSize = m_lExistLenth + lSrcLen;
        m_pCacheData = new unsigned char[m_lCacheDataSize];

        memcpy(m_pCacheData, pTemp, m_lExistLenth);
        memcpy(m_pCacheData + m_lExistLenth, src, lSrcLen);
        delete [] pTemp;

        //打印警告重新new数据
        LOGWARN("data parse cache new size" + to_string(m_lCacheDataSize));
    }
    else
    {
        memcpy(m_pCacheData + m_lExistLenth, src, lSrcLen);
    }
    m_lExistLenth = m_lExistLenth + lSrcLen;

    //LOGINFO("dataparse start" + to_string(m_lExistLenth));
}

void CDataParseBase::delExistData(unsigned long lDelLen)
{
    if(lDelLen == 0)
        return;

    if(lDelLen >= m_lExistLenth)
    {
        m_lExistLenth = 0;
    }
    else
    {
        m_lExistLenth = m_lExistLenth - lDelLen;
        memcpy(m_pCacheData, m_pCacheData + lDelLen, m_lExistLenth);
    }

    //LOGINFO("dataparse end" + to_string(m_lExistLenth));
}

unsigned long &CDataParseBase::getExistDataSize()
{
    return m_lExistLenth;
}

unsigned char *CDataParseBase::getCacheData()
{
    return m_pCacheData;
}
