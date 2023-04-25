/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:PowerUartDataLink.c
* 文件标识:
* 摘要: 主要完成通信数据的解析,打包函数功能
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2018.05.04
*/

#include "PowerUartDataLink.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////1.宏定义以及枚举/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum 
{
	WAIT_HEAD1 = 0,
	WAIT_HEAD2,
	WAIT_CHECK, 
	WAIT_LEN, 
	WAIT_DATA
};

////////////////////////////////////////////////////////////////////////////////
///////////////////////////2.全局变量的定义/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
PowerUartDataLinkFrameHeadStruct g_SendFrameLink;//接收帧帧头缓冲区
PowerUartDataLinkFrameHeadStruct g_RecvFrameLink;//发送帧帧头缓冲区

////////////////////////////////////////////////////////////////////////////////
///////////////////////////3.函数声明///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///////////////////////////4.函数定义///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*************************************************
Function:       unsigned short PowerUartWriteDataToFrameLink(unsigned short *pBuf, unsigned short destLen)
Description:    写入数据到帧链路层
Calls:
Called By:
Input:          pBuf:应用数据(包含命令以及数据)
                len:应用帧长,双字节长度
Output:         无
Return:         0表示失败,其余表示成功
Others:
*************************************************/
unsigned short PowerUartWriteDataToFrameLink(unsigned short *pBuf, unsigned short destLen)
{
	unsigned short ret = 0;
	unsigned short sLen = 0;
	
	g_SendFrameLink.m_Head1 = POWER_UART_PKG_HEAD_TAG;
	g_SendFrameLink.m_Head2 = POWER_UART_PKG_HEAD_TAG;
	g_SendFrameLink.m_Len = destLen;
	if(destLen > DATA_LEN_MAX + 1)//不能超过应用数据的上限,此时加上命令类型U16
		return ret;
	g_SendFrameLink.m_CheckSum = PowerUartCheckSum16(0, &g_SendFrameLink.m_Len, 1);//最后算包含长度的校验和
	g_SendFrameLink.m_CheckSum = PowerUartCheckSum16(g_SendFrameLink.m_CheckSum, pBuf, destLen);//计算应用数据校验和
	
	ret = MyQueue_Fifo8_UnusedCount(&g_PowerUartSendFifo8);
	sLen = (sizeof(PowerUartDataLinkFrameHeadStruct) + (destLen << 1));
	if(ret >= sLen)//FIFO缓存够
	{
		MyQueue_Fifo8_putArray(&g_PowerUartSendFifo8, (unsigned char *)&g_SendFrameLink, sizeof(PowerUartDataLinkFrameHeadStruct));//填充帧头
		MyQueue_Fifo8_putArray(&g_PowerUartSendFifo8, (unsigned char *)pBuf, (destLen << 1));//填充数据
		ret = sLen;
	}
	else
	{
		ret = 0;
	}
	
	return ret;
}

/*************************************************
Function:       uint16_t PowerUartReadLinkFrameData(uint16_t *pBuf, uint16_t maxBufLen)
Description:    读取帧链路层数据
Calls:
Called By:
Input:          pBuf:应用数据缓冲地址
                maxBufLen:最大可以接收的长度
Output:         无
Return:         0,不存在帧,>0存在帧
Others:
*************************************************/
unsigned short PowerUartReadLinkFrameData(unsigned short *pBuf, unsigned short maxBufLen)
{
	//解帧,直到FIFO中没有或者数据不够,或完成一帧接收
	unsigned short ret = 0;
	static unsigned char readStep = WAIT_HEAD1;
	static unsigned char curRecvCnt;
	static unsigned short recvDataCnt;
	static unsigned short recvDataOffset;//接收字节数计数

	while(MyQueue_Fifo8_UsedCount(&g_PowerUartRecvFifo8))
	{
		switch(readStep)
		{
			case WAIT_HEAD1:
			{
				ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
				                             (unsigned char *)&g_RecvFrameLink.m_Head1, \
																		 sizeof(g_RecvFrameLink.m_Head1), \
																		 1);
				if(ret == 1 && g_RecvFrameLink.m_Head1 == POWER_UART_PKG_HEAD_TAG)
				{
					readStep = WAIT_HEAD2;
				}
				ret = 0;
				break;
			}
			case WAIT_HEAD2:
			{
				ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
				                             (unsigned char *)&g_RecvFrameLink.m_Head2, \
					                           sizeof(g_RecvFrameLink.m_Head2), \
						                         1);
				if(ret == 1 && g_RecvFrameLink.m_Head2 == POWER_UART_PKG_HEAD_TAG)
				{
					readStep = WAIT_CHECK;
					curRecvCnt = 0;
				}
				else
				{
					readStep = WAIT_HEAD1;
				}
				ret = 0;
				break;
			}
			case WAIT_CHECK: //解析校验
			{
				if(curRecvCnt == 0)
				{
					ret = MyQueue_Fifo8_UsedCount(&g_PowerUartRecvFifo8);
					if(ret == 1)//只接受到一个字节
					{
						curRecvCnt++;
						ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
						                             (unsigned char *)&g_RecvFrameLink.m_CheckSum, \
							                           sizeof(g_RecvFrameLink.m_CheckSum), 
								                         1);
					}
					else if(ret > 1)//接受到超过2字节,长度接受完整
					{
						ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
						                             (unsigned char *)&g_RecvFrameLink.m_CheckSum, \
							                           sizeof(g_RecvFrameLink.m_CheckSum), \
								                         2);                                         
						readStep = WAIT_LEN;
						curRecvCnt = 0;
					}
				}
				else//已经接受一个字节
				{
					ret = MyQueue_Fifo8_UsedCount(&g_PowerUartRecvFifo8);
					if(ret >= 1)//存在第二字节
					{
						ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, (((unsigned char *)&g_RecvFrameLink.m_CheckSum) + 1), sizeof(g_RecvFrameLink.m_CheckSum), 1);
            readStep = WAIT_LEN;
						curRecvCnt = 0;
					}
				}
				ret = 0;
				break;
			}
			case WAIT_LEN: //解析长度
			{		
				if(curRecvCnt == 0)
				{
					ret = MyQueue_Fifo8_UsedCount(&g_PowerUartRecvFifo8);
					if(ret == 1)//只接受到一个字节
					{
						curRecvCnt++;
						ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
						                             (unsigned char *)&g_RecvFrameLink.m_Len, \
							                           sizeof(g_RecvFrameLink.m_Len), \
								                         1);
					}
					else if(ret > 1)//接受到超过2字节,长度接受完整
					{
						ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
						                             (unsigned char *)&g_RecvFrameLink.m_Len, \
							                           sizeof(g_RecvFrameLink.m_Len), \
								                         2);                                              
						if(g_RecvFrameLink.m_Len <= maxBufLen)//长度正常
						{
							readStep = WAIT_DATA;
							recvDataCnt = 0;
							recvDataOffset = 0;
						}
						else//数据长度错误
						{
							readStep = WAIT_HEAD1;
						}
						curRecvCnt = 0;
					}
				}
				else//已经接受一个字节
				{
					ret = MyQueue_Fifo8_UsedCount(&g_PowerUartRecvFifo8);
					if(ret >= 1)//存在第二字节
					{
						ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
						                             (((unsigned char *)&g_RecvFrameLink.m_Len) + 1), \
							                           sizeof(g_RecvFrameLink.m_Len), \
								                         1);
						if(g_RecvFrameLink.m_Len <= maxBufLen)//长度正常
						{
							readStep = WAIT_DATA;
							recvDataCnt = 0;
							recvDataOffset = 0;
						}
						else//数据长度错误
						{
							readStep = WAIT_HEAD1;
						}
						curRecvCnt = 0;
					}
				}
				ret = 0;
				break;
			}
			case WAIT_DATA: //解析数据
			{
				ret = MyQueue_Fifo8_UsedCount(&g_PowerUartRecvFifo8);
				if(ret > 0)//存在数据,读取
				{
					recvDataOffset = recvDataCnt;//保存此时的数据偏移量
					recvDataCnt = recvDataCnt + ret;//统计此时接收到的数据
					if(recvDataCnt >= (g_RecvFrameLink.m_Len << 1))//数据已够
					{
						if(recvDataOffset)//数据存在偏移,加上此时数据已经满足要求,则取(g_RecvFrame.m_Len << 1) - DataOffset数据量即可
						{
							ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
							                             ((unsigned char *)pBuf + recvDataOffset), \
								                           (DATA_LEN_MAX + 1) << 1, \
							                             (g_RecvFrameLink.m_Len << 1) - recvDataOffset);//取出数据
							ret = PowerUartCheckSum16(0, &g_RecvFrameLink.m_Len, 1);
							ret = PowerUartCheckSum16(ret, pBuf, g_RecvFrameLink.m_Len);
							if(g_RecvFrameLink.m_CheckSum == ret)
							{
								readStep = WAIT_HEAD1;
								return g_RecvFrameLink.m_Len;
							}
							readStep = WAIT_HEAD1;
						}
						else//直接一次性取出指定数据
						{
							MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
							                       ((unsigned char *)pBuf + recvDataOffset), \
							                       (DATA_LEN_MAX + 1) << 1, \
							                       (g_RecvFrameLink.m_Len << 1));//取出数据
							ret = PowerUartCheckSum16(0, &g_RecvFrameLink.m_Len, 1);
							ret = PowerUartCheckSum16(ret, pBuf, g_RecvFrameLink.m_Len);
							if(g_RecvFrameLink.m_CheckSum == ret)
							{
								readStep = WAIT_HEAD1;
								return g_RecvFrameLink.m_Len;
							}
							readStep = WAIT_HEAD1;
						}
					}
					else//数据不足,暂时取出缓存
					{
						ret = MyQueue_Fifo8_getArray(&g_PowerUartRecvFifo8, \
						                             ((unsigned char *)pBuf + recvDataOffset), \
						                             (DATA_LEN_MAX + 1) << 1, \
						                             ret);//取出数据
					}
				}
				ret = 0;
				break;
			}
			default:
				break;
		}
	}
	return ret;
}
