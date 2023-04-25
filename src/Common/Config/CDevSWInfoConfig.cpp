#include "CDevSWInfoConfig.h"
#include "commondefin.h"
#include "Logger/CLogger.h"

static DEV_SW_TEST_INFO defaultTestConfig[] = {
    {DEV_TYPE_BODY,         1,  0,    120,   "1-2"},
    {DEV_TYPE_VC,           2,  0,    30,  "4"},
    {DEV_TYPE_SBJ,          2,  1500, 30,  "5"},  //人体成分包含多个测试项不含单位
    {DEV_TYPE_SFB,          2,  100,  30,  "6"},
    {DEV_TYPE_PULL_UP,      1,  1500, 30,  "7"},
    {DEV_TYPE_SIT_UP,       1,  1500, 30,  "8"},
    {DEV_TYPE_RUN_SHORT,    1,  1500, 30,  "162"},
    {DEV_TYPE_RUN_MID,      1,  1500, 30,  "160-161"},
    {DEV_TYPE_ROPE_SKIP,    1,  1500, 30,  "163"},
    {DEV_TYPE_GRIP,         1,  1500, 30,  "18"},
    {DEV_TYPE_PUSH_UP,      1,  1500, 30,  "21"},
    {DEV_TYPE_EYESIGHT,     1,  1500, 30,  "19"},
    {DEV_TYPE_PVV,          1,  0,    30,  "353"},
    {DEV_TYPE_FOOTBALL,     1,  0,    30,  "23"},
    {DEV_TYPE_BASKETBALL,   1,  0,    30,  "24"},
    {DEV_TYPE_VOLLEYBALL,   1,  0,    30,  "25"},
    {DEV_TYPE_PUO,          1,  0,    30,  "26"},
    {DEV_TYPE_SOLIDBALL,    1,  0,    30,  "27"},
    {DEV_TYPE_VJUMP,        1,  0,    30,  "28"},
};

CDevSWInfoConfig::CDevSWInfoConfig()
{

}

CDevSWInfoConfig::~CDevSWInfoConfig()
{

}

bool CDevSWInfoConfig::Check_Config_Table()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool bRet = true;

    try
    {
        //获取默认实验个数
        int nDefaultItemSize = sizeof(defaultTestConfig) / sizeof (DEV_SW_TEST_INFO);
        bool bExit;
        for(int i = 0; i < nDefaultItemSize; i++)
        {
            DEV_SW_TEST_INFO &element = defaultTestConfig[i];
            bExit = select<DevSWInfoConfig>(*m_pDatabase, DevSWInfoConfig::TestDev == element.iDevType).count() > 0;

            if(!bExit)
            {
                DevSWInfoConfig info(*m_pDatabase);
                info.testDev = element.iDevType;
                info.testTimes = element.testTimes;
                info.testStartWaitTime = element.waitStartTimes;
                info.testPageCountTime = element.pageCountTime;
                info.testItem = GBK2UTF8(element.strTestItems);
                info.update();
            }
        }
    }
    catch (Except e)
    {
        bRet = false;
    }

    return bRet;
}

bool CDevSWInfoConfig::updateDevSWTestConfig(int devType, int testTimes, string testItems)
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
        vector<DevSWInfoConfig> vecInfo = select<DevSWInfoConfig>(*m_pDatabase, DevSWInfoConfig::TestDev == devType).all();

        bool bExist = false;
        if(vecInfo.size() > 0)
        {
            auto element = (*vecInfo.begin());
            element.testTimes = testTimes;
            element.testItem = testItems;
            element.update();
            bExist = true;
        }

        if(!bExist)
        {
            DevSWInfoConfig info(*m_pDatabase);
            info.testDev = devType;
            info.testTimes = testTimes;
            info.testItem = testItems;
            info.update();
        }

    }
    catch (Except e)
    {
        bRet = false;
    }

    unlock();
    return bRet;
}

bool CDevSWInfoConfig::getDevTestTimes(int devType, int &times)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRet = false;

    try
    {
        vector<DevSWInfoConfig> vecInfo = select<DevSWInfoConfig>(*m_pDatabase, DevSWInfoConfig::TestDev == devType).all();
        if(vecInfo.size() > 0)
        {
            times = vecInfo[0].testTimes;
            bRet = true;
        }
    }
    catch (Except e)
    {

    }

    unlock();
    return  bRet;
}

bool CDevSWInfoConfig::getDevTestStartWaitTime(int devType, int &time)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRet = false;

    try
    {
        vector<DevSWInfoConfig> vecInfo = select<DevSWInfoConfig>(*m_pDatabase, DevSWInfoConfig::TestDev == devType).all();
        if(vecInfo.size() > 0)
        {
            time = vecInfo[0].testStartWaitTime;
            bRet = true;
        }
    }
    catch (Except e)
    {

    }

    unlock();
    return  bRet;
}

int CDevSWInfoConfig::getDevTestPageCountTime(int devType)
{
    int time = 30;
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRet = false;

    try
    {
        vector<DevSWInfoConfig> vecInfo = select<DevSWInfoConfig>(*m_pDatabase, DevSWInfoConfig::TestDev == devType).all();
        if(vecInfo.size() > 0)
        {
            time = vecInfo[0].testPageCountTime;
            bRet = true;
        }
    }
    catch (Except e)
    {

    }

    unlock();
    return  time;
}

bool CDevSWInfoConfig::updateDevTestPageCountTime(int devType, int time)
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
        vector<DevSWInfoConfig> vecInfo = select<DevSWInfoConfig>(*m_pDatabase, DevSWInfoConfig::TestDev == devType).all();

        if(vecInfo.size() > 0)
        {
            auto element = (*vecInfo.begin());
            element.testPageCountTime = time;
            element.update();
            bRet = true;
        }
    }
    catch (Except e)
    {
        bRet = false;
    }

    unlock();
    return bRet;
}

bool CDevSWInfoConfig::getDevTestItemsVec(int devType, vector<short> &item)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRet = false;

    try
    {
        vector<DevSWInfoConfig> vecInfo = select<DevSWInfoConfig>(*m_pDatabase, DevSWInfoConfig::TestDev == devType).all();
        if(vecInfo.size() > 0)
        {
            QStringList qsItemList = QString::fromStdString(vecInfo[0].testItem).split('-', QString::SkipEmptyParts);
            for(int i=0 ; i<qsItemList.size();i++)
            {
                item.push_back(qsItemList[i].toShort());
            }
            bRet = true;
        }
    }
    catch (Except e)
    {

    }

    unlock();
    return  bRet;
}

bool CDevSWInfoConfig::getDevTestItemsStr(int devType, string &testItem)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRet = false;

    try
    {
        vector<DevSWInfoConfig> vecInfo = select<DevSWInfoConfig>(*m_pDatabase, DevSWInfoConfig::TestDev == devType).all();
        if(vecInfo.size() > 0)
        {
            testItem = vecInfo[0].testItem;
            bRet = true;
        }
    }
    catch (Except e)
    {

    }

    unlock();
    return  bRet;
}
