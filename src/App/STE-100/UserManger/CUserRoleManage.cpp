///////////////////////////////////////////////////////////
//  CUserRoleManage.cpp
//  Implementation of the Class CUserRoleManage
//  Created on:      13-9月-2021 18:39:23
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "UserRoleManage.h"


CUserRoleManage::CUserRoleManage()
{

}



CUserRoleManage::~CUserRoleManage()
{

}





/**
 * 获取使用者类型
 */
short CUserRoleManage::getUserType()
{

	return 0;
}


void CUserRoleManage::onUserChange()
{

}


void CUserRoleManage::setUserType(short shUserType)
{
    if(shUserType >= SYSTEM_USER_UNKNOW)
        return;
}
