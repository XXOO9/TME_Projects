///////////////////////////////////////////////////////////
//  CDatabaseManage.cpp
//  Implementation of the Class CDatabaseManage
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CDatabaseManage.h"
#include "Logger/CLogger.h"


CDatabaseManage::CDatabaseManage()
    : m_pDatabaseChecker(nullptr)
    , m_pUserIdentityInfo(nullptr)
    , m_pTestItemInfo(nullptr)
    , m_pTestItemResult(nullptr)
    , m_pDataSyncTime(nullptr)
    , m_pScoreRules(nullptr)
    , m_pClassInfo(nullptr)
    , m_pCameraInfo(nullptr)
    , m_pDevHWInfoConfig(nullptr)
    , m_pDevSWInfoConfig(nullptr)
    , m_pDevTestInfoConfig(nullptr)
    , m_pCommitTableConfig( nullptr )
{
    checkConfigDatabase();
    initailizeConfigObject();
    checkConfigDatatable();
}



CDatabaseManage::~CDatabaseManage()
{
    if (s_pManagerInstance != nullptr)
    {
        delete s_pManagerInstance;
    }

    if(m_pConfigVersion != nullptr)
        delete m_pConfigVersion;

    if(m_pUserIdentityInfo != nullptr)
        delete m_pUserIdentityInfo;

    if(m_pTestItemInfo != nullptr)
        delete m_pTestItemInfo;

    if(m_pTestItemResult != nullptr)
        delete m_pTestItemResult;

    if(m_pDataSyncTime != nullptr)
        delete m_pDataSyncTime;

    if(m_pScoreRules != nullptr)
        delete m_pScoreRules;

    if(m_pClassInfo != nullptr)
        delete  m_pClassInfo;

    if(m_pCameraInfo != nullptr)
        delete  m_pCameraInfo;

    if(m_pDevHWInfoConfig != nullptr)
        delete  m_pDevHWInfoConfig;

    if(m_pDevSWInfoConfig != nullptr)
        delete  m_pDevSWInfoConfig;

    if(m_pDevTestInfoConfig != nullptr)
        delete  m_pDevTestInfoConfig;

    if( nullptr != m_pCommitTableConfig ){
        delete m_pCommitTableConfig;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// <函    数>	Check_Config_Database
/// <功    能>
///				检查配置文件是否存在，生成，升级
///	</功   能>
/// <参    数>
///
///	</参   数>
/// <返 回 值>
///
///</返 回 值>
/// <说    明>
///              如果对数据库表的设计发生了变化，则需要升级
///</说   明>
/// <作  者> 		zhy 	 				</作	者>
/// <创建日期>		23:6:2016  </创建日期>
/// <修	   改>                              </修     改>
/// </函   数>
///////////////////////////////////////////////////////////////////////////////
bool CDatabaseManage::checkConfigDatabase()
{

    bool bAvailable = false;

    m_pDatabaseChecker = CDatabase_Checker::GetInstance();

    //检测配置数据库文件是否存在
    if (!m_pDatabaseChecker->Check_Configuration_Database())
    {
        m_pDatabaseChecker->Get_Configuration_Database()->begin();

        //如果不存在则创建数据库文件，并自动生成全部配置表
        bAvailable = m_pDatabaseChecker->Generate_Configuration_Database();

        m_pDatabaseChecker->Get_Configuration_Database()->commit();
    }

    //对数据库文件版本进行检查和升级，数据库文件一定存在
    bAvailable = m_pDatabaseChecker->Check_Database_Version();

    return bAvailable;
}

void CDatabaseManage::initailizeConfigObject()
{
    m_pConfigVersion    = new CConfig_Version();

    m_pUserIdentityInfo = new CTableUserIdentityInfo();
    m_pTestItemInfo     = new CTableTestItemInfo();
    m_pTestItemResult   = new CTableTestItemResult();
    m_pDataSyncTime     = new CTableDataSyncTime();
    m_pScoreRules       = new CTableScoreRules();
    m_pClassInfo        = new CTableClassInfo();
    m_pCameraInfo       = new CCameraConfig();
    m_pDevHWInfoConfig  = new CDevHWInfoConfig();
    m_pDevSWInfoConfig  = new CDevSWInfoConfig();
    m_pDevTestInfoConfig= new CDevTestInfoConfig();
    m_pCommitTableConfig= new CTableCommitPopupConfig();
}

bool CDatabaseManage::checkConfigDatatable()
{
    if(m_pDatabaseChecker == nullptr)
        return false;

    bool bUpdate = m_pDatabaseChecker->Is_Need_Upgrade();

    bool bRet = true;
    //版本表仅更新
    bRet &= checkAndUpdateTable(m_pConfigVersion, false);

    bRet &= checkAndUpdateTable(m_pUserIdentityInfo, bUpdate);
    bRet &= checkAndUpdateTable(m_pTestItemInfo, bUpdate);
    bRet &= checkAndUpdateTable(m_pTestItemResult, bUpdate);
    bRet &= checkAndUpdateTable(m_pDataSyncTime, bUpdate);
    bRet &= checkAndUpdateTable(m_pScoreRules, bUpdate);
    bRet &= checkAndUpdateTable(m_pClassInfo, bUpdate);
    bRet &= checkAndUpdateTable(m_pCameraInfo, bUpdate);
    bRet &= checkAndUpdateTable(m_pDevHWInfoConfig, bUpdate);
    bRet &= checkAndUpdateTable(m_pDevSWInfoConfig, bUpdate);
    bRet &= checkAndUpdateTable(m_pDevTestInfoConfig, bUpdate);
    bRet &= checkAndUpdateTable(m_pCommitTableConfig, bUpdate );

    return false;
}

bool CDatabaseManage::checkAndUpdateTable(CAbstractConfigInfo *pInfo, bool bUpdate)
{
    bool bRet = false;

    if(pInfo != nullptr)
    {
        bRet = pInfo->Check_Config_Table();

        if(bUpdate)
            pInfo->Upgrade_Config_Table_Data();
    }

    return bRet;
}

CTableCommitPopupConfig *CDatabaseManage::getPCommitTableConfig() const
{
    if( nullptr != m_pCommitTableConfig ){
        return m_pCommitTableConfig;
    }

    return nullptr;
}

CDatabaseManage &CDatabaseManage::GetInstance()
{
    /// 1.--------------获取单实例唯一指针---------------
    if( s_pManagerInstance == nullptr)
        {
            /// 1.1--------------多线程同时调用时，线程安全锁---------------
            s_csManager_Instance_Safe.lock();

            /// 1.2--------------新建CConfig_Manager对象--------------------------
            if(s_pManagerInstance == nullptr)
            {
                s_pManagerInstance = new CDatabaseManage();
            }

            /// 1.3--------------多程程同步锁，解锁-------------------------
            s_csManager_Instance_Safe.unlock();
        }

        /// 2.--------------返回对象指针---------------------
        return *s_pManagerInstance;
}

CTableUserIdentityInfo *CDatabaseManage::getTableUserIdentityInfo()
{
    if(m_pUserIdentityInfo != nullptr)
        return m_pUserIdentityInfo;

    return nullptr;
}

CTableTestItemInfo *CDatabaseManage::getTabTestItemInfo()
{
    if(m_pTestItemInfo != nullptr)
        return m_pTestItemInfo;

     return nullptr;
}

CTableTestItemResult *CDatabaseManage::getTabTestItemResult()
{
    if(m_pTestItemResult != nullptr)
        return m_pTestItemResult;

     return nullptr;
}

CTableDataSyncTime *CDatabaseManage::getTabDataSyncTime()
{
    if(m_pDataSyncTime != nullptr)
        return m_pDataSyncTime;

     return nullptr;
}

CTableScoreRules *CDatabaseManage::getTabScoreRules()
{
    if(m_pScoreRules != nullptr)
        return m_pScoreRules;

    return nullptr;
}

CTableClassInfo *CDatabaseManage::getTabClassInfo()
{
    if(m_pClassInfo != nullptr)
        return m_pClassInfo;

    return nullptr;
}

CCameraConfig *CDatabaseManage::getTabCameraInfo()
{
    if(m_pCameraInfo != nullptr)
        return m_pCameraInfo;

    return nullptr;
}

CDevHWInfoConfig *CDatabaseManage::getDevHWInfoConfig()
{
    if(m_pDevHWInfoConfig != nullptr)
        return m_pDevHWInfoConfig;

    return nullptr;
}

CDevSWInfoConfig *CDatabaseManage::getDevSWInfoConfig()
{
    if(m_pDevSWInfoConfig != nullptr)
        return m_pDevSWInfoConfig;

    return nullptr;
}

CDevTestInfoConfig *CDatabaseManage::getDevTestInfoConfig()
{
    if(m_pDevTestInfoConfig != nullptr)
        return m_pDevTestInfoConfig;

    return nullptr;
}

const CServerFunConfig& CDatabaseManage::getServerFunConfig() const
{
    // TODO: 在此处插入 return 语句
    return m_serverFunConfig;
}

const CTestRunConfig& CDatabaseManage::getTestRunConfig() const
{
    // TODO: 在此处插入 return 语句
    return m_testRunCofing;
}

CTestProcessVideoPathConfig &CDatabaseManage::getTestProcessVideoPathConfig()
{
    // TODO: 在此处插入 return 语句
    return m_testProcessVideoPathConfig;
}

QMutex CDatabaseManage::s_csManager_Instance_Safe;

CDatabaseManage * CDatabaseManage::s_pManagerInstance = nullptr;
