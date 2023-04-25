///////////////////////////////////////////////////////////
//  CUserRoleManage.h
//  Implementation of the Class CUserRoleManage
//  Created on:      13-9月-2021 18:39:23
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_9B59A687_9BE2_4c46_B28E_DEB6CB5E0D71_INCLUDED_)
#define EA_9B59A687_9BE2_4c46_B28E_DEB6CB5E0D71_INCLUDED_

/**
 * 用户角色管理
 */
class CUserRoleManage
{
public:
    enum enumUserRole{
        SYSTEM_USER_STUDENT,
        SYSTEM_USER_TEACHER,
        SYSTEM_USER_ADMIN,
        SYSTEM_USER_UNKNOW,
    };

	CUserRoleManage();
	virtual ~CUserRoleManage();

	short getUserType();
	void onUserChange();
	void setUserType(short shUserType);

private:
	short m_shUserType;

};
#endif // !defined(EA_9B59A687_9BE2_4c46_B28E_DEB6CB5E0D71_INCLUDED_)
