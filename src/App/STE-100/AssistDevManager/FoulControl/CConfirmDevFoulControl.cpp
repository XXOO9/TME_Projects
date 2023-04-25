#include "CConfirmDevFoulControl.h"
#include "AssistDevManager/PowerUart/PowerUartBaseDefine.h"

CConfirmDevFoulControl::CConfirmDevFoulControl()
{

}

void CConfirmDevFoulControl::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    if(pData == nullptr || nDataLen < 2)
        return;

    //成功使用解析函数解析
    m_bSendCommandSucess = true;
}
