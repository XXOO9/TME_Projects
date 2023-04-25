///////////////////////////////////////////////////////////
//  CResoureModule.h
//  Implementation of the Class CResoureModule
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_AF6E1C46_D32B_4e71_8F23_AC9D7F965763_INCLUDED_)
#define EA_AF6E1C46_D32B_4e71_8F23_AC9D7F965763_INCLUDED_
#include <QString>
#include <vector>
using namespace std;

class CResoureModule
{

public:
	CResoureModule();
	virtual ~CResoureModule();

    vector<QString> getFileName();
	void setResourePath();

};
#endif // !defined(EA_AF6E1C46_D32B_4e71_8F23_AC9D7F965763_INCLUDED_)
