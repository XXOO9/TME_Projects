///////////////////////////////////////////////////////////
//  CUserManage.h
//  Implementation of the Class CUserManage
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_3DDE9C06_29C8_4e4a_BCFF_81EDB401864A__INCLUDED_)
#define EA_3DDE9C06_29C8_4e4a_BCFF_81EDB401864A_INCLUDED_

#include "UserRoleManage.h"
#include "CUserInfo.h"
#include "CDatabaseManage.h"
#include <QObject>
#include <map>
#include <QDebug>
#include <QMetaType>

//在开启识别之后
class CUserManage : public QObject
{
    Q_OBJECT
public:
    explicit CUserManage();
	virtual ~CUserManage();
public slots:
signals:
private:
    CUserRoleManage m_obgUser_Role_Manage;
};
#endif // !defined(EA_3DDE9C06_29C8_4e4a_BCFF_81EDB401864A_INCLUDED_)
