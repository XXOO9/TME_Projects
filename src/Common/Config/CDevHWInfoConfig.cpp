#include "CDevHWInfoConfig.h"
#include "Logger/CLogger.h"

const static int TEST_CHANNEL(25);
const static int DEFAULT_CONNECT_COUNT(8);
const static int DEFAULT_DEV_NUMBER(1);

CDevHWInfoConfig::CDevHWInfoConfig()
{

}

CDevHWInfoConfig::~CDevHWInfoConfig()
{

}

bool CDevHWInfoConfig::Check_Config_Table()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool bSuccess = false;

    DevHWInfoConfig devHWConfig(*m_pDatabase);

    //如果不存在记录，则创建一条记录，
    auto iCount = select<DevHWInfoConfig>(*m_pDatabase).count();
    if (iCount == 0)
    {
        devHWConfig.channel = TEST_CHANNEL;
        devHWConfig.maxDevCount = DEFAULT_CONNECT_COUNT;
        devHWConfig.devNum = DEFAULT_DEV_NUMBER;
        devHWConfig.update();
    }

    bSuccess = true;

    return bSuccess;
}

bool CDevHWInfoConfig::updateDevChannelAndCount(int channel, int maxCount)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevHWInfoConfig devHWConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevHWInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevHWInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devHWConfig = select<DevHWInfoConfig>(*m_pDatabase).one();

                devHWConfig.channel = channel;
                devHWConfig.maxDevCount = maxCount;
                devHWConfig.update();
            }
            bRes =  true;
        }
        else
        {
            bRes =  false;
        }
    }
    catch (Except e)
    {
        bRes =  false;
    }

    unlock();
    return bRes;
}

bool CDevHWInfoConfig::updateDevNumber(int num)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevHWInfoConfig devHWConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevHWInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevHWInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devHWConfig = select<DevHWInfoConfig>(*m_pDatabase).one();

                devHWConfig.devNum = num;
                devHWConfig.update();
            }
            bRes =  true;
        }
        else
        {
            bRes =  false;
        }
    }
    catch (Except e)
    {
        bRes =  false;
    }

    unlock();
    return bRes;
}

bool CDevHWInfoConfig::getDevChannel(int &channel)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    DevHWInfoConfig devHWConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevHWInfoConfig_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<DevHWInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devHWConfig = select<DevHWInfoConfig>(*m_pDatabase).one();

            channel = devHWConfig.channel;
        }
        return true;
    }

    return false;
}

bool CDevHWInfoConfig::getDevMaxConnectCount(int &maxCount)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    DevHWInfoConfig devHWConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevHWInfoConfig_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<DevHWInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devHWConfig = select<DevHWInfoConfig>(*m_pDatabase).one();

            maxCount = devHWConfig.maxDevCount;
        }
        return true;
    }

    return false;
}

bool CDevHWInfoConfig::getDevNumber(int &num)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    DevHWInfoConfig devHWConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevHWInfoConfig_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<DevHWInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devHWConfig = select<DevHWInfoConfig>(*m_pDatabase).one();

            num = devHWConfig.devNum;
        }
        return true;
    }

    return false;
}
