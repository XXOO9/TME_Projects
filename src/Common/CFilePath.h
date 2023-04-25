#ifndef CFilePath_H
#define CFilePath_H

#include "common_global.h"
#include <string>
#include <QStringList>
#include <QString>
using namespace std;

class COMMONSHARED_EXPORT CFilePath
{
public:
    CFilePath();
public:
    static bool isDirExist(const string &fullPath, bool bCreat = false);
    static bool isFileExist(const string &fullPath, bool bCreat = false);
    static string getAppDirPath();
    static bool deleteFileOrFolder(const string &fullPath);
    static string getAppName();

    static void rename(const string &strOldName, const string &strNewName);
    static void cpyFile(const string &strSecName, const string &strDestName);
    static void getFileInfoInDir(const QString &qsDirFullPath, QStringList &listFileName, const QString &qsFilter = "*");

    //删除文件夹下所有空文件夹
    static void deleteEmptyFolder(const string &fullPath);
};

#endif
