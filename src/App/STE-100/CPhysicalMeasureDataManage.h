///////////////////////////////////////////////////////////
//  CPhysicalMeasureDataManage.h
//  Implementation of the Class CPhysicalMeasureDataManage
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_88F7B603_D72E_46de_AE86_E8BD1D7E5C2C_INCLUDED_)
#define EA_88F7B603_D72E_46de_AE86_E8BD1D7E5C2C_INCLUDED_

//#include "DatabaseManage.h"

class CPhysicalMeasureDataManage
{

public:
	CPhysicalMeasureDataManage();
	virtual ~CPhysicalMeasureDataManage();
    //CDatabaseManage *m_CDatabaseManage;

	bool addTestInfo();
	void getTestInfo();

};
#endif // !defined(EA_88F7B603_D72E_46de_AE86_E8BD1D7E5C2C_INCLUDED_)
