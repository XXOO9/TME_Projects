///////////////////////////////////////////////////////////
//  CDatabase_Checker.cpp
//  Implementation of the Class CDatabase_Checker
//  Created on:      29-十一月-2016 14:22:21
//  Original author: ZHY
///////////////////////////////////////////////////////////
#include "CDatabase_Checker.h"
#include "CConfig_Version.h"
#include "CFilePath.h"
#include "Logger/CLogger.h"


CDatabase_Checker * CDatabase_Checker::GetInstance()
{
	/// 1.--------------获取单实例唯一指针---------------
    if( s_pInstance == nullptr)
	{   
		/// 1.1--------------多线程同时调用时，线程安全锁---------------
        s_csInstance_Safe.lock();

		/// 1.2--------------新建CDatabase_Checker对象--------------------------
        if(s_pInstance == nullptr)
		{
			s_pInstance = new CDatabase_Checker();
		}

		/// 1.3--------------多程程同步锁，解锁-------------------------
        s_csInstance_Safe.unlock();
	}

	/// 2.--------------返回对象指针---------------------
	return s_pInstance;
}

CDatabase_Checker::CDatabase_Checker()
    : m_pConfigDatabase(nullptr)
    , m_bReCreated(false)
    , m_bUpgrade(false)
{

}

CDatabase_Checker::~CDatabase_Checker()
{
    if (m_pConfigDatabase != nullptr)
	{
		delete m_pConfigDatabase;
        m_pConfigDatabase = nullptr;
	}

    if (s_pInstance != nullptr)
	{
		delete s_pInstance;
        s_pInstance = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////
/// <函    数>	Check_Configuration_Database
/// <功    能>  	
///				判断BL-420N的数据库文件是否存在	
///	</功   能>
/// <参    数>	
///
///	</参   数>
/// <返 回 值>	    
///				bool：true	数据库文件存在
///                   false 数据库文件不存在
///</返 回 值>
/// <说    明>		
///              
///</说   明>
/// <作  者> 		zhy 	 				</作	者>
/// <创建日期>		23:6:2016  </创建日期>
/// <修	   改>                              </修     改>
/// </函   数>
///////////////////////////////////////////////////////////////////////////////
bool CDatabase_Checker::Check_Configuration_Database()
{
	bool retResult = false;

	///初始化数据库文件路径
    string strCurrent_Path = CFilePath::getAppDirPath();

    m_strConfigDBPath = strCurrent_Path + "/";
    m_strConfigDBPath += DBFileName;

    if (CFilePath::isFileExist(m_strConfigDBPath))
	{
		retResult = true;
	}
	else
	{
        retResult = false;
	}		

	return retResult;
}

///////////////////////////////////////////////////////////////////////////////
/// <函    数>	Generate_Configuration_Database
/// <功    能>  	
///				创建配置数据库文件	
///	</功   能>
/// <参    数>	
///
///	</参   数>
/// <返 回 值>	    
///				bool：true	创建数据库文件成功
///                   false 创建数据库文件失败		
///</返 回 值>
/// <说    明>		
///              前提是数据库文件不存在，需要创建，此时创建的数据库中已经包含了
///              各个配置项的表，对于表中的数据和记录需要在各个配置逻辑处理类中
///              进行判断是否存在、验证、产生默认值。
///</说   明>
/// <作  者> 		zhy 	 				</作	者>
/// <创建日期>		23:6:2016  </创建日期>
/// <修	   改>                              </修     改>
/// </函   数>
///////////////////////////////////////////////////////////////////////////////
bool CDatabase_Checker::Generate_Configuration_Database()
{
    bool retExist = false;

    string dbFilePathConfig = "database=" + m_strConfigDBPath;

    //创建数据库,configDB("sqlite3", "STE100Database.db");
    m_pConfigDatabase = new STE100Database(DatabaseName, dbFilePathConfig.c_str());

    //提交数据库事务，在这里会创建BL-420N全部的配置项的数据库表
    m_pConfigDatabase->verbose = true;
    m_pConfigDatabase->begin();
    m_pConfigDatabase->create();
    m_pConfigDatabase->commit();

    m_bReCreated = true;
	return retExist;
}

///////////////////////////////////////////////////////////////////////////////
/// <函    数>	Get_Configuration_Database
/// <功    能>  	
///				获取配置数据库操作对象指针	
///	</功   能>
/// <参    数>	
///
///	</参   数>
/// <返 回 值>	    
///				BL420NConfiguration * 返回数据库操作类对象指针				
///</返 回 值>
/// <说    明>		
///              通过该指针来操作数据
///</说   明>
/// <作  者> 		zhy 	 				</作	者>
/// <创建日期>		23:6:2016  </创建日期>
/// <修	   改>                              </修     改>
/// </函   数>
///////////////////////////////////////////////////////////////////////////////
STE100Database * CDatabase_Checker::Get_Configuration_Database()
{
    if (m_pConfigDatabase == nullptr)
	{
        string dbFilePathConfig = ("database=") + m_strConfigDBPath;

        //创建数据库,configDB("sqlite3", "STE100Database.db");
        m_pConfigDatabase = new STE100Database(DatabaseName, dbFilePathConfig.c_str());
	}	
	
	return m_pConfigDatabase;
}

///////////////////////////////////////////////////////////////////////////////
/// <函    数>	Check_Database_Version
/// <功    能>  	
///				判断数据是否需要升级，如果需要并进行升级	
///	</功   能>
/// <参    数>	
///
///	</参   数>
/// <返 回 值>	    
///				bool
///</返 回 值>
/// <说    明>		
///             如果在开发过程中对数据库的表进行了修改（也即是对配置项发生了变化），
///             此时需要对配置数据库进行升级
///</说   明>
/// <作  者> 		zhy 	 				</作	者>
/// <创建日期>		23:6:2016  </创建日期>
/// <修	   改>                              </修     改>
/// </函   数>
///////////////////////////////////////////////////////////////////////////////
bool CDatabase_Checker::Check_Database_Version()
{
	//默认为不成功
	bool bSuccess = false;

    //判断BL-420N配置数据是否存在
    if (m_pConfigDatabase == nullptr)
    {
        //如果不存在则重新获取
        Get_Configuration_Database();
    }

    //如果没有重新创建，则判断配置文件的版本
    if (!m_bReCreated)
    {
        CConfig_Version cfgVersion;
        //数据库文件版本配置为null，删除数据库文件重新生成，并且写入，默认配置
        if (cfgVersion.Get_Current_Config_Version().empty())
        {
            delete m_pConfigDatabase;
            m_pConfigDatabase = nullptr;
            CFilePath::deleteFileOrFolder(m_strConfigDBPath);

            Generate_Configuration_Database();
            Get_Configuration_Database();

            //写入默认配置
            CConfig_Version cfgVersion;
            cfgVersion.Check_Config_Table();
        }
        else
        {
            if (!cfgVersion.Is_Latest_Version())
            {
                m_bUpgrade = true;
            }
        }
    }

    //判断是否需要升级
    if (m_pConfigDatabase->needsUpgrade())
    {
        m_bUpgrade = true;

        //升级
        m_pConfigDatabase->upgrade();
    }
    bSuccess = true;

	return bSuccess;
}

bool CDatabase_Checker::Is_Need_Upgrade()
{
	return m_bUpgrade;
}

QMutex CDatabase_Checker::s_csInstance_Safe;

CDatabase_Checker	* CDatabase_Checker::s_pInstance = nullptr;
