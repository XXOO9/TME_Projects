#ifndef CLogger_H
#define CLogger_H
#include "common_global.h"
#include "CGlog.h"
#include "QMutex"

#define instLog CLogger::Get_Instence(CLogger::LOG_TYPE_GLOG)

#define LOGINFO(info) \
	{					\
        string sLog(__FILE__); \
		sLog += "--"; \
		sLog += __FUNCTION__; \
		sLog += "--"; \
		sLog += info; \
		instLog->log_Info(sLog); \
	}
	

#define LOGWARN(warn) \
	{					\
        string sLog(__FILE__); \
		sLog += "--"; \
		sLog += __FUNCTION__; \
		sLog += "--"; \
		sLog += warn; \
		instLog->log_Warring(sLog); \
	}

#define LOGERROR(error) \
	{					\
        string sLog(__FILE__); \
		sLog += "--"; \
		sLog += __FUNCTION__; \
		sLog += "--"; \
		sLog += error; \
		instLog->log_Error(sLog); \
	}

#define LOGFATAL(fatal) \
	{					\
        string sLog(__FILE__); \
		sLog += "--"; \
		sLog += __FUNCTION__; \
		sLog += "--"; \
		sLog += fatal; \
		instLog->log_Fatal(sLog); \
    }
class COMMONSHARED_EXPORT CLogger
{
public:
	//使用的日志类型
	enum LOG_TYPE
	{
		LOG_TYPE_GLOG
	};

	//日志等级
	enum LOG_LEVEL
	{
		LOG_INFO = 0, LOG_WARN, LOG_ERROR, LOG_FATAL, LOG_LEVEL_COUNT
	};
public:
	static CLogger* Get_Instence(short);
	static void release_Instence();

	void initialize();
	
	//以下设置函数需要在日志模块初始化之前调用
	void set_Log_Type(short shLogType);

    void set_Log_Path(const string& path);
    void set_Log_File(const string& file_Name);

    void set_Max_Log_Size(unsigned int);
    void set_Max_Log_Count(unsigned int);
	
	void set_Min_Output_Level(short shLevel);
	void set_Grade_Separation_Flag(bool bFlag);

	string get_Log_Path();
	string get_Log_File();
	int get_Max_Log_Size();
	int get_Max_Log_Count();
	short get_Min_Output_Level();
	bool get_Grade_Separation_Flag();

	bool log_Info(const string&);
	bool log_Warring(const string&);
	bool log_Error(const string&);
	bool log_Fatal(const string&);
protected:
	CLogger();
	CLogger(const CLogger& manager);
	CLogger& operator = (const CLogger& manager);
	~CLogger();
	CLogger(short shType);
protected:
	//文件名 默认文件名为 名称 + "-日期"
	string	m_strFile_Name;
	//文件路径
	string	m_strFile_Path;
	//日志最低输出等级
	short	m_shMin_Outout_Level;
	//日志文件最大大小,单位 M
    int m_nMax_Log_File_Size;
	//日志文件数量
    int m_nMax_Log_Count;
	//日志是否分级存储
	bool	 m_bGrade_Separation_Flag;

	//统一调用的日志文件
	ILog* m_pLog_Object;

	//当前使用日志类型
	short m_shLog_Type;
private:
	/**
	 * 配置管理者静态实例对象
	 */
	static CLogger* s_pManagerInstance;
    static QMutex s_mutex_Manager_Instance_Safe;
};
#endif
