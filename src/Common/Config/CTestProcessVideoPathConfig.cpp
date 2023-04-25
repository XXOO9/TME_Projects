#include "CTestProcessVideoPathConfig.h"
#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QVector>
#include <CFilePath.h>
#include <Logger/CLogger.h>

const QString TEST_PROCESS_VIDEO_PATH_CONFIG_PATH = "/cfg/videoPath.txt";

CTestProcessVideoPathConfig::CTestProcessVideoPathConfig()
{
    Init();
}

void CTestProcessVideoPathConfig::Init()
{
    QString strFilePath = QApplication::applicationDirPath() + TEST_PROCESS_VIDEO_PATH_CONFIG_PATH;
    QFileInfo fileInfo(strFilePath);
    QSettings settings( strFilePath, QSettings::IniFormat );
    if (!fileInfo.exists())
    {
        LOGERROR("Failed to load videoPath.txt, File don't exist, it will use default Conifg");
        Default(settings);
    }
}

QString CTestProcessVideoPathConfig::getINIDevKeyWords(short devType)
{
    QString keyWord = "General/DEV_TYPE_BODY";
    switch (devType) {
    case DEV_TYPE_BODY:
    {
        keyWord = "General/DEV_TYPE_BODY";
        break;
    }
    case DEV_TYPE_VC:
    {
        keyWord = "General/DEV_TYPE_VC";
        break;
    }
    case DEV_TYPE_SBJ:
    {
        keyWord = "General/DEV_TYPE_SBJ";
        break;
    }
    case DEV_TYPE_SFB:
    {
        keyWord = "General/DEV_TYPE_SFB";
        break;
    }
    case DEV_TYPE_PULL_UP:
    {
        keyWord = "General/DEV_TYPE_PULL_UP";
        break;
    }
    case DEV_TYPE_SIT_UP:
    {
        keyWord = "General/DEV_TYPE_SIT_UP";
        break;
    }
    case DEV_TYPE_RUN_SHORT:
    {
        keyWord = "General/DEV_TYPE_RUN_SHORT";
        break;
    }
    case DEV_TYPE_RUN_MID:
    {
        keyWord = "General/DEV_TYPE_RUN_MID";
        break;
    }
    case DEV_TYPE_ROPE_SKIP:
    {
        keyWord = "General/DEV_TYPE_ROPE_SKIP";
        break;
    }
    case DEV_TYPE_GRIP:
    {
        keyWord = "General/DEV_TYPE_GRIP";
        break;
    }
    case DEV_TYPE_PUSH_UP:
    {
        keyWord = "General/DEV_TYPE_PUSH_UP";
        break;
    }
    case DEV_TYPE_EYESIGHT:
    {
        keyWord = "General/DEV_TYPE_EYESIGHT";
        break;
    }
    case DEV_TYPE_PVV:
    {
        keyWord = "General/DEV_TYPE_PVV";
        break;
    }
    case DEV_TYPE_FOOTBALL:
    {
        keyWord = "General/DEV_TYPE_FOOTBALL";
        break;
    }
    case DEV_TYPE_BASKETBALL:
    {
        keyWord = "General/DEV_TYPE_BASKETBALL";
        break;
    }
    case DEV_TYPE_VOLLEYBALL:
    {
        keyWord = "General/DEV_TYPE_VOLLEYBALL";
        break;
    }
    case DEV_TYPE_PUO:
    {
        keyWord = "General/DEV_TYPE_PUO";
        break;
    }
    case DEV_TYPE_SOLIDBALL:
    {
        keyWord = "General/DEV_TYPE_SOLIDBALL";
        break;
    }
    case DEV_TYPE_VJUMP:
    {
        keyWord = "General/DEV_TYPE_VJUMP";
        break;
    }
    default:
        break;
    }
    return keyWord;
}

//获取指定设备播放路径
QString CTestProcessVideoPathConfig::getGudieVideoPath(short devType)
{
    QString strFilePath = QApplication::applicationDirPath() + TEST_PROCESS_VIDEO_PATH_CONFIG_PATH;
    QFileInfo fileInfo(strFilePath);
    QSettings settings( strFilePath, QSettings::IniFormat );
    if (!fileInfo.exists())
    {
        LOGERROR("Failed to load videoPath.txt, File don't exist, it will use default Conifg");
        Default(settings);
    }

    Check_Field(settings, getINIDevKeyWords(devType).toStdString());
    QString videoPath = settings.value(getINIDevKeyWords(devType)).toString();

    return videoPath;
}

//初始化默认配置
void CTestProcessVideoPathConfig::Default(QSettings& settings)
{
    QString strAppWorkPath = QApplication::applicationDirPath();
    settings.beginGroup("General");
    settings.setValue("DEV_TYPE_BODY",    strAppWorkPath + "/videos/DEV_TYPE_BODY.mp4");
    settings.setValue("DEV_TYPE_VC",      strAppWorkPath + "/videos/DEV_TYPE_VC.mp4");
    settings.setValue("DEV_TYPE_SBJ",     strAppWorkPath + "/videos/DEV_TYPE_SBJ.mp4");
    settings.setValue("DEV_TYPE_SFB",     strAppWorkPath + "/videos/DEV_TYPE_SFB.mp4");
    settings.setValue("DEV_TYPE_PULL_UP", strAppWorkPath + "/videos/DEV_TYPE_PULL_UP.mp4");
    settings.setValue("DEV_TYPE_SIT_UP",  strAppWorkPath + "/videos/DEV_TYPE_SIT_UP.mp4");
    settings.setValue("DEV_TYPE_RUN_SHORT",  strAppWorkPath + "/videos/DEV_TYPE_RUN_SHORT.mp4");
    settings.setValue("DEV_TYPE_RUN_MID",  strAppWorkPath + "/videos/DEV_TYPE_RUN_MID.mp4");
    settings.setValue("DEV_TYPE_ROPE_SKIP",  strAppWorkPath + "/videos/DEV_TYPE_ROPE_SKIP.mp4");
    settings.setValue("DEV_TYPE_GRIP",  strAppWorkPath + "/videos/DEV_TYPE_GRIP.mp4");
    settings.setValue("DEV_TYPE_PUSH_UP",  strAppWorkPath + "/videos/DEV_TYPE_PUSH_UP.mp4");
    settings.setValue("DEV_TYPE_EYESIGHT",  strAppWorkPath + "/videos/DEV_TYPE_EYESIGHT.mp4");
    settings.setValue("DEV_TYPE_PVV",  strAppWorkPath + "/videos/DEV_TYPE_PVV.mp4");
    settings.endGroup();
}

void CTestProcessVideoPathConfig::Check_Field(const QSettings& settings, const string& strField)
{
    if (!settings.contains(QString::fromStdString(strField)))
    {
        LOGERROR(TEST_PROCESS_VIDEO_PATH_CONFIG_PATH.toStdString() + " dont exist the " + strField + " Field");
    }
}
