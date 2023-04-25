///////////////////////////////////////////////////////////
//  CStudentIdentityInfo.h
//  Implementation of the Class CStudentIdentityInfo
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_BB29C6AD_0359_425c_8A75_1B5C1492DE99_INCLUDED_)
#define EA_BB29C6AD_0359_425c_8A75_1B5C1492DE99_INCLUDED_
#include "../common_global.h"
#include "ste100database.hpp"
#include "CGrade.h"
#include <string>
#include <iostream>
#include <list>
#include <QString>
#include <QDate>
using namespace std;
using namespace Database;


//游客模式ID，正常模式是大于0的
#define TOURIST_MODE_ID -999
#define UNKNOWN_ID -1000

class COMMONSHARED_EXPORT CUserInfo
{
public:
    enum emQuery_Condition{
        Query_Type_UniqueID, Query_Type_RoleType, Query_Type_Name, Query_Type_Brithday,Query_Type_Gender, Query_Type_StudentId, Query_Type_IdCardNo,
        Query_Type_Grade, Query_Type_Class, Query_Type_Nation, Query_Type_CampusMagcard};

    enum {
        SYSTEM_USER_UNKNOW,SYSTEM_USER_STUDENT,SYSTEM_USER_TEACHER,SYSTEM_USER_ADMIN
    };
public:
    CUserInfo();
    virtual ~CUserInfo();

    void reset();
    //获取查询条件
    static vector<short> getQueryItem(const CUserInfo &info);

    bool operator<(const CUserInfo &user) const
    {
        bool bRes = false;
        if(this->m_llUniqueID < user.m_llUniqueID)
        {
            bRes = true;
        }

        return bRes;
    }

    //用户id相同或者学号相同就认为相等
    bool operator==(const CUserInfo &user) const
    {
        bool bRes = false;
        if(this->m_llUniqueID == user.m_llUniqueID)
        {
            bRes = true;
        }

        if(!bRes)
        {
            if(!user.m_strStudentId.isEmpty())
            {
                bRes = user.m_strStudentId == this->m_strStudentId;
            }
        }

        return bRes;
    }

    CUserInfo &operator=(const CUserInfo &pt);

    //负数表示无效年龄
    short getAge() const;

    //获取年级，进行变更之后  m_nGrade 仅代表比如 入学时间比如2014那么并且当前年份为2014当前届别就是 初中1年级 高中1年级 仅仅知道是一年级，而不知道是哪个年级
    //有些学校从1~9年级 有些学校从初中开始就重新开始计 1年级
    //新增字段stage
    int getGrade() const;

    //获取年级
    int getGradeNew() const;
public:

    bool getGender();

    bool isTouristInfo() const;

    long long   m_llUniqueID{TOURIST_MODE_ID};     // id
    int         m_nRoleType{SYSTEM_USER_UNKNOW};         //role
    QString     m_strBrithday;
    qint64      longlongTimeStamp;   //时间戳
    QString     m_strName;           //  姓名
    bool        m_bGender{UserIdentityInfo::GenderType::FEMALE};             //性别
    QString     m_strStudentId;      //学号
    QString     m_strIdCardNo;       //身份号
    QString     m_strIdMagCard;      //身份证磁卡信息
    int         m_nGrade{-1};            //年级  目前是入学时间
    int         m_nClass{-1};            //班级
    QString     m_strNation;         //民族
    QString     m_strFaceFeature;    //人脸数据
    QString     m_strFaceImagePath;  //
    QString     m_strCampusMagcard;
    bool        m_bUseGenderToSelect{false};
    int         m_nStage{0};            //添加信息用于计算得分年级

    QString     m_qsClassName;

    ///下方变量为用户在测试过程中需要的变量
    //测试真实开始时间
    qint64 m_llTestActualStartTime{0};
    //额外信息的ID，例如中长跑的卡号，跳绳的绳子ID,或者对应测试设备的分机编号 短跑表示跑道号
    unsigned int m_nExtraInfoID{0};
    //是否进行时间检测
    bool m_bCheckTime{false};
    //当前设备类型
    short m_shDevType{0};

    static QDate s_dateCur;

    static CGrade s_grade;
};
#endif // !defined(EA_BB29C6AD_0359_425c_8A75_1B5C1492DE99_INCLUDED_)
