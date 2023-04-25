#include "CLogger.h"
#include <QDebug>
#include <QString>

CLogger::CLogger()
	: m_strFile_Name("log")
    , m_strFile_Path("")
    , m_shMin_Outout_Level(LOG_WARN)
    , m_nMax_Log_File_Size(10)
	, m_nMax_Log_Count(30)
	, m_bGrade_Separation_Flag(false)
    , m_pLog_Object(nullptr)
    , m_shLog_Type(LOG_TYPE_GLOG)
{
	
}

CLogger::CLogger(short shType)
    : m_strFile_Name("log")
    , m_strFile_Path("")
    , m_shMin_Outout_Level(LOG_WARN)
    , m_nMax_Log_File_Size(10)
    , m_nMax_Log_Count(30)
    , m_bGrade_Separation_Flag(false)
    , m_pLog_Object(nullptr)
    , m_shLog_Type(shType)
{

}

CLogger::~CLogger()
{
    if (m_pLog_Object == nullptr)
		return;

    if(m_pLog_Object != nullptr)
        delete m_pLog_Object;
}

CLogger* CLogger::Get_Instence(short shLogType)
{
	/// 1.--------------获取单实例唯一指针---------------
    if (s_pManagerInstance == nullptr)
	{
		/// 1.1--------------多线程同时调用时，线程安全锁---------------
        s_mutex_Manager_Instance_Safe.lock();

		/// 1.2--------------新建CConfig_Manager对象--------------------------
        if (s_pManagerInstance == nullptr)
		{
			s_pManagerInstance = new CLogger(shLogType);
		}

		/// 1.3--------------多程程同步锁，解锁-------------------------
        s_mutex_Manager_Instance_Safe.unlock();
	}

	/// 2.--------------返回对象指针---------------------
	return s_pManagerInstance;
	
}

void CLogger::release_Instence()
{
    if(s_pManagerInstance != nullptr)
	{
		delete s_pManagerInstance;
	}
}

void CLogger::initialize()
{
    qDebug() << "CLogger::initialize" << endl;
	if(m_shLog_Type == LOG_TYPE_GLOG)
	{
		m_pLog_Object = new CGlog();
	}

    if (m_pLog_Object != nullptr)
	{
        m_pLog_Object->set_Log_File(m_strFile_Name);
        m_pLog_Object->set_Log_Path(m_strFile_Path);
        m_pLog_Object->set_Max_Log_Count(static_cast<unsigned int>(m_nMax_Log_Count));
        m_pLog_Object->set_Max_Log_Size(static_cast<unsigned int>(m_nMax_Log_File_Size));
		m_pLog_Object->set_Grade_Separation_Flag(m_bGrade_Separation_Flag);

		m_pLog_Object->initialize();
	}
}

void CLogger::set_Log_Type(short shLogType)
{
	m_shLog_Type = shLogType;
}

void CLogger::set_Log_Path(const string& path)
{
	m_strFile_Path = path;
}

void CLogger::set_Log_File(const string& file_Name)
{
	m_strFile_Name = file_Name;
}

void CLogger::set_Max_Log_Size(unsigned int nSize)
{
    m_nMax_Log_File_Size = static_cast<int>(nSize);
}

void CLogger::set_Max_Log_Count(unsigned int nCount)
{
    m_nMax_Log_Count = static_cast<int>(nCount);
}

void CLogger::set_Min_Output_Level(short shLevel)
{
	if (shLevel < LOG_INFO || shLevel >= LOG_LEVEL_COUNT)
		return;

	m_shMin_Outout_Level = shLevel;
}

void CLogger::set_Grade_Separation_Flag(bool bFlag)
{
	m_bGrade_Separation_Flag = bFlag;
}

string CLogger::get_Log_Path()
{
	return m_strFile_Path;
}

string CLogger::get_Log_File()
{
	return m_strFile_Name;
}

int CLogger::get_Max_Log_Size()
{
	return m_nMax_Log_File_Size;
}

int CLogger::get_Max_Log_Count()
{
	return m_nMax_Log_Count;
}

short CLogger::get_Min_Output_Level()
{
	return m_shMin_Outout_Level;
}

bool CLogger::get_Grade_Separation_Flag()
{
	return m_bGrade_Separation_Flag;
}

bool CLogger::log_Info(const string& str)
{
    if (m_pLog_Object == nullptr)
		return false;

	m_pLog_Object->log_Info(str);

	return true;
}

bool CLogger::log_Warring(const string& str)
{
    if (m_pLog_Object == nullptr)
		return false;

	m_pLog_Object->log_Warring(str);

	return true;
}

bool CLogger::log_Error(const string& str)
{
    if (m_pLog_Object == nullptr)
		return false;

	m_pLog_Object->log_Error(str);

	return true;
}

bool CLogger::log_Fatal(const string& str)
{
    if (m_pLog_Object == nullptr)
		return false;

	m_pLog_Object->log_Fatal(str);

	return true;
}

QMutex CLogger::s_mutex_Manager_Instance_Safe;
CLogger* CLogger::s_pManagerInstance = nullptr;
