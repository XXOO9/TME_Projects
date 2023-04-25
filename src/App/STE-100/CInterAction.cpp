#include "CInterAction.h"
#include "CDevCamera.h"
#include "CLogger.h"
#include "CFilePath.h"
#include "CInterAction.h"
#include "CDatabaseManage.h"
#include "SteDevManager/CSteDeviceManager.h"
#include "CMainFrame.h"

#include <CFaceRecognition.h>

CInterAction::CInterAction(QObject *parent ) : QObject(parent)
  ,m_pHttp( nullptr )
  ,m_pRequestMgr( nullptr )
{
    init( );

    qRegisterMetaType<CUserInfo>( "CUserInfo" );
}

void CInterAction::init( )
{
    //设置软件版本， 暂时写死
    m_sSoftVersion = "V1.2";

    m_queueErrorTip.clear();
    connect(&m_timerErrorTipTimeOut, &QTimer::timeout, this, &CInterAction::onCheckErrorTipTimeOut);

    //读取配置文件
    readConfigFile();

    QDateTime curDateTime= QDateTime::currentDateTime();
    m_sCurYear = curDateTime.toString("yyyy");
    m_sCurMonth = curDateTime.toString("MM");
    m_sCurDay = curDateTime.toString("dd");

    qRegisterMetaType< structRequestHistory >( "structRequestHistory" );

    if( nullptr == m_pRequestMgr ){
        m_pRequestMgr = CHttpRequestMgr::getInstance();
    }

    connect( m_pRequestMgr, &CHttpRequestMgr::sigHeartBeatFinished, this, &CInterAction::setNetConnectState );
    //connect(qApp,&QApplication::aboutToQuit,this,&CInterAction::quit);
    //connect(qApp,&QApplication::lastWindowClosed,this,&CInterAction::quit);
}

void CInterAction::refreshDevDetailInfo()
{
    if( nullptr == m_pMainFrame ){
        return;
    }

    CMainFrame *pMainFrame = static_cast< CMainFrame* >( m_pMainFrame );

    QVariantMap tmpDetailMap = pMainFrame->getDevDetailInfos();

    m_systemSettingMgr.devConfiger().setDevNumberMap( tmpDetailMap.value( "devOrderMap" ).toMap() );
    pMainFrame = nullptr;
}

bool CInterAction::setStandingJumpStartPos(int startPos)
{
    CDatabaseManage::GetInstance().getDevTestInfoConfig()->setStadingJumpStartPos( startPos );
    if(m_pMainFrame == nullptr)
        return false;

    CMainFrame *pMain = reinterpret_cast<CMainFrame *>(m_pMainFrame);
    return pMain->setSBJStartPos(startPos);
}

void CInterAction::setDevConnectedInfo(bool directConnected, bool autoChangeConnectType )
{
    qDebug() << "target conn0ected model = " << directConnected;

    CDatabaseManage &devDatabase = CDatabaseManage::GetInstance();
    CDevTestInfoConfig *pTableDevTestInfo = devDatabase.getDevTestInfoConfig();
    if(pTableDevTestInfo != nullptr )
    {
        pTableDevTestInfo->updateDevConnectMode( directConnected );
        pTableDevTestInfo->updateDevConnectModeAuto( autoChangeConnectType );
    }

    if(m_pMainFrame != nullptr)
    {
        reinterpret_cast<CMainFrame *>(m_pMainFrame)->onDevConnectChangeSteFactory();
    }
}

QJsonObject CInterAction::getLocalDevConnectInfo()
{
    QJsonObject devConnectInfo;
    bool curSettingDevConnectModel = CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevConnectMode();
    bool curSettingAutoChangeConnect = CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevConnectModeAuto();

    devConnectInfo.insert( "connectModel", curSettingDevConnectModel );
    devConnectInfo.insert( "autoChange", curSettingAutoChangeConnect );

    return devConnectInfo;
}

QString CInterAction::calculateTimeDifference( short operation )
{
    CProjectEnums::HightCountTimer enumOperation = static_cast< CProjectEnums::HightCountTimer >( operation );
    QString tmpVal = 0;

    if( CProjectEnums::HightCountTimer::CALCULATETIMEDIFFERENCE  == enumOperation ){
        tmpVal = m_countTimer.calculateDiffenenceTillNow();
    }

    if( CProjectEnums::HightCountTimer::STARTORRESTARTCOUNTTIMER == enumOperation ){
        m_countTimer.start();
    }

    return tmpVal;

}

void CInterAction::setQmlEngine(QQmlApplicationEngine *engine )
{
    m_pQmlengine = engine;
}

void CInterAction::setDisplayUploadErrorString(const QString &errString)
{
//    QObject *pRoot = m_pQmlengine->rootObjects().first();

//    //一对一的两种弹窗都要加， 一对多的两种弹窗也要加
//    QObject *pTargetPop = pRoot->findChild( "   " )
}

void CInterAction::resetTest()
{
    m_sCurTestRet = "";
    m_commonDataModel.deleteAll();
    //m_studentDataModel.deleteAll();
    m_scoreDataModel.deleteAll();
}

void CInterAction::setCurrentPage( short page )
{
#if QT_NO_DEBUG
#else
    QMetaEnum tmp = QMetaEnum::fromType< CProjectEnums::ViewPage >();
    qDebug() << "change page to " << tmp.valueToKey( page );
#endif
    m_iCurPage = page;
    emit signalPageChanged();

}

bool CInterAction::checkNetConnected()
{
    return m_bNetConnect;
}

QString CInterAction::getCurStuName()
{
    return m_sCurStuName;
}

QString CInterAction::getCurStuAge()
{
    return m_sCurStuAge;
}

QString CInterAction::getCurStuGrade()
{
    return m_sCurStuGrade;
}

QString CInterAction::getCurStuClass()
{
    return m_sCurClass;
}

bool CInterAction::getCurStuSex()
{
    return  m_sCurStuSex;
}

QString CInterAction::getCurStuSchoolNO()
{
    return m_sCurStuSchoolNO;
}

QString CInterAction::getSoftWareVersion()
{
    return m_sSoftVersion;
}

void CInterAction::setSoftWareVersion( QString version )
{
    m_sSoftVersion = version;
}

QString CInterAction::getCurTestRet()
{
    return m_sCurTestRet;
}

void CInterAction::setCurTestRet( QString ret, bool bRefresh  )
{
    m_sCurTestRet = ret;

    if( !bRefresh )
    {
        return;
    }
    emit signalCurrentTestRetAvailable( m_sCurTestRet );
}

void CInterAction::setCurBodyTestRet(QString weight, QString height)
{
    emit signalCurrentBodyTestValue( weight, height);
}

void CInterAction::qmlLogout( QString content )
{
    //    LOGINFO( content.toStdString() );
    LOGINFO( QString::fromUtf8( content.toUtf8() ).toStdString() );
}

void CInterAction::setTestState( QString testState )
{
    m_sCurTestState = testState;
    emit updateTestState( m_sCurTestState );
    emit sendTestStateChangeSignal();
}

void CInterAction::setDevStatus(short devType, bool isDirectConnect, short testMode, bool bActualDirectConnect)
{
    if(devType != DEV_TYPE_UNKNOW)
    {
        m_bDirectConnect = isDirectConnect;
        m_devTestMode = testMode;
        m_iDevType = devType;
    }


    CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateDevConnectMode( isDirectConnect );

    //设置设备类型
    QVariantMap devTypeInfoMap;
    devTypeInfoMap.insert( "curDevTypeValue", devType );
    m_systemSettingMgr.setSpecificElementData( CProjectEnums::PAGE_HOST,
                                               CHostSettingConfig::HostElementData::CurrentDevTypeIndex, devTypeInfoMap );

    //根据设备类型去设置测试配置中的测试项
    QVariantMap usingInfoMap = { { "devCorrespondingTestItem", devType } };
    m_systemSettingMgr.setSpecificElementData( CProjectEnums::PAGE_USING,
                                               CUsingSettingConfig::CUsingElementData::DevTYPECorresponding, usingInfoMap );

    //设置设备展示出来的是直连还是组网模式
    QVariantMap wireLessConnecteMap = { { "wireLessConnectedModel", !isDirectConnect } };
    m_systemSettingMgr.setSpecificElementData( CProjectEnums::PAGE_HOST,
                                               CHostSettingConfig::HostElementData::ConnectedModel, wireLessConnecteMap );

    //设置设备展示出来的是异步还是同步模式
    int syncTestMode;
    CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevTestMode(syncTestMode);
    QVariantMap syncTestMap = { { "syncTestModel", syncTestMode } };
    m_systemSettingMgr.setSpecificElementData( CProjectEnums::PAGE_HOST,
                                               CHostSettingConfig::HostElementData::SyncTestModel, syncTestMap );


    //设置设备实际使用的是组网还是直连， 影响是否开放设置信道号
    QVariantMap loadChannelAreaMap = { { "loadChannelConfig", !bActualDirectConnect } };
    m_systemSettingMgr.setSpecificElementData( CProjectEnums::PAGE_HOST,
                                               CHostSettingConfig::HostElementData::LoadChannelConfig, loadChannelAreaMap );

    //设置是否自动切换连接模式
    bool manulChangeConnecMode = !CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevConnectModeAuto();
    QVariantMap autoChangeConnectModeMap = { { "manulChangeMode", manulChangeConnecMode } };
    m_systemSettingMgr.setSpecificElementData( CProjectEnums::PAGE_HOST,
                                               CHostSettingConfig::HostElementData::ManulChangeMode, autoChangeConnectModeMap );


    setTestInfoSetting();

    emit signalUpdateDevStatus(devType, isDirectConnect, testMode);
    emit signalUpdateDevType(devType);
}

void CInterAction::setBtnEnableCommit(const bool &bEnable)
{
    short shBtnCommit = CProjectEnums::UNKNOWN_BTN;
    if(!m_bDirectConnect)
    {
        shBtnCommit = m_devTestMode == TEST_SYNC ? CProjectEnums::OTM_SYNC_COMMIT_BTN : CProjectEnums::OTM_ASYNC_COMMIT_BTN;
        m_btnEnableMap[shBtnCommit] = bEnable;
    }
    else
    {
        shBtnCommit = CProjectEnums::OTO_COMMIT_BTN;
    }
    setBtnEnable(shBtnCommit, bEnable);
}

void CInterAction::setTestInfoSetting()
{
    //配置是否显示成绩
    bool bDisplay = true;
    bDisplay = CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevIsDisplayScore();

    //配置页面跳转时长
    int times = -1;
    times = CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestPageCountTime(m_iDevType);

    //配置测试次数
    int count = 1;
    if(!CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestTimes(m_iDevType, count))
    {
        LOGERROR("获取数据库测试次数配置失败！");
    }

    //配置测试项
    string testItems = "";
    if(!CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestItemsStr(m_iDevType, testItems))
    {
        LOGERROR("获取数据库测试项配置失败！");
    }

    //配置短跑起点模式
    bool flag = CDatabaseManage::GetInstance().getDevTestInfoConfig()->getUseRunShortStartFlag();
    QString qsFlag = flag ? "存在起点" : "取消起点";

    //配置立定跳远长度
    int startPos = 0;
    CDatabaseManage::GetInstance().getDevTestInfoConfig()->getStandingJumpStartPos(startPos);

    //配置跳绳超时时长
    int iDelay = 2000;
    CDatabaseManage::GetInstance().getDevTestInfoConfig()->getRopeDelayTime(iDelay);

    //配置中长跑一圈长度
    int iCircleLen = 800;
    CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevRunMidLength(iCircleLen);

    //根据设备类型去设置测试配置中的测试项
    QVariantMap usingInfoMap = { { "scoreConfig", bDisplay },
                                 { "timeoutConfig", times},
                                 { "testCntConfig", count },
                                 { "curTestItemName", QString::fromStdString(testItems)},
                                 { "satrtFlagConfig", qsFlag},
                                 { "satrtPosConfig", startPos},
                                 { "ropeDelayConfig", iDelay},
                                 { "circleLengthConfig", iCircleLen} };

    m_systemSettingMgr.setSpecificElementData( CProjectEnums::PAGE_USING, -1, usingInfoMap );
}

void CInterAction::setNetConnectState( bool isConnected )
{
    m_bNetConnect = isConnected;
    emit updateNetConnectStatus(m_bNetConnect);
}

void CInterAction::delSignatureFile()
{
    if( QFileInfo::exists( m_qsMarkPath ) ){
        QFile file( m_qsMarkPath );
        if( !file.remove( m_qsMarkPath ) ){
            qDebug() << "delete old signature file failed...";
        }
    }
}

void CInterAction::setTestingPicture( int index, QString filePath )
{
    emit signalSetTestingPicture( index, filePath );
}

void CInterAction::queryHistoryScore(int testItem, int timeInterval, int scoreType)
{
    clearModelData( COMMON_DATA_MODEL );


    emit signalQueryHistoryScore( testItem, timeInterval, scoreType );  //CMainFrame::onQueryHistoryScore
}

void CInterAction::quit()
{
    if(m_pMainFrame != nullptr)
    {
        CMainFrame *pMainFrame = reinterpret_cast<CMainFrame *>(m_pMainFrame);
        pMainFrame->close();
    }
}

void CInterAction::exitLogin()
{
    emit signalExitLogin();
}

void CInterAction::responseGradeTypeChange(int testItem)
{
    emit signalGradeTypeChange(testItem);
}

void CInterAction::startSignature(QString qsShowFlag)
{
    emit signalShowSignPainterWidget(qsShowFlag);
}

void CInterAction::clearSignPainter()
{
    emit signalClearPainter();
}

void CInterAction::saveWidgetSignPainter()
{
    //每一张图都进行保存，若路径为空，使用默认路径
    if( m_qsMarkPath.isEmpty() )
    {
        m_qsMarkPath = m_qsMarkDefaultPath;
    }

    if( !m_qsMarkPath.endsWith( ".png", Qt::CaseInsensitive ) ){
        m_qsMarkPath += ".png";
    }

    //发送信息保存图片文件
    emit signalSaveToPng( m_qsMarkPath );
}

void CInterAction::changedSignPainterPos(QString width, QString height, QString x, QString y)
{
    QString cmd = QString( "changeDialog %1 %2 %3 %4" ).arg( width ).arg( height ).arg( x ).arg( y );
    QString logInfo = "change signpainter size = " + cmd;
    LOGINFO( logInfo.toStdString() );
    emit sigChangeSignPainter( cmd );
}

int CInterAction::getRunMideLength()
{
    int length = 800;
    if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevRunMidLength(length))
    {
        LOGERROR("获取中长跑数据库长度配置失败");
    }
    return length;
}

void CInterAction::setStuTestStatus(bool bCommitable, bool bTestFinished)
{
    emit signalStuTestStatus(bCommitable, bTestFinished);
}

void CInterAction::setSyncTestCountTime(int time)
{
    emit sigUpdateTestCountTime(time);
}

void CInterAction::onCheckErrorTipTimeOut()
{
    //如果队列为空，则直接返回
    if(m_queueErrorTip.size() <= 0)
    {
        return ;
    }

    m_mutexErrorTip.lock();

    //根据先进先出的原则，优先处理第一条数据
    ERRORTIP errorTip;
    errorTip = m_queueErrorTip.dequeue();

    auto itFind = m_mapErrCode.find(errorTip.iErrorCode);
    int nTemp = 0;
    if(errorTip.bDisplay)
    {
        //不存在那么添加
        if(itFind == m_mapErrCode.end())
        {
            for(auto &element : m_mapErrCode)
            {
                if(element.second > nTemp)
                    nTemp = element.second;
            }

            if(m_mapErrCode.size() > 0)
                nTemp++;

            m_mapErrCode[errorTip.iErrorCode] = nTemp;

            QString errorString = CErrorCode::getErrorDescribe( errorTip.iErrorCode );
            if(!errorString.isEmpty())
            {
                emit sigAppendViewDisplayErrorString( errorString );
            }
        }
    }
    else
    {
        if(itFind != m_mapErrCode.end())
        {
            nTemp = itFind->second;
            removeSpecificViewErrorString( nTemp );
            m_mapErrCode.erase(itFind);

            for(auto &element : m_mapErrCode)
            {
                if(element.second >= nTemp)
                    element.second--;
            }
        }
    }

    m_mutexErrorTip.unlock();
}

void CInterAction::setViewErorrString( int errorCode, bool display )
{
    m_mutexErrorTip.lock();

    //组装提示信息结构体
    ERRORTIP errorTip;
    errorTip.iErrorCode = errorCode;
    errorTip.bDisplay   = display;

    m_queueErrorTip.enqueue(errorTip);

    m_mutexErrorTip.unlock();

    //判断定时器
    if(!m_timerErrorTipTimeOut.isActive())
    {
        m_timerErrorTipTimeOut.start(m_nCheckErrorTipTimeOutInterval);
    }

    //    //如果新增存在相同的错误, 则直接返回
    //    if( m_vecErrorCode.contains( errorCode ) && display ){
    //        locker.unlock();
    //        return;
    //    }

    //    //如果是不显示对应的错误信息
    //    if( !display ){
    //        int index = m_vecErrorCode.indexOf( errorCode );
    //        //-1 代表没有这个元素, 则直接退出
    //        if( -1 == index ){
    //            locker.unlock();
    //            return;
    //        }
    //        //如果有这个元素,则向界面发送取消显示的信号
    //        m_vecErrorCode.removeAt( index );
    //        removeSpecificViewErrorString( index );
    //        locker.unlock();
    //        return;
    //    }

    //    //如果是显示对应的错误信息,则直接添加显示
    //    m_vecErrorCode << errorCode;
    //    QString errorString = CErrorCode::getErrorDescribe( errorCode );
    //    if(errorString == ""){
    //        locker.unlock();
    //        return;
    //    }
    //    locker.unlock();
    //    emit sigAppendViewDisplayErrorString( errorString );
}

void CInterAction::removeSpecificViewErrorString(int index)
{
    emit sigRemoveSpecificErorrString( index );
}

void CInterAction::clearViewErrorString()
{
    emit sigClearViewErorrStrings();
}

void CInterAction::setSelfCheckPersent(CInterAction::SELF_CHECK_ITEM item, bool ok)
{
    static short totalItemCount = static_cast<short>(SELF_CHECK_ITEM::Self_check_end);
    static short finishedCount = 0;

    //如果当前自检项 item 失败, 则执行对应的操作
    if( !ok ){
        return;
    }

    //如果传入的是  Self_check_end， 则直接自检完成
    if( Self_check_end == item ){
        setProcessVal( 1.0 );
        return;
    }

    //如果当前自检项成功, 则增加进度条
    finishedCount += 1;
    float tmpTotal = static_cast<float>(totalItemCount);
    float tmpFinished = static_cast<float>(finishedCount);
    float checkPersent = tmpFinished / tmpTotal;

    //只保留小数点后两位
    QString tmpPersent = QString::number( checkPersent, 'f', 2 );
    checkPersent = tmpPersent.toFloat();
    setProcessVal( checkPersent );
}

void CInterAction::testSetSelfCheck(int val)
{
    SELF_CHECK_ITEM item = static_cast<SELF_CHECK_ITEM>(val);
    setSelfCheckPersent( item );
}

void CInterAction::closeCommitPopup()
{
    emit sigCloseCommitScorePopup();
}

void CInterAction::testFunc1()
{
    //    QVector<QMap< QString, QVariant >> vecs;
    //    vecs << testFunc2("2834");
    //    vecs << testFunc2("2835");
    //    vecs << testFunc2("2836");
    //    vecs << testFunc2("2837");
    //    vecs << testFunc2("2838");
    //    vecs << testFunc2("2839");
    //    vecs << testFunc2("2840");
    //    vecs << testFunc2("2841");
    //    vecs << testFunc2("2842");
    //    vecs << testFunc2("2843");
    //    vecs << testFunc2("2844");
    //    vecs << testFunc2("2845");
    //    vecs << testFunc2("2846");
    //    vecs << testFunc2("2847");
    //    vecs << testFunc2("2848");
    //    vecs << testFunc2("2849");
    //    vecs << testFunc2("2850");
    //    vecs << testFunc2("2851");
    //    vecs << testFunc2("2852");
    //    vecs << testFunc2("2853");
    //    vecs << testFunc2("2854");
    //    vecs << testFunc2("2855");
    //    vecs << testFunc2("2856");
    //    vecs << testFunc2("2857");
    //    vecs << testFunc2("2858");
    //    vecs << testFunc2("2859");
    //    vecs << testFunc2("2860");
    //    vecs << testFunc2("2861");
    //    vecs << testFunc2("2862");
    //    vecs << testFunc2("2863");
    //    vecs << testFunc2("2864");
    //    vecs << testFunc2("2865");
    //    vecs << testFunc2("2866");
    //    vecs << testFunc2("2867");
    //    vecs << testFunc2("2868");
    //    vecs << testFunc2("2869");
    //    vecs << testFunc2("2870");
    //    vecs << testFunc2("2871");
    //    vecs << testFunc2("2872");
    //    vecs << testFunc2("2873");
    //    vecs << testFunc2("2874");
    //    vecs << testFunc2("2875");
    //    vecs << testFunc2("2876");
    //    vecs << testFunc2("2877");
    //    vecs << testFunc2("2878");
    //    vecs << testFunc2("2879");
    //    vecs << testFunc2("2880");
    //    vecs << testFunc2("2881");
    //    vecs << testFunc2("2882");
    //    vecs << testFunc2("2883");
    //    vecs << testFunc2("2884");
    //    vecs << testFunc2("2885");
    //    vecs << testFunc2("2886");
    //    vecs << testFunc2("2887");
    //    vecs << testFunc2("2888");
    //    vecs << testFunc2("2889");
    //    vecs << testFunc2("2890");


    //    //    m_pRequestMgr->setSyncCommonScores( vecs );
    //    //    m_pRequestMgr->startSyncLocalScore();
    //    m_pRequestMgr->setVecUnUploadCommonScore( vecs );
}

QMap<QString, QVariant> CInterAction::testFunc2(QString userId)
{
    QMap< QString, QVariant > tmp;
    tmp.insert( HTTP_KEYS::userId, userId );
    tmp.insert( HTTP_KEYS::devCode, "test_Dev_1" );
    tmp.insert( HTTP_KEYS::devType, DEV_TYPE::HTTP_DEV_TYPE_RUN_MID );
    tmp.insert( HTTP_KEYS::type, testType::examType );
    tmp.insert( HTTP_KEYS::testItem, TEST_ITEM::HTTP_TEST_ITEM_RUN_1000 );
    tmp.insert( HTTP_KEYS::result, "50" );
    tmp.insert( HTTP_KEYS::recordTime, QString::number( CHttpAbstractNetWork::currentTimeStamp() ) );

    return tmp;
}

void CInterAction::testFunc3()
{
    //学号  studentID_
    emit sigTestSignal( "AA1101" );           //queryStudentByUniqueID
}

void CInterAction::testFunc4()
{
    saveCommitPopupParamToDataBase(1, 0, 0, 6 );
}

void CInterAction::tryToGetMaxTestResult()
{
    emit sigTryTogetMaxTestResult();
}

void CInterAction::closeConfirmPopup()
{
    emit sigCloseConfirmPopup();
}

void CInterAction::setDisplayPowerType(bool bIsBatteryExisted)
{
    QString powerType = "220V";
    if( bIsBatteryExisted ){
        powerType = "电池供电";
    }

    QString logIngo = "交互类->收到电源类型变更为: " + powerType;
    LOGINFO( logIngo.toStdString() );
    emit signalSetPowerType( bIsBatteryExisted );
}

void CInterAction::setDisplayChargeState(bool bCharged)
{
    emit signalSetDisplayChargeState( bCharged );
}

void CInterAction::setDisplayBattreyQuantity(int quantity)
{
    QString tmpQuantity = QString::number( quantity );
    emit signalBatteryQuantityChanged( tmpQuantity );
}

void CInterAction::changedTestModel(short testModel)
{
    m_devTestMode = testModel;

    emit sigTestModelChanged( testModel );
}

bool CInterAction::changedRunMidLength(int iLength)
{
    //保存至数据库
    if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateDevRunMidLength(iLength))
    {
        LOGERROR("中长跑长度配置更新至数据库失败！");
        return false;
    }
    //下发至硬件
    emit sigRunMidLengthChanged(iLength);
    return true;
}

void CInterAction::setIsDisplayScore(bool bDisplay)
{
    if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateDevDisplayScore(bDisplay))
    {
        LOGERROR("成绩显示：是/否更新至数据库失败！");
    }
}

bool CInterAction::getIsDisplayScore()
{
    bool bDisplay = true;
    bDisplay = CDatabaseManage::GetInstance().getDevTestInfoConfig()->getDevIsDisplayScore();
    return bDisplay;
}

void CInterAction::getDevChannelConfig()
{
    emit sigGetCurDevChannel();     //CMainFrame::onGetCurDevChannel
}

void CInterAction::getDevMaxConnectCount()
{
    emit sigGetCurDevConnectNum();
}

void CInterAction::reTest()
{
    emit signalClickBtnReTest();
}

void CInterAction::setChannelConfig(int channel, int devMaxNum)
{
    emit sigDevChannelConfigChanged(channel, devMaxNum);
}

QString CInterAction::getChannelCongigJsonStr()
{
    return CDevDataCfg::getS_mapChaInfo();
}

void CInterAction::setCurDevNo(QString dev, int num)
{
    emit sigSetDevNumber(dev, num); //CMainFrame::onSetDevExtensionNum
}

void CInterAction::delStudentScore(QString index)
{
    emit sigDelScore(index);
}

//index 表中行号
void CInterAction::unBindStudent(QString index)
{
    emit sigUnbindUser(index);
}

void CInterAction::procAddUserID(bool bStart, QString id)
{
    emit sigProcAddUserID(bStart, id); //onStartProcAddUserID
}

QString CInterAction::getTestState()
{
    return m_sCurTestState;
}

float CInterAction::getProcessVal()
{
    return m_iProcessVal;
}

void CInterAction::setProcessVal( float selfCheckPersent )
{
    m_iProcessVal = selfCheckPersent;
    emit updateProcessVal( m_iProcessVal );
}

void CInterAction::readConfigFile()
{
    m_isDeveloper = CDatabaseManage::GetInstance().getServerFunConfig().getIsDeveloper();
}

void CInterAction::cleanDataModel()
{
    m_commonDataModel.deleteAll();
    m_studentDataModel.deleteAll();
    m_scoreDataModel.deleteAll();
    m_devNumDataModel.deleteAll();
}

void CInterAction::setUpdateCurTestRet(bool bSuccess)
{
    emit signalGetUpdateTestRet( bSuccess );
}

void CInterAction::advancedReportToViewRecogRet( int ret )
{
    emit signalAdvanceReportToViewRecogRet( ret );

    //每识别一次就检测一次是否存在签字文件,如果存在则删除
    delSignatureFile();
}

void CInterAction::onFaceRecogSuccess( const CUserInfo &userInfo )
{
    m_objCurStuInfo = userInfo;
    m_sCurStuName = userInfo.m_strName;
    m_sCurStuAge  = userInfo.m_strBrithday;

    short shAge = userInfo.getAge();
    if(shAge >= 0)
    {
        m_sCurStuAge = QString::number(shAge);
    }


    if(!userInfo.m_qsClassName.isEmpty())
    {
        m_sCurClass = userInfo.m_qsClassName;
    }
    else
    {
        calculateStuGrade(userInfo.m_nClass);
    }

    m_sCurStuGrade = QString::number( userInfo.getGrade());
    //m_sCurClass = QString::number( stuInfo.m_nClass );

    m_sCurStuSchoolNO = userInfo.m_strStudentId;
    m_sCurStuSex =  userInfo.m_bGender ;
    emit signalIdentifyUserSuccess();
}

void CInterAction::onSelfCheckFinished( bool bSuccess )
{
    qDebug()<< "check finish" << bSuccess;
    emit signalReportSelfCheckRet( bSuccess );
}

void CInterAction::textToSpeech(QString content)
{
    CTextToSpeech::GetInstance().textToSpeech( content );
}

void CInterAction::stopSpeech()
{
    CTextToSpeech::GetInstance().stop();
}

void CInterAction::getAllDevToSetNum()
{
    emit sigGetAllDevToSetNum();    //onGetAllDevExtensionNum
}

void CInterAction::setCorrectProgress( int step, unsigned int devNumber, QVariant val )
{
#if QT_NO_DEBUG
#else
    QString debugInfo = QString( "correct step = %1, devNumber = %2, val = %3" )
            .arg( QString::number( step ) ).arg( QString::number( devNumber )).arg( val.toString() );
    qDebug() << debugInfo;
#endif
    emit sigCorrectProgress( step, devNumber, val );
}

void CInterAction::updateDevModelData(QString dev, QString qsVisble)
{
    updateModelCellData(DEV_NUM_DATA_MODEL, 0, dev, 3, qsVisble);
}

void CInterAction::startElapsedTimer()
{
    m_time = QTime::currentTime();
}

void CInterAction::stopElapsedTimer(QString tips)
{
    QTime endTime = QTime::currentTime();
    int cost = m_time.msecsTo( endTime );
    qDebug() << "cost " << cost;
    tips = tips + " 用时 " + QString::number( cost ) + "ms ####################";
    LOGINFO( tips.toStdString() );
}

QVariantMap CInterAction::getSystemConfigInfo(int viewPageIndex, int elementIndex)
{
    //刷新设备号等信息
    refreshDevDetailInfo();
    QVariantMap ret =  m_systemSettingMgr.getSpecificElementData( viewPageIndex, elementIndex );

    return ret;
}

void CInterAction::setSystemConfigInfo( int viewPageIndex, QVariantMap newConfigInfo, int elementIndex )
{
#if QT_NO_DEBUG// enum SystemSettingPage
#else
    QMetaEnum tmp = QMetaEnum::fromType<CProjectEnums::SystemSettingPage>();
    qDebug() << "new config view name = " << tmp.valueToKey( viewPageIndex );
#endif
    m_systemSettingMgr.setSpecificElementData( viewPageIndex, elementIndex, newConfigInfo );

    //如果是测试配置界面的改动，发这个信号
    if( viewPageIndex == CProjectEnums::PAGE_USING ){
        emit sigTestItemChanged();
    }

    if(m_systemSettingMgr.isExistChangeItem())
    {
        if(m_pMainFrame == nullptr)
        {
            return;
        }

        CMainFrame *pMain = reinterpret_cast<CMainFrame *>(m_pMainFrame);
        pMain->onCfgChange();
    }
}

QStringList CInterAction::queryCorrespondingChannelList(int devType)
{
    QStringList retList = CDevDataCfg::getDevSignalChannelList( devType );
    return retList;
}

void CInterAction::setCamerData(const cv::Mat &matImage)
{
    if( matImage.empty() )
        return;

    if(!CDevDataCfg::isHomePage(m_iCurPage))
    {
        return;
    }

    cv::Mat partImage = matImage.clone();
    dealImage(partImage);
    //cv::imshow("hhh", partImage);
    //cv::waitKey(1);

    //处理摄像头桌面和显示图像实际逻辑相反问题，将图像水平翻转
    cvtColor(partImage, partImage, cv::ColorConversionCodes::COLOR_BGR2RGB);
    m_objImgProvider.setimage( partImage.data, partImage.cols, partImage.rows);
    emit signalNewQimageAvailable();
}

void CInterAction::calculateStuGrade(int stuClass)
{
    //保存到数据库中
    CTableClassInfo * pClassInfo = nullptr;

    pClassInfo = CDatabaseManage::GetInstance().getTabClassInfo();
    if(pClassInfo != nullptr)
    {
        string m_stuClass;
        bool ret = pClassInfo->getClassInfo( stuClass,m_stuClass);
        if(ret)
        {
            QString stuClass = QString::fromStdString( m_stuClass);
            m_sCurClass = stuClass;
        }
    }
}

void CInterAction::otmTestUserRecog()
{
    static int index = 1;

    if( index >= 21 ){
        index = 1;
    }

    static QString testUserId = "";
    if( index <= 9 && index >= 1 ){
        testUserId = "st0" + QString::number( index );
    }else{
        testUserId = "st" + QString::number( index );
    }
    index++;
    emit sigTestSignal( testUserId );
}

CCommonDataModel *CInterAction::getDataModelByType(const int &iDataModelType)
{
    CCommonDataModel *pDataModel = nullptr;
    switch (iDataModelType) {
    case COMMON_DATA_MODEL:
    {
        pDataModel = &m_commonDataModel;
        break;
    }
    case STUDENT_DATA_MODEL:
    {
        pDataModel = &m_studentDataModel;
        break;
    }
    case SCORE_DATA_MODEL:
    {
        pDataModel = &m_scoreDataModel;
        break;
    }
    case DEV_NUM_DATA_MODEL:
    {
        pDataModel = &m_devNumDataModel;
        break;
    }
    default:
        break;
    }

    return pDataModel;
}

CSystemSettingMgr& CInterAction::getSystemSettingMgr()
{
    return m_systemSettingMgr;
}

short CInterAction::getCurCorrectItem() const
{
    return m_curCorrectItem;
}

int CInterAction::getPageTimeout()
{
    return CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestPageCountTime(m_iDevType);
}

void CInterAction::correctHeight()
{
    m_curCorrectItem = TEST_ITEM_HEIGHT;
//    setCorrectProgress( 0 );
}

void CInterAction::correctWidth( int weight )
{
    m_curCorrectItem = TEST_ITEM_WEIGHT;
    //    setCorrectProgress( 0 );
}

void CInterAction::setCurCorrectItem(short item)
{
    m_curCorrectItem = item;
}

bool CInterAction::getIsDeveloper() const
{
    return m_isDeveloper;
}

void CInterAction::setIsDeveloper(bool ok)
{
    if( ok == m_isDeveloper ){
        return;
    }
    m_isDeveloper = ok;

    emit sigIsDeveloperChanged();
}

void CInterAction::closeRootPopup()
{
    emit sigCloseRootPopup();
}

void CInterAction::appendPVVChartPoint(int chartType, float xVal, float yVal, int stageType)
{
    emit sigAddPointsToPVVChart(chartType, stageType, xVal, yVal);
}

void CInterAction::setPVVSamplingRate(int iSR)
{
    emit sigChangePVVSampleRate(iSR);
}

void CInterAction::setConnectedDevNumbers(const QMap<unsigned int, int> &devs)
{
    if( devs.size() <= 0 ){
        return;
    }

    QVariantMap tmpVariantMap;
    m_listConnectedDevNumbers.clear();
    for( auto iter = devs.begin(); iter != devs.end(); iter++ ){
        tmpVariantMap.clear();
        tmpVariantMap[ "devNumber" ] = iter.key();
        tmpVariantMap[ "devOrder" ]  = iter.value();
        m_listConnectedDevNumbers << tmpVariantMap;
    }
}

bool CInterAction::getBtnEnableState(short btnType)
{
    bool bEnable = false;
    auto it = m_btnEnableMap.find(btnType);
    if(it != m_btnEnableMap.end())
    {
        bEnable = it.value();
    }
    return bEnable;
}

QVariantList CInterAction::getConnectedDevNumbers()
{
    //    for( int i = 0 ; i < 2; i++ ){
    //        QVariantMap tmp;
    //        tmp[ "devNumber" ] = 20 + i;
    //        tmp[ "devOrder" ]  = 22 + i;

    //        m_listConnectedDevNumbers << tmp;
    //    }
    return m_listConnectedDevNumbers;
}

int CInterAction::getRopeSkipRecvDelay()
{
    int iTime = 2000;
    if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->getRopeDelayTime(iTime))
    {
        LOGERROR("获取中长跑数据库长度配置失败");
    }
    return iTime;
}

bool CInterAction::getRunShortStartFlag()
{
    bool flag = CDatabaseManage::GetInstance().getDevTestInfoConfig()->getUseRunShortStartFlag();
    return flag;
}

void CInterAction::setRunshortStartFlag(bool startFlag)
{
    if(!CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateUseRunShortStartFlag(startFlag))
    {
        LOGERROR("数据库更新测试相关配置失败！");
    }
}

void CInterAction::readCommitPopupConfigFromDataBase()
{
    CCommitPopupConfig infos;
    bool ok = CDatabaseManage::GetInstance().getPCommitTableConfig()->queryPopupConfig( infos );
    
    int btnEnable = 1;
    int type = 0;
    int signature = 0;
    int countTime = 0;
    int userInfoPageExisted = 1;

    if( ok ){
        btnEnable           = infos.m_delBtnEnable;
        type                = infos.m_commitType;
        signature           = infos.m_signatureExisted;
        countTime           = infos.m_autoCommitCountTime;
        userInfoPageExisted = infos.m_userInfoPageExisted;
    }

    setCommitPopupParam( btnEnable, type, signature, countTime, userInfoPageExisted );
}

void CInterAction::changeDevConfigInfo(QString jsonString)
{
    QJsonDocument jsDoc = QJsonDocument::fromJson( jsonString.toUtf8() );
    QJsonObject jsObj = jsDoc.object();

    QVariantMap varMap = jsObj.toVariantMap();
    //    qDebug() << "ret = " << varMap.value( "devType" ).toString();
    emit sigDevConfigInfoChanged( varMap );
}

bool CInterAction::setDevTestConfig(int devType, int testTimes, int waitTimes, QString qsTestItems)
{
    if(!CDatabaseManage::GetInstance().getDevSWInfoConfig()->updateDevSWTestConfig(devType, testTimes, qsTestItems.toStdString()))
    {
        LOGERROR("数据库更新测试相关配置失败！");
        return false;
    }

    static QVariantMap infoMap;
    infoMap.insert( "curDevTypeValue", devType );
    m_systemSettingMgr.setSpecificElementData( CProjectEnums::SystemSettingPage::PAGE_HOST, 5, infoMap );

    emit sigTestItemChanged();
    return true;
}

int CInterAction::getDevTestTimes(int devType)
{
    int times = 1;
    if(!CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestTimes(devType, times))
    {
        LOGERROR("获取数据库测试次数配置失败！");
    }
    return times;
}

int CInterAction::getDevWaitStartTime(int devType)
{
    int startTime = 1500;
    if(!CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestStartWaitTime(devType, startTime))
    {
        LOGERROR("数据库更新测试开始等待时间失败！");
    }
    return startTime;
}

QString CInterAction::getDevTestItems(int devType)
{
    QString qsItem = QString::number(CProjectEnums::TestHeight) + "-" + QString::number(CProjectEnums::TestWeight);
    string items = qsItem.toStdString();
    if(!CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestItemsStr(devType, items))
    {
        LOGERROR("数据库更新测试开始等待时间失败！");
    }
    return QString::fromStdString(items);
}

QString CInterAction::getMultiDevCount() const
{
    return m_multiDevCount;
}

void CInterAction::setMultiDevCount(const QString &multiDevCount)
{
    m_multiDevCount = multiDevCount;
    emit sigMultiDevCountChanged();
}

QString CInterAction::getMultiUserCount() const
{
    return m_multiUserCount;
}

void CInterAction::setMultiUserCount(const QString &multiUserCount)
{
    m_multiUserCount = multiUserCount;
    emit sigMultiUserCountChanged();
}

QString CInterAction::getCommitPersent() const
{
    return m_commitPersent;
}

void CInterAction::setCommitPersent(const QString &commitPersent)
{
    m_commitPersent = commitPersent;
}

QString CInterAction::getOtoMaxRet() const
{
    return m_otoMaxRet;
}

void CInterAction::setOtoMaxRet(const QString &otoMaxRet)
{
    m_otoMaxRet = otoMaxRet;
    qDebug() << "set oto max ret = " << otoMaxRet;
    LOGINFO("================     set oto max ret=  " + otoMaxRet.toStdString() );
    emit sigOtoMaxResultChanged();
}

void CInterAction::setOtoBodyValue(const QString &weight, const QString &height)
{
    m_otoWeightVal = weight;
    m_otoHeightVal = height;
}

QString CInterAction::getOtoBodyWeight()
{
    return m_otoWeightVal;
}

QString CInterAction::getOtoBodyHeight()
{
    return m_otoHeightVal;
}

void CInterAction::testStarted()
{
    emit sigTestStarted();
}

void CInterAction::setBtnEnable(short btnType, bool bEnable)
{
    m_btnEnableMap[btnType] = bEnable;
    emit sigSetBtnEnable(btnType, bEnable);
}

QString CInterAction::getSignatureFilePath(const bool &bDefault) const
{
    if( bDefault ){
        return m_qsMarkDefaultPath;
    }
    return m_qsMarkPath;
}

void CInterAction::procBtnClicked(short btnType)
{
    switch (btnType) {
    //开始按钮 一对一 用户/游客 一对多
    case CProjectEnums::OTO_START_STU_TEST_BTN:
    case CProjectEnums::OTM_START_TEST_BTN:
    case CProjectEnums::OTO_START_TUR_TEST_BTN:
    {
        bool bTur = btnType == CProjectEnums::OTO_START_TUR_TEST_BTN;
        emit signalClickBtnStartTest(bTur);
        break;
    }
        //查询成绩：暂不统一处理
    case CProjectEnums::OTO_QUERY_SCORCE_BTN:
    {
        break;
    }
        //信息页面返回人脸识别页面
    case CProjectEnums::OTO_INFO_BACK_HOME_BTN:
    {
        CTextToSpeech::GetInstance().textToSpeech("返回首页", 1300);
        emit signalClickBtnInfoPageBack();
        break;
    }
        //测试页面返回
    case CProjectEnums::OTO_TEST_BACK_INFO_BTN:
    {
        emit signalClickBtnTestPageBack();
        break;
    }
        //信息页面返回
    case CProjectEnums::OTO_TEST_BACK_HOME_BTN:
    {
        emit signalClickBtnBackHome();
        break;
    }
        //提交按钮
    case CProjectEnums::OTO_COMMIT_BTN:
    case CProjectEnums::OTM_SYNC_COMMIT_BTN:
    case CProjectEnums::OTM_ASYNC_COMMIT_BTN:
    {
        emit signalClickBtnSubmitResult();
        break;
    }
        //提交确认按钮
    case CProjectEnums::OTO_VERIFY_SIGN_BTN:
    case CProjectEnums::OTM_VERIFY_SIGN_BTN:
    {
        emit signalClickBtnVerifySubmitResult();    //CMainFrame::onClickBtnVerifySubmitResult
        break;
    }
        //清除签字板
    case CProjectEnums::OTO_CLEAR_SIGN_BTN:
    case CProjectEnums::OTM_CLEAR_SIGN_BTN:
    {
        clearSignPainter();
        break;
    }
        //一对多同步取消测试
    case CProjectEnums::OTM_CANCEL_TEST_BTN:
    {
        emit signalClickBtnCancelTest();
        break;
    }
    case CProjectEnums::OTM_ASYNC_CANCEL_BTN:
    {
        //异步退出测试
        emit signalClickBtnCancelTest();
        break;
    }

    case CProjectEnums::OTM_RESTART_TEST_BTN:
    {
        //重测点击
        emit signalClickBtnReTest();
        break;
    }
    default:
    {
        LOGERROR("不能识别的按钮类型");
    }
    }
}

bool CInterAction::getIsCommitTableEmpty() const
{
    return m_isCommitTableEmpty;
}

void CInterAction::setIsCommitTableEmpty(bool isCommitTableEmpty)
{
    m_isCommitTableEmpty = isCommitTableEmpty;
    emit sigCommitTableChanged(isCommitTableEmpty);
}

QString CInterAction::getDevVideoPath(short devType)
{
    return CDatabaseManage::GetInstance().getTestProcessVideoPathConfig().getGudieVideoPath(devType);
}

bool CInterAction::checkDevVideoPathExist(QString path)
{
    QFile videoFile(path);
    if(videoFile.exists())
    {
        return true;
    }
    return false;
}

void CInterAction::autoCommite()
{
    emit sigOpenCommitPopup();
}

void CInterAction::setCommitPopupParam(int delScoreBtnEnable, int commitType, int signatureExist, int autoCountTime, int userInfoPage )
{
    emit sigCommitPopupParamChanged( delScoreBtnEnable, commitType, signatureExist, autoCountTime, userInfoPage );
}

void CInterAction::catchScanCodeInfo(QString code)
{
    emit sigCatchScanCodeInfo(code);
}
void CInterAction::closeCommitStatePopupAbNorlmally()
{
    emit sigCloseCommitStatePopup();
}

void CInterAction::saveCommitPopupParamToDataBase(int delScoreBtnEnable, int type, int signatureExist, int countTime)
{
    if( delScoreBtnEnable < 0 || delScoreBtnEnable > 1 ){
        delScoreBtnEnable = 0;
    }

    if( type < 0 || type > 1 ){
        type = 0;
    }

    if( signatureExist < 0 || signatureExist >1 ){
        signatureExist = 0;
    }

    if( countTime < 0 ){
        countTime = 0;
    }

    CCommitPopupConfig infos;
    infos.m_delBtnEnable = delScoreBtnEnable;
    infos.m_commitType = type;
    infos.m_signatureExisted = signatureExist;
    infos.m_autoCommitCountTime = countTime;

    bool ok = CDatabaseManage::GetInstance().getPCommitTableConfig()->updatePopupConfig( infos );

    if( !ok ){

    }
}

QString CInterAction::getCurUserName() const
{
    return m_curUserName;
}

void CInterAction::setCurUserName(const QString &curUserName)
{
    m_curUserName = curUserName;
    emit sigRecogNewUser();
}

QString CInterAction::getSerrorString() const
{
    return m_serrorString;
}

void CInterAction::setErrorString(const QString &serrorString)
{
    m_serrorString = serrorString;
    emit sigErrorStringChanged();
}

int CInterAction::getCommitTotalCount() const
{
    return m_commitTotalCount;
}

void CInterAction::setCommitTotalCount(int commitTotalCount)
{
    m_commitTotalCount = commitTotalCount;
}

int CInterAction::getCommitFinished() const
{
    return m_commitFinished;
}

void CInterAction::setCommitFinished(int commitFinished)
{
    m_commitFinished = commitFinished;
    emit sigCommitFinishedCountChanged();
}

short CInterAction::getIDevType() const
{
    return m_iDevType;
}

void CInterAction::setIDevType(short iDevType)
{
    m_iDevType = iDevType;
}

void CInterAction::appendModelData(int modelType, QStringList data, bool isOrdered)
{
    CCommonDataModel *pDataModel = getDataModelByType(modelType);
    if(pDataModel == nullptr)
    {
        LOGERROR("unknowDataModel modelType = " + to_string(modelType));
        return;
    }

    if(modelType == COMMON_DATA_MODEL)
    {
        pDataModel->addRow(data, isOrdered);
    }
    else
    {
        pDataModel->addRow(data);
    }
}

void CInterAction::updateModelRowData(int modelType, int columnIndex, QString columnData, QStringList data)
{
    if(columnIndex < 0)
    {
        LOGERROR("update Index less than 0");
        return;
    }

    CCommonDataModel *pDataModel = getDataModelByType(modelType);
    if(pDataModel == nullptr)
    {
        LOGERROR("unknowDataModel modelType = " + to_string(modelType));
        return;
    }

    pDataModel->updateRowData(columnIndex, columnData, data);
}

void CInterAction::updateModelCellData(int modelType, int columnIndex, QString columnData, int updateIndex, QString data)
{
    if(columnIndex < 0 || updateIndex < 0)
    {
        LOGERROR("update Index less than 0");
        return;
    }

    CCommonDataModel *pDataModel = getDataModelByType(modelType);
    if(pDataModel == nullptr)
    {
        LOGERROR("unknowDataModel modelType = " + to_string(modelType));
        return;
    }

    pDataModel->updateCellData(columnIndex, columnData, updateIndex, data);
}

void CInterAction::deleteModelData(int modelType, int columnIndex, QString columnData)
{
    if(columnIndex < 0)
    {
        LOGERROR("update Index less than 0");
        return;
    }

    CCommonDataModel *pDataModel = getDataModelByType(modelType);
    if(pDataModel == nullptr)
    {
        LOGERROR("unknowDataModel modelType = " + to_string(modelType));
        return;
    }

    pDataModel->deleteRow(columnIndex, columnData);
}

bool CInterAction::checkModelDataExist(int modelType, const int &colIndex, const QString &columData, int limitRowIndex)
{
    if(colIndex < 0)
    {
        LOGERROR("checkModelDataExist Index less than 0");
        return false;
    }

    CCommonDataModel *pDataModel = getDataModelByType(modelType);
    if(pDataModel == nullptr)
    {
        LOGERROR("unknowDataModel modelType = " + to_string(modelType));
        return false;
    }

    return pDataModel->checkCellDataExist(colIndex, columData, limitRowIndex);
}

int CInterAction::getModelDataRows(int modelType)
{
    int nRows = 0;
    CCommonDataModel *pDataModel = getDataModelByType(modelType);
    if(pDataModel != nullptr)
    {
        LOGERROR("unknowDataModel modelType = " + to_string(modelType));
        nRows = pDataModel->getRows();
    }

    return nRows;
}

void CInterAction::clearModelData(int modelType)
{
    switch (modelType) {
    case COMMON_DATA_MODEL:
    {
        m_commonDataModel.deleteAll();
        break;
    }
    case STUDENT_DATA_MODEL:
    {
        m_studentDataModel.deleteAll();
        break;
    }
    case SCORE_DATA_MODEL:
    {
        m_scoreDataModel.deleteAll();
        break;
    }
    case DEV_NUM_DATA_MODEL:
    {
        m_devNumDataModel.deleteAll();
        break;
    }
    case ALL_DATA_MODEL:
    {
        m_commonDataModel.deleteAll();
        m_studentDataModel.deleteAll();
        m_scoreDataModel.deleteAll();
        m_devNumDataModel.deleteAll();
        break;
    }
    default:
    {
        LOGERROR("表格类型未知，数据添加失败");
    }
    }
}

QString CInterAction::getModelCellData(const int &modelType, int nRow, int nCol)
{
    QString qsRet("");
    if(nCol >= 0 && nRow >= 0)
    {
        CCommonDataModel *pDataModel = getDataModelByType(modelType);
        if(pDataModel != nullptr)
            qsRet = pDataModel->getCellData(nCol, nRow);
        else
            LOGERROR("unknowDataModel modelType = " + to_string(modelType));
    }
    else
        LOGERROR("nCol || nRow < 0");

    return qsRet;
}

//更新中长跑设备有效刷卡倒计时
void CInterAction::setNFCVaildTime(int iMsec)
{
    emit updateNFCRecogTime(iMsec);
}

void CInterAction::setTestTipText(QString tip)
{
    emit siganUpdateTestTip(tip);
}

void CInterAction::stopNFCValidTimer()
{
    emit sigStopNFCRecogTimer();
}

void CInterAction::closeBindPopup()
{
    emit sigCloseBindPopup();
}

void CInterAction::nfcValidTimeOut()
{
    emit sigNfcValidTimeOut();
}

//设置当前测试类型
void CInterAction::setCurTestType(short testType)
{
    m_iCurTestType = testType;
}

//获取当前测试类型
short CInterAction::getCurTestType()
{
    return m_iCurTestType;
}

//发送设备异常信号
void CInterAction::setDevTestError()
{
    emit signalDevTestError();
}

void CInterAction::showAbNormalPopup()
{
    emit signalShowAbNormalPopup();
}
