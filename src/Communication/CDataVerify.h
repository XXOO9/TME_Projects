#ifndef CDATAVERIFY_H
#define CDATAVERIFY_H

/*************************************************
  <类    名>    CDataVerify
  <功能描述>     数据检验类

  <作    者>    zhy
  <时    间>    2021-08-30
**************************************************/
#include <QObject>
#include <QQueue>
#include <memory>
typedef  unsigned char   U8;
typedef  unsigned int    U32;

class CDataVerify: public QObject
{
public:
    CDataVerify();
    ~CDataVerify();

public:

    /**
     * @brief CRC32校验
     * @param data 数据缓存
     * @param length 数据长度字节数
     * @return 是否正确
     */
    bool verifyCRC32(int length,char *data);

    /**
     * @brief 校验和校验
     * @param data 数据缓存
     * @param length 数据长度字节数
     * @return 是否正确
     */
    bool verifySum(U32 length ,char *data);

private:
    /**
     * @brief 模2除法
     * @param data 数据缓存
     * @param length 数据长度字节数
     * @return 计算结果
     */
    U32 HAL_CRC_CalculateWithoutPrevalue_BySoftware( U32 length,U32 *ptrStart);
};

#endif // CDATAVERIFY_H
