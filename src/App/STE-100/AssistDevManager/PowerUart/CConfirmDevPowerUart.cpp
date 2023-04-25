#include "CConfirmDevPowerUart.h"
#include "AssistDevManager/PowerUart/PowerUartBaseDefine.h"

CConfirmDevPowerUart::CConfirmDevPowerUart()
{

}

void CConfirmDevPowerUart::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    if(pData == nullptr || nDataLen < 2)
        return;

    //成功使用解析函数解析
    m_bSendCommandSucess = true;
}
