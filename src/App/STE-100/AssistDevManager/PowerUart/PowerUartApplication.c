/*
* Copyright(c)2018,成都泰盟软件有限公司
* All rights reserved.
*
* 文件名称:PowerUartApplication.c
* 文件标识:
* 摘要: 主要完成电源串口应用数据的获取以及发送函数功能
*/

/*当前版本:1.0.0
*作者:廖建伟,修改日期:2021.09.15
*/

#include "PowerUartApplication.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
///////////////////////////1.宏定义以及枚举/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///////////////////////////2.全局变量的定义/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
PowerUartApplicationDataStruct g_AppRecvFrame;//应用接收帧
PowerUartApplicationDataStruct g_AppSendFrame;//应用发送帧

////////////////////////////////////////////////////////////////////////////////
///////////////////////////3.函数声明///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///////////////////////////4.函数定义///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*************************************************
Function:       uint16_t PowerUartWriteApplicationDataSegment(uint16_t cmdType, uint16_t *pBuf, uint16_t destLen)
Description:    写入应用数据段
Calls:
Called By:
Input:          cmdType:命令类型
                pBuf:数据参数存储地址
                destLen:数据参数长度U16个数
Output:         无
Return:         0填入失败,失败因为FIFO满或者输入长度不合法,>0成功
Others:
*************************************************/
unsigned short PowerUartWriteApplicationDataSegment(unsigned short cmdType, unsigned short *pBuf, unsigned short destLen)
{
	unsigned short ret = 0;
	
	memcpy(&g_AppSendFrame.m_CmdType, &cmdType, sizeof(g_AppSendFrame.m_CmdType));	
	if(destLen)//存在命令参数
	{
		memcpy(&g_AppSendFrame.m_Data, pBuf, destLen << 1);	
	}

	ret = PowerUartWriteDataToFrameLink((unsigned short *)&g_AppSendFrame, (destLen + 1));
	
	return ret;
}

/*************************************************
Function:       uint16_t PowerUartReadApplicationDataSegment(uint16_t *pCmdType, uint16_t *pBuf)
Description:    读取应用数据段
Calls:
Called By:
Input:          pCmdType:命令存储地址
                pBuf:数据参数存储地址
Output:         无
Return:         命令加数据总长度U16个数
Others:
*************************************************/
unsigned short PowerUartReadApplicationDataSegment(unsigned short *pCmdType, unsigned short *pBuf)
{
	unsigned short ret = 0;
	
	ret = PowerUartReadLinkFrameData((unsigned short *)&g_AppRecvFrame, DATA_LEN_MAX + 1);
	if(ret)//解析成功一帧数据,返回的是数据U16长度
	{
		memcpy(pCmdType, &g_AppRecvFrame.m_CmdType, sizeof(g_AppRecvFrame.m_CmdType));//复制帧类型
		if(ret > 1)//说明命令后面包含数据
		{
			memcpy(pBuf, &g_AppRecvFrame.m_Data, (ret - 1) << 1);//复制帧类型
		}
	}
	return ret;
}

/*************************************************
Function:       void PowerUartApplicationInit(void)
Description:    读取应用数据段
Calls:
Called By:
Input:          无
Output:         无
Return:         无
Others:
*************************************************/
void PowerUartApplicationInit(void)
{
	PoweUartFifoInit();
}
