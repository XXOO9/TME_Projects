#pragma once
#include "CAbstractConfigInfo.h"

using namespace Database;
using namespace litesql;

class COMMONSHARED_EXPORT CConfig_Version : public CAbstractConfigInfo
{
public:
	CConfig_Version(void);
	~CConfig_Version(void);

    string Get_Current_Config_Version();
	bool	Is_Latest_Version();

	virtual bool Check_Config_Table();
};

