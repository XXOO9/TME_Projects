#ifndef ILog_H
#define ILog_H
/**************************************************/
/********** imports / exports Logger.dll **********/

//#ifdef LOGGER_EXPORTS
//#define LOGGER_API __declspec(dllexport)
////#define TRY_INLINE	inline
//#else
//#define LOGGER_API __declspec(dllimport)
////#define TRY_INLINE
//#endif

#include <string>
using namespace std;


//class LOGGER_API ILog
class ILog
{
public:
    virtual ~ILog(){}
public:
	 /*************************************************
     <函数名称>   Initialize
     <功    能>  对日志模块的初始化
     <参数说明>

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void initialize() = 0;
    virtual void unInitialize() = 0;

    /*************************************************
     <函数名称>   set_Log_Path
     <功    能>  设置日志模块存储路径
     <参数说明>  path 路径字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void set_Log_Path(const string& path) = 0;

    /*************************************************
     <函数名称>    set_Log_File
     <功    能>   设置日志文件名称
     <参数说明>    file_Name日志文件名称字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void set_Log_File(const string& file_Name) = 0;

    /*************************************************
     <函数名称>    log_Info
     <功    能>   一般性提示日志
     <参数说明>    info输出为日志信息的字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void log_Info(const string& info) = 0;

    virtual void set_Max_Log_Size(unsigned int) = 0;
    virtual void set_Max_Log_Count(unsigned int) = 0;

    /*************************************************
     <函数名称>    set_Grade_Separation_Flag
     <功    能>   设置日志分级显示
     <参数说明>    bFlag 是否进行分级显示日志信息

     <返回值>     无
     <函数说明>
     <作    者>   cl
     <时    间>   2021/6/8
    **************************************************/
    virtual void set_Grade_Separation_Flag(bool bFlag) = 0;

    /*************************************************
    <函数名称>    set_Output_Level
    <功    能>   设置日志信息输出等级
    <参数说明>    nLevel 最低输出等级

    <返回值>     无
    <函数说明>
    <作    者>   cl
    <时    间>   2021/6/8
   **************************************************/
    virtual void set_Output_Level(int nLevel) = 0;

    /*************************************************
     <函数名称>    log_Warring
     <功    能>   警告性日志
     <参数说明>    warring输出为日志信息的字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void log_Warring(const string& warring) = 0;

    /*************************************************
     <函数名称>   log_Error
     <功    能>  错误性日志
     <参数说明>   error输出为日志信息的字符串

     <返回值>     无
     <函数说明>
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void log_Error(const string& error) = 0;

    /*************************************************
     <函数名称>     log_Assert
     <功    能>    错误性日志
     <参数说明>     fatal输出为日志信息的字符串;

     <返回值>     无
     <函数说明>  
     <作    者>   tcy
     <时    间>   2018/4/26
    **************************************************/
    virtual void log_Fatal(const string& fatal) = 0;
};
#endif
