///////////////////////////////////////////////////////////
//  CTestStateManage.cpp
//  Implementation of the Class CTestStateManage
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CTestStateManage.h"
#include <QDebug>


CTestStateManage::CTestStateManage()
    : m_shTestState(TEST_NOT_START)
    , m_shTestModel(USER_MODE)
{

}

CTestStateManage::~CTestStateManage()
{

}

void CTestStateManage::setTestState(const short &shTestStatus)
{
    if (shTestStatus != m_shTestState)
    {
        m_shTestState = shTestStatus;
        emit signalTestStateChange(shTestStatus);
    }
}
