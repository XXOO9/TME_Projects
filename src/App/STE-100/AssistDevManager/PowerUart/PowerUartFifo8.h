#ifndef __PowerUartfifo8__h__
#define __PowerUartfifo8__h__

/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:PowerUartFifo8.h
* 文件标识:
* 摘要: 该文件主要数据的缓存发送接收
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2018.05.04
*/

#define  POWER_UART_FIFO_BUFSIZE   256

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    unsigned char *pBase;
    unsigned short front;
    unsigned short rear;
    unsigned short maxsize; //循环队列最大存储空间
    unsigned short maxSizeBoundary;  // maxize-1;
}QueueFifo8Struct;

extern QueueFifo8Struct g_PowerUartSendFifo8, g_PowerUartRecvFifo8;

void PoweUartFifoInit(void);
void MyQueue_Fifo8_init(QueueFifo8Struct *Q, unsigned short maxsize, unsigned char *buf);
unsigned short MyQueue_Fifo8_putArray(QueueFifo8Struct *Q, unsigned char *buf, unsigned short len);
unsigned short MyQueue_Fifo8_getArray(QueueFifo8Struct *Q, unsigned char *pData, unsigned short bufLen, unsigned short destLen);
unsigned short MyQueue_Fifo8_UsedCount(QueueFifo8Struct *Q);
unsigned short MyQueue_Fifo8_UnusedCount(QueueFifo8Struct *Q);

#ifdef __cplusplus
}
#endif

#endif
