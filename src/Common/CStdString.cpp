#include "CStdString.h"
#include <algorithm>
#include <chrono>

#include <locale>
#include <codecvt>


vector<string> CStdString::splitString(const string &strSrc, string delimit, bool bKeepEmptyElement)
{
    vector<string> result;
    string strTemp, str(strSrc);
    size_t pos = str.find(delimit);
    str += delimit;//将分隔符加入到最后一个位置，方便分割最后一位

    while (pos != string::npos) {
        strTemp = str.substr(0, pos);

        //不保存空元素的情况下，资源为空不进行保存
        if(!(!bKeepEmptyElement && strTemp.empty()))
        {
            result.push_back(strTemp);
        }

        str = str.substr(pos + 1);//substr的第一个参数为起始位置，第二个参数为复制长度，默认为string::npos到最后一个位置
        pos = str.find(delimit);
    }

    //删除最后一个为空字符的内容
    size_t szSize = result.size();
    if(szSize > 0)
    {
        if(result[szSize - 1].empty())
        {
            result.erase(result.begin() + static_cast<int>(szSize) - 1);
        }
    }
    //不存在则表明只有一个数据
    else
    {
        result.push_back(strSrc);
    }

    return result;
}

void CStdString::trimString(string &strSrc)
{
    if(strSrc.empty())
        return;

    auto s = strSrc.find_first_not_of(" ");
    auto e = strSrc.find_last_not_of(" ");

    //都没有找到或者 字符是 " "
    if(s == string::npos || e == string::npos)
    {
        strSrc.clear();
    }
    else
    {
        strSrc = strSrc.substr(s,e-s+1);
    }
}

void CStdString::replaceString(string &src, string strOld, string strNew)
{
    for (string::size_type pos(0); pos != string::npos; pos += strNew.length())
    {
        if ((pos = src.find(strOld, pos)) != string::npos)
        {
            src.replace(pos, strOld.length(), strNew);
        }
        else
            break;
    }
}

string CStdString::toUpper(const string &strSrc)
{
    string str(strSrc);
    transform(str.begin(),str.end(),str.begin(),::toupper);
    return str;
}

string CStdString::toLower(const string &strSrc)
{
    string str(strSrc);
    transform(str.begin(),str.end(),str.begin(),::tolower);
    return str;
}

string CStdString::leftJustified(const string &strSrc, const size_t &width, char fill)
{
    string str(strSrc);
    size_t szLen = strSrc.length();
    if(szLen > width)
    {
        str = strSrc.substr(0, width);
    }
    else
    {
        for(size_t i = szLen; i < width; i++)
            str += fill;
    }

    return str;

}

void CStdString::getValidDataEDF(string &str)
{
    auto pos = str.find_last_not_of(FILL_CHAR);
    if(pos == string::npos)
    {
        str.clear();
    }
    else
    {
        str = str.substr(0, pos + 1);
    }
}

string CStdString::GBKToUTF8(const string &strGBK)
{
    const char* GBK_LOCALE_NAME = "CHS";  //GBK在windows下的locale name(.936, CHS ), linux下的locale名可能是"zh_CN.GBK"

    std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>>
        conv(new std::codecvt<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));
    std::wstring wString = conv.from_bytes(strGBK);    // string => wstring

    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    std::string utf8str = convert.to_bytes(wString);     // wstring => utf-8

    return utf8str;
}

string CStdString::UTF8ToGBK(const string &strUTF8)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::wstring wString = conv.from_bytes(strUTF8);    // utf-8 => wstring

    std::wstring_convert<std::codecvt< wchar_t, char, std::mbstate_t>>
        convert(new std::codecvt< wchar_t, char, std::mbstate_t>("CHS"));
    std::string str = convert.to_bytes(wString);     // wstring => string

    return str;
}

