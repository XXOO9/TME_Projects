#include "CFindUserByMagneticCard.h"
#include "CDatabaseManage.h"

#include <CHttpRequestMgr.h>

CFindUserByMagneticCard::CFindUserByMagneticCard()
{
    m_nIdentifyType = IDENTIFY_BY_MAG_CARD;
}

void CFindUserByMagneticCard::setFindUserInfo(const CUserInfo &userInfo)
{
    if(userInfo.m_strCampusMagcard.isEmpty())
        return;

    CFindUserBasic::setFindUserInfo(userInfo);
}

void CFindUserByMagneticCard::findOnLocal(CUserInfo &findCondition, vector<CUserInfo> &vecFindRes)
{
    vecFindRes.clear();

    CTableUserIdentityInfo *pTableUserInfo = CDatabaseManage::GetInstance().getTableUserIdentityInfo();
    if(pTableUserInfo != nullptr)
    {
        pTableUserInfo->getUserInfo(findCondition, vecFindRes);
    }
}

void CFindUserByMagneticCard::findOnServer(CUserInfo &findCondition)
{
    //发送到服务器     2
//    CHttpRequestMgr::getInstance()->userIdentify(findCondition.m_strFaceFeature, TMEc_Soft, findCondition.m_strCampusMagcard);
    userIndentify( findCondition.m_strFaceFeature, findCondition.m_strCampusMagcard,  TMEc_Soft );
}
