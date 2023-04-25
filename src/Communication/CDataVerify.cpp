#include "CDataVerify.h"

#define HAL_CRC_POLY 0x04C11DB7 //CRC32生成多项式
#define HAL_CRC_BIT31(reg) (reg & 0x80000000)
#define HAL_CRC_REG_LEFT_SHIFT(reg, regPrepare) ((reg << 1) | ((regPrepare & 0x80000000) >> 31))
#define HAL_CRC_REGPREPARE_LEFT_SHIFT(regPrepare) (regPrepare << 1)
#define HAL_CRC_REG_RESHAPE(reg) (((reg & 0xFF000000) >> 24) | ((reg & 0x00FF0000) >> 8)\
    | ((reg & 0x0000FF00) << 8) | ((reg & 0x000000FF) << 24))

CDataVerify::CDataVerify()
{

}

CDataVerify::~CDataVerify()
{

}

U32 CDataVerify::HAL_CRC_CalculateWithoutPrevalue_BySoftware(U32 length,U32 *ptrStart)
{
    U32 poly;
    U32 reg;
    U32 regPrepare;
    U32 crc;
    // 参数初始化
    poly = HAL_CRC_POLY;
    crc = 0x00000000;
    length >>= 2;
    //模2除法计算CRC
    reg = *ptrStart++;
    reg = HAL_CRC_REG_RESHAPE(reg);
    /* 逐个32位数据进行CRC计算 */
    for (U32 i=1; i<length; i++)
    {
        regPrepare = *ptrStart++;
        /* 每个字节逐位计算 */
        for (U8 j=0; j<32; j++)
        {
            /* 最高比特位为1，进行异或操作 */
            if (HAL_CRC_BIT31(reg) != 0)
            {
                /* 左移一位，后续数据递补 */
                reg = HAL_CRC_REG_LEFT_SHIFT(reg, regPrepare);
                regPrepare = HAL_CRC_REGPREPARE_LEFT_SHIFT(regPrepare);
                /* 异或操作 */
                reg ^= poly;
            }
            else
            {
                /* 左移一位，后续数据递补 */
                reg = HAL_CRC_REG_LEFT_SHIFT(reg, regPrepare);
                regPrepare = HAL_CRC_REGPREPARE_LEFT_SHIFT(regPrepare);
            }
        }
    }
    /* CRC结果赋值 */
    crc = reg;
    return (crc);
}

//函数无效
bool CDataVerify::verifyCRC32( int length, char *data)
{
    //校验数据末尾需要添加4个0字段字节，同时要保证校验数据长度为4的整数倍，可能后面还会增加1到3个0字节字段
    int add_Num = (length + 4) % 4;
    char* pData;
    pData = new char[static_cast<uint>(length + 4 +add_Num)];
    memcpy(pData,data,static_cast<size_t>(length));
    int i = length;
    for(; i < length + 4 + add_Num; ++i)
    {
        *(pData + i) = 0;
    }
    uint* pCheck_Code = reinterpret_cast<uint*>(pData);
    uint iCheck_Code = *pCheck_Code;
    *pCheck_Code = 0;

    U32* pU32Data = reinterpret_cast<U32*>(pData);
    U32 value = HAL_CRC_CalculateWithoutPrevalue_BySoftware(static_cast<U32>(length + 4 +add_Num),pU32Data);

    delete[] pData;
    pData = nullptr;

    if(iCheck_Code == value)
    {
        return true;
    }

    return false;
}

//函数无效
bool CDataVerify::verifySum( U32 length ,char *data)
{
    //取出校验码
    uint* pVerify_Code = reinterpret_cast<uint*>(data);
    uint  verify_Code  = *pVerify_Code;
    //校验码位以0填充
    *pVerify_Code = 0;

    int iLength;
    char* pchData;
    if(length % sizeof(uint) != 0)
    {
        iLength = (length + (4 - length % sizeof(uint))) / 4;
        pchData = new char[length + (4 - length % sizeof(uint))];
        memcpy(pchData,data,length);
        int i = 0;
        for(;i < static_cast<int>(4 - length % sizeof(uint)); ++i)
        {
            *(pchData + i + length) = 0;
        }
    }
    else
    {
        iLength = length / 4;
        pchData = new char[length];
        memcpy(pchData,data,length);
    }
    uint* pData = reinterpret_cast<uint*>(pchData);
    uint iSum = 0;
    //数据位依次相加(16进制数)
    int i = 0;
    for(; i < iLength; ++i)
    {
        iSum += *(pData + i);
    }

    delete[] pchData;
    pchData = nullptr;

    if(iSum == verify_Code)
    {
        return true;
    }
    return false;
}


