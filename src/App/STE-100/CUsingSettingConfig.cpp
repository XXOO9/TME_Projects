#include "CUsingSettingConfig.h"

CUsingSettingConfig::CUsingSettingConfig()
{
    m_timeoutSelectList << "30" << "60" << "90" << "120" << "150";
    m_testCountList << "1" << "2" << "3" << "4" << "5";
    m_startFlagList << QString::fromLocal8Bit("存在起点") << QString::fromLocal8Bit("取消起点");
    m_sbjStatrPosList << "0" << "80";

    m_scoreSelectMap = { { QString::fromLocal8Bit( "显示" ), true },
                         { QString::fromLocal8Bit( "不显示" ), false }};

    initTestItemList();
    initQueryTestItemList();
}

QVariantMap CUsingSettingConfig::getSpecificDataElement(const int element)
{
    QVariantMap usingJson;

    switch ( element ) {

    case CUsingSettingConfig::CUsingElementData::Timeout:
        usingJson[ timeoutConfig ] = m_timeout;
        break;

    case CUsingSettingConfig::CUsingElementData::TestCount:
        usingJson[ testCntConfig ] = m_testCnt;
        break;
    case CUsingSettingConfig::CUsingElementData::TestStartPos:
        usingJson[ satrtPosConfig ] = m_standingJumpStartPos;
        break;
    case CUsingSettingConfig::CUsingElementData::TestCircleLen:
        usingJson[ circleLengthConfig ] = m_runMidLength;
        break;
    case CUsingSettingConfig::CUsingElementData::QueryTestItem:
        usingJson[ queryTestItemConfig ] = translateVariantMap2JsonArray(m_queryItemMap, "itemName", "itemValue");
        break;
    default:
        usingJson[ timeoutConfig ]       = m_timeout;
        usingJson[ testCntConfig ]       = m_testCnt;
        usingJson[ scoreSelectConfig ]   = translateVariantMap2JsonArray(m_scoreSelectMap, "itemName", "itemValue");
        usingJson[ timeoutSelectConfig ] = m_timeoutSelectList;
        usingJson[ testCntSelectConfig ] = m_testCountList;
        usingJson[ startFlagSelectConfig ] = m_startFlagList;
        usingJson[ satrtFlagConfig ]     = m_runShortStartFlag;
        usingJson[ sbjStartPosSelectConfig ] = m_sbjStatrPosList;
        usingJson[ satrtPosConfig ]      = m_standingJumpStartPos;
        usingJson[ ropeDelayConfig ]     = m_ropeRecvDealyTime;
        usingJson[ circleLengthConfig ]  = m_runMidLength;
        usingJson[ scoreConfig ]         = m_scoreSelectMap.key(m_scoreDisPlay);
        usingJson[ devType ]             = translateVariantMap2JsonArray( m_curTestItemMap, "itemName", "itemValue" );
        usingJson[ curTestItemName ]     = m_curTestItemMap.key( m_curTestItemValue );
        usingJson[ curTestItem ]         = m_curTestItemValue;
        break;
    }

    return usingJson;
}

void CUsingSettingConfig::setSpecificElementData( const int &elementIndex, const QVariantMap &newConfigInfo )
{
    switch ( elementIndex ) {
    case CUsingSettingConfig::CUsingElementData::Timeout:
        m_timeout = newConfigInfo.value( timeoutConfig ).toInt();
        break;

    case CUsingSettingConfig::CUsingElementData::TestCount:
        m_testCnt = newConfigInfo.value( testCntConfig ).toInt();
        break;

    case CUsingSettingConfig::CUsingElementData::DevTYPECorresponding:
        setCurDevCorrespondingTestItems( newConfigInfo.value( devType ).toInt() );
        break;
    default:
        setAllConfig( newConfigInfo );
        break;
    }
}

void CUsingSettingConfig::setAllConfig(const QVariantMap &srcMap)
{
    resetChangeItem();

    //成绩显示更换
    if(m_scoreDisPlay != srcMap.value(scoreConfig).toBool())
    {
        m_lChangeItem |= changeItem::user::CHANGE_SCORE_DISPLAY;
        m_scoreDisPlay = srcMap.value(scoreConfig).toBool();
    }

    //测试次数更换
    if(m_testCnt != srcMap.value( testCntConfig ).toInt())
    {
        m_lChangeItem |= changeItem::user::CHANGE_TEST_ITEM;
        m_testCnt = srcMap.value( testCntConfig ).toInt();
    }

    //测试时无操作页面更换
    if(m_timeout != srcMap.value( timeoutConfig ).toInt())
    {
        m_lChangeItem |= changeItem::user::CHANGE_PAGE_COUNTTIME;
        m_timeout = srcMap.value( timeoutConfig ).toInt();
    }

    //测试项目更换
    bool testItemchanged = false;
    QString stratFlag = srcMap.value(satrtFlagConfig).toString(); // ? m_startFlagList[0] : m_startFlagList[1];

    if(m_curTestItemValue != srcMap.value( curTestItemName ).toString())
    {
        testItemchanged = true;
        m_curTestItemValue = srcMap.value( curTestItemName ).toString();
    }

    //短跑起点模式更换时需要通知后端设备
    if(stratFlag != m_runShortStartFlag)
    {
        testItemchanged = true;
        m_runShortStartFlag = stratFlag;
    }

    if(testItemchanged)
    {
        m_lChangeItem |= changeItem::user::CHANGE_TEST_ITEM;
    }

    //立定跳远起跳变更
    if(m_standingJumpStartPos != srcMap.value(satrtPosConfig).toInt())
    {
        m_standingJumpStartPos = srcMap.value(satrtPosConfig).toInt();
        m_lChangeItem |= changeItem::user::CHANGE_START_POS;
    }

    //跳绳时长变更
    if(m_ropeRecvDealyTime != srcMap.value(ropeDelayConfig).toInt() && srcMap.value(ropeDelayConfig).toInt()!=0)
    {
        m_ropeRecvDealyTime = srcMap.value(ropeDelayConfig).toInt();
        m_lChangeItem |= changeItem::user::CHANGE_ROPE_DELAY;
    }

    //中长跑长度变更 - onTestChange中计算了更改 因此再通知项目更改就行
    if(m_runMidLength != srcMap.value(circleLengthConfig).toInt() && srcMap.value(circleLengthConfig).toInt()!=0)
    {
        m_runMidLength = srcMap.value(circleLengthConfig).toInt();
        m_lChangeItem |= changeItem::user::CHANGE_TEST_ITEM;
    }

    saveIntoDatabase( srcMap );
}

QStringList CUsingSettingConfig::testCountList() const
{
    return m_testCountList;
}

void CUsingSettingConfig::setTestCountList(const QStringList &testCountList)
{
    m_testCountList = testCountList;
}

QStringList CUsingSettingConfig::timeoutSelectList() const
{
    return m_timeoutSelectList;
}

void CUsingSettingConfig::setTimeoutSelectList(const QStringList &timeoutSelectList)
{
    m_timeoutSelectList = timeoutSelectList;
}

void CUsingSettingConfig::saveIntoDatabase(const QVariantMap &srcMap)
{
    bool bRes = false;
    int tmpDevType = m_totalTestItemMap.key( m_curTestItemMap );
    int testCnt = srcMap.value( testCntConfig ).toInt();
    int timeout = srcMap.value( timeoutConfig ).toInt();
    bool bDisPlayScore = srcMap.value( scoreConfig ).toBool();
    bool bStartFlag = srcMap.value( satrtFlagConfig ) == m_startFlagList[0] ? true : false;

    //更新测试时长、测试次数、测试项目
    if(m_lChangeItem & changeItem::user::CHANGE_TEST_ITEM)
    {
        bRes = CDatabaseManage::GetInstance().getDevSWInfoConfig()->updateDevSWTestConfig( tmpDevType, testCnt, m_curTestItemValue.toStdString() );
        if(tmpDevType == DEV_TYPE_RUN_SHORT)
        {
            //更新短跑配置
            bRes =  CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateUseRunShortStartFlag(bStartFlag);
        }
        else if (tmpDevType == DEV_TYPE_RUN_MID)
        {
            //更新中长跑一圈长度配置
            bRes = CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateDevRunMidLength(m_runMidLength);
        }
    }

    //更新无操作等待时间
    if(m_lChangeItem & changeItem::user::CHANGE_PAGE_COUNTTIME)
    {
        bRes = CDatabaseManage::GetInstance().getDevSWInfoConfig()->updateDevTestPageCountTime(tmpDevType, timeout);
    }

    //更新成绩显示配置
    if(m_lChangeItem & changeItem::user::CHANGE_SCORE_DISPLAY)
        bRes = CDatabaseManage::GetInstance().getDevTestInfoConfig()->updateDevDisplayScore(bDisPlayScore);

    //更新立定跳远起跳点
    if(m_lChangeItem & changeItem::user::CHANGE_START_POS)
        bRes = CDatabaseManage::GetInstance().getDevTestInfoConfig()->setStadingJumpStartPos(m_standingJumpStartPos);
}

void CUsingSettingConfig::setCurDevCorrespondingTestItems(const int &devType)
{
    m_curTestItemMap   = m_totalTestItemMap.value( devType );
    string tmpTestItem = "";

    if( !CDatabaseManage::GetInstance().getDevSWInfoConfig()->getDevTestItemsStr( devType, tmpTestItem ) ){
        return;
    }

    m_curTestItemValue = QString::fromStdString( tmpTestItem );
}

void CUsingSettingConfig::initTestItemList()
{
    m_totalTestItemMap.clear();
    QVariantMap BodyTestItemMap       = { { QString::fromLocal8Bit( "身高-体重"  ), QString::number(TEST_ITEM_HEIGHT) + '-' +QString::number( TEST_ITEM_WEIGHT) },
                                          { QString::fromLocal8Bit( "身高-体重-体成分"  ), QString::number(TEST_ITEM_HEIGHT) + '-' + QString::number(TEST_ITEM_WEIGHT)
                                                                                        + '-' + QString::number(TEST_ITEM_HBC)}};

    QVariantMap VCTestItemMap         = { { QString::fromLocal8Bit( "肺活量" ), TEST_ITEM_VC } };
    QVariantMap SBJTestItemMap        = { { QString::fromLocal8Bit( "立定跳远" ), TEST_ITEM_SBJ } };
    QVariantMap SFBTestItemMap        = { { QString::fromLocal8Bit( "坐位体前屈" ), TEST_ITEM_SFB } };
    QVariantMap PULLUPTestItemMap     = { { QString::fromLocal8Bit( "引体向上" ), TEST_ITEM_PULL_UP } };
    QVariantMap SITUPTestItemMap      = { { QString::fromLocal8Bit( "仰卧起坐" ), TEST_ITEM_SIT_UP } };

    QVariantMap RUNSHORTTestItemMap   = { { QString::fromLocal8Bit( "50米跑" ),        TEST_ITEM_RUN_50 },
                                          { QString::fromLocal8Bit( "50 x 8往返跑" ),   TEST_ITEM_RUN_BACK_8_50 },
                                          { QString::fromLocal8Bit( "60米跑" ),        TEST_ITEM_RUN_60 },
                                          { QString::fromLocal8Bit( "100米跑" ),       TEST_ITEM_RUN_100 },
                                          { QString::fromLocal8Bit( "110米栏跑" ),     TEST_ITEM_RUN_HURDLES_110 },
                                          { QString::fromLocal8Bit( "200米跑" ),       TEST_ITEM_RUN_200 },
                                          { QString::fromLocal8Bit( "400米跑" ),       TEST_ITEM_RUN_400 },
                                          { QString::fromLocal8Bit( "4 x 100米接力" ), TEST_ITEM_RUN_4_100 },
                                          { QString::fromLocal8Bit( "100米栏跑" ),     TEST_ITEM_RUN_HURDLES_100 },
                                          { QString::fromLocal8Bit( "4 x 10米往返跑" ),  TEST_ITEM_RUN_BACK_4_10 }
                                        };

    QVariantMap RUNMIDTestItemMap   = { { QString::fromLocal8Bit( "女800米跑-男1000米跑" ), QString::number(TEST_ITEM_RUN_800)+ "-" + QString::number(TEST_ITEM_RUN_1000) },
                                        { QString::fromLocal8Bit( "1000米跑" ), TEST_ITEM_RUN_1000 },
                                        { QString::fromLocal8Bit( "800米跑" ),   TEST_ITEM_RUN_800 },
                                        { QString::fromLocal8Bit( "1500米跑" ),  TEST_ITEM_RUN_1500 },
                                        { QString::fromLocal8Bit( "2000米跑" ),  TEST_ITEM_RUN_2000 }
                                      };

    QVariantMap ROPESKIPTestItemMap     = { { QString::fromLocal8Bit( "跳绳" ), TEST_ITEM_ROPE_SKIP } };
    QVariantMap GRIPTestItemMap         = { { QString::fromLocal8Bit( "握力" ), TEST_ITEM_GRIP } };
    QVariantMap PUSHUPTestItemMap       = { { QString::fromLocal8Bit( "俯卧撑" ), TEST_ITEM_PUSHUP } };
    QVariantMap EYESIGHTTestItemMap     = { { QString::fromLocal8Bit( "视力" ), TEST_ITEM_EYESIGHT } };
    QVariantMap FOOTBALLTestItemMap     = { { QString::fromLocal8Bit( "足球" ), TEST_ITEM_FOOTBALL } };
    QVariantMap BASKETBALLTestItemMap   = { { QString::fromLocal8Bit( "篮球" ), TEST_ITEM_BASKETBALL } };
    QVariantMap VOLLEYBALLTestItemMap   = { { QString::fromLocal8Bit( "排球" ), TEST_ITEM_VOLLEYBALL } };
    QVariantMap PUOTestItemMap          = { { QString::fromLocal8Bit( "斜身引体向上" ), TEST_ITEM_PUO } };
    QVariantMap SOLDBALLTestItemMap     = { { QString::fromLocal8Bit( "实心球" ), TEST_ITEM_SOLIDBALL } };
    QVariantMap VERTICALJUMPTestItemMap = { { QString::fromLocal8Bit( "纵跳摸高" ), TEST_ITEM_VJUMP } };

    m_totalTestItemMap[ DEV_TYPE_BODY ]       = BodyTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_VC ]         = VCTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_SBJ ]        = SBJTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_SFB ]        = SFBTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_PULL_UP ]    = PULLUPTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_SIT_UP ]     = SITUPTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_RUN_SHORT ]  = RUNSHORTTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_RUN_MID ]    = RUNMIDTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_ROPE_SKIP ]  = ROPESKIPTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_GRIP ]       = GRIPTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_PUSH_UP ]    = PUSHUPTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_EYESIGHT ]   = EYESIGHTTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_FOOTBALL ]   = FOOTBALLTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_BASKETBALL ] = BASKETBALLTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_VOLLEYBALL ] = VOLLEYBALLTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_PUO ]        = PUOTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_SOLIDBALL ]  = SOLDBALLTestItemMap;
    m_totalTestItemMap[ DEV_TYPE_VJUMP ]      = VERTICALJUMPTestItemMap;
}

void CUsingSettingConfig::initQueryTestItemList()
{
    m_queryItemMap.clear();
    m_queryItemMap = { { QString::fromLocal8Bit( "身高体重" ), TEST_ITEM_HEIGHT },
                           { QString::fromLocal8Bit( "肺活量" ), TEST_ITEM_VC },
                           { QString::fromLocal8Bit( "立定跳远" ), TEST_ITEM_SBJ },
                           { QString::fromLocal8Bit( "坐位体前屈" ), TEST_ITEM_SFB },
                           { QString::fromLocal8Bit( "引体向上" ), TEST_ITEM_PULL_UP },
                           { QString::fromLocal8Bit( "仰卧起坐" ), TEST_ITEM_SIT_UP },
                           { QString::fromLocal8Bit( "50米跑" ), TEST_ITEM_RUN_50 },
                           { QString::fromLocal8Bit( "50 x 8往返跑" ), TEST_ITEM_RUN_BACK_8_50 },
                           { QString::fromLocal8Bit( "60米跑" ), TEST_ITEM_RUN_60 },
                           { QString::fromLocal8Bit( "100米跑" ),       TEST_ITEM_RUN_100 },
                           { QString::fromLocal8Bit( "110米栏跑" ),     TEST_ITEM_RUN_HURDLES_110 },
                           { QString::fromLocal8Bit( "200米跑" ),       TEST_ITEM_RUN_200 },
                           { QString::fromLocal8Bit( "400米跑" ),       TEST_ITEM_RUN_400 },
                           { QString::fromLocal8Bit( "4 x 100米接力" ), TEST_ITEM_RUN_4_100 },
                           { QString::fromLocal8Bit( "100米栏跑" ),     TEST_ITEM_RUN_HURDLES_100 },
                           { QString::fromLocal8Bit( "4 x 10米往返跑" ),  TEST_ITEM_RUN_BACK_4_10 },
                           { QString::fromLocal8Bit( "1000米跑" ), TEST_ITEM_RUN_1000 },
                           { QString::fromLocal8Bit( "800米跑" ),   TEST_ITEM_RUN_800 },
                           { QString::fromLocal8Bit( "1500米跑" ),  TEST_ITEM_RUN_1500 },
                           { QString::fromLocal8Bit( "2000米跑" ),  TEST_ITEM_RUN_2000 },
                           { QString::fromLocal8Bit( "跳绳" ), TEST_ITEM_ROPE_SKIP },
                           { QString::fromLocal8Bit( "握力" ), TEST_ITEM_GRIP },
                           { QString::fromLocal8Bit( "俯卧撑" ), TEST_ITEM_PUSHUP },
                           { QString::fromLocal8Bit( "视力" ), TEST_ITEM_EYESIGHT },
                           { QString::fromLocal8Bit( "足球" ), TEST_ITEM_FOOTBALL },
                           { QString::fromLocal8Bit( "篮球" ), TEST_ITEM_BASKETBALL },
                           { QString::fromLocal8Bit( "排球" ), TEST_ITEM_VOLLEYBALL },
                           { QString::fromLocal8Bit( "斜身引体向上" ), TEST_ITEM_PUO },
                           { QString::fromLocal8Bit( "实心球" ), TEST_ITEM_SOLIDBALL },
                           { QString::fromLocal8Bit( "纵跳摸高" ), TEST_ITEM_VJUMP }};
}
