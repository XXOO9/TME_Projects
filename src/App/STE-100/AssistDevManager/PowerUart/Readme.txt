驱动函数使用说明

①串口初始化
波特率9600
停止位1位,
数据位8位
无奇偶校验

②函数调用
驱动函数在PowerUartApplication.h文件中,如下
void PowerUartApplicationInit(void);在串口初始化时一起初始化
unsigned short PowerUartWriteApplicationDataSegment(unsigned short cmdType, unsigned short *pBuf, unsigned short destLen);向下位机发送命令数据,具体使用见函数实体。
unsigned short PowerUartReadApplicationDataSegment(unsigned short *pCmdType, unsigned short *pBuf);读取下位机命令数据,需要周期调用。具体使用见函数实体。

③上位机需要编写驱动
串口发送函数,需要一直调用
功能：读取发送FIFO8中的数据,发送至底层串口(读取FIFO函数MyQueue_Fifo8_getArray)
      例子：
	  {
    	  unsigned short ret = MyQueue_Fifo8_getArray(&g_PowerUartSendFifo8, pBuf, POWER_UART_FIFO_BUFSIZE, POWER_UART_FIFO_BUFSIZE);
		  //上位机发送数据到串口,长度为ret
	  }
串口接收函数
功能：接收到底层串口数据,填充至接收FIFO8
      例子：
	  {
	      //上位机读取到串口数据,长度为len
    	  unsigned short ret = MyQueue_Fifo8_putArray(&g_PowerUartRecvFifo8, pBuf, len);
	  }
④测试指令

/* 心跳指令 */
FC FC 02 80 01 00 01 80

/* 设置平板启动充电指令 */
FC FC 05 80 02 00 02 80 01 00

/* 设置平板启动停止指令 */
FC FC 04 80 02 00 02 80 00 00

/* 设置灯带模式呼吸指令 */
FC FC 06 80 02 00 04 80 00 00

/* 设置灯带模式循环指令 */
FC FC 07 80 02 00 04 80 01 00

