#include "CServerFunConfig.h"
#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QVector>
#include <CFilePath.h>
#include <Logger/CLogger.h>
#include "../Common/commondefin.h"
#include "../Config/CDatabaseManage.h"
using namespace soft;
const QString SERVER_FUNC_CONFIG_PATH = "/cfg/httpConfig.txt";

CServerFunConfig::CServerFunConfig()
{
    Init();
}

void CServerFunConfig::Init()
{
    QString strFilePath = QApplication::applicationDirPath() + SERVER_FUNC_CONFIG_PATH;
    QFileInfo fileInfo(strFilePath);
    QSettings settings(strFilePath, QSettings::IniFormat);
    if (!fileInfo.exists())
    {
        LOGERROR("Failed to load httpConfig.txt, File don't exist, it will use default Conifg");
        Default(settings);
    }
    
    Check_Field(settings, "http/baseIp");
    Check_Field(settings, "http/videoSync");
    Check_Field(settings, "http/scoreStandardSync");
    Check_Field(settings, "http/userInfoSync");
    Check_Field(settings, "http/upDateTestRet");
    Check_Field(settings, "http/upDataHeartBeat");
    Check_Field(settings, "http/getHistoryScore");
    Check_Field(settings, "http/classInfoSync");
    Check_Field(settings, "http/queryClassInfo");
    Check_Field(settings, "http/upDateHumanFormMeasure");
    Check_Field(settings, "http/userIdentify");
    Check_Field(settings, "http/touristUsageRecord");
    Check_Field(settings, "http/timeout");

    Read(settings);
}

const string& CServerFunConfig::getServerUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strServerUrl;
}

const string& CServerFunConfig::getTestInstructionVideSyncFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strTestInstructionVideSyncFuncUrl;
}

const string& CServerFunConfig::getScoreStandardSyncFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strScoreStandardSyncFuncUrl;
}

const string& CServerFunConfig::getUserInfoSyncFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return m_strUserInfoSyncFuncUrl;
}

const string& CServerFunConfig::getUpdateTestResultFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strUpdateTestResultFuncUrl;
}

const string& CServerFunConfig::getHeartBeatFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strHeartBeatFuncUrl;
}

const string& CServerFunConfig::getQueryHistoryScoreFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strQueryHistoryScoreFuncUrl;
}

const string& CServerFunConfig::getClassInfoSyncFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return m_strClassInfoSyncFuncUrl;
}

const string& CServerFunConfig::getQueryClassInfoFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strQueryClassInfoFuncUrl;
}

const string& CServerFunConfig::getUpdateHumanInfoFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strUpdateHumanInfoFuncUrl;
}

const string& CServerFunConfig::getUserIdentifyFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return    m_strUserIdentifyFuncUrl;
}

const string& CServerFunConfig::getUpdateTouristUsageRecordFuncUrl() const
{
    // TODO: 在此处插入 return 语句
    return m_strUpdateTouristUsageRecordFuncUrl;
}

const string &CServerFunConfig::getUpLoadEyeSightFuncUrl() const
{
    return m_strUpLoadEyesScore;
}

float CServerFunConfig::getServerTimeOutValue() const
{
    return m_fServerTimeOutValue;
}

bool CServerFunConfig::getHumanConisitSwitchState() const
{
    return m_humanConisitSwitch;
}

void CServerFunConfig::setHumanConisitSwitchState(bool isSwitch)
{
    QString strFilePath = QApplication::applicationDirPath() + SERVER_FUNC_CONFIG_PATH;
    QSettings settings(strFilePath, QSettings::IniFormat);

    settings.setValue( "http/humanConsistSwitch", isSwitch );
}

void CServerFunConfig::Default(QSettings& settings)
{
    settings.beginGroup("http");

    int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();

    switch (nSoftVersion) {
    case VERSION_CD_PSB:
        settings.setValue("baseIp",                    "http://172.16.16.16:7601");
        break;
    case VERSION_QLNJXX:
        settings.setValue("baseIp",                    "http://qionglai-auth.cdwalker.com");
        break;
    default:
        settings.setValue("baseIp",                    "http://dev.cdwalker.com");
        break;
    }

    settings.setValue("videoSync",                 "/api/standard/sample-video/sync");
    settings.setValue("scoreStandardSync",         "/api/standard/sport-rule-detail/sync");
    settings.setValue("userInfoSync",              "/api/base/user/sync");
    settings.setValue("upDateTestRet",             "/api/standard/device-record");
    settings.setValue("upDataHeartBeat",           "/api/standard/device/heart");
    settings.setValue("getHistoryScore",           "/api/standard/device-record/student-history-record");
    settings.setValue("classInfoSync",             "/api/base/clazz/sync");
    settings.setValue("queryClassInfo",            "/api/base/clazz/");
    settings.setValue("upDateHumanFormMeasure",    "/api/standard/device-record/bc-data");
    settings.setValue("userIdentify",              "/api/base/user/identify");
    settings.setValue("touristUsageRecord",        "/api/standard/device-usage/visitor");
    settings.setValue("upLoadEyesScore",           "/api/standard/device-record/vision");
    settings.setValue("multiUploadCommonScore",    "/api/standard/device-record/batch");
    settings.setValue("multiUploadBodyScore",      "/api/standard/device-record/bc-data/batch");
    settings.setValue("multiUploadEyeSightScore",  "/api/standard/device-record/vision/batch");
    settings.setValue("multiHeartBeat",            "/api/standard/device/heart/batch");
    settings.setValue("timeout",                   5000);
    settings.setValue("updateCardInfo",            "/api/base/user-card" );
    settings.setValue("login",                     "/userCenter/user/login" );

    //是否测试人体成分开关
    settings.setValue( "humanConsistSwitch",        true );
    settings.setValue( "isDeveloperModel",          false );

    //页面超时跳转时间( 单位：秒 ), 测试项是身高体重时，默认是读出来的两倍
    settings.setValue( "pageTimeout", 120 );

    //2023.02.27 token账号和密码
    settings.setValue( "accountName",   "padadmin" );
    settings.setValue( "accountPasswd", "Tme@Tme#123" );
    settings.endGroup();
}

void CServerFunConfig::Read(const QSettings& settings)
{
    m_strServerUrl                        =   settings.value("http/baseIp", "").toString().toStdString();
    m_strTestInstructionVideSyncFuncUrl   =   m_strServerUrl + settings.value("http/videoSync", "").toString().toStdString();
    m_strScoreStandardSyncFuncUrl         =   m_strServerUrl + settings.value("http/scoreStandardSync", "").toString().toStdString();
    m_strUserInfoSyncFuncUrl              =   m_strServerUrl + settings.value("http/userInfoSync", "").toString().toStdString();
    m_strUpdateTestResultFuncUrl          =   m_strServerUrl + settings.value("http/upDateTestRet", "").toString().toStdString();
    m_strHeartBeatFuncUrl                 =   m_strServerUrl + settings.value("http/upDataHeartBeat", "").toString().toStdString();
    m_strQueryHistoryScoreFuncUrl         =   m_strServerUrl + settings.value("http/getHistoryScore", "").toString().toStdString();
    m_strClassInfoSyncFuncUrl             =   m_strServerUrl + settings.value("http/classInfoSync", "").toString().toStdString();
    m_strQueryClassInfoFuncUrl            =   m_strServerUrl + settings.value("http/queryClassInfo", "").toString().toStdString();
    m_strUpdateHumanInfoFuncUrl           =   m_strServerUrl + settings.value("http/upDateHumanFormMeasure", "").toString().toStdString();
    m_strUserIdentifyFuncUrl              =   m_strServerUrl + settings.value("http/userIdentify", "").toString().toStdString();
    m_strUpdateTouristUsageRecordFuncUrl  =   m_strServerUrl + settings.value("http/touristUsageRecord", "").toString().toStdString();
    m_strUpLoadEyesScore                  =   m_strServerUrl + settings.value("http/upLoadEyesScore", "").toString().toStdString();
    m_fServerTimeOutValue                 =   settings.value("http/timeout", 5000.0f).toFloat();       // 默认5000ms
    m_humanConisitSwitch                  =   settings.value( "http/humanConsistSwitch" ).toBool();
    m_isDeveloper                         =   settings.value( "http/isDeveloperModel" ).toBool();
    m_strMultiUpLoadCommonScoreUrl        =   m_strServerUrl + settings.value("http/multiUploadCommonScore").toString().toStdString();
    m_strMultiUpLoadBodyScoreUrl          =   m_strServerUrl + settings.value("http/multiUploadBodyScore").toString().toStdString();
    m_strMultiUpLoadEyeSightScoreUrl      =   m_strServerUrl + settings.value("http/multiUploadEyeSightScore").toString().toStdString();
    m_strMultiHeartBeatUrl                =   m_strServerUrl + settings.value("http/multiHeartBeat").toString().toStdString();
    m_pageTimeout                         =   settings.value( "http/pageTimeout" ).toInt();
    m_updateCardInfoUrl                   =   m_strServerUrl + settings.value("http/updateCardInfo").toString().toStdString();
    m_loginUrl                            =   m_strServerUrl + settings.value( "http/login" ).toString().toStdString();
    m_tokenAccount                        =   settings.value( "http/accountName" ).toString();
    m_tokenPasswd                         =   settings.value( "http/accountPasswd" ).toString();
}

void CServerFunConfig::Check_Field(const QSettings& settings, const string& strField)
{
    if ( !settings.contains( QString::fromStdString(strField) ) )
    {
        LOGERROR(SERVER_FUNC_CONFIG_PATH.toStdString() + " dont exist the " + strField + " Field");
    }
}

QString CServerFunConfig::getTokenPasswd() const
{
    return m_tokenPasswd;
}

QString CServerFunConfig::getTokenAccount() const
{
    return m_tokenAccount;
}

string CServerFunConfig::getLoginUrl() const
{
    return m_loginUrl;
}

string CServerFunConfig::getUpdateCardInfoUrl() const
{
    return m_updateCardInfoUrl;
}

int CServerFunConfig::getPageTimeout() const
{
    return m_pageTimeout;
}

string CServerFunConfig::getStrMultiHeartBeatUrl() const
{
    return m_strMultiHeartBeatUrl;
}

string CServerFunConfig::getStrMultiUpLoadEyeSightScoreUrl() const
{
    return m_strMultiUpLoadEyeSightScoreUrl;
}

string CServerFunConfig::getStrMultiUpLoadBodyScoreUrl() const
{
    return m_strMultiUpLoadBodyScoreUrl;
}

string CServerFunConfig::getStrMultiUpLoadCommonScoreUrl() const
{
    return m_strMultiUpLoadCommonScoreUrl;
}

bool CServerFunConfig::getIsDeveloper() const
{
    return m_isDeveloper;
}
