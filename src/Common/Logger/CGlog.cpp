//#define  GLOG_NO_ABBREVIATED_SEVERITIES
#include "CGlog.h"
#include "logging.h"
#include "CFilePath.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfoList>
#include <QDebug>

CGlog::CGlog()
    : m_bInitialize(false)
    , m_iMax_File_Count(5)
	, m_iMax_Size(10)
    , m_bGrade_Separation_Flag(false)
	, m_iMin_Output_Level(google::GLOG_INFO)

{
	//初始化成员变量
    QString qsTemp;
    qsTemp = QCoreApplication::applicationDirPath();
    m_strFile_Path = qsTemp.toStdString();

    m_strFile_Name = "ste-100";
}

CGlog::~CGlog()
{
	unInitialize();
}

void CGlog::initialize()
{
    if(!CFilePath::isDirExist(m_strFile_Path, true))
    {
        //日志创建失败
    }

	checkLogFileCount(m_strFile_Path);

	std::string fileNameStr(m_strFile_Name);
	const char* fileName = fileNameStr.c_str();

    QString qs = QString::fromStdString(fileNameStr);
    qDebug() << qs;
    qs = QString::fromStdString(m_strFile_Path);
    qDebug() << qs;
	google::InitGoogleLogging(fileName);


	//是否将所有日志输出到stderr，而非文件.
    FLAGS_logtostderr = 0;

    std::string pathStr(m_strFile_Path);

	//设置日志路径
	FLAGS_log_dir = pathStr;

	//设置日志最大大小
	FLAGS_max_log_size = static_cast<google::int32>(m_iMax_Size);

	//磁盘满后是否停写
	FLAGS_stop_logging_if_full_disk = true;

	//设置按等级区分log文件
	FLAGS_servitysinglelog = true;

	//不分级输出日志文件(输出至一个文件)
	string destDir = m_strFile_Path + m_strFile_Name + "-";

	std::string destDirStr(destDir);

	string strErr, strWarning, strInfo, strFatal;
	strErr = strWarning = strFatal = strInfo = destDirStr;


	if (m_bGrade_Separation_Flag)
	{
		strErr += "-ERROR-";
		strInfo += "-INFO-";
		strWarning += "-WARNING-";
		strFatal += "-FATAL-";
	}

	//设置 google::FATAL 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::GLOG_FATAL, strFatal.c_str());

	//设置 google::ERROR 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::GLOG_ERROR, strErr.c_str());

	//设置 google::WARNING 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::GLOG_WARNING, strWarning.c_str());

	//设置 google::INFO 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::GLOG_INFO, strInfo.c_str());

	//设置文件名扩展，此处科自由添加自己需要的扩展名
	google::SetLogFilenameExtension("Tme");


	//缓冲日志输出，默认为30秒，此处改为立即输出
	FLAGS_logbufsecs = 0;

	//标识该类已被初始化
	m_bInitialize = true;
	
	google::FlushLogFiles(m_iMin_Output_Level);
}

void CGlog::unInitialize()
{
	google::ShutdownGoogleLogging();
	m_bInitialize = false;
}

void CGlog::set_Log_Path(const string& path)
{
	m_strFile_Path = path;
}

void CGlog::set_Max_Log_Size(unsigned int size)
{
    m_iMax_Size = static_cast<int>(size);
}

void CGlog::set_Max_Log_Count(unsigned int count)
{
    m_iMax_File_Count = static_cast<int>(count);
}

void CGlog::set_Grade_Separation_Flag(bool bFlag)
{
	m_bGrade_Separation_Flag = bFlag;
}

void CGlog::set_Output_Level(int nLevel)
{
	if (nLevel >= google::GLOG_INFO && nLevel <= google::GLOG_FATAL)
	{
		m_iMin_Output_Level = nLevel;
	}
}

void CGlog::set_Log_File(const string& file_Name)
{
	m_strFile_Name = file_Name;
}

void CGlog::log_Info(const string& info)
{
	if (!m_bInitialize)
	{
		return;
	}

	LOG(INFO) << info;
}

void CGlog::log_Warring(const string& warring)
{
	if (!m_bInitialize)
	{
		return;
	}

	LOG(WARNING) << warring;
}

void CGlog::log_Error(const string& error)
{
	if (!m_bInitialize)
	{
		return;
	}
	LOG(ERROR) << error;
}

void CGlog::log_Fatal(const string& fatal)
{
	if (!m_bInitialize)
	{
		return;
	}
	LOG(FATAL) << fatal;
}

void CGlog::checkLogFileCount(string logFilePath)
{
	int count = getLogFileCount(logFilePath);
	if (count > m_iMax_File_Count)
	{
		deleteLogFile(logFilePath);
	}
}

int CGlog::getLogFileCount(string logFilePath)
{
    //判断路径是否存在
    QString qsTemp;
    qsTemp = qsTemp.fromStdString(logFilePath);
    QDir dir (qsTemp);
    if(!dir.exists())
    {
        qDebug()<<"Checked file path does not exist!";
    }

    //通过文件类型来过滤
    QStringList filter;
    filter<<"*.log";

    dir.setNameFilters(filter);
    dir.setFilter( QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting( QDir::Time | QDir::Reversed );    //按照时间排序

    QFileInfoList folder_List = dir.entryInfoList();

    QString log_File_Name;
    for( int i=0 ; i<folder_List.size(); i++ )
    {
        log_File_Name = folder_List.at(i).absoluteFilePath();
        m_LogFile_Name_Array.push_back(log_File_Name.toStdString());
    }

    int log_File_Count = static_cast<int>(m_LogFile_Name_Array.size());

    return log_File_Count;
}

void CGlog::deleteLogFile(string logFilePath)
{
	string tempDateString;

	vector<string>::iterator iter = m_LogFile_Name_Array.begin();
	string firstLogFileName = *iter;
    unsigned int index = 0;

	string firstDate = getLogFileDate(firstLogFileName);

	for (; iter != m_LogFile_Name_Array.end(); ++iter)
	{
		tempDateString = getLogFileDate(*iter);
		if (firstDate > tempDateString)
		{
			firstDate = tempDateString;
			++index;
		}
	}

    string needDeleteFileName = m_LogFile_Name_Array[index];
    QString qsTemp;
    qsTemp = qsTemp.fromStdString(needDeleteFileName);
    QFile delFile(qsTemp);
    if(delFile.exists())
    {
        delFile.remove();
    }
}

string CGlog::getLogFileDate(string strfileName)
{
    int dashPos, pointPos, lengthOfDate;
    QString dateStr, fileName(strfileName.c_str());

    dashPos = fileName.lastIndexOf('-');
    pointPos = fileName.lastIndexOf('.');
    lengthOfDate = pointPos - dashPos;
    dateStr = fileName.mid(dashPos+1, lengthOfDate-1);

    return dateStr.toStdString();
}
