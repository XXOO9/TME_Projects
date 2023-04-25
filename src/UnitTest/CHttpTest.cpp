#include "CHttpTest.h"

CHttpTest::CHttpTest(QObject *parent) : QObject(parent)
{
}

void CHttpTest::initTestCase()
{
    qDebug() << "this is function initTestCase...";
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = CHttpRequestMgr::getInstance();
    }
}

void CHttpTest::case_syncMultiLocalScores()
{
    QVERIFY( nullptr != m_pNetWorkMgr );

    //实际使用的时候，响应 CHttpRequestMgr::sigSyncMultiLocalScoresFinished 这个信号
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigSyncMultiLocalScoresFinished );

    //生成需要同步的成绩( 实际使用的时候, 跳过这个步骤, 这个步骤单纯为了生成假数据 )

    int commonCount = 256;
    int bodyCount = 256;
    int eyeSightCount = 256;
    QVariantList commonList = generateCommonScores( commonCount, false );
    QVariantList bodyList = generateBodyScores( bodyCount, false );
    QVariantList eyeSightList = generateEyeSightScores( eyeSightCount, false );

    QElapsedTimer timer;
    timer.start();
    //调用同步接口
    m_pNetWorkMgr->syncMultiLocalScores( commonList, bodyList, eyeSightList );

    sigListener.wait( 2 * 60 * 1000 );
    qDebug() << "sync local " << commonCount * bodyCount * eyeSightCount << " scores, cost " << timer.elapsed();


    //接收到同步完成的信号后， 获取每条成绩的提交结果
    auto common = m_pNetWorkMgr->getPSyncMultiLocalScores()->pSyncCommonScores()->getVecUploadMulRetInfos();
    auto body = m_pNetWorkMgr->getPSyncMultiLocalScores()->pSyncBodyScores()->getVecUploadMulRetInfos();
    auto eyeSight = m_pNetWorkMgr->getPSyncMultiLocalScores()->pSyncEyeSightScores()->getVecUploadMulRetInfos();
}

void CHttpTest::case_translateBodyComposition()
{
    QVariantMap tmpMap1;
    tmpMap1[ UPLOADBODYSCORE_KEY_HEIGHT ] = "1.75";
    tmpMap1[ UPLOADBODYSCORE_KEY_WEIGHT ] = "1.76";
    tmpMap1[ UPLOADBODYSCORE_KEY_FARATE ] = "1.77";
    tmpMap1[ UPLOADBODYSCORE_KEY_BMI ] = "1.15";
    tmpMap1[ UPLOADBODYSCORE_KEY_HEADWEIGHT ] = "1.43";
    tmpMap1[ UPLOADBODYSCORE_KEY_HEADFAT ] = "1.56";
    tmpMap1[ UPLOADBODYSCORE_KEY_HEADMUSCLE ] = "1.67";
    tmpMap1[ UPLOADBODYSCORE_KEY_TBMM ] = "1.64";
    tmpMap1[ UPLOADBODYSCORE_KEY_WBIS ] = "1.56";
    tmpMap1[ UPLOADBODYSCORE_KEY_TBW ] = "1.89;";
    tmpMap1[ UPLOADBODYSCORE_KEY_WHR ] = "1.45";
    tmpMap1[ UPLOADBODYSCORE_KEY_VFA ] = "1.98";

    QVariantMap tmpMap2;
    tmpMap2[ UPLOADBODYSCORE_KEY_HEIGHT ] = "1.75";
    tmpMap2[ UPLOADBODYSCORE_KEY_WEIGHT ] = "1.76";

}

void CHttpTest::cleanupTestCase()
{

}

QVariantList CHttpTest::generateCommonScores( int maxCount, bool isRepeatTimeStamp )
{
    QVariantList list;
    QVariantMap tmpUser;

    for( int count = 0; count < maxCount; count++ ){
        tmpUser.clear();
        tmpUser[ HTTP_KEYS::devCode ] = "testDevCode001";
        tmpUser[ HTTP_KEYS::devType ] = DEV_TYPE::HTTP_DEV_TYPE_VC;
        tmpUser[ HTTP_KEYS::testItem ] = TEST_ITEM::HTTP_TEST_ITEM_VC;
        tmpUser[ HTTP_KEYS::userId ]   = QString::number( 4200 + count );
        tmpUser[ HTTP_KEYS::type ] = testType::examType;
        tmpUser[ HTTP_KEYS::result ] = QString::number( 1500 + count );
        tmpUser[ "recordTime" ] = isRepeatTimeStamp ? "1658385807441" : QString::number( QDateTime::currentMSecsSinceEpoch() );

        //为了测试重复提交，固定死时间戳( 不测试重复提交时屏蔽掉 )
        //        tmpUser[ HTTP_KEYS::recordTime ] = "1658385807441";

        list << tmpUser;
    }

    return list;
}

QVariantList CHttpTest::generateBodyScores( int maxCount, bool isRepeatTimeStamp )
{
    QVariantList list;

    //包含12项人体数据的模板
    QVariantMap tmpMap;
    QVariantMap tmpUser;

    for( int count = 0; count < maxCount; count++ ){
        tmpMap.clear();
        tmpMap[ UPLOADBODYSCORE_KEY_HEIGHT ] = "175";
        tmpMap[ UPLOADBODYSCORE_KEY_WEIGHT ] = "76";
        tmpMap[ UPLOADBODYSCORE_KEY_FARATE ] = "1.77";
        tmpMap[ UPLOADBODYSCORE_KEY_BMI ] = "23";
        tmpMap[ UPLOADBODYSCORE_KEY_HEADWEIGHT ] = "1.43";
        tmpMap[ UPLOADBODYSCORE_KEY_HEADFAT ] = "1.56";
        tmpMap[ UPLOADBODYSCORE_KEY_HEADMUSCLE ] = "1.67";
        tmpMap[ UPLOADBODYSCORE_KEY_TBMM ] = "1.64";
        tmpMap[ UPLOADBODYSCORE_KEY_WBIS ] = "1.56";
        tmpMap[ UPLOADBODYSCORE_KEY_TBW ] = "1.89";
        tmpMap[ UPLOADBODYSCORE_KEY_WHR ] = "1.45";
        tmpMap[ UPLOADBODYSCORE_KEY_VFA ] = "1.98";

        tmpUser.insert( HTTP_KEYS::devCode, "devCodeTest001" );
        tmpUser.insert( HTTP_KEYS::devType,  DEV_TYPE::HTTP_DEV_TYPE_BODY );
        tmpUser.insert( HTTP_KEYS::userId, QString::number( 4200 + count ) );
        tmpUser.insert( HTTP_KEYS::recordListStr, CHttpUpLoadMultiBodyScores::translateBodyComposition( tmpMap ) );
        tmpUser.insert( HTTP_KEYS::recordTime, isRepeatTimeStamp ? "1658385807441" : QString::number( QDateTime::currentMSecsSinceEpoch() ) );

        list << tmpUser;
    }

    return list;
}

QVariantList CHttpTest::generateEyeSightScores( int maxCount, bool isRepeatTimeStamp )
{
    QVariantList list;
    QVariantMap tmpUser;

    for( int i = 0; i < maxCount; i++ ){
        tmpUser.clear();
        tmpUser.insert( HTTP_KEYS::devCode, "devCodeTest001" );
        tmpUser.insert( HTTP_KEYS::devType,  DEV_TYPE::HTTP_DEV_TYPE_VISION );
        tmpUser.insert( HTTP_KEYS::userId, QString::number( 4200 + i ) );
        tmpUser.insert( HTTP_KEYS::recordListStr, "1=5.0@2=5.2" );
        tmpUser.insert( HTTP_KEYS::recordTime, isRepeatTimeStamp ? "1658385807441" : QString::number( QDateTime::currentMSecsSinceEpoch() ) );

        list << tmpUser;
    }

    return list;
}

void CHttpTest::init()
{

}

void CHttpTest::case_syncScoreRules()
{
    CHttpSyncScoreStd *ptr = new CHttpSyncScoreStd();
    QSignalSpy sigListener( ptr, &CHttpSyncScoreStd::sigRequestRetAvailable );

    ptr->httpRequest();

    sigListener.wait( 10000 );
}

void CHttpTest::case_getServerToken()
{
    CGetServerToken *ptr = new CGetServerToken();
    QSignalSpy sigListener( ptr, &CGetServerToken::sigRequestRetAvailable );

    QVariantMap paraMap;
    paraMap[ getServerTokenKeys::userName ] = "superadmin";
    paraMap[ getServerTokenKeys::password ] = "123";

    ptr->setLoginParaMap( paraMap );

    ptr->httpRequest();

    sigListener.wait( 3000 );
}

void CHttpTest::case_queryReport()
{
    CQuerySandReport *ptr = new CQuerySandReport();
    QSignalSpy sigListener( ptr, &CQuerySandReport::sigRequestRetAvailable );

    ptr->setQueryCondition( "10087", "0" );

    ptr->httpRequest();

    sigListener.wait( 3000 );
}

void CHttpTest::case_Batch_EPS_commit()
{
    case_EPS_commit();

    case_EPS_commit();
}

void CHttpTest::case_EPS_commit()
{
    CEPSDataBatchCommit *ptr = new CEPSDataBatchCommit();
    QSignalSpy sigListener( ptr, &CEPSDataBatchCommit::sigRequestRetAvailable );

    QList< CEPSElement > paraList;
    CEPSElement tmpEleEps1;
    CEPSElement tmpEleEps2;

    /**************** 组装第一个人的信息**************/
    tmpEleEps1.m_userId = "10118";  //pure number required
    tmpEleEps1.m_recordTime = QString::number( QDateTime::currentMSecsSinceEpoch() );
    tmpEleEps1.m_sandboxName = "user1_sandBoxName";
    tmpEleEps1.m_sandboxScenes = "user1_sandScenes";
    tmpEleEps1.m_sandboxTimeCost = "50";  //pure number required
    tmpEleEps1.m_filePath = "./1.png";

    //组装 Track 信息
    QVariantMap user1TrackMap1;
    user1TrackMap1[ EPSKeys::useOrder ]  = "10";
    user1TrackMap1[ EPSKeys::itemName ]  = "user1Map1";
    user1TrackMap1[ EPSKeys::category ]  = "user1category1Track";
    user1TrackMap1[ EPSKeys::isDeleted ] = "0";

    QVariantMap user1TrackMap2;
    user1TrackMap2[ EPSKeys::useOrder ]  = "12";
    user1TrackMap2[ EPSKeys::itemName ]  = "user1Map2";
    user1TrackMap2[ EPSKeys::category ]  = "user1category2Track";
    user1TrackMap2[ EPSKeys::isDeleted ] = "0";

    //组装 Statis 信息
    QVariantMap user1Statis1;
    user1Statis1[ EPSKeys::coldNum ]  = "10";
    user1Statis1[ EPSKeys::warmNum ]  = "11";
    user1Statis1[ EPSKeys::category ]  = "XXXXXXX1";

    QVariantMap user1Statis2;
    user1Statis2[ EPSKeys::coldNum ]  = "12";
    user1Statis2[ EPSKeys::warmNum ]  = "13";
    user1Statis2[ EPSKeys::category ]  = "user1category2Statis";

    //组装 Meaing 信息
    QVariantMap user1MeaingMap1;
    user1MeaingMap1[ EPSKeys::index ]    = "13";
    user1MeaingMap1[ EPSKeys::itemName ] = "user1Map1Item";
    user1MeaingMap1[ EPSKeys::meaning ]  = "user1category1";

    QVariantMap user1MeaingMap2;
    user1MeaingMap2[ EPSKeys::index ]  = "12";
    user1MeaingMap2[ EPSKeys::itemName ]  = "user1Map2Item";
    user1MeaingMap2[ EPSKeys::meaning ]  = "user1Map2Meaing";

    tmpEleEps1.m_itemUseTrackList  << user1TrackMap1 << user1TrackMap2;
    tmpEleEps1.m_itemUseStatisList << user1Statis1  << user1Statis2;
    tmpEleEps1.m_itemMeaningList   << user1MeaingMap1 << user1MeaingMap2;


    /**************** 组装第二个人的信息**************/
    tmpEleEps2.m_userId = "10119";  //pure number required
    tmpEleEps2.m_recordTime = QString::number( QDateTime::currentMSecsSinceEpoch() );
    tmpEleEps2.m_sandboxName = "user2_sandBoxName";
    tmpEleEps2.m_sandboxScenes = "user2_sandScenes";
    tmpEleEps2.m_sandboxTimeCost = "50";  //pure number required
    tmpEleEps2.m_filePath = "./2.png";

    //组装 Track 信息
    QVariantMap user2TrackMap1;
    user2TrackMap1[ EPSKeys::useOrder ]  = "10";
    user2TrackMap1[ EPSKeys::itemName ]  = "user2Map1";
    user2TrackMap1[ EPSKeys::category ]  = "user2category1Track";
    user2TrackMap1[ EPSKeys::isDeleted ] = "0";

    QVariantMap user2TrackMap2;
    user2TrackMap2[ EPSKeys::useOrder ]  = "12";
    user2TrackMap2[ EPSKeys::itemName ]  = "user2Map2";
    user2TrackMap2[ EPSKeys::category ]  = "user2category2Track";
    user2TrackMap2[ EPSKeys::isDeleted ] = "0";

    //组装 Statis 信息
    QVariantMap user2Statis1;
    user2Statis1[ EPSKeys::coldNum ]  = "10";
    user2Statis1[ EPSKeys::warmNum ]  = "11";
    user2Statis1[ EPSKeys::category ]  = QString::fromLocal8Bit( "交通" );
    qDebug() << "ret = " << user2Statis1.value( EPSKeys::category ).toString();


    QVariantMap user2Statis2;
    user2Statis2[ EPSKeys::coldNum ]  = "12";
    user2Statis2[ EPSKeys::warmNum ]  = "13";
    user2Statis2[ EPSKeys::category ]  = "user2category2Statis";

    //组装 Meaing 信息
    QVariantMap user2MeaingMap1;
    user2MeaingMap1[ EPSKeys::index ]    = "13";
    user2MeaingMap1[ EPSKeys::itemName ] = "user2Map1Item";
    user2MeaingMap1[ EPSKeys::meaning ]  = "user2category1";

    QVariantMap user2MeaingMap2;
    user2MeaingMap2[ EPSKeys::index ]  = "12";
    user2MeaingMap2[ EPSKeys::itemName ]  = "user2Map2Item";
    user2MeaingMap2[ EPSKeys::meaning ]  = "user2Map2Meaing";

    tmpEleEps2.m_itemUseTrackList  << user2TrackMap1  << user2TrackMap2;
    tmpEleEps2.m_itemUseStatisList << user2Statis1    << user2Statis2;
    tmpEleEps2.m_itemMeaningList   << user2MeaingMap1 << user2MeaingMap2;

    ptr->setCommitParameters( QList<CEPSElement>() << tmpEleEps1 << tmpEleEps2 );
    ptr->httpRequest();

    sigListener.wait();
}

void CHttpTest::cleanup()
{

}

void CHttpTest::case_heartBeat()
{
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigHeartBeatFinished );
    devStatus state = devStatus::STATUS_CONNECTED;
    QString battery = "99";
    QString devCode = "fhl001";
    short devType   = 4;

    m_pNetWorkMgr->sendHeartBeat( state, battery, devCode, devType );

    QVERIFY( sigListener.wait( 3000 ) );

    QVariantList sigList = sigListener.takeFirst();
    bool ok = sigList.takeFirst().toBool();
    qDebug() << "ok = " << ok;
    QVERIFY( ok );

}

void CHttpTest::case_userIdentity()
{
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigUserIdentifyFinished );
    m_pNetWorkMgr->setIsOnline( true );
    QString faceFeature = "00000000e4e4333b00000000bc3f7d3a00000000cd05813b000000007640963b000000000000000000000000769ac23d00000000000000000000000000000000b14a043c0000000082ad003cf33f613c000000004b5a703ba9e1383c00000000000000002159713bd5a0e83b00000000000000000000000000000000b4ee073c3bae643d000000000000000000000000aa6f013d389fa53b0000000052e3b63b00000000f291883dc50d013cd611d53b000000000000000000000000000000008feb813db7567b3d00000000000000000000000000000000000000000000000000000000f62fce3d000000006df6443d6f8cdb3c81e06b3c0000000060da133c31daf83a05959f3ccd4ad03dcb20bc3b5955233dc11f753bce1a833c00000000000000006b6c5c3d0ffcf93b0000000000000000e59bc23d40710c3d19cb3b3d000000006ba1053c000000000000000000000000bfd3e13c0000000000000000000000000000000000000000bc8d433c0000000066d8213d0000000000000000fc039a3de82e4c3cad5d393d0000000000000000f0f7f33bb47d283c39767f3b0000000000000000000000000000000000000000a6bf703d6711e73d67864a3d0000000000000000532fe73bef18373c3dd9c33b0000000000000000652a193c6927373d3246483c6223753c7dcd323d00000000000000004902e23c0428843d37a5fb3b00000000000000000000000000000000731a133ee8957c3d77b05b3d6940be3d0000000000000000000000001803873b00000000000000000000000000000000ba96e33cc6df573c00000000000000006a89d13c000000006513f13cb3af4f3b59a1ae3d0000000000000000000000003db7323d000000000000000000000000000000000000000014a4433d00000000e8c3d43c763fee3b754ed33c33851c3c14470f3d702b793cfa14003dee38273c0000000000000000d30b5a3c9d2d963d68b3053c3012683d00000000b2fd083d0000000000000000eaa29d3cc5269f3d19f21c3d000000000511fb3c0000000025b8c93ab405b93c000000000000000094b68e3b000000000000000040032b3d129de53c1853273c000000000000000000000000cac83c3e65c4a33c17a2093c0000000000000000000000000000000060d3823d000000005b07173c0000000026a24b3d00000000719a3d3c000000007b4e033c0685163dc600b43d4585203c33802a3d0000000000000000c5b71a3c94f7e53d000000006264203d000000000000000000000000171c183d00000000c6c2e03bd865fa3c000000000000000000000000000000000000000054feb13c000000009f396b3c47ecda3c0000000000000000e6d10f3d000000000000000000000000a7a5e13c6a568c3b7277923c000000003467023d8511d73b000000000000000021cbdb3ce626493d0000000000000000e59a4c3c000000007e891d3dff84603c000000000000000000000000afca433b90d38d3dbb2e063e1a529f3b0000000000000000119a0a3ce79a9a3d17e6373c000000000000000071a91e3d000000000000000000000000f959c33c00000000aaed223d000000007c89223c000000009d9ec53c2f871f3d40bebc3b0000000000000000f69a183df086313d00000000000000000000000070b45a3d8140403b32691b3c2ae28f3a000000000000000034455f3dc22def3bf7790f3ddaa8823d6366383d1d0c2f3d0000000073ed333dc55d183d00000000f6439a3b000000000000000000000000000000003562ba3d00000000000000000000000000000000a8adab3db8be0e3d94abe43bea1fc93cfe67573c1dc8933bac046c3ce181de3b85ee193d1efebe3c00000000f789a33c00000000d2318f3c902e393c9178e03c00000000078f853b000000000000000000000000377b523c571e2a3c781e713b68be993c0000000042756b3d898bd83b00000000f17fbc3df5bd993b8825ea3b40ce423c350ab23c0000000067bf5b3bb38b033ce0b4863df4980f3d00000000000000000000000000000000000000004004183d00f9ab3c0000000083e4563defaaa43d1264013d00000000d1333c3c22078e3d6406843d0000000000000000000000006d16973d3f82543c000000008f68ea3daefd473c467f0a3daaf38e3c00000000611b793c8a78583dd521aa3d5eaad33d368e113e000000003e00063d000000000f1b0c3c00000000fb8cd93c00000000a435573c5d95f73c3df7c53c0000000000000000bb0f9c3d000000002b66273d00000000000000009348203d241ef33c3f0f6b3c00000000bb743a3d133f2c3c11aa433d0000000000000000f808433ce790983d2dd7aa3b2ed7b63c000000007f75a53b00000000000000003e78af3d0d0cb33d000000001adbea3bef3e5a3d8992073dcef2fc3caa776d3d000000000e21463c6a90743d0000000000000000bbb5e93c0000000000000000743b893a000000000000000080af853d53cbd83b5b83333c4965a43a0000000000000000736fdc3b00000000c212963c000000000289d93b000000004fb10c3c09b32b3c0000000000000000a635df3c00000000a77e663c0000000000000000000000000f50103c000000009572603c000000000000000000000000c167e63b000000000000000000000000cec3703b00000000136ade3d0000000063129a3c00000000fe5c8b3c000000000000000000000000c8438b3c169dc33c000000002699403ceca7b53b995ba63c86abec3c587db53d214b573b0000000085625c3c0000000000000000fa54453c923be33c00000000a3740f3c0eba123da037843d00000000a591aa3b1b7e643d6e1eb53c0000000016af133c0000000000000000000000002713013cb1b3083d56d51c3da41c8f3bd4385a3c970b673cc5e3223d00000000000000001bbd513d000000000d21923cf21a583d890a033e0000000008241e3cd03eee3bed344c3d000000000000000000000000eb16c43b0000000000000000d6328d3c1a85413c0000000000000000ed984f3d0000000000000000fff4733c0000000015a2b03d464b0e3c00000000e314483c000000000000000000000000ef322d3d210c203c15ea6f3d0000000000000000dc164a3c80a5a23c0000000000000000b7d93c3c63cdef3d907b8d3c6decd33c00000000000000007d7c443d08d31e3c0000000000000000000000000000000000000000620e8c3d000000000000000000000000c0ca543c832cf43b3fd1043ddd01253c6ea8083debd1093b000000000a64423c6d228d3c000000000000000000704b3d0000000073e2613c00000000000000000000000000000000e72ec63cadbe563d53ae543c2b27c73bac091d3d68095c3d000000000000000000000000000000000000000000000000cc196f3d7fc1083d0000000000000000ffaf4e3ce1ea6a3d0a7e743b0000000000000000000000003837f83c0000000059a1063e0000000008bfb73c000000000000000010319d3c9b07e43dc4714b3d7a48f83b737a253dc5838e3c91cb463d75df363c00000000414bf63b00000000476dc83b273a683b000000009653813d000000009aae9f3c00000000326ac83b000000006bd29f3b25cd6e3d533afd3baae66d3d7af5fd3ccc9f363b0000000072f9843b90f3623c000000009f965b3c8cbc5f3d000000004983443d0000000000000000333ed43cd4e3fe3b00000000000000000000000000000000b69c0e3c4e208c3c87c6703c00000000989d613b000000007553f63c000000007137ac3d00000000cbe90f3dd521f73b05ade63d00000000000000000936a03b0000000000000000fb14bc3d773a943c000000009d62a53b0000000000000000adfc723d000000005903a13d9dad8e3d98a3153ced7c233c000000000000000000000000000000002413a43b00000000d14b8e3d6058373d000000000000000000000000000000000abc553c2172603c0000000000000000e50a703d1630053dc5e6353cc899bc3c00000000c157ba3c7ec3913c00000000280bb93d00000000000000000000000000000000000000000000000000000000962d493d000000003232023e0889013d000000000000000000000000000000000000000000000000bce4683c63527f3b00000000000000000000000071d79b3d0000000027d4cc3ba817e93c3f856e3b34e7203d731a343c0000000000000000c8e9b23bf506503d9bdb503d000000008755c33c000000000000000069f4403c3fd20a3c36a1a23b000000000228c63c3789543cdea15b3c109d0b3e00000000a1a7ae3b58add43cbcf6b73d0000000008b6213d6d49233ce95aef3ca849b53c000000005cb09b3d00000000000000000000000000000000d2474e3cfa83a43d0f71e33c000000006d573b3d00000000e963093d0835c33b00000000000000000000000040b4a63b0b92803b0000000000000000000000000000000000000000000000000000000056dfa43c8537403d00000000bd38373d39a9143c000000000000000000000000000000003550923c000000000000000000000000c668793d31e55d3b00000000000000000000000000000000bcf06e3cfa83a23b46015d3cafcc613c0000000000000000000000000000000048a3cf3c00000000bcad5b3d00000000000000000b433e3c80975e3d000000000000000000000000000000005b4f873d0000000000000000b50d6e3c00000000000000004d4a383d0000000000000000d3cca03be048543c0000000052d9653d40bd8b3cdf36533c00000000000000000000000070e66f3d0000000066c3f23cd1cb843c0000000000000000cd60673c5bb6b23b000000000000000000000000000000000000000068b2203c61d5be3d0602763cc551083d2b7f393c998b973c63f7603d0000000000000000000000004ad6183d000000009c73273d00000000d5d1323c8d804f3cd8f60d3c0000000027f8603cd29b823bde20813ce7f3023e000000003161d23b5cb9853cb0463e3c8190123cb6a66b3c00000000e96c8c3d3221913c4dc20c3daedb233c4d5b2b3c460c9e3b00000000000000000000000049f7513c0d12243c00000000cca4e73d9a67ac3cce545a3c00000000000000000000000000000000000000000000000000000000583b793c00000000c2ae873a00000000a3e3843b2f18223d0000000089d74a3d000000008e00b73b2981673d000000000000000042a3113d3149cd3cbcd22c3b2cbda53b4cfee43cc343013b000000000000000000000000000000000000000000000000000000000000000029b6aa3c000000000000000000000000000000000000000000000000b3b90b3d04c70a3e9e60003dd37e7c3c00000000000000002627e23c0000000000000000000000000000000000000000000000005cd44c3c528ab73b000000000b39503c1067dd3d00000000cfaf7a3d3a18bc3c29a2283b653e863deedae23b00000000000000000000000069f4853c7e2d6c3b0230203c000000004075d13b00000000e604a13d000000000000000000000000124eae3d0000000000000000ede7513c000000002aeb143d00000000817a2c3d00000000000000000275273d0000000000000000a9ea813d352c483d00000000ec4ccf3d0000000065b0153ce09dad3b00000000000000005d155d3c";
    m_pNetWorkMgr->userIdentify( faceFeature );

    QVERIFY( sigListener.wait( 100000 ) );
}

//void CHttpTest::case_syncUserInfo()
//{
//    QVERIFY( nullptr != m_pNetWorkMgr );
//    m_pNetWorkMgr->setIsOnline( true );
//    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigSyncUserInfoFinished );
//    m_pNetWorkMgr->syncUserInfo( "0" );

//    QVERIFY( sigListener.wait( 100000 ) );

//    //判断获取到的学生信息条数
//    QVector<QMap<QString, QString> > vecUsers = m_pNetWorkMgr->getVecUserInfo();
//    QVERIFY( vecUsers.size() > 0 );
//}

void CHttpTest::case_syncClassInfo()
{
    qDebug() << "coming in sync class info...";
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigSyncClassInfoFinished );
    m_pNetWorkMgr->setIsOnline( true );
    m_pNetWorkMgr->syncClassInfo();

    QVERIFY( sigListener.wait( 100000 ) );

    //判断获取到的班级信息条数
    QVector<QMap<QString, QString> > vecClassInfos = m_pNetWorkMgr->getVecClassInfo();
    QVERIFY( vecClassInfos.size() > 0 );

}

void CHttpTest::case_uploadCommonScore()
{
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigUploadCommonScore );
    m_pNetWorkMgr->setIsOnline( true );

    QString userId = "341";
    testType scoreType = examType;
    TEST_ITEM item = HTTP_TEST_ITEM_VC;
    QString ret = "50";
    QString recordTime = "1652320533145";
    QString devCode = "test";
    short devType = 2;

    QVector<QMap<QString, QVariant>> vecRetMap;
    QMap<QString, QVariant> retMap;


    retMap.insert( "userId", userId );
    retMap.insert( "type", scoreType );
    retMap.insert( "testItem", item );
    retMap.insert( "result", ret );
    retMap.insert( "recordTime", recordTime );
    retMap.insert( "deviceCode", devCode );
    retMap.insert( "deviceType", devType );

    vecRetMap << retMap;

    m_pNetWorkMgr->setVecUnUploadCommonScore( vecRetMap );
    QVERIFY( sigListener.wait( 100000 ) );
}

void CHttpTest::case_uploadBodyScore()
{
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigUploadBodyScore );
    m_pNetWorkMgr->setIsOnline( true );

    QVector< QMap<QString, QVariant> > vecRetMap;
    QMap<QString, QVariant> infos;

    infos.insert( HTTP_KEYS::userId, "339" );
    infos.insert( HTTP_KEYS::devCode, "test" );
    infos.insert( HTTP_KEYS::devType, 1 );
    infos.insert( HTTP_KEYS::type, 1 );
    infos.insert( HTTP_KEYS::recordTime, "1653988891495" );

    infos.insert( UPLOADBODYSCORE_KEY_HEIGHT,     "160" );
    infos.insert( UPLOADBODYSCORE_KEY_WEIGHT,     "50" );
    infos.insert( UPLOADBODYSCORE_KEY_FARATE,     "53" );
    infos.insert( UPLOADBODYSCORE_KEY_BMI,        "52" );
    infos.insert( UPLOADBODYSCORE_KEY_HEADWEIGHT, "51" );
    infos.insert( UPLOADBODYSCORE_KEY_HEADFAT,    "50" );
    infos.insert( UPLOADBODYSCORE_KEY_HEADMUSCLE, "49" );
    infos.insert( UPLOADBODYSCORE_KEY_TBMM,       "48" );
    infos.insert( UPLOADBODYSCORE_KEY_WBIS,       "47" );
    infos.insert( UPLOADBODYSCORE_KEY_TBW,        "46" );
    infos.insert( UPLOADBODYSCORE_KEY_WHR,        "45" );
    infos.insert( UPLOADBODYSCORE_KEY_VFA,        "44" );

    vecRetMap << infos;

    m_pNetWorkMgr->setVecUnUploadBodyScore( vecRetMap );
    QVERIFY( sigListener.wait( 100000 ) );


}

void CHttpTest::case_uploadEyeSightScore()
{
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigUploadEyeSightScore );

    QVector< QMap<QString, QVariant> > vecRetMap;
    QMap<QString, QVariant> infos;

    infos.insert( HTTP_KEYS::userId, "339" );
    infos.insert( HTTP_KEYS::devCode, "test" );
    infos.insert( HTTP_KEYS::devType, 1 );
    infos.insert( HTTP_KEYS::type, 1 );
    infos.insert( HTTP_KEYS::recordTime, "1653988891495" );

    vecRetMap << infos;


    m_pNetWorkMgr->setVecUnUploadEyesightScore( vecRetMap );
    QVERIFY( sigListener.wait( 3000 ) );
}

void CHttpTest::case_touristUsageRecord()
{
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigTouristUsageRecordFinished );
    m_pNetWorkMgr->touristUsageRecord( "test", 4 , QString::number(QDateTime::currentMSecsSinceEpoch()));
    QVERIFY( sigListener.wait( 3000 ) );
}

void CHttpTest::case_queryHistoryScore()
{
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigQueryHistoryScoreFinished );
    QVariantMap infoMap;

    infoMap.insert( "userId", "339" );
    infoMap.insert( "testItem", 4 );
    infoMap.insert( "timeInterval", 3 );
    infoMap.insert( "scoreType", 2 );
    m_pNetWorkMgr->queryHistoryScore( infoMap );

    QVERIFY( sigListener.wait( 3000 ) );
}

void CHttpTest::case_uploadMultiCommonScores()
{
    QElapsedTimer countTimer;
    countTimer.start();
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigUploadMultiCommonScoreFinished );

    QVariantList list;
    QVariantMap tmpUser;

    for( int count = 0; count < 256; count++ ){
        tmpUser[ HTTP_KEYS::devCode ] = "testDevCode001";
        tmpUser[ HTTP_KEYS::devType ] = DEV_TYPE::HTTP_DEV_TYPE_VC;
        tmpUser[ HTTP_KEYS::testItem ] = TEST_ITEM::HTTP_TEST_ITEM_VC;
        tmpUser[ HTTP_KEYS::userId ]   = QString::number( 4200 + count );
        tmpUser[ HTTP_KEYS::type ] = testType::examType;
        tmpUser[ HTTP_KEYS::result ] = QString::number( 1500 + count );
        tmpUser[ "recordTime" ] = QString::number( QDateTime::currentMSecsSinceEpoch() );

        //为了测试重复提交，固定死时间戳( 不测试重复提交时屏蔽掉 )
        //        tmpUser[ HTTP_KEYS::recordTime ] = "1658385807441";

        list << tmpUser;
    }

    qDebug() << "config list cost " << countTimer.restart();

    //将list作为参数，发起上传请求
    m_pNetWorkMgr->uploadMultiCommonScores( list );

    /*
     * 以下代码仅供单元测试使用, 实际项目调用须响应 CHttpRequestMgr 的 sigUploadMultiCommonScoreFinished 信号
     *  判断 sigUploadMultiCommonScoreFinished信号 带过来 bool 类型参数 ok 是 true 还是 false， 来判断批量提交是否全部提交成功
     *  1.如果为 true, 就是全部提交成功
     * 2. 如果为false:
     *               a. http请求错误( 网络超时, 服务器没开, 地址不对等外部原因导致的错误 )
     *               b. 成绩数据导致的错误( 重复上传, 数据缺失等内部原因... )
     *
     *              tips: 区分的方法: 获取上传失败成绩的条目 list, 如果 list 是空的，则是外部原因导致的错误, 反之就是内部原因导致的
     *
     *              exp: QVariantList list = m_pNetWorkMgr->getUploadMultiCommonScoreObj()->getFailedScoreInfoList();
     *
     *                   if( list.isEmpty() ){
     *                       //外部原因导致的错误.....
     *                   }else{
     *                      //内部原因导致的错误( 可以获取具体的错误详情, 参考下面的单元测试代码 )
     *                      ...
     *                      ...
     *                      ...
     *                   }
     *
     */
    QVERIFY( sigListener.wait( 120 * 1000 ) );

    qDebug() << "recv reply cost " << countTimer.restart();

    QVariant sigContent = sigListener.first();

    //如果存在上传失败的条目
    //    if( !sigContent.toBool() ){
    ////        QVariantList list = m_pNetWorkMgr->getUploadMultiCommonScoreObj()->getFailedScoreInfoList();
    //        for( auto &ele : list ){
    //            //获取具体的错误详情
    //            qDebug() << "errorIndex = " << ele.toMap().value( HTTP_KEYS::errorIndex ).toInt();
    //            qDebug() << "errorCode = " << ele.toMap().value( HTTP_KEYS::errorCode ).toInt();
    //            qDebug() << "errorMsg = " << ele.toMap().value( HTTP_KEYS::errorMsg ).toString();
    //            qDebug() << "***********************************************************************\n";
    //        }
    //    }

    qDebug() << "display error cost " << countTimer.restart();

    //获取所有成绩条目的提交情况, 如果提交超时， vecRetInfos的第一条的错误码就是 -10086,  m_upLoadSucces值为 false
    auto vecRetInfos = m_pNetWorkMgr->getUploadMultiCommonScoreObj()->getVecUploadMulRetInfos();

    qDebug() << "parse data cost " << countTimer.restart();

}

void CHttpTest::case_MIT_commit()
{
    CMITDataBatchCommit *ptr = new CMITDataBatchCommit();
    QSignalSpy sigListener( ptr, &CMITDataBatchCommit::sigRequestRetAvailable );


    /**********************第一个用户的所有信息   start******************************/
    //该用户的所有信息
    QVariantMap trainCollection1;
    //第一个用户的姓名
    trainCollection1[ MITKeys::userId ]        = "10087";
    //该条测试记录的时间戳
    trainCollection1[ MITKeys::recordTime ]    = QString::number( QDateTime::currentMSecsSinceEpoch() );
    //训练耗时
    trainCollection1[ MITKeys::trainTimeCost ] = "10";



    //1.打地鼠游戏的每条测试详情
    QVariantList hlddsRecordListList;
    //欢乐打地鼠记录1
    QVariantMap hlddsRecord1;
    hlddsRecord1[ MITKeys::playIndex ] = "1";               //游戏次数
    hlddsRecord1[ MITKeys::trainStartTime ] = "10:40:22";   //训练开始时间
    hlddsRecord1[ MITKeys::trainTimeCost ] = "20'12";       //训练耗时
    hlddsRecord1[ MITKeys::hitRate1 ] = "73%";              //第一关命中率
    hlddsRecord1[ MITKeys::hitRate2 ] = "89%";              //第二关命中率
    hlddsRecord1[ MITKeys::hitRate3 ] = "75%";              //第三关命中率
    hlddsRecord1[ MITKeys::hitRateTotal ] = "88";           //总命中率
    //欢乐打地鼠记录2
    QVariantMap hlddsRecord2;
    hlddsRecord2[ MITKeys::playIndex ] = "2";
    hlddsRecord2[ MITKeys::trainStartTime ] = "10:49:22";
    hlddsRecord2[ MITKeys::trainTimeCost ] = "20'12";
    hlddsRecord2[ MITKeys::hitRate1 ] = "73%";
    hlddsRecord2[ MITKeys::hitRate2 ] = "89%";
    hlddsRecord2[ MITKeys::hitRate3 ] = "75%";
    hlddsRecord2[ MITKeys::hitRateTotal ] = "88";

    //将所有的换了打地鼠游戏记录汇总
    hlddsRecordListList << hlddsRecord1 << hlddsRecord2;



    //2.划消游戏的游戏测试详情
    QVariantList hxyxList;
    //划消游戏记录1
    QVariantMap hxyxRecord1;
    hxyxRecord1[ MITKeys::playIndex ] = "1";
    hxyxRecord1[ MITKeys::trainStartTime ] = "11:01:56";
    hxyxRecord1[ MITKeys::trainTimeCost ] = "20'12";
    hxyxRecord1[ MITKeys::directTimeCost ] = "20'12";
    hxyxRecord1[ MITKeys::divertTimeCost ] = "20'12";
    hxyxRecord1[ MITKeys::selectTimeCost ] = "20'12";
    hxyxRecord1[ MITKeys::computeTimeCost ] = "20'12";

    hxyxList << hxyxRecord1;



    //3.反应时训练游戏记录详情
    QVariantList fysxlList;
    QVariantMap fysxlRecord1;
    fysxlRecord1[ MITKeys::playIndex ] = "1";
    fysxlRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    fysxlRecord1[ MITKeys::trainTimeCost ] = "20'12";
    fysxlRecord1[ MITKeys::mistakeTimes ] = "1";
    fysxlRecord1[ MITKeys::AvgReactionTime ] = "1";
    fysxlRecord1[ MITKeys::totalReactionTime ] = "2";

    fysxlList << fysxlRecord1;


    //4.舒尔特记录
    QVariantList setfkList;
    QVariantMap setfkRecord1;
    setfkRecord1[ MITKeys::playIndex ] = "1";
    setfkRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    setfkRecord1[ MITKeys::trainTimeCost ] = "20'12";
    setfkRecord1[ MITKeys::trainDifficulty ] = QString::fromUtf8( "困难" );
    setfkRecord1[ MITKeys::trainLevel ] = "3*3";
    setfkRecord1[ MITKeys::result ] = QString::fromUtf8( "正常水平" );

    setfkList << setfkRecord1;

    //5.时序记忆训练记录
    QVariantList sxjyxlList;
    QVariantMap sxjyxlRecord1;
    sxjyxlRecord1[ MITKeys::playIndex ] = "1";
    sxjyxlRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    sxjyxlRecord1[ MITKeys::trainTimeCost ] = "20'12";
    sxjyxlRecord1[ MITKeys::trainDifficulty ] = QString::fromUtf8( "困难" );
    sxjyxlRecord1[ MITKeys::trainLevel ] = "3*3";
    setfkRecord1[ MITKeys::result ] = QString::fromUtf8( "正常水平" );

    sxjyxlList << sxjyxlRecord1;


    //6.工作记忆训练
    QVariantList gzjyxlList;
    QVariantMap gzjyxlRecord1;
    gzjyxlRecord1[ MITKeys::playIndex ] = "1";
    gzjyxlRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    gzjyxlRecord1[ MITKeys::trainTimeCost ] = "20'12";
    gzjyxlRecord1[ MITKeys::memoryNum ] = "7";
    gzjyxlRecord1[ MITKeys::score ] = "88";

    gzjyxlList << gzjyxlRecord1;



    //7.图像记忆训练记录
    QVariantList txjyxlList;
    QVariantMap txjyxlRecord1;
    txjyxlRecord1[ MITKeys::playIndex ] = "1";
    txjyxlRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    txjyxlRecord1[ MITKeys::trainDifficulty ] = QString::fromUtf8( "中等" );
    txjyxlRecord1[ MITKeys::mode ] = "3*3";

    txjyxlList << txjyxlRecord1;



    //8.vr迷宫记录
    QVariantList vrmgList;
    QVariantMap vrmgRecord1;
    vrmgRecord1[ MITKeys::playIndex ] = "1";
    vrmgRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    vrmgRecord1[ MITKeys::trainTimeCost ] = "20'12";

    vrmgList << vrmgRecord1;


    //9.双手协调训练记录
    QVariantList ssxtxlList;
    QVariantMap ssxtxlRecord1;
    ssxtxlRecord1[ MITKeys::playIndex ] = "1";
    ssxtxlRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    ssxtxlRecord1[ MITKeys::trainTimeCost ] = "20'12";
    ssxtxlRecord1[ MITKeys::avgCompleteTime ] = "30";
    ssxtxlRecord1[ MITKeys::avgMistakeTimes ] = "3";

    ssxtxlList << ssxtxlRecord1;


    //10.联想绘画记录
    QVariantList lxhhList;
    QVariantMap lxhhRecord1;
    lxhhRecord1[ MITKeys::playIndex ] = "1";
    lxhhRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    lxhhRecord1[ MITKeys::trainTimeCost ] = "20'12";
    lxhhRecord1[ MITKeys::avgCompleteTime ] = "30";
    lxhhRecord1[ MITKeys::avgMistakeTimes ] = "3";

    lxhhList << lxhhRecord1;



    //11.头脑风暴记录
    QVariantList tnfbList;
    QVariantMap tnfbRecord1;
    tnfbRecord1[ MITKeys::playIndex ] = "1";
    tnfbRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    tnfbRecord1[ MITKeys::trainTimeCost ] = "20'12";

    tnfbList << tnfbRecord1;


    //12.vr森林记录
    QVariantList vrslList;
    QVariantMap vrslRecord1;
    vrslRecord1[ MITKeys::playIndex ] = "1";
    vrslRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    vrslRecord1[ MITKeys::trainTimeCost ] = "20'12";

    vrslList << vrslRecord1;


    //13.vr雪地记录
    QVariantList vrxdList;
    QVariantMap vrxdRecord1;
    vrxdRecord1[ MITKeys::playIndex ] = "1";
    vrxdRecord1[ MITKeys::trainStartTime ] = "10:39:22";
    vrxdRecord1[ MITKeys::trainTimeCost ] = "20'12";

    vrxdList << vrxdRecord1;


    //汇总所有数据
    trainCollection1[ MITKeys::hlddsList ] = hlddsRecordListList;
    trainCollection1[ MITKeys::hxyxList ] = hxyxList;
    trainCollection1[ MITKeys::fysxlList ] = fysxlList;
    trainCollection1[ MITKeys::setfkList ] = setfkList;
    trainCollection1[ MITKeys::sxjyxlList ] = sxjyxlList;
    trainCollection1[ MITKeys::gzjyxlList ] = gzjyxlList;
    trainCollection1[ MITKeys::txjyxlList ] = txjyxlList;
    trainCollection1[ MITKeys::vrmgList ] = vrmgList;
    trainCollection1[ MITKeys::ssxtxlList ] = ssxtxlList;
    trainCollection1[ MITKeys::lxhhList ] = lxhhList;
    trainCollection1[ MITKeys::tnfbList ] = tnfbList;
    trainCollection1[ MITKeys::vrslList ] = vrslList;
    trainCollection1[ MITKeys::vrxdList ] = vrxdList;

    /**********************第一个用户的所有信息   end******************************/


    //汇总所有用户的信息
    QVariantList totalUserRecordInfoMapList;
    totalUserRecordInfoMapList << trainCollection1;

    ptr->setCommitParameters( totalUserRecordInfoMapList );
    ptr->httpRequest();

    sigListener.wait();

}

void CHttpTest::case_VRB_commit()
{
    CVRBDataBatchCommit *ptr = new CVRBDataBatchCommit();
    QSignalSpy sigListener( ptr, &CVRBDataBatchCommit::sigRequestRetAvailable );

    QList< QVariantMap > paraList;
    QVariantMap tmpEleParaMap1;
    QVariantMap tmpEleParaMap2;

    tmpEleParaMap1[ VRBKeys::userId ]          = "10087";       //pure number required
    tmpEleParaMap1[ VRBKeys::recordTime ]      = QString::number( QDateTime::currentMSecsSinceEpoch() );
    tmpEleParaMap1[ VRBKeys::rideMode ]        = "10";          //pure number required
    tmpEleParaMap1[ VRBKeys::rideMap ]         = "ChengDu";
    tmpEleParaMap1[ VRBKeys::rideMusic ]       = "see you again";
    tmpEleParaMap1[ VRBKeys::rideBikeType ]    = "moutain bike";
    tmpEleParaMap1[ VRBKeys::rideDistance ]    = "100";         //pure number required
    tmpEleParaMap1[ VRBKeys::rideSpeed ]       = "50";
    tmpEleParaMap1[ VRBKeys::rideSpeedAvg ]    = "10";
    tmpEleParaMap1[ VRBKeys::rideHrMax ]       = "175";
    tmpEleParaMap1[ VRBKeys::rideHrAvg ]       = "160";
    tmpEleParaMap1[ VRBKeys::rideCalorieCost ] = "5000";
    tmpEleParaMap1[ VRBKeys::rideTimeCost ]    = "60";

    tmpEleParaMap2[ VRBKeys::userId ]          = "10088";       //pure number required
    tmpEleParaMap2[ VRBKeys::recordTime ]      = QString::number( QDateTime::currentMSecsSinceEpoch() );
    tmpEleParaMap2[ VRBKeys::rideMode ]        = "11";          //pure number required
    tmpEleParaMap2[ VRBKeys::rideMap ]         = "ChengDu";
    tmpEleParaMap2[ VRBKeys::rideMusic ]       = "sold out";
    tmpEleParaMap2[ VRBKeys::rideBikeType ]    = "moutain bike";
    tmpEleParaMap2[ VRBKeys::rideDistance ]    = "120";          //pure number required
    tmpEleParaMap2[ VRBKeys::rideSpeed ]       = "60";
    tmpEleParaMap2[ VRBKeys::rideSpeedAvg ]    = "30";
    tmpEleParaMap2[ VRBKeys::rideHrMax ]       = "155";
    tmpEleParaMap2[ VRBKeys::rideHrAvg ]       = "120";
    tmpEleParaMap2[ VRBKeys::rideCalorieCost ] = "7000";
    tmpEleParaMap2[ VRBKeys::rideTimeCost ]    = "65";

    paraList << tmpEleParaMap1 << tmpEleParaMap2;

    ptr->setCommitParameters( paraList );
    ptr->httpRequest();

    sigListener.wait();

}

void CHttpTest::case_directUploadMultiCommonScore()
{
    CHttpUpLoadMultiCommonScore *pUploadMultiScore = new CHttpUpLoadMultiCommonScore( this );

    QSignalSpy sigListener( pUploadMultiScore, &CHttpUpLoadMultiCommonScore::sigRequestRetAvailable );

    QVariantList list;
    QVariantMap tmpUser;

    QVariantMap user1;

    for( int count = 0; count < 256; count++ ){
        tmpUser[ HTTP_KEYS::devCode ] = "testDevCode001";
        tmpUser[ HTTP_KEYS::devType ] = DEV_TYPE::HTTP_DEV_TYPE_VC;
        tmpUser[ HTTP_KEYS::testItem ] = TEST_ITEM::HTTP_TEST_ITEM_VC;
        tmpUser[ HTTP_KEYS::userId ]   = QString::number( 4200 + count );
        tmpUser[ HTTP_KEYS::type ] = testType::examType;
        tmpUser[ HTTP_KEYS::result ] = QString::number( 1500 + count );
        tmpUser[ "recordTime" ] = QString::number( QDateTime::currentMSecsSinceEpoch() );
        list << tmpUser;
    }

    QElapsedTimer timer;
    timer.start();
    pUploadMultiScore->setUpLoadMultiScores( list );
    pUploadMultiScore->httpRequest();
    QNetworkReply *pReply = pUploadMultiScore->getPReply();

    qDebug() << pReply->isRunning();

    sigListener.wait( 5 * 60 * 1000 );
    qDebug() << "recv reply cost " << timer.elapsed();

}

void CHttpTest::case_uploadMultiBodyScores()
{
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigUploadMultiBodyScoreFinished );

    QVariantList list;

    //包含12项人体数据的模板
    QVariantMap user1;
    QVariantMap tmpMap1;

    tmpMap1[ UPLOADBODYSCORE_KEY_HEIGHT ] = "175";
    tmpMap1[ UPLOADBODYSCORE_KEY_WEIGHT ] = "76";
    tmpMap1[ UPLOADBODYSCORE_KEY_FARATE ] = "1.77";
    tmpMap1[ UPLOADBODYSCORE_KEY_BMI ] = "23";
    tmpMap1[ UPLOADBODYSCORE_KEY_HEADWEIGHT ] = "1.43";
    tmpMap1[ UPLOADBODYSCORE_KEY_HEADFAT ] = "1.56";
    tmpMap1[ UPLOADBODYSCORE_KEY_HEADMUSCLE ] = "1.67";
    tmpMap1[ UPLOADBODYSCORE_KEY_TBMM ] = "1.64";
    tmpMap1[ UPLOADBODYSCORE_KEY_WBIS ] = "1.56";
    tmpMap1[ UPLOADBODYSCORE_KEY_TBW ] = "1.89";
    tmpMap1[ UPLOADBODYSCORE_KEY_WHR ] = "1.45";
    tmpMap1[ UPLOADBODYSCORE_KEY_VFA ] = "1.98";

    user1.insert( HTTP_KEYS::devCode, "devCodeTest001" );
    user1.insert( HTTP_KEYS::devType,  DEV_TYPE::HTTP_DEV_TYPE_BODY );
    user1.insert( HTTP_KEYS::userId, "339" );
    user1.insert( HTTP_KEYS::recordListStr, CHttpUpLoadMultiBodyScores::translateBodyComposition( tmpMap1 ) );
    user1.insert( HTTP_KEYS::recordTime, "1658389807442" );
    user1.insert( HTTP_KEYS::signPic, "./4.png" );
    user1.insertMulti( HTTP_KEYS::files, "./1.png" );
    user1.insertMulti( HTTP_KEYS::files, "./2.png" );
    user1.insertMulti( HTTP_KEYS::files, "./3.png" );

    //只包含身高体重的模板
    QVariantMap user2;
    QVariantMap tmpMap2;

    tmpMap2[ UPLOADBODYSCORE_KEY_HEIGHT ] = "175";
    tmpMap2[ UPLOADBODYSCORE_KEY_WEIGHT ] = "75";

    user2.insert( HTTP_KEYS::devCode, "devCodeTest001" );
    user2.insert( HTTP_KEYS::devType,  DEV_TYPE::HTTP_DEV_TYPE_BODY );
    user2.insert( HTTP_KEYS::userId, "340" );
    user2.insert( HTTP_KEYS::recordListStr, CHttpUpLoadMultiBodyScores::translateBodyComposition( tmpMap2 ) );
    user2.insert( HTTP_KEYS::recordTime, "1658389807442" );
    user2.insert( HTTP_KEYS::signPic, "./8.png" );
    user2.insertMulti( HTTP_KEYS::files, "./5.png" );
    user2.insertMulti( HTTP_KEYS::files, "./6.png" );
    user2.insertMulti( HTTP_KEYS::files, "./7.png" );

    list << user1 << user2;

    //发送请求后, 只需要响应 CHttpRequestMgr 的 sigUploadMultiBodyScoreFinished 信号
    m_pNetWorkMgr->uploadMultiBodyScores( list );

    sigListener.wait( 60000 );
    QVariant sigContent = sigListener.first();

    //如果存在上传失败的条目,可以取出来做出对应的处理
    if( !sigContent.toBool() ){
        QVariantList failedList = m_pNetWorkMgr->getUploadMultiBodyScoreObj()->getFailedScoreInfoList();
    }
}

void CHttpTest::case_uploadMultiEyeSightScores()
{
    QVERIFY( nullptr != m_pNetWorkMgr );
    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigUploadMultiEyeSightScoreFinished );

    QVariantList list;
    QVariantMap user1;
    user1.insert( HTTP_KEYS::devCode, "devCodeTest001" );
    user1.insert( HTTP_KEYS::devType,  DEV_TYPE::HTTP_DEV_TYPE_VISION );
    user1.insert( HTTP_KEYS::userId, "339" );
    user1.insert( HTTP_KEYS::recordListStr, "1=5.0@2=5.2" );
    user1.insert( HTTP_KEYS::recordTime, "1658385807441" );
    user1.insert( HTTP_KEYS::signPic, "./4.png" );
    user1.insertMulti( HTTP_KEYS::files, "./1.png" );
    user1.insertMulti( HTTP_KEYS::files, "./2.png" );
    user1.insertMulti( HTTP_KEYS::files, "./3.png" );

    QVariantMap user2;
    user2.insert( HTTP_KEYS::devCode, "devCodeTest001" );
    user2.insert( HTTP_KEYS::devType,  DEV_TYPE::HTTP_DEV_TYPE_VISION );
    user2.insert( HTTP_KEYS::userId, "340" );
    user2.insert( HTTP_KEYS::recordListStr, "1=4.8@2=4.6" );
    user2.insert( HTTP_KEYS::recordTime, "1658385807441" );
    user2.insert( HTTP_KEYS::signPic, "./8.png" );
    user2.insertMulti( HTTP_KEYS::files, "./5.png" );
    user2.insertMulti( HTTP_KEYS::files, "./6.png" );
    user2.insertMulti( HTTP_KEYS::files, "./7.png" );

    list << user1 << user2;

    m_pNetWorkMgr->uploadMultiEyeSightScores( list );

    sigListener.wait( 120 * 1000 );
    QVariant sigContent = sigListener.first();

    if( !sigContent.toBool() ){
        QVariantList failedList = m_pNetWorkMgr->getUploadMultiEyeSightScoreObj()->getFailedScoreInfoList();
    }
}

void CHttpTest::case_multiHeartBeatInfo()
{
    //    QSignalSpy sigListener( m_pNetWorkMgr, &CHttpRequestMgr::sigSendMultiHeartBeatFinished );
    //    QVariantMap tmpMap1;
    //    tmpMap1[ HEARTBEAT_KEY_CODE ]    = "multi_heart_code01";
    //    tmpMap1[ HEARTBEAT_KEY_TYPE ]    = "2";
    //    tmpMap1[ HEARTBEAT_KEY_INDEX ]   = "1";
    //    tmpMap1[ HEARTBEAT_KEY_STATUS ]  = "1";
    //    tmpMap1[ HEARTBEAT_KEY_BATTERY ] = "80";

    //    QVariantMap tmpMap2;
    //    tmpMap2[ HEARTBEAT_KEY_CODE ]    = "multi_heart_code02";
    //    tmpMap2[ HEARTBEAT_KEY_TYPE ]    = "2";
    //    tmpMap2[ HEARTBEAT_KEY_INDEX ]   = "2";
    //    tmpMap2[ HEARTBEAT_KEY_STATUS ]  = "1";
    //    tmpMap2[ HEARTBEAT_KEY_BATTERY ] = "80";

    //    QVariantList list;
    //    list << tmpMap2;

    //    m_pNetWorkMgr->sendMultiHeartBeat( list );
    //    sigListener.wait( 6000 );
}

void CHttpTest::case_benchmark_case_userIdentity()
{
    QBENCHMARK{
        case_userIdentity();
    }
}

void CHttpTest::case_benchmark_case_uploadMultiCommon()
{
    QBENCHMARK{
        case_uploadMultiCommonScores();
    }

}
