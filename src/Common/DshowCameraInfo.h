#ifndef DSHOWCAMER_H
#define DSHOWCAMER_H
#include <vector>
#include <string>
#include "common_global.h"

using namespace std;



class COMMONSHARED_EXPORT CDshowCameraInfo
{

public:
    CDshowCameraInfo();

	//遍历摄像机或者采集卡的名称
    vector<string> & Enumerate_Cameras_Name();

    int Get_Camera_Index(const string& strName);

private:
	//保存列举到的全部设备名称
    vector<string> m_vCameraInfo;
};

#endif
