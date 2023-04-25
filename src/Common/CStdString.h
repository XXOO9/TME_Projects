#ifndef EDFCOMMON_H
#define EDFCOMMON_H
#include "common_global.h"

#include <string>
#include <vector>
#include <time.h>
#include <sstream>
#include <iomanip>

using namespace std;

#define FILL_CHAR ' '

class COMMONSHARED_EXPORT CStdString
{
public:
    //参数 3表示是否保留空元素
    static vector<string> splitString(const string &strSrc, string delimit, bool bKeepEmptyElement = true);

    //去除前后空格
    static void trimString(string &strSrc);

    //替换
    static void replaceString(string &src, string strOld, string strNew);

    static string toUpper(const string &strSrc);
    static string toLower(const string &strSrc);

    //返回一个大小为width的字符串，该字符串包含由填充字符填充的字符串。左对齐
    static string leftJustified(const string &strSrc, const size_t &width, char fill);

    //获取数据有几位，只针对整形,对小数还需要进行其他处理
    template<class T>
    static size_t getValueLen(T value);

    //获取有效数据
    static void getValidDataEDF(string &str);

    //通过字符获取数据
    template<class T>
    static T getValue(const string& str);

    //针对浮点型将float 或者 double按照固定小数位数输出
    template<class T>
    static string number(const T& value, const unsigned short shDecimalDigits);

    //针对整形
    template<class T>
    static string number(const T& value);

    //类型调用错误会导致软件崩溃
    static string GBKToUTF8(const string& strGBK);
    static string UTF8ToGBK(const string& strUTF8);
};

template<class T>
size_t CStdString::getValueLen(T value)
{
    size_t count = 0;
    while(value != 0)
    {
        value /= 10;
        count++;
    }

    return count;
}

//通过字符获取数据

template<class T>
inline T CStdString::getValue(const string& str)
{
    T value;
    memset(&value, 0, sizeof(T));
    stringstream ss;
    ss << str;
    ss >> value;

    return value;
}

//针对浮点型将float 或者 double按照固定小数位数输出

template<class T>
inline string CStdString::number(const T& value, const unsigned short shDecimalDigits)
{
    stringstream ss;
    ss << std::setiosflags(std::ios::fixed) << std::setprecision(shDecimalDigits) << value;
    string str = ss.str();

    return str;
}

//针对整形

template<class T>
inline string CStdString::number(const T& value)
{
    stringstream ss;
    ss << value;
    string str = ss.str();

    return str;
}
#endif // EDFCOMMON_H
