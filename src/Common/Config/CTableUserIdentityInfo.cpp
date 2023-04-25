#include "CTableUserIdentityInfo.h"
#include "Logger/CLogger.h"
#include <QDebug>

CTableUserIdentityInfo::CTableUserIdentityInfo()
{

}

CTableUserIdentityInfo::~CTableUserIdentityInfo()
{

}

bool CTableUserIdentityInfo::Check_Config_Table()
{
    //当前表格信息不需要进行核对
    return true;
}

bool CTableUserIdentityInfo::updateInfo(CUserInfo &userInfo)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;
    if(m_pDatabase != nullptr)
    {
        vector<UserIdentityInfo> vecAllRecord = select<UserIdentityInfo>(*m_pDatabase, UserIdentityInfo::CodeUniqueID == userInfo.m_llUniqueID).all();

        try
        {
            if(vecAllRecord.size() > 0)
            {
                for(auto &element : vecAllRecord)
                {
                    equal(element, userInfo, false);
                    element.update();
                }
            }
            else
            {
                UserIdentityInfo info(*m_pDatabase);
                equal(info, userInfo);
                info.update();
            }


        } catch (Except e)
        {
            bRes = false;
        }
    }

    unlock();
    return bRes;
}

bool CTableUserIdentityInfo::updateInfos(vector<CUserInfo> &vecInfo)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    if(m_pDatabase != nullptr)
    {
        m_pDatabase->begin();

        try
        {
            for(auto &userInfo : vecInfo)
            {
                //判断是否存在唯一ID
                auto cursor = select<UserIdentityInfo>(*m_pDatabase, UserIdentityInfo::CodeUniqueID == userInfo.m_llUniqueID).cursor();

                //存在变量
                UserIdentityInfo infoTemp(*m_pDatabase);

                //若存在不替换id
                if(cursor.rowsLeft())
                {
                    infoTemp = (*cursor);
                    equal(infoTemp, userInfo, false);
                }
                else
                {
                    equal(infoTemp, userInfo);
                }

                infoTemp.update();

                //若数据库中人脸特征没有数据，尝试重新更新
                auto cursor1 = select<UserIdentityInfo>(*m_pDatabase, UserIdentityInfo::CodeUniqueID == userInfo.m_llUniqueID).cursor();

                if(cursor1.rowsLeft())
                {
                    //仅是数据中的人脸特征插入出错
                    if((*cursor1).faceFeature != userInfo.m_strFaceFeature.toStdString())
                    {
                        infoTemp.update();
                        LOGERROR("facefeature not write m_llUniqueID = " + to_string(userInfo.m_llUniqueID) + " name = " + userInfo.m_strName.toStdString());
                    }
                }
                //没有查到指定的数据
                else
                {
                    LOGERROR("not exist llUniqueID = " + to_string(userInfo.m_llUniqueID) + " name = " + userInfo.m_strName.toStdString());
                }
            }
        }
        catch(Except e)
        {
            bRes = false;
        }
        m_pDatabase->commit();
    }

    unlock();
    return bRes;
}

//获取所有符合需求的条件信息
bool CTableUserIdentityInfo::getUserInfo(const CUserInfo &userInfo, vector<CUserInfo> &vecUserInfo, bool bGetAll /*= false*/)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    vecUserInfo.clear();
    bool bRes = true;

    try
    {
        vector<UserIdentityInfo> vecSearch;

        if(bGetAll)
        {
            vecSearch = select<UserIdentityInfo>(*m_pDatabase).all();
        }
        else
        {
            string query;
            if(gendrateQueryCriteria(userInfo, query))
            {
                vecSearch = select<UserIdentityInfo>(*m_pDatabase, RawExpr(query)).all();
            }
            else
            {
                bRes = false;
            }
        }

        CUserInfo info;
        for(auto &element : vecSearch)
        {
            equal(info, element);
            vecUserInfo.push_back(info);
        }
    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

bool CTableUserIdentityInfo::getUserInfo(const CUserInfo &userInfo, vector<UserIdentityInfo> &vecUserInfo, bool bGetAll)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    vecUserInfo.clear();
    bool bRes = true;

    try
    {
        if(bGetAll)
        {
            vecUserInfo = select<UserIdentityInfo>(*m_pDatabase).all();
        }
        else
        {
            string query;

            if(gendrateQueryCriteria(userInfo, query))
            {
                vecUserInfo = select<UserIdentityInfo>(*m_pDatabase, RawExpr(query)).all();
            }
            else
            {
                bRes = false;
            }
        }
    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

void CTableUserIdentityInfo::equal(UserIdentityInfo &targeInfo, CUserInfo &srcInfo, bool bFlag /* = true*/)
{
    if(bFlag)
    {
        targeInfo.codeUniqueID = srcInfo.m_llUniqueID;
    }
    targeInfo.roleType         = srcInfo.m_nRoleType;
    targeInfo.name             = srcInfo.m_strName.toStdString();
    targeInfo.brithday         = srcInfo.m_strBrithday.toStdString();
    targeInfo.gender           = srcInfo.m_bGender;
    targeInfo.studentID        = srcInfo.m_strStudentId.toStdString();
    targeInfo.iDCardNo         = srcInfo.m_strIdCardNo.toStdString();
    targeInfo.iDMagCard        = srcInfo.m_strIdMagCard.toStdString();
    targeInfo.nGrade           = srcInfo.m_nGrade;
    targeInfo.nClass           = srcInfo.m_nClass;
    targeInfo.nation           = srcInfo.m_strNation.toStdString();
    targeInfo.faceFeature      = srcInfo.m_strFaceFeature.toStdString();
    targeInfo.faceImagePath    = srcInfo.m_strFaceImagePath.toStdString();
    targeInfo.campusMagcard    = srcInfo.m_strCampusMagcard.toStdString();
    targeInfo.timeStamp        = QString::number(srcInfo.longlongTimeStamp).toStdString();
    targeInfo.stage            = srcInfo.m_nStage;
}

void CTableUserIdentityInfo::equal(CUserInfo &targeInfo, UserIdentityInfo &srcInfo, bool bFlag /* = true*/)
{
    if(bFlag)
    {
        targeInfo.m_llUniqueID   = srcInfo.codeUniqueID;
    }
    targeInfo.m_nRoleType        = srcInfo.roleType;
    targeInfo.m_strName          = QString::fromStdString(srcInfo.name);
    targeInfo.m_strBrithday      = QString::fromStdString(srcInfo.brithday);
    targeInfo.m_bGender          = srcInfo.gender;
    targeInfo.m_strStudentId     = QString::fromStdString(srcInfo.studentID);
    targeInfo.m_strIdCardNo      = QString::fromStdString(srcInfo.iDCardNo);
    targeInfo.m_strIdMagCard     = QString::fromStdString(srcInfo.iDMagCard);
    targeInfo.m_nGrade           = srcInfo.nGrade;
    targeInfo.m_nClass           = srcInfo.nClass;
    targeInfo.m_strNation        = QString::fromStdString(srcInfo.nation);
    targeInfo.m_strFaceFeature   = QString::fromStdString(srcInfo.faceFeature);
    targeInfo.m_strFaceImagePath = QString::fromStdString(srcInfo.faceImagePath);
    targeInfo.m_strCampusMagcard = QString::fromStdString(srcInfo.campusMagcard);
    targeInfo.longlongTimeStamp  = QString::fromStdString(srcInfo.timeStamp).toLongLong();
    targeInfo.m_nStage           = srcInfo.stage;
}

bool CTableUserIdentityInfo::DeleteInfo(const long long &lUniqueID)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    //按照逻辑来说仅有一份数据
    bool bRet = false;
    if(m_pDatabase != nullptr)
    {
        vector<UserIdentityInfo> vecAllRecord = select<UserIdentityInfo>(*m_pDatabase, UserIdentityInfo::CodeUniqueID == lUniqueID).all();
        for(auto &element : vecAllRecord)
        {
            element.del();
        }

        bRet = true;
    }

    unlock();
    return bRet;
}

bool CTableUserIdentityInfo::DeleteInfo(vector<int> &vecDeleteinfo)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();

    bool bRet = false;
    if(m_pDatabase != nullptr)
    {
        stringstream patientIdsExpr;
        for(vector<int>::iterator iter = vecDeleteinfo.begin(); iter !=  vecDeleteinfo.end(); iter++)
        {
            patientIdsExpr<< ","<<*iter;
        }

        m_pDatabase->delete_(UserIdentityInfo::table__, In(UserIdentityInfo::CodeUniqueID, patientIdsExpr.str().erase(0,1)));
        bRet = true;
    }

    unlock();

    return bRet;
}

bool CTableUserIdentityInfo::DeleteInfoByTime(const long long &llTimeStamp)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    //按照逻辑来说仅有一份数据
    vector<UserIdentityInfo> vecAllRecord = select<UserIdentityInfo>(*m_pDatabase).all();
    vector<long long> vecDelCodeID;
    for(auto &element : vecAllRecord)
    {
        long long llTime = QString::fromStdString(element.timeStamp).toLongLong();
        if(llTime < llTimeStamp)
        {
            vecDelCodeID.push_back(element.codeUniqueID);
            //element.del();
        }
    }

    if(vecDelCodeID.size() > 0)
    {
        stringstream patientIdsExpr;
        for(vector<long long>::iterator iter = vecDelCodeID.begin(); iter !=  vecDelCodeID.end(); iter++)
        {
            patientIdsExpr<< ","<<*iter;
        }

        m_pDatabase->delete_(UserIdentityInfo::table__, In(UserIdentityInfo::CodeUniqueID, patientIdsExpr.str().erase(0,1)));
    }

    unlock();
    return true;
}

bool CTableUserIdentityInfo::DeleteAllInfo()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();

    bool bRet = true;

    if(m_pDatabase != nullptr)
    {
        try
        {
            m_pDatabase->begin();

             m_pDatabase->delete_(UserIdentityInfo::table__);

             m_pDatabase->commit();
        }
        catch (exception e)
        {
            bRet = false;
        }
    }

     unlock();

     return bRet;
}

bool CTableUserIdentityInfo::gendrateQueryCriteria(const CUserInfo &Info, string &query)
{
    //查询合法的条件
    vector<short> vecType = Info.getQueryItem(Info);

    size_t szType = vecType.size();
    if(szType == 0)
    {
        return false;
    }

    //清除查询值
    string strQuery, strQueryTemp;
    bool bFirst = true, bSearch = false;
    for(auto &type : vecType)
    {
        if(bFirst)
        {
            getQurty(type, Info, query);
            bFirst = false;
            bSearch = true;
        }
        else
        {
            if(getQurty(type, Info, strQueryTemp))
            {
                query = (RawExpr(query) && RawExpr(strQueryTemp)).asString();
            }
        }
    }

    return bSearch;
}

bool CTableUserIdentityInfo::queryStudentByUniqueID(const QString &stuUniqueID, CUserInfo &srcUserInfo )
{
    bool ok = false;
    vector<UserIdentityInfo> tmpRecords = select<UserIdentityInfo>( *m_pDatabase, UserIdentityInfo::CodeUniqueID == stuUniqueID.toInt() ).all();
    if( tmpRecords.size() <= 0 ){
        return ok;
    }

    ok = true;
    equal( srcUserInfo, tmpRecords[0] );
    return ok;
}

QString CTableUserIdentityInfo::queryMaxTimeStamp()
{
    if( nullptr == m_pDatabase ){
        return "0";
    }

    Records records = m_pDatabase->query( "select max( timeStamp_ ) from UserIdentityInfo_" );
    std::string maxTimeStamp = records.front().front();
    QString &&tmpMaxTimeStamp = QString::fromStdString( maxTimeStamp );

    if( tmpMaxTimeStamp.isNull() || tmpMaxTimeStamp.isEmpty() || tmpMaxTimeStamp == "NULL" ){
        tmpMaxTimeStamp = "0";
    }

#if QT_NO_DEBUG
#else
    qDebug() << "user table max timeStamp = " << tmpMaxTimeStamp;
#endif
    return tmpMaxTimeStamp;

}

void CTableUserIdentityInfo::updateMaxTimeStamp(const QString &newMaxTimeStamp)
{
    if( nullptr == m_pDatabase ){
        return ;
    }

    CUserInfo tmpUser;
    QString &&localMaxTimeStamp = queryMaxTimeStamp();
    vector<UserIdentityInfo> vecSearchRets = select<UserIdentityInfo>( *m_pDatabase, UserIdentityInfo::TimeStamp == localMaxTimeStamp.toLongLong() ).all();

    for( auto &ele : vecSearchRets ){
        equal( tmpUser, ele );
        tmpUser.longlongTimeStamp = newMaxTimeStamp.toLongLong();
        updateInfo( tmpUser );
    }
}

bool CTableUserIdentityInfo::getQurty(short shType, const CUserInfo &Info, string &query)
{
    bool bRet = true;

    switch (shType)
    {
    case CUserInfo::Query_Type_UniqueID:
        query = (UserIdentityInfo::CodeUniqueID == Info.m_llUniqueID).asString();
        break;
    case CUserInfo::Query_Type_RoleType:
        query = (UserIdentityInfo::RoleType == Info.m_nRoleType).asString();
        break;
    case CUserInfo::Query_Type_Name:
        query = (UserIdentityInfo::Name == Info.m_strName.toStdString()).asString();
        break;
    case CUserInfo::Query_Type_Brithday:
        query = (UserIdentityInfo::Brithday == Info.m_strBrithday.toStdString()).asString();
        break;
    case CUserInfo::Query_Type_Gender:
        query = (UserIdentityInfo::Gender == Info.m_bGender).asString();
        break;
    case CUserInfo::Query_Type_StudentId:
        query = (UserIdentityInfo::StudentID == Info.m_strStudentId.toStdString()).asString();
        break;
    case CUserInfo::Query_Type_IdCardNo:
        query = (UserIdentityInfo::IDCardNo == Info.m_strIdCardNo.toStdString()).asString();
        break;
    case CUserInfo::Query_Type_Grade:
        query = (UserIdentityInfo::NGrade == Info.m_nGrade).asString();
        break;
    case CUserInfo::Query_Type_Class:
        query = (UserIdentityInfo::NClass == Info.m_nClass).asString();
        break;
    case CUserInfo::Query_Type_Nation:
        query = (UserIdentityInfo::Nation == Info.m_strNation.toStdString()).asString();
        break;
    case CUserInfo::Query_Type_CampusMagcard:
        query = (UserIdentityInfo::CampusMagcard == Info.m_strCampusMagcard.toStdString()).asString();
        break;
    default:
        bRet = false;
        break;
    }

    return bRet;
}
