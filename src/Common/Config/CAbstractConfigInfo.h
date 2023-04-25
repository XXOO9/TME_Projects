///////////////////////////////////////////////////////////
//  CAbstractConfigInfo.h
//  Implementation of the Class CAbstractConfigInfo
//  Created on:      29-十一月-2016 14:22:21
//  Original author: ZHY
///////////////////////////////////////////////////////////

#if !defined(EA_5E46B88C_57C5_4c3e_80A1_B23EB043FC2C_INCLUDED_)
#define EA_5E46B88C_57C5_4c3e_80A1_B23EB043FC2C_INCLUDED_


#include "litesql.hpp"
#include "ste100database.hpp"
#include "common_global.h"
#include <QMutex>
#include <string>

using std::string;

using namespace Database;
using namespace litesql;

#define  RESERVE_BASE_MAX  100000
/**
 * 配置逻辑父类
 */
class CAbstractConfigInfo
{

public:
	CAbstractConfigInfo();
	virtual ~CAbstractConfigInfo();

	/**
	 * 检查配置数据库中的对应的表中配置项是否存在
	 */
	virtual bool Check_Config_Table();

	/**
	 * 对需要升级的表中的列填入数据
	 */
	virtual bool Upgrade_Config_Table_Data();


    static bool isEqual(const float &a, const float &b);
protected:
    virtual void lock();
    virtual void unlock();

    static std::string GBK2UTF8(std::string &str);
private:
	/**
	 * 加载BL-420N数据库，获取数据库操作类对象
	 */
    void Load_Config_Database();

protected:

    //数据库
    STE100Database *m_pDatabase;

	//BL-420N系统默认的配置名称
    string	m_strDefaultConfigName;

    QMutex m_mutex;
};

#endif // !defined(EA_5E46B88C_57C5_4c3e_80A1_B23EB043FC2C_INCLUDED_)
