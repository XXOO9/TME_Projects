///////////////////////////////////////////////////////////
//  CDatabaseManage.h
//  Implementation of the Class CDatabaseManage
//  Created on:      13-9月-2021 18:39:21
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_FD82E560_B830_4901_A579_BE07604FBD38_INCLUDED_)
#define EA_FD82E560_B830_4901_A579_BE07604FBD38_INCLUDED_
#include "CUserInfo.h"
#include "CTestResultInfo.h"
#include "CDatabase_Checker.h"
#include "CConfig_Version.h"
#include "CTableUserIdentityInfo.h"
#include "CTableTestItemInfo.h"
#include "CTableTestItemResult.h"
#include "CTableDataSyncTime.h"
#include "CTableScoreRules.h"
#include "CTableClassInfo.h"
#include "CCameraConfig.h"
#include "CServerFunConfig.h"
#include "CTestRunConfig.h"
#include "CTestProcessVideoPathConfig.h"
#include "CDevHWInfoConfig.h"
#include "CDevSWInfoConfig.h"
#include "CDevTestInfoConfig.h"
#include "CTableCommitPopupConfig.h"
#include "CCommonCfg.h"

class COMMONSHARED_EXPORT CDatabaseManage
{
private:
    CDatabaseManage();
    ~CDatabaseManage();
    CDatabaseManage(const CDatabaseManage &manager);
    CDatabaseManage &operator = (const CDatabaseManage &manager);

    /**
         * 配置管理者静态实例对象
         */
    static CDatabaseManage		*s_pManagerInstance;
    static QMutex				s_csManager_Instance_Safe;

    //------------------------------配置文件相关操作-------------------------------
    //检查配置数据库，包括：文件存在、数据库升级、配置默认值
    bool	checkConfigDatabase();

    //在检查完数据库之后初始化配置逻辑处理类对象
    void	initailizeConfigObject();

    //检查数据表中是否存在配置数据，如果不存在写入默认项
    bool	checkConfigDatatable();

    //更新检查同时更新
    bool checkAndUpdateTable(CAbstractConfigInfo *info, bool bUpdate);
private:
    //数据库检测者，完成对数据库文件的一些操作
    CDatabase_Checker *m_pDatabaseChecker;

    //表格操作对象
    CConfig_Version        *m_pConfigVersion;

    CTableUserIdentityInfo        *m_pUserIdentityInfo;
    CTableTestItemInfo            *m_pTestItemInfo;
    CTableTestItemResult          *m_pTestItemResult;
    CTableDataSyncTime            *m_pDataSyncTime;
    CTableScoreRules              *m_pScoreRules;
    CTableClassInfo               *m_pClassInfo;
    CCameraConfig                 *m_pCameraInfo;
    CServerFunConfig               m_serverFunConfig;
    CTestRunConfig                 m_testRunCofing;
    CTestProcessVideoPathConfig    m_testProcessVideoPathConfig;
    CDevHWInfoConfig              *m_pDevHWInfoConfig;
    CDevSWInfoConfig              *m_pDevSWInfoConfig;
    CDevTestInfoConfig            *m_pDevTestInfoConfig;
    CTableCommitPopupConfig       *m_pCommitTableConfig;
public:
    static CDatabaseManage &GetInstance();

    CTableUserIdentityInfo *getTableUserIdentityInfo();
    CTableTestItemInfo     *getTabTestItemInfo();
    CTableTestItemResult   *getTabTestItemResult();
    CTableDataSyncTime     *getTabDataSyncTime();
    CTableScoreRules       *getTabScoreRules();
    CTableClassInfo        *getTabClassInfo();
    CCameraConfig          *getTabCameraInfo();
    CDevHWInfoConfig       *getDevHWInfoConfig();
    CDevSWInfoConfig       *getDevSWInfoConfig();
    CDevTestInfoConfig     *getDevTestInfoConfig();
    const CServerFunConfig& getServerFunConfig() const;
    const CTestRunConfig&   getTestRunConfig() const;
    CTestProcessVideoPathConfig& getTestProcessVideoPathConfig();
    CTableCommitPopupConfig *getPCommitTableConfig() const;
};
#endif // !defined(EA_FD82E560_B830_4901_A579_BE07604FBD38_INCLUDED_)
