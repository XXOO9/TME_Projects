#include "CConfig_Version.h"
#include "Logger/CLogger.h"

const static string CURRENT_CONFIG_VRESION("0.0.0.2");

CConfig_Version::CConfig_Version(void)
{
}


CConfig_Version::~CConfig_Version(void)
{
}

bool CConfig_Version::Check_Config_Table()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

	bool bSuccess = false;

    ConfigVersion version(*m_pDatabase);

    //如果不存在记录，则创建一条记录，
    auto iCount = select<ConfigVersion>(*m_pDatabase).count();
    if (iCount == 0)
    {
        version.cfgVersion = CURRENT_CONFIG_VRESION;
        version.update();
    }
    else
    {
        //如果记录存在，则更新改记录的版本号，注意不能创建新的记录
        version = select<ConfigVersion>(*m_pDatabase).one();
        version.cfgVersion = CURRENT_CONFIG_VRESION;
        version.update();
    }

    bSuccess = true;

	return bSuccess;
}

string CConfig_Version::Get_Current_Config_Version()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return "";
    }

    string strVersion;

    ConfigVersion version(*m_pDatabase);

    Records recods = m_pDatabase->query("SELECT * FROM sqlite_master where type='table' and name='ConfigVersion_'");
    size_t count = recods.size();

    if (count != 0)
    {
        size_t iRecordCount = select<ConfigVersion>(*m_pDatabase).count();
        if (iRecordCount > 0)
        {
            version = select<ConfigVersion>(*m_pDatabase).one();

            strVersion = version.cfgVersion;
        }
    }

	return strVersion;
}

bool CConfig_Version::Is_Latest_Version()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

	ConfigVersion version = select<ConfigVersion>(*m_pDatabase).one();

    string strVersion = version.cfgVersion;

	if (strVersion < CURRENT_CONFIG_VRESION)
	{		
		return false;
	}
	else
	{
		return true;
	}
}
