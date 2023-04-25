#ifndef SOFTMACHINEMODE_H
#define SOFTMACHINEMODE_H
#include <QMutex>

class CSoftMachineMode
{
public:
    //未知模式 主机模式 仆从模式
    enum emSoftMachineModel{
        Mode_Unknow, Mode_Master_Machine, Mode_Servant_Machine
    };
private:
    CSoftMachineMode();
    ~CSoftMachineMode();
    CSoftMachineMode &operator = (const CSoftMachineMode &machineMode);

    static CSoftMachineMode				*s_pManagerInstance;
    static QMutex				s_csManager_Instance_Safe;

    short  m_shSoftMachineMode;
public:
    static CSoftMachineMode * getInstance();
    static void releaseInstance();

    void setMode(short shMode);
    short getMode();
};

#endif // SOFTMACHINEMODE_H
