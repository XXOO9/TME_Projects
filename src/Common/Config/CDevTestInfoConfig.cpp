#include "CDevTestInfoConfig.h"
#include "Logger/CLogger.h"

CDevTestInfoConfig::CDevTestInfoConfig()
{

}

CDevTestInfoConfig::~CDevTestInfoConfig()
{

}

bool CDevTestInfoConfig::Check_Config_Table()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool bSuccess = false;

    DevTestInfoConfig devTestInfoConfig(*m_pDatabase);

    //如果不存在记录，则创建一条记录，
    auto iCount = select<DevTestInfoConfig>(*m_pDatabase).count();
    if (iCount == 0)
    {
        devTestInfoConfig.testMode = DevTestInfoConfig::TestMode::Sync;
        devTestInfoConfig.bDisplayScore = true;
        devTestInfoConfig.bUseRunShortStart = true;
        devTestInfoConfig.runMidLength = 800;
        devTestInfoConfig.ropeDelayTime = 20;
        devTestInfoConfig.devConnectMode = false;
        devTestInfoConfig.devConnectModeAuto = true;
        devTestInfoConfig.update();
    }

    bSuccess = true;

    return bSuccess;
}

bool CDevTestInfoConfig::updateDevTestMode(int testMode)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevTestInfoConfig devTestConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
                if(testMode == DevTestInfoConfig::TestMode::Sync)
                {
                    devTestConfig.testMode = DevTestInfoConfig::TestMode::Sync;
                }
                else
                {
                    devTestConfig.testMode = DevTestInfoConfig::TestMode::Async;
                }

                devTestConfig.update();
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

bool CDevTestInfoConfig::updateDevDisplayScore(bool bDisplayScore)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevTestInfoConfig devTestConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
                devTestConfig.bDisplayScore = bDisplayScore;
                devTestConfig.update();
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

bool CDevTestInfoConfig::updateUseRunShortStartFlag(bool bUseRunShortStartFlag)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevTestInfoConfig devTestConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
                devTestConfig.bUseRunShortStart = bUseRunShortStartFlag;
                devTestConfig.update();
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

bool CDevTestInfoConfig::updateDevRunMidLength(int length)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevTestInfoConfig devTestConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
                devTestConfig.runMidLength = length;
                devTestConfig.update();
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

bool CDevTestInfoConfig::updateRopeDelayTime(int iTime)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevTestInfoConfig devTestConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
                devTestConfig.ropeDelayTime = iTime;
                devTestConfig.update();
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

bool CDevTestInfoConfig::getDevTestMode(int &testMode)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    DevTestInfoConfig devTestConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
            testMode = devTestConfig.testMode;
        }
        return true;
    }

    return false;
}

bool CDevTestInfoConfig::getDevIsDisplayScore()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    DevTestInfoConfig devTestConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
    size_t count = recods.size();
    bool bDisplayScore = false;

    if (count != 0)
    {
        size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
            bDisplayScore = devTestConfig.bDisplayScore;
        }
    }

    return bDisplayScore;
}

bool CDevTestInfoConfig::getUseRunShortStartFlag()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    DevTestInfoConfig devTestConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
    size_t count = recods.size();
    bool bUseRunShortStartFlag = true;

    if (count != 0)
    {
        size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
            bUseRunShortStartFlag = devTestConfig.bUseRunShortStart;
        }
    }

    return bUseRunShortStartFlag;
}

bool CDevTestInfoConfig::getDevRunMidLength(int &length)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    DevTestInfoConfig devTestConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
            length = devTestConfig.runMidLength;
        }
        return true;
    }

    return false;
}

bool CDevTestInfoConfig::getRopeDelayTime(int &iTime)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    DevTestInfoConfig devTestConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
            iTime = devTestConfig.ropeDelayTime;
        }
        return true;
    }
    return false;
}

bool CDevTestInfoConfig::updateDevConnectMode(bool bDirectConnect)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevTestInfoConfig devTestConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
                devTestConfig.devConnectMode = bDirectConnect;
                devTestConfig.update();
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

bool CDevTestInfoConfig::getDevConnectMode()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool bDirectConnect = false;

    DevTestInfoConfig devTestConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
            bDirectConnect = devTestConfig.devConnectMode;
        }
    }

    return bDirectConnect;
}

bool CDevTestInfoConfig::updateDevConnectModeAuto(bool bAutoSwitch)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try {
        DevTestInfoConfig devTestConfig(*m_pDatabase);

        Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
        size_t count = recods.size();

        if (count != 0)
        {
            size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
            if (iRecordCount > 0)
            {
                devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
                devTestConfig.devConnectModeAuto= bAutoSwitch;
                devTestConfig.update();
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

bool CDevTestInfoConfig::getDevConnectModeAuto()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool bDirectConnectAuto = false;

    DevTestInfoConfig devTestConfig(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='DevTestInfoConfig_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<DevTestInfoConfig>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            devTestConfig = select<DevTestInfoConfig>(*m_pDatabase).one();
            bDirectConnectAuto = devTestConfig.devConnectModeAuto;
        }
    }

    return bDirectConnectAuto;
}

bool CDevTestInfoConfig::getStandingJumpStartPos(int &startPos)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }
    DevTestInfoConfig devTestConfig(*m_pDatabase);

    devTestConfig = select<DevTestInfoConfig>( *m_pDatabase ).one();
    startPos =  devTestConfig.standingJumpStartPos;

    return true;
}

bool CDevTestInfoConfig::setStadingJumpStartPos(const int &taregtStartPos)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }
    DevTestInfoConfig devTestConfig(*m_pDatabase);

    devTestConfig = select<DevTestInfoConfig>( *m_pDatabase ).one();
    devTestConfig.standingJumpStartPos = taregtStartPos;
    devTestConfig.update();
    return true;
}
