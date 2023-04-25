///////////////////////////////////////////////////////////
//  CDatabase_Checker.h
//  Implementation of the Class CDatabase_Checker
//  Created on:      29-十一月-2016 14:22:21
//  Original author: ZHY
///////////////////////////////////////////////////////////

#if !defined(EA_A2CB816A_9812_41e5_B30B_EBBB15E40ACA_INCLUDED_)
#define EA_A2CB816A_9812_41e5_B30B_EBBB15E40ACA_INCLUDED_

#include "litesql.hpp"
#include "ste100database.hpp"
#include <string>
#include <QMutex>

using namespace Database;
using namespace litesql;

using std::string;

const string DatabaseName				= "sqlite3";				//配置数据库名称
const string DBFileName					= "STE100Database.db";		//默认数据文件名称

class CDatabase_Checker
{

private:
	CDatabase_Checker();

	CDatabase_Checker(const CDatabase_Checker &);													
	CDatabase_Checker& operator = (const CDatabase_Checker &);	

public:
	virtual ~CDatabase_Checker();

public:
	static CDatabase_Checker *GetInstance();

	//检查配置数据文件是否存在
	bool	Check_Configuration_Database();

    //生成数据库配置文件，前提是数据库配置文件不存在
	bool	Generate_Configuration_Database();

	//如果数据库文件存在，检查数据库文件的版本，并进行必要的升级
	bool	Check_Database_Version();

	//判断数据库文件是否需要升级，此时已经完成了数据结构上的升级，
	//需要在每个配置类中对数据进行升级
	bool	Is_Need_Upgrade();

	//获取BL-420N数据库配置对象
    STE100Database *Get_Configuration_Database();
private:
	static CDatabase_Checker			*s_pInstance;
    static QMutex                       s_csInstance_Safe;

	//配置文件路径
    string								m_strConfigDBPath;
    STE100Database                      *m_pConfigDatabase;

	//配置文件是否重新生成
	bool								m_bReCreated;

	//是否需要对数据进行升级
	bool								m_bUpgrade;

};
#endif // !defined(EA_A2CB816A_9812_41e5_B30B_EBBB15E40ACA_INCLUDED_)
