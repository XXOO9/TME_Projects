#include "CCamerInfoForTest.h"
#include "CDatabaseManage.h"
#include "commondefin.h"
#include "CLogger.h"
using namespace camerType;
//记录实验摄像头信息
CCamerInfoForTest::CCamerInfoForTest()
{

}

//测试项目摄像头索引，进行测试拍照和识别时只会开启其中一个，所以无需担心竟态问题
int CCamerInfoForTest::getTestItem(short shSteDevType, short shUseType)
{
    int nCamerIndex = -1;

    if(shSteDevType == DEV_TYPE_UNKNOW)
        return nCamerIndex;

    vector<string> &vCamera = m_vCamera;
    size_t szCamerSize = vCamera.size();

    //无摄像头直接返回错误值
    if(szCamerSize == 0)
        return nCamerIndex;

    size_t pos;
    //根据详细配置摄像头信息进行判断
    loadCameraCfgFromDataBase();

    //仅有一个摄像头返回0(可能引起程序问题,添加限制为当前摄像头不为数据库中的摄像头)
    if(szCamerSize == 1)
    {
        bool bReturn = true;
        for(auto element : m_mapTargetCamerName)
        {
            if(element.second.find(vCamera[0]) != string::npos)
            {
                bReturn = false;
            }
        }

        if(bReturn)
        {
            return 0;
        }
    }

    string strCameraName;

    auto itFind = m_mapTargetCamerName.find(shUseType);

    if(itFind != m_mapTargetCamerName.end())
    {
        strCameraName = itFind->second;
        for(size_t i = 0; i < szCamerSize; i++)
        {
            pos = vCamera[i].find(strCameraName);

            if(pos != string::npos)
            {
                nCamerIndex = static_cast<int>(i);
                break;
            }
        }
    }

    return nCamerIndex;
}

void CCamerInfoForTest::loadCameraCfgFromDataBase()
{
    if(m_mapTargetCamerName.size() != 0)
        return;

    CCameraConfig *pCamerConfig = CDatabaseManage::GetInstance().getTabCameraInfo();
    if(pCamerConfig != nullptr)
    {
        m_mapTargetCamerName[CCameraConfig::FACERECOGNITiON] = pCamerConfig->getCameraInfoByType(CCameraConfig::FACERECOGNITiON);
        m_mapTargetCamerName[CCameraConfig::PHOTOGRAPH] = pCamerConfig->getCameraInfoByType(CCameraConfig::PHOTOGRAPH);
    }
}

bool CCamerInfoForTest::checkCamerInfo(short shSteDevType, bool bDirectConnect)
{
    bool bRet = false;

    //初始不存在错误
    m_lLastErr = camer_OK;

    //进来一次获取一次摄像头信息
    m_vCamera = m_camerainfo.Enumerate_Cameras_Name();

    switch(shSteDevType)
    {
    //未知设备类型（不需要摄像头设备） 不需要考虑直连和组网
    case DEV_TYPE_UNKNOW:
        m_nRecognition = -1;
        m_nPhotograph = -1;
        m_lLastErr |= camer_Recognition;
        m_lLastErr |= camer_Photograph;
        break;
    //身高体重仪器(仅需要进行识别设备判断) 不需要考虑直连和组网
    case DEV_TYPE_BODY:
    case DEV_TYPE_EYESIGHT:
        m_nRecognition = getTestItem(shSteDevType, CCameraConfig::FACERECOGNITiON);
        m_nPhotograph = -1;
        if(m_nRecognition < 0)
        {
            m_lLastErr |= camer_Recognition;
        }
        break;
    //肺活量仪器(需要进行识别 和 作弊拍照 使用同一个设备) 非直连模式不进行拍照
    case DEV_TYPE_VC:
    case DEV_TYPE_GRIP:
        m_nRecognition = getTestItem(shSteDevType, CCameraConfig::FACERECOGNITiON);
        m_nPhotograph = bDirectConnect ? m_nRecognition : -1;

        if(m_nRecognition < 0)
        {
            m_lLastErr |= camer_Recognition;
            m_lLastErr |= camer_Photograph;
        }

        break;
    //识别 和 作弊拍照 使用不同设备(组网不需要防作弊)
    case DEV_TYPE_SBJ:
    case DEV_TYPE_SFB:
    case DEV_TYPE_PULL_UP:
    case DEV_TYPE_PUO:
    case DEV_TYPE_VJUMP:
    case DEV_TYPE_SIT_UP:
    case DEV_TYPE_PUSH_UP:
        m_nRecognition = getTestItem(shSteDevType, CCameraConfig::FACERECOGNITiON);

        if(m_nRecognition < 0)
        {
            m_lLastErr |= camer_Recognition;
        }

        if(bDirectConnect)
        {
            m_nPhotograph = getTestItem(shSteDevType, CCameraConfig::PHOTOGRAPH);
            if(m_nPhotograph < 0)
            {
                m_lLastErr |= camer_Photograph;
            }
        }
        break;

    //需要进行识别 不需要 防作弊拍照 不需要判断直连组网
    case DEV_TYPE_RUN_SHORT:
    case DEV_TYPE_RUN_MID:
    case DEV_TYPE_ROPE_SKIP:
    case DEV_TYPE_BASKETBALL:
    case DEV_TYPE_FOOTBALL:
    case DEV_TYPE_VOLLEYBALL:
    case DEV_TYPE_SOLIDBALL:
        m_nRecognition = getTestItem(shSteDevType, CCameraConfig::FACERECOGNITiON);
        m_nPhotograph = -1;
        if(m_nRecognition < 0)
        {
            m_lLastErr |= camer_Recognition;
        }
        break;
    //保证识别设备可用即可
    default:
        m_nRecognition = getTestItem(shSteDevType, CCameraConfig::FACERECOGNITiON);
        m_nPhotograph = -1;

        bRet = m_nRecognition > 0;
        if(!bRet)
        {
            m_lLastErr |= camer_Recognition;
        }
        break;
    }

    bRet = m_lLastErr == camer_OK;
    return bRet;
}

void CCamerInfoForTest::cfgCamer(CDevCamera *pRecognition, CDevCamera *pPhotograph)
{
    if(pRecognition != nullptr)
    {
        pRecognition->setCameraIndex(m_nRecognition);
    }

    if(pPhotograph != nullptr)
    {
        pPhotograph->setCameraIndex(m_nPhotograph);
    }
}

vector<long> CCamerInfoForTest::getErrCamer()
{
    return getErrCamer(m_lLastErr);
}

vector<long> CCamerInfoForTest::getErrCamer(const long &lErrCode)
{
    vector<long> vecErr;

    if(lErrCode & camer_Recognition)
    {
        vecErr.push_back(camer_Recognition);
        LOGWARN("camer_Recognition Not Find");
    }

    if(lErrCode & camer_Photograph)
    {
        vecErr.push_back(camer_Photograph);
        LOGWARN("camer_Photograph Not Find");
    }

    return vecErr;
}

const long &CCamerInfoForTest::getErrCode()
{
    return m_lLastErr;
}
