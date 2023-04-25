#ifndef CUSBCOM_H
#define CUSBCOM_H


#include <QMutex>
#include "ICommunicate.h"
#include "CSerialPortCom.h"

class CUSBCom : public ICommunicate
{
public:
    CUSBCom();
};

#endif // CUSBCOM_H
