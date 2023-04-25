#ifndef CGlog_H
#define CGlog_H

#include "ILog.h"
#include <vector>

/********************************************************************
    created:	2020/04/07
    created:	9:40
    filename: 	..\BL-420N\CommonService\CGlog.h
    file path:	..\BL-420N\CommonService
    file ext:	h
    author:		tcy

    purpose:	通过调用glog封装的类（田楚玥），用于Logger.h中
                日志模块统一调用
*********************************************************************/

using namespace std;

class CGlog :public ILog
{
public:
    CGlog();
    virtual ~CGlog();
    /*************************************************
     <函数名称>   Initialize
     <功    能>  对日志模块的初始化
     <参数说明>

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void initialize() override;
    virtual void unInitialize() override;

    /*************************************************
     <函数名称>   set_Log_Path
     <功    能>  设置日志模块存储路径
     <参数说明>  path 路径字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void set_Log_Path(const string& path) override;

    /*************************************************
     <函数名称>    set_Log_File
     <功    能>   设置日志文件名称
     <参数说明>    file_Name日志文件名称字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void set_Log_File(const string& file_Name) override;

    virtual void set_Max_Log_Size(unsigned int) override;
    virtual void set_Max_Log_Count(unsigned int) override;

    /*************************************************
     <函数名称>    set_Grade_Separation_Flag
     <功    能>   设置日志分级显示
     <参数说明>    bFlag 是否进行分级显示日志信息

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    void set_Grade_Separation_Flag(bool bFlag) override;

    /*************************************************
    <函数名称>    set_Output_Level
    <功    能>   设置日志信息输出等级
    <参数说明>    nLevel 最低输出等级

    <返回值>     无
    <函数说明>
    <作    者>   tcy
    <时    间>   2018/4/26
   **************************************************/
    void set_Output_Level(int nLevel) override;


    /*************************************************
     <函数名称>     log_Info
     <功    能>    一般性提示日志
     <参数说明>     info输出为日志信息的字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void log_Info(const string& info) override;

    /*************************************************
     <函数名称>     log_Warring
     <功    能>    警告性日志
     <参数说明>     warring输出为日志信息的字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void log_Warring(const string& warring) override;

    /*************************************************
     <函数名称>     log_Error
     <功    能>    错误性日志
     <参数说明>     error输出为日志信息的字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void log_Error(const string& error) override;

    /*************************************************
     <函数名称>     log_Assert
     <功    能>    错误性日志
     <参数说明>     fatal输出为日志信息的字符串;

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void log_Fatal(const string& fatal) override;
private:
    /*************************************************
     <函数名称>    checkLogFileCount
     <功能描述>    对所给目录下的日志文件数目加以控制
     <参数说明>    logFilePath  文件目录

     <返 回 值>    无
     <函数说明>
     <作    者>   tcy
     <时    间>   2019/11/14
     **************************************************/
    void checkLogFileCount(string logFilePath);

    /*************************************************
     <函数名称>    getLogFileCount
     <功能描述>    获取当前目录下日志文件数目
     <参数说明>    logFilePath 日志文件目录

     <返 回 值>    日志文件个数
     <函数说明>
     <作    者>   tcy
     <时    间>   2019/11/14
     **************************************************/
    int getLogFileCount(string logFilePath);

    /*************************************************
     <函数名称>    delteLogFile
     <功能描述>    删除超过文件数目的日志
     <参数说明>

     <返 回 值>    无
     <函数说明>
     <作    者>   tcy
     <时    间>   2019/11/14
     **************************************************/
    void deleteLogFile(string logFilePath);

    /*************************************************
     <函数名称>    getLogFileDate
     <功能描述>    获取日志文件生成日期
     <参数说明>    fileName 日志文件名

     <返 回 值>    日期字符串
     <函数说明>
     <作    者>   tcy
     <时    间>   2019/11/14
     **************************************************/
    string getLogFileDate(string fileName);

private:
    vector <string>  m_LogFile_Name_Array;
	string  m_strFile_Path;
	string  m_strFile_Name;
	bool    m_bInitialize;
	int     m_iMax_File_Count;
	int     m_iMax_Size;
	bool    m_bGrade_Separation_Flag;
	int     m_iMin_Output_Level;
private:
    CGlog(const CGlog&);
    //  禁止拷贝
    const CGlog& operator = (const CGlog&);
};

#endif
