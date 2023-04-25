#include "CTableClassInfo.h"
#include "Logger/CLogger.h"
#include <QString>

CTableClassInfo::CTableClassInfo()
{

}

bool CTableClassInfo::getClassInfo(int nUniqueID, string &strClassInfo)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try
    {
        //float值不能作为判断条件只能通过查询之后的值进行判断
        vector<ClassInfo> vecSearch;

        //生成条件出错标识rule不符合插入或者更新条件
        vecSearch = select<ClassInfo>(*m_pDatabase, ClassInfo::ClassUniqueID == nUniqueID).all();

        if(vecSearch.size() == 0)
        {
            bRes = false;
        }
        else
        {
            strClassInfo = vecSearch[0].classDescribe;
        }

    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

bool CTableClassInfo::updateClassInfo(const CClassInfo &info)
{
    return updateClassInfo(info.m_nUniqueID, info.m_strClassDescribe, info.m_timeStamp);
}

bool CTableClassInfo::updateClassInfo(const int &nUniqueID, const string &strClassInfo,  const string &strTimeStamp)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try
    {
        //float值不能作为判断条件只能通过查询之后的值进行判断
        ClassInfo info(*m_pDatabase);

        info.classUniqueID = nUniqueID;
        info.classDescribe = strClassInfo;
        info.timeStamp = strTimeStamp;
        info.update();
    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

bool CTableClassInfo::updateClassInfos(const vector<CClassInfo> &vecInfo)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;
    m_pDatabase->begin();

    try
    {
        //float值不能作为判断条件只能通过查询之后的值进行判断
        for(auto &element : vecInfo)
        {
            //判断是否存在唯一ID
            auto vecClass = select<ClassInfo>(*m_pDatabase, ClassInfo::ClassUniqueID == element.m_nUniqueID).all();
            if(vecClass.size() > 0)
            {
                vecClass[0].classDescribe = element.m_strClassDescribe;
                vecClass[0].update();
            }
            else
            {
                ClassInfo infoTemp(*m_pDatabase);
                infoTemp.classUniqueID = element.m_nUniqueID;
                infoTemp.classDescribe = element.m_strClassDescribe;
                infoTemp.timeStamp = element.m_timeStamp;
                infoTemp.update();
            }
        }

    }
    catch(Except e)
    {
        //bRes = false;
    }

    m_pDatabase->commit();
    unlock();
    return bRes;
}

bool CTableClassInfo::DeleteInfo(vector<int> &vecDeleteinfo)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();

    stringstream patientIdsExpr;
    for(vector<int>::iterator iter = vecDeleteinfo.begin(); iter !=  vecDeleteinfo.end(); iter++)
    {
        patientIdsExpr<< ","<<*iter;
    }

    m_pDatabase->delete_(ClassInfo::table__, In(ClassInfo::ClassUniqueID, patientIdsExpr.str().erase(0,1)));

    unlock();

    return true;
}

bool CTableClassInfo::DeleteInfoByTime(const long long &llTimeStamp)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    //按照逻辑来说仅有一份数据
    vector<int> vecDelClassID;
    vector<ClassInfo> vecAllRecord = select<ClassInfo>(*m_pDatabase).all();
    for(auto &element : vecAllRecord)
    {
        long long llTime = QString::fromStdString(element.timeStamp).toLongLong();
        if(llTime < llTimeStamp)
        {
            vecDelClassID.push_back(element.classUniqueID);
            //element.del();
        }
    }

    if(vecDelClassID.size() > 0)
    {
        stringstream patientIdsExpr;
        for(vector<int>::iterator iter = vecDelClassID.begin(); iter !=  vecDelClassID.end(); iter++)
        {
            patientIdsExpr<< ","<<*iter;
        }

        m_pDatabase->delete_(ClassInfo::table__, In(ClassInfo::ClassUniqueID, patientIdsExpr.str().erase(0,1)));
    }

    unlock();
    return true;
}

bool CTableClassInfo::deleteAllInfo()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();

    bool bRet = true;

    try
    {
        m_pDatabase->begin();

         m_pDatabase->delete_(ClassInfo::table__);

         m_pDatabase->commit();
    }
    catch (exception e)
    {
        bRet = false;
    }

     unlock();

     return bRet;
}
