#ifndef CSERVERFUNCONFIG_H
#define CSERVERFUNCONFIG_H
#include <string>
#include <QSettings>
#include "../common_global.h"
using namespace std;

class COMMONSHARED_EXPORT CServerFunConfig
{
public:
    CServerFunConfig();

    void Init();

    ///  服务器根地址
    const string& getServerUrl() const;

    //   测试过程展示视频同步更新功能接口地址
    const string& getTestInstructionVideSyncFuncUrl() const;

    //   同步更新成绩规则功能接口地址
    const string& getScoreStandardSyncFuncUrl() const;

    //   同步更新用户信息相关功能接口地址
    const string& getUserInfoSyncFuncUrl() const;

    //   更新测试成绩结果功能接口地址 
    const string& getUpdateTestResultFuncUrl() const;

    //   心跳功能接口地址
    const string& getHeartBeatFuncUrl() const;
    
    //   查询历史成绩接口地址
    const string& getQueryHistoryScoreFuncUrl() const;

    //   同步更新班级信息接口地址
    const string& getClassInfoSyncFuncUrl() const;

    //   查询班级信息功能接口地址
    const string& getQueryClassInfoFuncUrl() const;

    //   人体体质信息上传接口地址
    const string& getUpdateHumanInfoFuncUrl() const;

    //   测试者识别相关功能接口地址
    const string& getUserIdentifyFuncUrl() const;

    //   上传游客使用信息记录功能接口地址
    const string& getUpdateTouristUsageRecordFuncUrl() const;

    const string& getUpLoadEyeSightFuncUrl() const;

    //   与服务连接超时时间阈值 单位 ms 
    float         getServerTimeOutValue() const;

    bool        getHumanConisitSwitchState() const;

    void        setHumanConisitSwitchState( bool isSwitch );

    bool        getIsDeveloper() const;

    // 批量提交Common 成绩的接口地址
    string getStrMultiUpLoadCommonScoreUrl() const;

    // 批量提交Body 成绩的接口地址
    string getStrMultiUpLoadBodyScoreUrl() const;

    // 批量提交EyeSight 成绩的接口地址
    string getStrMultiUpLoadEyeSightScoreUrl() const;

    // 批量上传心跳的接口地址
    string getStrMultiHeartBeatUrl() const;

    int getPageTimeout() const;

    string getUpdateCardInfoUrl() const;

    string getLoginUrl() const;

    QString getTokenAccount() const;

    QString getTokenPasswd() const;

private:

    void      Default(QSettings& settings);

    void      Read(const QSettings& settings);

    void      Check_Field(const QSettings& settings, const string& strField);

    QString   m_tokenAccount = "";
    QString   m_tokenPasswd  = "";
    string    m_strServerUrl;
    string    m_strTestInstructionVideSyncFuncUrl;
    string    m_strScoreStandardSyncFuncUrl;
    string    m_strUserInfoSyncFuncUrl;
    string    m_strUpdateTestResultFuncUrl;
    string    m_strHeartBeatFuncUrl;
    string    m_strQueryHistoryScoreFuncUrl;
    string    m_strClassInfoSyncFuncUrl;
    string    m_strQueryClassInfoFuncUrl;
    string    m_strUpdateHumanInfoFuncUrl;
    string    m_strUserIdentifyFuncUrl;
    string    m_strUpdateTouristUsageRecordFuncUrl;
    string    m_strUpLoadEyesScore;
    float     m_fServerTimeOutValue = 5000.0;       // 默认5000ms

    //是否测量人体阻抗(暂时写这，防止过多的配置文件)
    bool      m_humanConisitSwitch = true;

    //用于打开开发者模式
    bool      m_isDeveloper = false;

    string    m_strMultiUpLoadCommonScoreUrl;
    string    m_strMultiUpLoadBodyScoreUrl;
    string    m_strMultiUpLoadEyeSightScoreUrl;
    string    m_strMultiHeartBeatUrl;
    string    m_updateCardInfoUrl;
    string    m_loginUrl;

    int       m_pageTimeout = 120;
};

#endif // CSERVERFUNCONFIG_H
