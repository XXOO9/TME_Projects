#include "CFilePath.h"
#include <QDir>
#include <QFile>
#include <QString>
#include <QCoreApplication>

CFilePath::CFilePath()
{

}

bool CFilePath::isDirExist(const string &fullPath, bool bCreat)
{
    QString qsTemp = QString::fromStdString(fullPath);
    QDir dir(qsTemp);

    bool bRes = false;

    if(dir.exists())
    {
        bRes = true;
    }
    else
    {
        if(bCreat)
        {
            bRes = dir.mkdir(qsTemp);
        }
    }

    return  bRes;
}

bool CFilePath::isFileExist(const string &fullPath, bool bCreat)
{
    QString qsPath = QString::fromStdString(fullPath);
    bool ok = QFile::exists(qsPath);

    if(!ok && bCreat)
    {
        //文件夹不存在创建路径
        QString sPath = qsPath.left(qsPath.lastIndexOf('/'));
        isDirExist(sPath.toStdString(), bCreat);

        //当以Write方式打开时，若文件不存在则自动创建
        QFile file(qsPath);
        ok = file.open(QIODevice::WriteOnly);
    }

    return ok;
}

string CFilePath::getAppDirPath()
{
    return  QCoreApplication::applicationDirPath().toStdString();
}

bool CFilePath::deleteFileOrFolder(const string &fullPath)
{
    QString strPath = QString::fromStdString(fullPath);
    if (strPath.isEmpty() || !QDir().exists(strPath))//是否传入了空的路径||路径是否存在
            return false;

    QFileInfo FileInfo(strPath);

    if (FileInfo.isFile())//如果是文件
        QFile::remove(strPath);
    else if (FileInfo.isDir())//如果是文件夹
    {
        QDir qDir(strPath);
        qDir.removeRecursively();
    }
    return true;
}

string CFilePath::getAppName()
{
    return QCoreApplication::applicationName().toStdString();
}

void CFilePath::rename(const string &strOldName, const string &strNewName)
{
    QFile::rename(QString::fromStdString(strOldName), QString::fromStdString(strNewName));
}

void CFilePath::cpyFile(const string &strSrcName, const string &strDestName)
{
    QFile::copy(QString::fromStdString(strSrcName), QString::fromStdString(strDestName));
}

void CFilePath::getFileInfoInDir(const QString &qsDirFullPath, QStringList &listFileName, const QString &qsFilter)
{
    listFileName.clear();
    //判断路径是否存在
    QDir dir(qsDirFullPath);
    if(!dir.exists())
        return;

    //查看路径中后缀为.cfg格式的文件
    QStringList filters;
    filters << qsFilter;
    dir.setFilter(QDir::Files | QDir::NoSymLinks); //设置类型过滤器，只为文件格式
    dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式

    //统计的文件个数
    int dir_count = static_cast<int>(dir.count());
    if(dir_count <= 0)
        return;

    //存储文件名称
    for(int i = 0; i < dir_count; i++)
    {
        QString file_name = dir[i];  //文件名称
        listFileName.append(file_name);
    }
}

void CFilePath::deleteEmptyFolder(const string &fullPath)
{
    //判断路径是否存在
    QDir dir(QString::fromStdString(fullPath));
    if(!dir.exists())
        return;

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    auto lisDir = dir.entryInfoList();
    QList<QFileInfo> listFile;

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    for(auto &element : lisDir)
    {
        dir.setPath(element.absoluteFilePath());
        listFile = dir.entryInfoList();
        if(listFile.size() == 0)
            deleteFileOrFolder(element.absoluteFilePath().toStdString());
    }
}
