#include "CDataBaseManager.h"

CDataBaseManager::CDataBaseManager(QObject *parent) : QObject(parent), m_pStuDataBase( nullptr )
{

}

void CDataBaseManager::openStuDataBase()
{
    m_pStuDataBase = CStudentInfoDataBase::getInstance();
}

CStudentInfoDataBase *CDataBaseManager::pStuDataBase() const
{
    return m_pStuDataBase;
}
