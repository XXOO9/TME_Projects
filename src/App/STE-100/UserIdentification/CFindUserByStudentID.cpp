#include "CFindUserByStudentID.h"
#include "CDatabaseManage.h"

CFindUserByStudentID::CFindUserByStudentID()
{
    m_nIdentifyType = IDENTIFY_BY_USER_ID;
}

void CFindUserByStudentID::setFindUserInfo(const CUserInfo &userInfo)
{
    if(userInfo.m_strStudentId.length() == 0)
        return;

    CFindUserBasic::setFindUserInfo(userInfo);
}

void CFindUserByStudentID::findOnLocal(CUserInfo &findCondition, vector<CUserInfo> &vecFindRes)
{
    vecFindRes.clear();

    CTableUserIdentityInfo *pTableUserInfo = CDatabaseManage::GetInstance().getTableUserIdentityInfo();
    if(pTableUserInfo != nullptr)
    {
        pTableUserInfo->getUserInfo(findCondition, vecFindRes);
    }
}

void CFindUserByStudentID::findOnServer(CUserInfo &findCondition)
{
    if(findCondition.isTouristInfo())
        return;

//    userIdentify( findCondition.m_strFaceFeature, TMEc_Soft, findCondition.m_strCampusMagcard, findCondition.m_strStudentId );
    userIndentify( findCondition.m_strFaceFeature, findCondition.m_strCampusMagcard, TMEc_Soft, findCondition.m_strStudentId );
}

void CFindUserByStudentID::dealThreadLogic()
{

    //CFindUserBasic::dealThreadLogic();
    initServer();

    m_vecFindRes.clear();

    //优先在本地识别用户信息
    findOnLocal(m_objUserInfo, m_vecFindRes);

    if(m_vecFindRes.size() > 0)
    {
        LOGINFO("Mode is : StudentID Local, Name is : " + m_vecFindRes[0].m_strName.toStdString() +
                " Gender is : " + to_string(m_vecFindRes[0].m_bGender));

        emit signalFindSucess(m_vecFindRes.at(0), this);
    }
    else
    {
        emit signalFindFail(this);
    }
}
