///////////////////////////////////////////////////////////
//  CIdentifyUser.cpp
//  Implementation of the Class CIdentifyUser
//  Created on:      05-1月-2022 17:28:41
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CIdentifyUser.h"
#include "CDatabaseManage.h"
#include "CLogger.h"


std::atomic<bool> CIdentifyUser::s_bFindingFace(false);

CIdentifyUser::CIdentifyUser() : m_pRequestMgr( nullptr )
{
    //注册人脸识别信号响应
    qRegisterMetaType<string>("string");
    m_pRequestMgr = CHttpRequestMgr::getInstance();
    connect(&m_objFaceRecognition, &CFaceRecognition::signalFindFaceFeature, this, &CIdentifyUser::onFindFaceFeature, Qt::BlockingQueuedConnection);

    //绑定心跳是否发送成功
    connect( m_pRequestMgr, &CHttpRequestMgr::sigHeartBeatFinished, this, &CIdentifyUser::onServerConnectState );

    m_setFindUser.insert(&m_objFindFaceFeature);
    m_setFindUser.insert(&m_objFindMagCard);
    m_setFindUser.insert(&m_objFindStudentID);

    qRegisterMetaType<CUserInfo>( "CUserInfo" );
    for(auto &element : m_setFindUser)
    {
        connect(element, &CFindUserBasic::signalFindSucess,
                this, &CIdentifyUser::onIdentifySuccess, Qt::BlockingQueuedConnection);

        connect(element, &CFindUserBasic::signalFindFail,
                this, &CIdentifyUser::onIdentifyFalse, Qt::BlockingQueuedConnection);
    }

    //当前识别人脸状态变更
    connect(&m_objFindFaceFeature, &CFindUserByFaceFeature::signalFindFaceState, this, &CIdentifyUser::onFindFaceStateChange, Qt::DirectConnection);
}

CIdentifyUser::~CIdentifyUser()
{
    stop();
}

void CIdentifyUser::start()
{
    if(m_bIdentityState && m_objFindFaceFeature.isAvailable() && m_objFindMagCard.isAvailable() && m_objFindStudentID.isAvailable())
        return;

    m_bIdentityState = true;

    //开启查找所需要的所有设备
    LOGINFO("startIdentity");

    m_objFindFaceFeature.startFind();
    m_objFindMagCard.startFind();
    m_objFindStudentID.startFind();

    m_objFaceRecognition.startFaceRecognition();
}

void CIdentifyUser::stop()
{
    //外部需要进行注销，同时内部标识为停止识别
    m_objFaceRecognition.stopFaceRecognition();

    //关闭识别线程
    m_objFindFaceFeature.stopFind();
    m_objFindMagCard.stopFind();
    m_objFindStudentID.stopFind();

    m_bIdentityState = false;
}

void CIdentifyUser::pause()
{
    m_bIdentityState = false;
    m_objFaceRecognition.pauseFaceRecognition();
}

void CIdentifyUser::resum()
{
    m_bIdentityState = true;

    if(m_nIdentifyType & CFindUserBasic::IDENTIFY_BY_FACE_FEATURE)
        m_objFaceRecognition.resumeFaceRecognition();
}

void CIdentifyUser::setFindSrcFaceFeature(const QString &strFeature)
{
    if(!m_bIdentityState)
        return;

    CUserInfo userInfo;
    userInfo.m_strFaceFeature = strFeature;

    procPIV(userInfo);
}

void CIdentifyUser::setFindSrcMagneticCard(const QString &strSrc)
{
    if(!m_bIdentityState)
        return;

    CUserInfo userInfo;
    userInfo.m_strCampusMagcard = strSrc;

    procPIV(userInfo);
}

void CIdentifyUser::setFindSrcStudentID(const QString &strSrc)
{
    if(!m_bIdentityState)
        return;

    if(strSrc.length() == 0)
        return;

    CUserInfo user;
    user.m_strStudentId = strSrc;

    procPIV(user);
}

void CIdentifyUser::setIdentifyType(int nIdentifyType)
{
    if(nIdentifyType == m_nIdentifyType)
    {
        return;
    }

    m_nLastIdentifyType = m_nIdentifyType;
    m_nIdentifyType = nIdentifyType;

    if((m_nIdentifyType & CFindUserBasic::IDENTIFY_BY_FACE_FEATURE) != CFindUserBasic::IDENTIFY_BY_FACE_FEATURE)
    {
        m_objFaceRecognition.pauseFaceRecognition();
    }
    else
    {
        m_objFaceRecognition.resumeFaceRecognition();
    }
}

void CIdentifyUser::resumIdentifyType()
{
    setIdentifyType(m_nLastIdentifyType);
}

CFaceRecognition *CIdentifyUser::getFeaceRecongition()
{
    return &m_objFaceRecognition;
}

void CIdentifyUser::reloadFaceFeature()
{
    m_objFindFaceFeature.reloadFaceFeature();
}

void CIdentifyUser::onFindCardID(string strCardID)
{
    if(!m_bIdentityState)
        return;

    setFindSrcMagneticCard(QString::fromStdString(strCardID));
}

void CIdentifyUser::onFindFaceFeature(QString strFeature)
{
    if(!m_bIdentityState)
        return;

    setFindSrcFaceFeature(strFeature);
}

void CIdentifyUser::onFindStudentID(QString strUserID)
{
    if(!m_bIdentityState)
        return;

    setFindSrcStudentID(strUserID);
}

void CIdentifyUser::onServerConnectState(bool bConnect)
{
    m_objFindMagCard.onServerConnectState(bConnect);
    m_objFindStudentID.onServerConnectState(bConnect);
    m_objFindFaceFeature.onServerConnectState(bConnect);
}

void CIdentifyUser::onIdentifySuccess(CUserInfo userInfo, void *pLookUpBasic)
{
    if(pLookUpBasic == nullptr)
        return;

    CFindUserBasic *pTemp = reinterpret_cast<CFindUserBasic *>(pLookUpBasic);
    int shIdentifyType = pTemp->getIdentifyType();
    if((shIdentifyType & m_nIdentifyType) == 0)
        return;

//    qDebug()<<"pTemp->shIdentifyType();"<<pTemp->shIdentifyType();
//    //场景是，若有3种识别方式同时识别，其中一种成功，将另外两种正在识别的设置为无效
//    //实际使用时确定逻辑是否有用若无用需要删除处理逻辑
//    for(auto &element : m_setFindUser)
//    {
//        //识别方式不可用可以表示粗略为正在识别
//        if(!element->isAvailable() && element != pLookUpBasic)
//        {
//            element->setCurFindValid(false);
//        }
//    }
    if(!m_bIdentityState)
        return;

    if(shIdentifyType == CFindUserBasic::IDENTIFY_BY_FACE_FEATURE)
    {
        LOGINFO("face### " + to_string(QDateTime::currentMSecsSinceEpoch() - CFaceRecognition::getStartTime()));
    }


    emit signalIdentifySucess(userInfo);
}

void CIdentifyUser::onIdentifyFalse(void *pLookUpBasic)
{
    if(pLookUpBasic == nullptr)
        return;

    CFindUserBasic *pTemp = reinterpret_cast<CFindUserBasic *>(pLookUpBasic);
    int shIdentifyType = pTemp->getIdentifyType();
    if((shIdentifyType & m_nIdentifyType) == 0)
        return;

    if(!m_bIdentityState)
        return;

    if(shIdentifyType == CFindUserBasic::IDENTIFY_BY_FACE_FEATURE)
    {
        LOGINFO("face### " + to_string(QDateTime::currentMSecsSinceEpoch() - CFaceRecognition::getStartTime()));
    }

    //识别失败
    emit signalIdentifyFalse();
}

void CIdentifyUser::procPIV(CUserInfo &userInfo)
{
    if(!m_bIdentityState)
        return;

    CFindUserBasic *pFindBasic = nullptr;
    bool bFind = false;

    if (userInfo.m_strFaceFeature.length() != 0)
    {
        bFind = ((m_nIdentifyType & CFindUserBasic::IDENTIFY_BY_FACE_FEATURE) == CFindUserBasic::IDENTIFY_BY_FACE_FEATURE);
        pFindBasic = &m_objFindFaceFeature;
    }
    else if (!userInfo.m_strCampusMagcard.isEmpty())
    {
        bFind = ((m_nIdentifyType & CFindUserBasic::IDENTIFY_BY_MAG_CARD) == CFindUserBasic::IDENTIFY_BY_MAG_CARD);
        pFindBasic = &m_objFindMagCard;
    }
    else if (!userInfo.m_strStudentId.isEmpty())
    {
        bFind = ((m_nIdentifyType & CFindUserBasic::IDENTIFY_BY_USER_ID)) == CFindUserBasic::IDENTIFY_BY_USER_ID;
        pFindBasic = &m_objFindStudentID;
    }

    if(pFindBasic == nullptr || !bFind)
        return;

    pFindBasic->setFindUserInfo(userInfo);
}
