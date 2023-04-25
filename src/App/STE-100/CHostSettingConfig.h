#ifndef CHOSTSETTINGCONFIG_H
#define CHOSTSETTINGCONFIG_H

#include <QObject>
#include <QDebug>

#include "CAbstractSettingElement.h"
#include "CDevDataCfg.h"

const QString currentDevTypeName     = "curDevTypeName";
const QString currentDevValue        = "curDevTypeValue";
const QString autoCommitTime         = "autoCommitTime";
const QString delScoreBtnEnable      = "delScoreBtnEnable";
const QString requireSignature       = "requireSignature";
const QString changeMode             = "manulChangeMode";
const QString wireLessConnectedModel = "wireLessConnectedModel";
const QString syncTestModel          = "syncTestModel";
const QString requiredUserInfoPage   = "requiredUserInfoPage";
const QString devTypeListArray       = "devTypeList";
const QString devName                = "fieldName";
const QString devValue               = "fieldVal";
const QString devSignalChannel       = "siganlChannel";
const QString signalChannelList      = "signalChannelList";
const QString maxConnectedCnt        = "maxConnectCnt";
const QString loadChannelConfig      = "loadChannelConfig";
const QString curDevChannel          = "curDevChannel";

class CHostSettingConfig : public CAbstractSettingElement
{
public:

    enum HostElementData{
        DevtypeList = 0,
        AutoCommitTime,
        DelScoreBtnEnable,
        RequireSignature,
        ManulChangeMode, //  手动或者自动切换
        ConnectedModel, //无线还是有线
        SyncTestModel, //同步还是异步
        RequireUserInfoView,    //用户信息页面
        CurrentDevTypeIndex,
        DevSignalChannel,
        LoadChannelConfig,
        CurDevChannel,
        MaxConnectedCnt,
    };

    CHostSettingConfig();

    virtual QVariantMap getSpecificDataElement( const int element ) override;
    virtual void setSpecificElementData( const int &elementIndex, const QVariantMap &newConfigInfo ) override;

    void setDevTypeMaps(const QVariantMap &devTypeMaps);

    void setCurDevValue(int curDevValue);

    void setDevChannelSelectList(const QStringList &devChannelMap);

    void setRequeirSignature(bool requeirSignature);

    void setLoadChannalConfigArea(bool loadChannalConfigArea);

    void setCurDevChannel(const QString &curDevChannel);

    void setDevCommitCfg(const QVariantMap &srcMap);

    QString getCurDevChannel() const;

    int getMaxConnectedCnt() const;
    void setMaxConnectedCnt(int maxConnectedCnt);

private:

    virtual void saveIntoDatabase( const QVariantMap &srcMap ) override;
    QVariantList constructJsonArrayFromDevTypeList() const;

    QVariantMap getAllConfigInfos() const;
    void setAllConfig( const QVariantMap &srcMap );

    QString getCurDevTypeName() const;

    QStringList getSpecificDevSignalChannels();


private:
    //存放所有的设备类型
    QVariantMap m_devTypeMaps;

    //组网模式下，可供选择的信道号
    QStringList m_channelList;

    //当前设备类型的值
    int         m_curDevValue = DEV_TYPE_PUO;

    //手动切换 还是  自动切换 连接模式
    bool        m_manulChangeMode = false;

    //无线还是有线
    bool        m_wirelessConnected = false;

    //同步还是异步
    bool        m_syncTestMode = true;

    //是否需要用户信息界面
    bool        m_requeirUserInfoPage = true;

    //最大连接数量
    int         m_maxConnectedCnt = 30;

    //当前信道号
    QString     m_curDevChannel = "-1";

    // 是否在界面上加载信道配置区域
    bool        m_loadChannalConfigArea = true;

    //提交相关配置
    //是否需要签字
    bool        m_requeirSignature = false;

    //是否可以取消成绩
    bool        m_delScoreBtnEnable = true;

    //自动提交倒计时
    int         m_autoCommitTime = 5;
};

#endif // CHOSTSETTINGCONFIG_H
