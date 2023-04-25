#include "CFindUserByFaceFeature.h"
#include "CFaceRecognition.h"
#include "CLogger.h"
#include "CFilePath.h"
#include "CDatabaseManage.h"
#include "CInterAction.h"

#include <fstream>
#include <QJsonArray>
#include <omp.h>

#include "../../Common/HttpClient/CHttpUserIdentify.h"

CFindUserByFaceFeature::CFindUserByFaceFeature() : CFindUserBasic ()
{
    m_nIdentifyType = IDENTIFY_BY_FACE_FEATURE;
    loadFaceConfidence(m_fFaceConfidence);
}

CFindUserByFaceFeature::~CFindUserByFaceFeature()
{

}

void CFindUserByFaceFeature::setFindUserInfo(const CUserInfo &userInfo)
{
    if(userInfo.m_strFaceFeature.length() == 0)
    {
        return;
    }

    CFindUserBasic::setFindUserInfo(userInfo);
}

/*************************************************
  <函数名称>    loadFeature
  <功能描述>    加载本地数据存储文件中的用户信息
  <参数说明>

  <返 回 值>

  <函数说明>    该方法不能在软件启动或对象创建的构造函数中加载
               根据软件逻辑，需要先与服务器确定是否需要重新
               同步用户数据，然后确定在什么时间加载人脸特征

  <作    者>   zhy
  <时    间>   2022-03-20
  **************************************************/
bool CFindUserByFaceFeature::loadFeature()
{
    CInterAction &ui = CInterAction::Get_Instance();

    //获取用户数据表格
    CTableUserIdentityInfo *pUserInfo = CDatabaseManage::GetInstance().getTableUserIdentityInfo();
    if(pUserInfo == nullptr)
        return false;

    //清除已经加载好的人脸特征集合
    m_vFrozenFeatures.clear();

    CUserInfo userInfoTemp;
    vector<UserIdentityInfo> vecInfo;

    if(!(pUserInfo->getUserInfo(userInfoTemp, vecInfo,true)))
        return false;

    ui.setViewErorrString(MINFO_LOAD_FACE_FEATURE, true);

    //数据库当中存储的是十六进制编码的string数据,是预定长度的两倍
    unsigned int nHexSize = faceFeatureProperty::featureLen * 2;
    QByteArray arrHex;
    arrHex.resize(static_cast<int>(nHexSize));

    QByteArray arrBinary;
    unsigned int nFeatureLen = faceFeatureProperty::featureLen;
    arrBinary.resize(static_cast<int>(nFeatureLen));
    UserFaceFeature userFeature;
    string strTemp;

    for(auto &element : vecInfo)
    {
        //只有有人脸特征的用户才用去加载
        strTemp = element.faceFeature.value();
        if(!strTemp.empty() && strTemp.length() == nHexSize)
        {
            userFeature.lUserID = element.codeUniqueID;
            memcpy(arrHex.data(), strTemp.c_str(), nHexSize);
            arrBinary = QByteArray::fromHex(arrHex);
            memcpy(userFeature.fFeatures, arrBinary.data(), nFeatureLen);

            m_vFrozenFeatures.push_back(userFeature);
        }
    }

    ui.setViewErorrString(MINFO_LOAD_FACE_FEATURE, false);

    return m_vFrozenFeatures.size() > 0;
}

/*************************************************
  <函数名称>    loadFaceConfidence
  <功能描述>    加载本地配置数据文件中的人脸匹配可信度
  <参数说明>

  <返 回 值>

  <函数说明>

  <作    者>   zhy
  <时    间>   2022-03-20
  **************************************************/
void CFindUserByFaceFeature::loadFaceConfidence(float &fConfidence)
{
    fConfidence = CCommonCfg::GetInstance().getFaceConfidence();
}

/*************************************************
  <函数名称>    calculateConfidenceScore(QString &strHotFeature, QString &strFrozenFeature)
  <功能描述>    计算两个人脸匹配可信度
  <参数说明>    QString &strHotFeature 实时识别到的人脸特征
               QString &strFrozenFeature 已经存储的人脸特征

  <返 回 值>

  <函数说明>    计算方法来自于SeetaFace6官方文档，不同的人脸识别库，存在计算方法不一致

  <作    者>   zhy
  <时    间>   2022-03-20
  **************************************************/
float CFindUserByFaceFeature::calculateConfidenceScore(float *fHotFeature, float *fFrozenFeature, int featureSize)
{
    float fRet = 0.0;

    if(fHotFeature != nullptr && fFrozenFeature != nullptr)
    {
        for (int i = 0; i < featureSize; i++)
        {
            fRet += fHotFeature[i] * fFrozenFeature[i];
        }
    }

    return fRet;
}


void CFindUserByFaceFeature::setUpdateFaceFeature(bool bUpdate)
{
    //将变量更新为false就可进行重新加载
    m_bLoadFaceFeature = !bUpdate;
}

void CFindUserByFaceFeature::startFind()
{
    //开启之后运行一次线程，用于加载特征值
    CFindUserBasic::startFind();

    m_semaphoreThread.release();
}

void CFindUserByFaceFeature::reloadFaceFeature()
{
    m_bLoadFaceFeature = false;
    m_semaphoreThread.release();
}

void CFindUserByFaceFeature::dealThreadLogic()
{
    if(m_bLoadFaceFeature)
    {
        if(m_objUserInfo.m_strFaceFeature.length() < 1024)
            return;
    }


    emit signalFindFaceState(true);

    LOGINFO("find_FaceFeature");
    vector<CUserInfo> vecUserInfo;
    if(!m_bLoadFaceFeature)
    {
        //开始加载特征值
        m_bLoadFaceFeature = loadFeature();
        m_bLoadFaceFeature = true;
    }
    else
    {
        //继续进行父类的查找逻辑
        CFindUserBasic::dealThreadLogic();
    }

    emit signalFindFaceState(false);
}

/*************************************************
  <函数名称>    findOnLocal(CUserInfo &findCondition, vector<CUserInfo> &vecFindRes)
  <功能描述>    利用人脸特征值在本地查找用户信息
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    不在该方法中等待服务器查找结束，而在服务器返回信号响应槽函数中
               判定是否已经返回

  <作    者>   zhy
  <时    间>   2022-03-20
  **************************************************/
void CFindUserByFaceFeature::findOnLocal(CUserInfo &findCondition, vector<CUserInfo> &vecFindRes)
{
    if(findCondition.m_strFaceFeature.length() == 0)
    {
        return;
    }

    vecFindRes.clear();

    long long userID = 0;

    double startTime = clock();

    QByteArray hotArray = QByteArray::fromHex(findCondition.m_strFaceFeature.toLocal8Bit());

    float hotFeature[FEATURE_SIZE];
    memcpy(hotFeature, hotArray.data(), static_cast<unsigned int>(hotArray.size()));

    unsigned int iSize = static_cast<unsigned int>(m_vFrozenFeatures.size());

    float fTempScore, fMaxScore = 0.0f;

    for (unsigned int i = 0; i < iSize; i++)
    {
        fTempScore = calculateConfidenceScore(hotFeature, m_vFrozenFeatures[i].fFeatures, FEATURE_SIZE);
        if(fTempScore > fMaxScore)
        {
            userID = m_vFrozenFeatures[i].lUserID;
            fMaxScore = fTempScore;
        }
    }

    LOGINFO("face score : " + to_string(fMaxScore) + " userID = " + to_string(userID));

    if(fMaxScore < m_fFaceConfidence)
    {
        userID = 0;
    }

    double endTime = clock();

    qDebug() << "CPU: " << omp_get_num_procs();
    qDebug() << "Time interval: " << endTime - startTime;
    QString strLog;
    strLog = QString("findOnLocal time consume:") + QString::number(endTime - startTime);
    LOGINFO(strLog.toStdString());

    if(userID == 0)
        return;

    //根据ID值查询学生信息
    CUserInfo findInfo;
    findInfo.m_llUniqueID = userID;//element.first;

    CTableUserIdentityInfo *pTableUserInfo = CDatabaseManage::GetInstance().getTableUserIdentityInfo();
    if(pTableUserInfo != nullptr)
    {
        pTableUserInfo->getUserInfo(findInfo, vecFindRes);
    }

    if(vecFindRes.size() > 0)
    {
        LOGINFO("findOnLocal name:" + vecFindRes.at(0).m_strName.toStdString());
    }
    else
    {
        LOGERROR("findOnLocal failly");
    }
}

/*************************************************
  <函数名称>    findOnServer(CUserInfo &findCondition)
  <功能描述>    利用人脸特征值在服务器上查找用户信息
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    不在该方法中等待服务器查找结束，而在服务器返回信号响应槽函数中
               判定是否已经返回

  <作    者>   zhy
  <时    间>   2022-03-20
  **************************************************/
void CFindUserByFaceFeature::findOnServer(CUserInfo &findCondition)
{
    //重置标志位，等待服务器的返回
    m_isHttpReply = false;

    userIndentify( findCondition.m_strFaceFeature, "", TMEc_Soft );

    QString strLog = "findOnServer bytes:" + QString::number( findCondition.m_strFaceFeature.size());
    LOGINFO(strLog.toStdString());
}
