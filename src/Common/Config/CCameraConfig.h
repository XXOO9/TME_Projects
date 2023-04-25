#ifndef CCAMERACONFIG_H
#define CCAMERACONFIG_H

#include <string>

#include "CAbstractConfigInfo.h"

class COMMONSHARED_EXPORT CCameraConfig : public CAbstractConfigInfo
{
public:
    enum CAMERA_TYPE{ FACERECOGNITiON = 0, PHOTOGRAPH};
    CCameraConfig();

   bool Check_Config_Table();

   string getCameraInfoByType(int iType);

};

#endif // CCAMERACONFIG_H
