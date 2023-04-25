#include "CDataParsePowerUart.h"

#include "AssistDevManager/PowerUart/PowerUartFifo8.h"
#include "AssistDevManager/PowerUart/PowerUartApplication.h"

CDataParsePowerUart::CDataParsePowerUart()
{

}

void CDataParsePowerUart::clearDataCache()
{
    CDataParseBase::clearDataCache();
    g_PowerUartRecvFifo8.rear = 0;
    g_PowerUartRecvFifo8.front = 0;
    m_bHaveBeenData = false;
}

void CDataParsePowerUart::dataParse(unsigned int iLen, void *data)
{
    if(iLen == 0 || data == nullptr)
        return;

    m_bHaveBeenData = true;

    MyQueue_Fifo8_putArray(&g_PowerUartRecvFifo8, reinterpret_cast<unsigned char *>(data), static_cast<unsigned short>(iLen));

    unsigned short shCmd;
    //电源管理解析出来的命令在 m_arrCmd中  shCmdAndParamNum的数量为 命令（1） + 参数个数
    //这里直接将命令放在 索引1所在位置，将命令放在0当中
    unsigned short shCmdAndParamNum = PowerUartReadApplicationDataSegment(&shCmd, &m_arrCmd[1]);
    if(shCmdAndParamNum > 0 && m_pFunc != nullptr)
    {
        m_arrCmd[0] = shCmd;
        //一个short大小为2，左移一位就是 * 2
        m_pFunc(m_arrCmd, static_cast<unsigned int>(shCmdAndParamNum << 1));
    }
}
