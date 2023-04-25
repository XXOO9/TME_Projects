#include "CHostSettingConfig.h"
#include "../../Common/commondefin.h"
#include "../common/Config/CDatabaseManage.h"

#include <QJsonDocument>
#include <QTextCodec>

#pragma execution_character_set("utf-8")

CHostSettingConfig::CHostSettingConfig()
{

    m_devTypeMaps[ "身高体重" ]     = DEV_TYPE_BODY;
    m_devTypeMaps[ "肺活量" ]       = DEV_TYPE_VC;
    m_devTypeMaps[ "立定跳远仪" ]   = DEV_TYPE_SBJ;
    m_devTypeMaps[ "坐位体前屈" ]   = DEV_TYPE_SFB;
    m_devTypeMaps[ "引体向上" ]     = DEV_TYPE_PULL_UP;
    m_devTypeMaps[ "仰卧起坐" ]     = DEV_TYPE_SIT_UP;
    m_devTypeMaps[ "中长跑" ]      = DEV_TYPE_RUN_MID;
    m_devTypeMaps[ "短跑" ]        = DEV_TYPE_RUN_SHORT;
    m_devTypeMaps[ "跳绳" ]        = DEV_TYPE_ROPE_SKIP;
    m_devTypeMaps[ "握力" ]        = DEV_TYPE_GRIP;
    m_devTypeMaps[ "俯卧撑" ]      = DEV_TYPE_PUSH_UP;
    m_devTypeMaps[ "视力" ]        = DEV_TYPE_EYESIGHT;
    m_devTypeMaps[ "足球" ]        = DEV_TYPE_FOOTBALL;
    m_devTypeMaps[ "篮球" ]        = DEV_TYPE_BASKETBALL;
    m_devTypeMaps[ "排球" ]        = DEV_TYPE_VOLLEYBALL;
    m_devTypeMaps[ "斜身引体向上" ] = DEV_TYPE_PUO;
    m_devTypeMaps[ "实心球" ]      = DEV_TYPE_SOLIDBALL;
    m_devTypeMaps[ "纵跳摸高" ]     = DEV_TYPE_VJUMP;

    m_curDevValue = DEV_TYPE_VC;

}

QVariantMap CHostSettingConfig::getSpecificDataElement(const int element)
{
    QVariantMap retMap;

    switch ( element ) {
    case CHostSettingConfig::HostElementData::DevtypeList:
        retMap = { { devTypeListArray, m_devTypeMaps } };
        return retMap;

    case CHostSettingConfig::HostElementData::DelScoreBtnEnable:
        retMap = { { delScoreBtnEnable, m_delScoreBtnEnable } };
        return retMap;

    case CHostSettingConfig::HostElementData::RequireSignature:
        retMap = { { requireSignature, m_requeirSignature } };
        return retMap;

    case CHostSettingConfig::HostElementData::AutoCommitTime:
        retMap = { { autoCommitTime, m_autoCommitTime } };
        return retMap;

    case CHostSettingConfig::HostElementData::ManulChangeMode:
        retMap = { { changeMode, m_manulChangeMode } };
        return retMap;

    case CHostSettingConfig::HostElementData::ConnectedModel:
        retMap = { { wireLessConnectedModel, m_wirelessConnected } };
        return retMap;

    case CHostSettingConfig::HostElementData::SyncTestModel:
        //同步模式常量定义为0，异步模式为1，因此这里要取反
        retMap = { { syncTestModel, !m_syncTestMode } };
        return retMap;

    case CHostSettingConfig::HostElementData::RequireUserInfoView:
        retMap = { { requiredUserInfoPage, m_requeirUserInfoPage } };
        return retMap;

    case CHostSettingConfig::HostElementData::MaxConnectedCnt:
        retMap = { { maxConnectedCnt, m_maxConnectedCnt } };
        return retMap;

    case CHostSettingConfig::HostElementData::CurDevChannel:
        retMap = { { curDevChannel, m_curDevChannel } };
        return retMap;

    default:
        return getAllConfigInfos();
    }
}

void CHostSettingConfig::setSpecificElementData(const int &elementIndex, const QVariantMap &newConfigInfo)
{
    switch ( elementIndex ) {
    case CHostSettingConfig::HostElementData::DevtypeList:
        setDevTypeMaps( newConfigInfo.value( devTypeListArray ).toMap() );
        break;

    case CHostSettingConfig::HostElementData::DelScoreBtnEnable:
        m_delScoreBtnEnable = newConfigInfo.value(delScoreBtnEnable).toBool();
        break;

    case CHostSettingConfig::HostElementData::AutoCommitTime:
        m_autoCommitTime = newConfigInfo.value(autoCommitTime).toInt();
        break;

    case CHostSettingConfig::HostElementData::RequireSignature:
        m_requeirSignature = newConfigInfo.value( requireSignature ).toBool();
        break;

    case CHostSettingConfig::HostElementData::ManulChangeMode:
        m_manulChangeMode = newConfigInfo.value( changeMode ).toBool();
        break;

    case CHostSettingConfig::HostElementData::ConnectedModel:
        m_wirelessConnected = newConfigInfo.value( wireLessConnectedModel ).toBool();
        break;

    case CHostSettingConfig::HostElementData::SyncTestModel:
        //0：同步模式 1：异步模式 要取反
        m_syncTestMode = !newConfigInfo.value( syncTestModel ).toBool();
        break;

    case CHostSettingConfig::HostElementData::RequireUserInfoView:
        m_requeirUserInfoPage = newConfigInfo.value( requiredUserInfoPage ).toBool();
        break;

    case CHostSettingConfig::HostElementData::CurrentDevTypeIndex:
        setCurDevValue( newConfigInfo.value( currentDevValue ).toInt() );
        break;

    case CHostSettingConfig::HostElementData::LoadChannelConfig:
        setLoadChannalConfigArea( newConfigInfo.value( loadChannelConfig ).toBool() );
        break;

    case CHostSettingConfig::HostElementData::CurDevChannel:
        setCurDevChannel( newConfigInfo.value( curDevChannel ).toString() );
        break;

        //仅有界面的保存设置会经过这项
    default:
        setAllConfig( newConfigInfo );
        break;
    }
}

QVariantList CHostSettingConfig::constructJsonArrayFromDevTypeList() const
{
    QVariantMap tmpMap;
    QVariantList retList;

    for( auto iter = m_devTypeMaps.constBegin(); iter != m_devTypeMaps.constEnd(); iter++ ){
        tmpMap.clear();
        tmpMap[ devName ]  = iter.key();
        tmpMap[ devValue ] = iter.value();
        retList << tmpMap;
    }

    return retList;
}

QVariantMap CHostSettingConfig::getAllConfigInfos() const
{
    QVariantMap retMap;

    retMap[ devTypeListArray ]        =   constructJsonArrayFromDevTypeList();
    retMap[ signalChannelList ]       =   m_channelList;
    retMap[ maxConnectedCnt ]         =   QString::number( m_maxConnectedCnt );
    retMap[ loadChannelConfig ]       =   m_loadChannalConfigArea;
    retMap[ delScoreBtnEnable ]       =   m_delScoreBtnEnable;
    retMap[ autoCommitTime ]          =   m_autoCommitTime;
    retMap[ requireSignature ]        =   m_requeirSignature;
    retMap[ changeMode ]              =   m_manulChangeMode;
    retMap[ wireLessConnectedModel ]  =   m_wirelessConnected;
    retMap[ syncTestModel ]           =   m_syncTestMode;
    retMap[ requiredUserInfoPage ]    =   m_requeirUserInfoPage;
    retMap[ currentDevTypeName ]      =   getCurDevTypeName();
    retMap[ curDevChannel ]           =   m_curDevChannel;
    retMap[ currentDevValue ]         =   m_curDevValue;

    QJsonDocument jsDoc( QJsonObject::fromVariantMap( retMap ) );
    //    qDebug() << "json = " << QString::fromUtf8( jsDoc.toJson() );

    return retMap;
}

void CHostSettingConfig::setAllConfig(const QVariantMap &srcMap)
{
    //重置改变标志位
    resetChangeItem();

    //设置当前设备的值，设置完了过后，顺带也设置了对应的信道号列表
    setCurDevValue( srcMap.value( currentDevValue ).toInt() );

    //设置提交相关配置
    setDevCommitCfg( srcMap );

    //获取改变连接的模式，手动切换还是自动切换
    bool ConnectedChangeFlag = !m_manulChangeMode == srcMap.value( changeMode ).toBool();
    m_manulChangeMode        = srcMap.value( changeMode ).toBool();

    // 如果是手动切换，则需要获取 选择的是直连还是组网
    bool wireLessChangeFlag  = !srcMap.value( wireLessConnectedModel ).toBool() == m_wirelessConnected;
    m_wirelessConnected      = srcMap.value( wireLessConnectedModel ).toBool();

    //如果是一对多模式 需要设置当前是同步模式还是异步模式
    if(m_wirelessConnected)
    {
        bool syncTestModeFlag = srcMap.value( syncTestModel ).toBool();
        if(m_syncTestMode != syncTestModeFlag)
        {
            m_syncTestMode = syncTestModeFlag;
            m_lChangeItem |= changeItem::host::CHANGE_DEV_SYNC_MODE;
        }
    }

    bool connectModeChanged = ConnectedChangeFlag | wireLessChangeFlag;

    //是否需要开启用户信息过渡页
    m_requeirUserInfoPage   = srcMap.value( requiredUserInfoPage ).toBool();

    //是否开启切换信道页面
    if( srcMap.contains( loadChannelConfig ) ){
        m_loadChannalConfigArea = srcMap.value( loadChannelConfig ).toBool();
    }

    //需要存数据库的信息全部存入数据库
    saveIntoDatabase( srcMap );

    //如果开启了切换信道页面，获取当前选择的信道和最大链接数量
    auto channel = srcMap.value( curDevChannel ).toString();
    auto maxCnt = srcMap.value( maxConnectedCnt ).toInt();

    //如果信道或者最大连接数量发生了变化，则改变 change 标志
    if( channel != m_curDevChannel || maxCnt != m_maxConnectedCnt ){
        m_lChangeItem |= changeItem::host::CHANGE_DEV_Channel;
        m_curDevChannel = channel;
        m_maxConnectedCnt = maxCnt;
    }
    //如果信道号，最大连接数量没有改变, 但是连接模式有改变， 则只执行切换模式
    else if( connectModeChanged ){
        m_lChangeItem |= changeItem::host::CHANGE_DEV_CONNECTED_MODE;
    }
}

QString CHostSettingConfig::getCurDevTypeName() const
{
    return m_devTypeMaps.key( m_curDevValue );
}

int CHostSettingConfig::getMaxConnectedCnt() const
{
    return m_maxConnectedCnt;
}

void CHostSettingConfig::setMaxConnectedCnt(int maxConnectedCnt)
{
    m_maxConnectedCnt = maxConnectedCnt;
}

void CHostSettingConfig::saveIntoDatabase(const QVariantMap &srcMap)
{
    CDatabaseManage &devDatabase = CDatabaseManage::GetInstance();
    CDevTestInfoConfig *pTableDevTestInfo = devDatabase.getDevTestInfoConfig();
    if(pTableDevTestInfo == nullptr )
    {
        return;
    }

    //存入手动切换还是自动切换， 手动模式向数据库存入  false
    pTableDevTestInfo->updateDevConnectModeAuto( !srcMap.value( changeMode ).toBool() );

    //存入展示出来的是组网还是直连模式,  组网模式向数据库存入 false
    pTableDevTestInfo->updateDevConnectMode( !srcMap.value( wireLessConnectedModel ).toBool() );

    //存入组网模式下同步测试模式 0:同步 1：异步 传入参数为 是否同步，所以取反
    pTableDevTestInfo->updateDevTestMode(!srcMap.value( syncTestModel ).toBool());

    //修改数据库提交配置和是否存入用户信息页面改动信息
    if(m_lChangeItem & changeItem::host::CHANGE_DEV_AUTO_COMMIT ||
            m_lChangeItem & changeItem::host::CHANGE_DEV_USERINFOPAGE_DISPLAY )
    {
        CCommitPopupConfig infos;
        infos.m_delBtnEnable = m_delScoreBtnEnable;
        infos.m_commitType = !m_requeirSignature;
        infos.m_signatureExisted = m_requeirSignature;
        infos.m_autoCommitCountTime = m_autoCommitTime;
        infos.m_userInfoPageExisted = m_requeirUserInfoPage;

        CDatabaseManage::GetInstance().getPCommitTableConfig()->updatePopupConfig( infos );
    }
}

QString CHostSettingConfig::getCurDevChannel() const
{
    return m_curDevChannel;
}

void CHostSettingConfig::setCurDevChannel(const QString &curDevChannel)
{
    m_curDevChannel = curDevChannel;

    //2023.01.11 如果获取到的信道号为 0 ， 则设置当前设备类型为未知项
    if( "0" == curDevChannel ){
        setCurDevValue( DEV_TYPE_UNKNOW );
    }
}

void CHostSettingConfig::setDevCommitCfg(const QVariantMap &srcMap)
{
    //获取是否签字版
    bool requeirSignature = srcMap.value( requireSignature ).toBool();
    if( requeirSignature != m_requeirSignature)
    {
        m_lChangeItem |= changeItem::host::CHANGE_DEV_AUTO_COMMIT;
        m_requeirSignature = srcMap.value( requireSignature ).toBool();
    }

    //是否可以取消成绩
    bool delBtnEnable = srcMap.value( delScoreBtnEnable ).toBool();
    if( delBtnEnable != m_delScoreBtnEnable)
    {
        m_lChangeItem |= changeItem::host::CHANGE_DEV_AUTO_COMMIT;
        m_delScoreBtnEnable = srcMap.value( delScoreBtnEnable ).toBool();
    }

    //修改自动提交定时
    int commitTime = srcMap.value( autoCommitTime ).toInt();
    if( commitTime != m_autoCommitTime)
    {
        m_lChangeItem |= changeItem::host::CHANGE_DEV_AUTO_COMMIT;
        m_autoCommitTime = commitTime;
    }

    //是否显示用户信息页面
    bool userInfoPageRequired = srcMap.value( requiredUserInfoPage ).toBool();
    if( userInfoPageRequired != m_requeirUserInfoPage ){
        m_lChangeItem |= changeItem::host::CHANGE_DEV_USERINFOPAGE_DISPLAY;
        m_requeirUserInfoPage = userInfoPageRequired;
    }
}

void CHostSettingConfig::setLoadChannalConfigArea(bool loadChannalConfigArea)
{
    m_loadChannalConfigArea = loadChannalConfigArea;
}

void CHostSettingConfig::setRequeirSignature(bool requeirSignature)
{
    m_requeirSignature = requeirSignature;
}

void CHostSettingConfig::setCurDevValue(int curDevValue)
{
    m_curDevValue = curDevValue;

    //设置完设备类型后，立马获取对应的信道号
    m_channelList = CDevDataCfg::getDevSignalChannelList( static_cast<short>(m_curDevValue) );
}

void CHostSettingConfig::setDevTypeMaps(const QVariantMap &devTypeMaps)
{
    m_devTypeMaps = devTypeMaps;
}
