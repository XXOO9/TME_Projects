#include "CSoftMachineMode.h"

CSoftMachineMode::CSoftMachineMode()
    : m_shSoftMachineMode(Mode_Unknow)
{

}

CSoftMachineMode::~CSoftMachineMode()
{

}

CSoftMachineMode *CSoftMachineMode::getInstance()
{
    if( s_pManagerInstance == nullptr)
    {
        s_csManager_Instance_Safe.lock();

        if(s_pManagerInstance == nullptr)
        {
            s_pManagerInstance = new CSoftMachineMode();
        }

        s_csManager_Instance_Safe.unlock();
    }

    return s_pManagerInstance;
}

void CSoftMachineMode::releaseInstance()
{
    if(s_pManagerInstance != nullptr)
        delete s_pManagerInstance;
}

void CSoftMachineMode::setMode(short shMode)
{
    m_shSoftMachineMode = shMode;
}

short CSoftMachineMode::getMode()
{
    return m_shSoftMachineMode;
}

QMutex CSoftMachineMode::s_csManager_Instance_Safe;
CSoftMachineMode * CSoftMachineMode::s_pManagerInstance = nullptr;
