#ifndef __PowerUartApplication__h__
#define __PowerUartApplication__h__

/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:PowerUartApplication.h
* 文件标识:
* 摘要: 该文件完成电源串口应用数据的获取以及发送函数声明
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2021.09.15
*/

#include "PowerUartDataLink.h"

#ifdef __cplusplus
extern "C"
{
#endif

void PowerUartApplicationInit(void);
unsigned short PowerUartWriteApplicationDataSegment(unsigned short cmdType, unsigned short *pBuf, unsigned short destLen);
unsigned short PowerUartReadApplicationDataSegment(unsigned short *pCmdType, unsigned short *pBuf);

#ifdef __cplusplus
}
#endif

#endif
