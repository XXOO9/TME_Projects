#ifndef __PowerUartDataLink__h__
#define __PowerUartDataLink__h__
/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:DataLink.h
* 文件标识:
* 摘要: 该文件完成通信数据的解析,打包函数功能声明
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2018.05.04
*/

#include "PowerUartBaseDefine.h"
#include "PowerUartFifo8.h"

unsigned short PowerUartWriteDataToFrameLink(unsigned short *pBuf, unsigned short destLen);
unsigned short PowerUartReadLinkFrameData(unsigned short *pBuf, unsigned short maxBufLen);

#endif
