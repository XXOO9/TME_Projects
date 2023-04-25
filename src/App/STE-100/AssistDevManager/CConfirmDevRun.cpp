#include "CConfirmDevRun.h"

CConfirmDevRun::CConfirmDevRun()
{

}

void CConfirmDevRun::onDataParseSucess(void *pData, unsigned int &nDataLen)
{
    if(pData == nullptr || nDataLen == 0)
        return;

    m_bSendCommandSucess = true;
}
