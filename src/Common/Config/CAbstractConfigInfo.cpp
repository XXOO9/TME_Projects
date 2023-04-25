///////////////////////////////////////////////////////////
//  CAbstractConfigInfo.cpp
//  Implementation of the Class CAbstractConfigInfo
//  Created on:      29-十一月-2016 14:22:22
//  Original author: ZHY
///////////////////////////////////////////////////////////
#include "CAbstractConfigInfo.h"
#include "CDatabase_Checker.h"
#include <cmath>
#include <QString>

CAbstractConfigInfo::CAbstractConfigInfo()
    : m_pDatabase(nullptr)
{
    m_strDefaultConfigName = ("STE100");

    Load_Config_Database();
}

CAbstractConfigInfo::~CAbstractConfigInfo()
{

}

bool CAbstractConfigInfo::Check_Config_Table()
{
	return true;
}

bool CAbstractConfigInfo::Upgrade_Config_Table_Data()
{
    return true;
}

bool CAbstractConfigInfo::isEqual(const float &a, const float &b)
{
    return fabs(a - b) < 0.0000001;
}

void CAbstractConfigInfo::lock()
{
    m_mutex.lock();
}

void CAbstractConfigInfo::unlock()
{
    m_mutex.unlock();
}

string CAbstractConfigInfo::GBK2UTF8(string &str)
{
    QString temp = QString::fromLocal8Bit(str.c_str());
    std::string ret = temp.toUtf8().data();
    return ret;
}

///////////////////////////////////////////////////////////////////////////////
/// <函    数>	Load_Bl420N_Config_Database
/// <功    能>  	
///				加载BL-420N配置数据库	
///	</功   能>
/// <参    数>	
///
///	</参   数>
/// <返 回 值>	    
///				
///</返 回 值>
/// <说    明>		
///              加载配置文件的Sqlite3数据库操作类
///</说   明>
/// <作  者> 		zhy 	 				</作	者>
/// <创建日期>		23:6:2016  </创建日期>
/// <修	   改>                              </修     改>
/// </函   数>
///////////////////////////////////////////////////////////////////////////////
void CAbstractConfigInfo::Load_Config_Database()
{
	m_pDatabase = CDatabase_Checker::GetInstance()->Get_Configuration_Database();
}
