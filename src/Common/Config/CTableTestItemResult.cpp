#include "CTableTestItemResult.h"
#include "CFilePath.h"
#include <Logger/CLogger.h>

CTableTestItemResult::CTableTestItemResult()
{

}

CTableTestItemResult::~CTableTestItemResult()
{

}

bool CTableTestItemResult::Check_Config_Table()
{
    return true;
}

bool CTableTestItemResult::addResultInfo(const CTestResultInfo &info)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    if(!info.isResultCorrect())
    {
        return false;
    }

    lock();

    bool bRes = false;
    try
    {
        //插入之前先查询有没有相同的数据，若存在不进行插入
        bool bTestResultExist = false;

        vector<TestItemResult> vecSearch;
        string query;
        if(gendrateQueryCriteria(info, query))
        {
            vecSearch = select<TestItemResult>(*m_pDatabase, RawExpr(query)).all();
        }
        bTestResultExist = vecSearch.size() > 0;

        if(!bTestResultExist)
        {
            TestItemResult itemRes(*m_pDatabase);
            equal(itemRes, info);
            itemRes.update();
        }
        else
        {
            //已经存在相同测试结果(唯一ID,测试项目,开始时间)
            LOGERROR("### Same Test Result : ID is : " + to_string(info.m_llUniqueID)
                     + " Item is : " + to_string(info.m_nItem)
                     + " Time is : " + info.m_strStartTime);
        }

        bRes = true;
    }
    catch (Except e)
    {

    }

    unlock();
    return bRes;
}

bool CTableTestItemResult::updateResultInfo(const CTestResultInfo &info)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    if(!info.isResultCorrect())
    {
        unlock();
        return false;
    }

    bool bRes = true;
    try
    {
        vector<TestItemResult> vecSearch;
        string query;
        if(gendrateQueryCriteria(info, query))
        {
            vecSearch = select<TestItemResult>(*m_pDatabase, RawExpr(query)).all();
        }

        bool bExist = false;

        for(auto &element : vecSearch)
        {
            equal(element, info);
            element.update();
            bExist = true;
        }

        if(!bExist)
        {
            TestItemResult itemRes(*m_pDatabase);
            equal(itemRes, info);
            itemRes.update();
        }
    }
    catch (Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;

}

bool CTableTestItemResult::addResultInfo(const vector<CTestResultInfo> &vecResult)
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
        bool bTestResultExist;
        vector<TestItemResult> vecSearch;
        string query;
        for (auto &element : vecResult)
        {
            //清除上一次缓存
            vecSearch.clear();
            query = "";

            if(gendrateQueryCriteria(element, query))
            {
                vecSearch = select<TestItemResult>(*m_pDatabase, RawExpr(query)).all();
            }

            //是否存在相同数据
            bTestResultExist = vecSearch.size() > 0;

            if(!bTestResultExist)
            {
                TestItemResult itemRes(*m_pDatabase);
                equal(itemRes, element);
                itemRes.update();
            }
            else
            {

                //已经存在相同测试结果(唯一ID,测试项目,开始时间)
                LOGERROR("### Same Test Result : ID is : " + to_string(element.m_llUniqueID)
                         + " Item is : " + to_string(element.m_nItem)
                         + " Time is : " + element.m_strStartTime);
            }
        }

    }
    catch (Except e)
    {

    }

    m_pDatabase->commit();
    unlock();
    return bRes;
}

bool CTableTestItemResult::getResultByInfo(const CTestResultInfo &info, vector<CTestResultInfo> &vecResult, bool bGetAll)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    vecResult.clear();
    bool bRes = true;

    try
    {
        vector<TestItemResult> vecSearch;

        if(bGetAll)
        {
            vecSearch = select<TestItemResult>(*m_pDatabase).all();
        }
        else
        {
            string query;
            if(gendrateQueryCriteria(info, query))
            {
                vecSearch = select<TestItemResult>(*m_pDatabase, RawExpr(query)).all();
            }
            else
            {
                bRes = false;
            }
        }

        CTestResultInfo infoTemp;
        for(auto &element : vecSearch)
        {
            equal(infoTemp, element);
            vecResult.push_back(infoTemp);
        }
    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

//vecResult: 测试结果存储
//llUserID:  用户唯一ID
//iItem:     测试项目
//llTime：   时间戳
//iNature:   测试性质
bool CTableTestItemResult::getResultByInfo(vector<CTestResultInfo> &vecResult, const long long &llUserID, const int &iItem, const long long &llTime, const int &iNature)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    vecResult.clear();
    bool bRes = true;

    try
    {
        vector<TestItemResult> vecSearch;

        //生成条件出错标识rule不符合插入或者更新条件
        vecSearch = select<TestItemResult>( *m_pDatabase, TestItemResult::CodeUniqueID == llUserID &&
                                            TestItemResult::TestItem == iItem && TestItemResult::TestNature == iNature ).all();

        if(vecSearch.size() <= 0)
        {
            bRes = false;
        }
        else
        {
            //查找符合时间戳的项目
            CTestResultInfo info;
            for (unsigned int i = 0; i < vecSearch.size() ; i++)
            {
                if(llTime <= QString::fromStdString( vecSearch[i].testTime ).toLongLong())
                {
                    equal(info, vecSearch[i]);
                    vecResult.push_back(info);
                }
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

bool CTableTestItemResult::getResultByInfo(const CTestResultInfo &info, vector<TestItemResult> &vecResult, bool bGetAll)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    vecResult.clear();
    bool bRes = true;

    try
    {
        vector< TestItemResult > vecSearch;

        if(bGetAll)
        {
            vecResult = select<TestItemResult>(*m_pDatabase).all();
        }
        else
        {
            string query;
            if(gendrateQueryCriteria(info, query))
            {
                vecResult = select<TestItemResult>(*m_pDatabase, RawExpr(query)).all();

                for(auto &element : vecResult)
                {
                    element.syncServer = true;
                    element.update();
                }
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

bool CTableTestItemResult::deleteResultInfo(const CTestResultInfo &info, bool bDeleteAll)
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
        vector<TestItemResult> vecSearch;

        if(bDeleteAll)
        {
            m_pDatabase->begin();

            vecSearch = select<TestItemResult>(*m_pDatabase).all();
            for(auto &element : vecSearch)
            {
                element.del();
            }

            m_pDatabase->commit();
        }
        else
        {
            string query;
            if(gendrateQueryCriteria(info, query))
            {
                auto vecResult = select<TestItemResult>(*m_pDatabase, RawExpr(query)).all();
                for(auto &testRes : vecResult)
                {
                    testRes.del();
                }
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

bool CTableTestItemResult::deleteResultInfo(const vector<CTestResultInfo> &vecResult)
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
        vector<TestItemResult> vecSearch;
        string query;
        for (auto &element : vecResult)
        {
            //清除上一次缓存
            vecSearch.clear();
            query = "";

            if(gendrateQueryCriteria(element, query))
            {
                vecSearch = select<TestItemResult>(*m_pDatabase, RawExpr(query)).all();
                for(auto &testRes : vecSearch)
                {
                    testRes.del();
                }
            }
        }

    }
    catch (Except e)
    {

    }

    m_pDatabase->commit();
    unlock();
    return bRes;
}

bool CTableTestItemResult::updateSync(const long long &llID, const int &iItem, const string &strTime, bool bFlag)
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
        size_t existCount = select<TestItemResult>(*m_pDatabase, TestItemResult::CodeUniqueID == llID&&TestItemResult::TestItem == iItem&&TestItemResult::TestTime == strTime).count();
        if (existCount > 0)
        {
            auto vecTestRes = select<TestItemResult>(*m_pDatabase, TestItemResult::CodeUniqueID == llID&&TestItemResult::TestItem == iItem&&TestItemResult::TestTime == strTime).all();

            for(auto &testRes : vecTestRes)
            {
                testRes.syncServer = bFlag;
                testRes.update();
            }
        }else {
            QString logInfo = QString( "query conditidon is not exitsed, ID = %1, Item = %2, Timesatamp = %3" ).arg( QString::number( llID ), QString::number( iItem ), QString::fromStdString( strTime ) );

            LOGERROR( logInfo.toStdString() );
        }
    }
    catch (Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

void CTableTestItemResult::equal(TestItemResult &targeInfo, const CTestResultInfo &srcInfo)
{
    targeInfo.codeUniqueID     = srcInfo.m_llUniqueID;
    targeInfo.testItem         = srcInfo.m_nItem;
    targeInfo.testNature       = srcInfo.m_nNature;
    targeInfo.testTime         = srcInfo.m_strStartTime;
    targeInfo.testMulitResult  = srcInfo.m_strMulitResult;
    targeInfo.processDataPath  = srcInfo.m_strProcessDataPath;
    targeInfo.confidence       = srcInfo.m_strConfidence;
    targeInfo.resultSignature  = srcInfo.m_strResSignature;
    targeInfo.syncServer       = srcInfo.m_bSyncServer;
    targeInfo.deviceID         = srcInfo.m_sDeviceID;
}

void CTableTestItemResult::equal(CTestResultInfo &targeInfo, const TestItemResult &srcInfo)
{
    targeInfo.m_llUniqueID         = srcInfo.codeUniqueID;
    targeInfo.m_nItem              = srcInfo.testItem;
    targeInfo.m_nNature            = srcInfo.testNature;
    targeInfo.m_strStartTime       = srcInfo.testTime;
    targeInfo.m_strMulitResult     = srcInfo.testMulitResult;
    targeInfo.m_strProcessDataPath = srcInfo.processDataPath;
    targeInfo.m_strConfidence        = srcInfo.confidence;
    targeInfo.m_strResSignature    = srcInfo.resultSignature;
    targeInfo.m_bSyncServer        = srcInfo.syncServer;
    targeInfo.m_sDeviceID          = srcInfo.deviceID;
}

bool CTableTestItemResult::gendrateQueryCriteria(const CTestResultInfo &Info, string &query)
{
    vector<short> vecType = Info.getQueryItem(Info);

    size_t szType = vecType.size();
    if(szType == 0)
    {
        return false;
    }

    string strQueryTemp;
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

bool CTableTestItemResult::getQurty(short shType, const CTestResultInfo &Info, string &query)
{
    bool bRet = true;
    switch (shType)
    {
        case CTestResultInfo::Query_Type_UniqueID:
            query = (TestItemResult::CodeUniqueID == Info.m_llUniqueID).asString();
            break;
        case CTestResultInfo::Query_Type_Item:
            query = (TestItemResult::TestItem == Info.m_nItem).asString();
            break;
        case CTestResultInfo::Query_Type_Time:
            query = (TestItemResult::TestTime == Info.m_strStartTime).asString();
            break;
        case CTestResultInfo::Query_Type_Nature:
            query = (TestItemResult::TestNature == Info.m_nNature).asString();
            break;
        case CTestResultInfo::Query_Type_Nature_Unequal:
            query = (TestItemResult::TestNature != Info.m_nNature).asString();
            break;
        case CTestResultInfo::Query_Type_SyncServer:
            query = (TestItemResult::SyncServer == Info.m_bSyncServer).asString();
            break;
        default:
            bRet = false;
            break;
    }

    return bRet;
}
