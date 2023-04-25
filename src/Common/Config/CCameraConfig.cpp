#include "CCameraConfig.h"
#include "exception"
#include "Logger/CLogger.h"
CCameraConfig::CCameraConfig()
{

}


bool CCameraConfig::Check_Config_Table()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool bSuccess = true;

    try
    {
        size_t recordCount = select<CameraDeviceConfig>(*m_pDatabase).count();
        if (!recordCount)
        {
            /// 人脸识别摄像机
            CameraDeviceConfig config(*m_pDatabase);

            config.camName       = "OV2680";
            config.camUseType    = CameraDeviceConfig::CamUseType::FaceRecognition;
            config.update();
        }
        if(recordCount < 2)
        {
            /// 拍照摄像机
            CameraDeviceConfig config(*m_pDatabase);

            config.camName       = "USB Camera";
            config.camUseType    = CameraDeviceConfig::CamUseType::PhotoGraph;
            config.update();
        }
    }
    catch (exception)
    {
        bSuccess = false;
    }

    return bSuccess;
}


string CCameraConfig::getCameraInfoByType(int iType)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return "";
    }

    string strCameraName("");
    vector<CameraDeviceConfig> vCameraInfo = select<CameraDeviceConfig>(*m_pDatabase).orderBy(CameraDeviceConfig::Id).all();
    if(vCameraInfo.size() > 0)
    {
        for(auto it = vCameraInfo.begin(); it != vCameraInfo.end(); it++)
        {
            if(it->camUseType != iType)
            {
                continue;
            }
            strCameraName = it->camName;
            break;
        }
    }

    return strCameraName;
}


