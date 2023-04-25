#include "CTestRunConfig.h"
#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QVector>
#include <CFilePath.h>
#include <Logger/CLogger.h>

const QString TEST_RUN_CONFIG_PATH = "//cfg//run.txt";

CTestRunConfig::CTestRunConfig()
{
    Init();
}

void CTestRunConfig::Init()
{
    QString strFilePath = QApplication::applicationDirPath() + TEST_RUN_CONFIG_PATH;
    QFileInfo fileInfo(strFilePath);
    QSettings settings(strFilePath, QSettings::IniFormat);
    if (!fileInfo.exists())
    {
        Default(settings);
    }

    //Check_Field(settings, "General/circleLength");
    //Check_Field(settings, "General/RunItem");

    Read(settings);
}

void CTestRunConfig::Default(QSettings& settings)
{
    settings.beginGroup("General");
    settings.setValue("startingGunCom",   "COM101");
    settings.setValue("RunShortTime", "350");
    settings.endGroup();
}

void CTestRunConfig::Read(const QSettings& settings)
{
    m_qsStartingGunComName = settings.value("General/startingGunCom").toString();

    m_qsRunShortTime = settings.value("General/RunShortTime").toString();
}

void CTestRunConfig::Check_Field(const QSettings& settings, const string& strField)
{
    if (!settings.contains(QString::fromStdString(strField)))
    {
        LOGERROR(TEST_RUN_CONFIG_PATH.toStdString() + " dont exist the " + strField + " Field");
    }
}
