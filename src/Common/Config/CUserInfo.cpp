///////////////////////////////////////////////////////////
//  CStudentIdentityInfo.cpp
//  Implementation of the Class CStudentIdentityInfo
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CUserInfo.h"
#include <map>
#include <list>
#include <QDateTime>

CGrade CUserInfo::s_grade;

QDate CUserInfo::s_dateCur(QDate::currentDate());

CUserInfo::CUserInfo()
{

}

CUserInfo::~CUserInfo()
{

}

void CUserInfo::reset()
{
    m_llUniqueID        = TOURIST_MODE_ID;
    m_nRoleType         = SYSTEM_USER_STUDENT;
    m_strName           = "";
    m_bGender           = UserIdentityInfo::GenderType::FEMALE;
    m_strStudentId      = "";
    m_strIdCardNo       = "";
    m_strIdMagCard      = "";
    m_nGrade            = -1;
    m_nClass            = -1;
    m_strNation         = "";
    m_strFaceFeature.clear();
    m_strFaceImagePath  = "";
    m_bUseGenderToSelect= false;
    m_nStage            = 0;
}

vector<short> CUserInfo::getQueryItem(const CUserInfo &info)
{
    vector<short> vecType;
    if(info.m_llUniqueID != TOURIST_MODE_ID)
        vecType.push_back(emQuery_Condition::Query_Type_UniqueID);

    if(!info.m_strIdCardNo.isEmpty())
        vecType.push_back(emQuery_Condition::Query_Type_IdCardNo);

    if(!info.m_strStudentId.isEmpty())
        vecType.push_back(emQuery_Condition::Query_Type_StudentId);

    if(!info.m_strCampusMagcard.isEmpty())
        vecType.push_back(emQuery_Condition::Query_Type_CampusMagcard);

    //以上为精准查询需要的一些，只要具备其中一个就能进行查找
    if(vecType.size() > 0)
        return vecType;

    if(info.m_nRoleType >= SYSTEM_USER_STUDENT && info.m_nRoleType <= SYSTEM_USER_ADMIN)
        vecType.push_back(emQuery_Condition::Query_Type_RoleType);
        
    if(!info.m_strName.isEmpty())
        vecType.push_back(emQuery_Condition::Query_Type_Name);

    if(info.m_bUseGenderToSelect)
        vecType.push_back(emQuery_Condition::Query_Type_Gender);

    if(info.m_nGrade >= 0)
        vecType.push_back(emQuery_Condition::Query_Type_Grade);

    if(info.m_nClass >= 0)
        vecType.push_back(emQuery_Condition::Query_Type_Class);

    if(!info.m_strNation.isEmpty())
        vecType.push_back(emQuery_Condition::Query_Type_Nation);

    return vecType;
}

CUserInfo &CUserInfo::operator=(const CUserInfo &pt)
{
    m_llUniqueID            = pt.m_llUniqueID;
    m_nRoleType             = pt.m_nRoleType;
    m_strBrithday           = pt.m_strBrithday;
    longlongTimeStamp       = pt.longlongTimeStamp;
    m_strName               = pt.m_strName;
    m_bGender               = pt.m_bGender;
    m_strStudentId          = pt.m_strStudentId;
    m_strIdCardNo           = pt.m_strIdCardNo;
    m_strIdMagCard          = pt.m_strIdMagCard;
    m_nClass                = pt.m_nClass;
    m_strNation             = pt.m_strNation;
    m_strFaceFeature        = pt.m_strFaceFeature;
    m_strFaceImagePath      = pt.m_strFaceImagePath;
    m_strCampusMagcard      = pt.m_strCampusMagcard;
    m_bUseGenderToSelect    = pt.m_bUseGenderToSelect;
    m_qsClassName           = pt.m_qsClassName;
    m_llTestActualStartTime = pt.m_llTestActualStartTime;
    m_nExtraInfoID          = pt.m_nExtraInfoID;
    m_bCheckTime            = pt.m_bCheckTime;
    m_shDevType             = pt.m_shDevType;
    m_nGrade                = pt.m_nGrade;
    m_nStage                = pt.m_nStage;
    return *this;
}

short CUserInfo::getAge() const
{
    short shAge = -1;
    //存储的字符为 2010-10-05
    if(m_strBrithday.length() < 10)
        return shAge;

    int year = m_strBrithday.left(4).toInt();
    int month =  m_strBrithday.mid(5,2).toInt();
    int day = m_strBrithday.right(2).toInt();

    if( year <= 0 )
        return shAge;

    QDate dateUser(year, month, day);

    //判断时间是否合理
    if(dateUser > s_dateCur)
        return shAge;

    // 如在 97.3月1日 出生 98年3月算作一岁
    shAge = static_cast<short>(s_dateCur.year() - year);
    if(shAge > 0 && s_dateCur.month() < month)
    {
        shAge--;
    }

    return shAge;
}

int CUserInfo::getGrade() const
{
    QDate currentdate = QDate::currentDate();
    int iYear = currentdate.year();
    int iMounth = currentdate.month();

    return iMounth >= 9 ? iYear - m_nGrade + 1 : iYear - m_nGrade;
}

int CUserInfo::getGradeNew() const
{
    QDate currentdate = QDate::currentDate();
    return s_grade.getGradeByStageAndYearAndDate(currentdate, m_nStage, m_nGrade).scoreGradeNo;
}

bool CUserInfo::getGender()
{
    return m_bGender;
}

bool CUserInfo::isTouristInfo() const
{
    bool bFlag = false;
    if(m_llUniqueID == TOURIST_MODE_ID)
    {
        bFlag = true;
    }
    return bFlag;
}
