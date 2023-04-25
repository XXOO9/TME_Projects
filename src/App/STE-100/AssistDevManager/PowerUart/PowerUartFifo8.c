/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:PowerUartFifo8.c
* 文件标识:
* 摘要: 主要数据的缓存发送接收
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2018.05.04
*/

#include "PowerUartFifo8.h"

////////////////////////////////////////////////////////////////////////////////
///////////////////////////1.宏定义以及枚举/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///////////////////////////2.全局变量的定义/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
QueueFifo8Struct g_PowerUartSendFifo8;
QueueFifo8Struct g_PowerUartRecvFifo8;
unsigned char g_PowerUartSendFifo8Buf[POWER_UART_FIFO_BUFSIZE];//FIFO的发送缓存区
unsigned char g_PowerUartRecvFifo8Buf[POWER_UART_FIFO_BUFSIZE];//FIFO的接收缓存区

////////////////////////////////////////////////////////////////////////////////
///////////////////////////3.函数声明///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///////////////////////////4.函数定义///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*************************************************
Function:       void PoweUartFifoInit(void)
Description:    电源串口初始化
Calls:          无
Called By:     	无
Input:          无
Output:         无
Return:         无
Others:
*************************************************/
void PoweUartFifoInit(void)
{
	MyQueue_Fifo8_init(&g_PowerUartSendFifo8, POWER_UART_FIFO_BUFSIZE, g_PowerUartSendFifo8Buf);
	MyQueue_Fifo8_init(&g_PowerUartRecvFifo8, POWER_UART_FIFO_BUFSIZE, g_PowerUartRecvFifo8Buf);	
}

/*************************************************
Function:       void MyQueue_Fifo8_init(QueueFifo8Struct *Q, unsigned short maxsize, unsigned *buf)
Description:    单个fifo通道的初始化
Calls:          void
Called By:     	void InitFifo8Queue(void)
Input:          Q:fifo指针
								maxsize:fifo存储最大容量,必须是1,2,4,8...2的整数幂
								buf:fifo存储基地址
Output:         无
Return:         无
Others:
*************************************************/
void MyQueue_Fifo8_init(QueueFifo8Struct *Q, unsigned short maxsize, unsigned char *buf)
{
	Q->pBase = buf;
	Q->front = 0;
	Q->rear = 0;
	Q->maxsize = maxsize;
	Q->maxSizeBoundary = maxsize-1;
	return;
}

/*************************************************
Function:       unsigned char MyQueue_Fifo8_put(QueueFifo8Struct *Q, unsigned char val)
Description:    向FIFO 中写入一个数据
Calls:          无
Called By:     	用户
Input:          Q:fifo指针
								val:待写入数据
Output:         无
Return:         1:成功,0:失败
Others:
*************************************************/
unsigned char MyQueue_Fifo8_put(QueueFifo8Struct *Q, unsigned char val)
{
	if(Q->front == ((Q->rear + 1) & Q->maxSizeBoundary))	//fifo满,返回0
		return 0;
	else
	{

		Q->pBase[Q->rear] = val;
		Q->rear = (Q->rear + 1) & Q->maxSizeBoundary;

		return 1;
	}
}

/*************************************************
Function:       unsigned short MyQueue_Fifo8_putArray(QueueFifo8Struct *Q, unsigned char* buf, unsigned short len)
Description:    向FIFO 写入一组数据
Calls:          无
Called By:     	用户
Input:          Q:fifo指针
								len:待写入数据长度
								buf:待写入数据指针
Output:         无
Return:         实际放入数据的数量
Others:
*************************************************/
unsigned short MyQueue_Fifo8_putArray(QueueFifo8Struct *Q, unsigned char* buf, unsigned short len)
{
	unsigned short i;
	unsigned short ret = 0;
	unsigned char *p = buf;
	for(i = len; i; i--)
	{
		ret += MyQueue_Fifo8_put(Q, *p);
		p++;
	}
	return ret;
}

/*************************************************
Function:       uint8_t MyQueue_Fifo8_get(QueueFifo8Struct *Q, unsigned char *pdata)
Description:    从FIFO取出一个数据
Calls:          无
Called By:
Input:          Q:fifo指针
Output:         pdata:取出的数据
Return:         1:成功,0:失败
Others:
*************************************************/
unsigned char MyQueue_Fifo8_get(QueueFifo8Struct *Q, unsigned char *pData)
{
	if(Q->front == Q->rear)	//fifo空
	{
		return 0;
	}
	else
	{
		*pData = Q->pBase[Q->front];
		Q->front = (Q->front + 1) & Q->maxSizeBoundary;
		return 1;
	}
}

/*************************************************
Function:       uint8_t MyQueue_Fifo8_getArray(QueueFifo8Struct *Q, unsigned char *pData, unsigned short bufLen, unsigned short destLen)
Description:    从FIFO取出一段数据
Calls:          
Called By:
Input:          Q:fifo指针,bufLen pdata空间,destLen 目标数据量
Output:         pdata 取出的数据
Return:         0:失败,>0实际取出数据的长度
Others:
*************************************************/
unsigned short MyQueue_Fifo8_getArray(QueueFifo8Struct *Q, unsigned char *pData, unsigned short bufLen, unsigned short destLen)
{
	unsigned short i;
	unsigned short ret = 0;
	unsigned char *p;
	
	p = pData;
	if(destLen > bufLen)
	{
		destLen = bufLen;
	}
	for(i = 0; i < destLen; i++)
	{
		ret += MyQueue_Fifo8_get(Q, p);
		p++;
	}
	return (ret);
}

/*************************************************
Function:       MyQueue_Fifo8_UsedCount( QueueFifo8Struct *Q)
Description:    查询FIFO缓冲区被使用容量
Calls:          无
Called By:
Input:          Q:fifo指针
Output:         无
Return:         已使用的容量值
Others:
*************************************************/
unsigned short MyQueue_Fifo8_UsedCount(QueueFifo8Struct *Q)
{
   return (unsigned short)((Q->maxsize + Q->rear - Q->front) & Q->maxSizeBoundary);
}

/*************************************************
Function:       MyQueue_fifo16_UnusedCount
Description:    查询FIFO缓冲区剩余容量
Calls:          无
Called By:
Input:          Q:fifo指针
Output:         无
Return:         统计剩余空间数量
Others:
*************************************************/
unsigned short MyQueue_Fifo8_UnusedCount(QueueFifo8Struct *Q)
{
   return (unsigned short)(Q->maxsize - MyQueue_Fifo8_UsedCount(Q));
}
