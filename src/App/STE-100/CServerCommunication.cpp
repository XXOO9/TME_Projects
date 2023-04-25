///////////////////////////////////////////////////////////
//  CServerCommunication.cpp
//  Implementation of the Class CServerCommunication
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CServerCommunication.h"
#include "CLogger.h"
#include "CMainFrame.h"
#include "commondefin.h"
#include "CDevDataCfg.h"
#include "SteTestGuide/CTestGuideManager.h"

CServerCommunication::CServerCommunication()
    : CThreadResourceCtrlBase ()
{
    setUpdateFunc(UPDATE_TYPE_TEST_RESULT, &CServerCommunication::updateTestResult, *this);
    setUpdateFunc(UPDATE_TYPE_VEDIO_VC, &CServerCommunication::updateVedios, *this);
    setUpdateFunc(UPDATE_TYPE_VEDIO_SBJ, &CServerCommunication::updateVedios, *this);
    setUpdateFunc(UPDATE_TYPE_VEDIO_SFB, &CServerCommunication::updateVedios, *this);
    setUpdateFunc(UPDATE_TYPE_VEDIO_PULLUP, &CServerCommunication::updateVedios, *this);
    setUpdateFunc(UPDATE_TYPE_VEDIO_SITUP, &CServerCommunication::updateVedios, *this);
    setUpdateFunc(UPDATE_TYPE_VEDIO_RUN, &CServerCommunication::updateVedios, *this);
    setUpdateFunc(UPDATE_TYPE_TOURIST_USAGE, &CServerCommunication::updateTouristUsageCount, *this);

    initNetWork();
}

CServerCommunication::~CServerCommunication()
{

}

void CServerCommunication::startActive()
{
    //开启线程
    startThread();
}

void CServerCommunication::stopActive()
{
    //停止线程
    stopThread();
}

void CServerCommunication::setUpdateType( short shType )
{
    if( shType < 0 || shType > UPDATE_TYPE_COUNT )
        return;

    m_mutexSource.lock();
    m_vecUpdateList.push_back( shType );
    m_mutexSource.unlock();
    m_semaphoreThread.release();
}

void CServerCommunication::startDownloadSampleVideo( QVector< structSampleVideoSync > vecSampleVideoInfos )
{
//    m_pVideoDownloader = new HttpDownloadThread( this );
//    QString curWorkPath = QCoreApplication::applicationDirPath();
//    m_pVideoDownloader->setDownloadDir( curWorkPath );
//    m_pVideoDownloader->startDownloadFies( vecSampleVideoInfos );
}

void CServerCommunication::onHttpDownloadProcess( QString downloadPersent )
{
    qDebug() << "mainFrame download persent = " << downloadPersent;
}

void CServerCommunication::onHttpDownloadFinished()
{
    qDebug() << "http download completed!";
    if( m_pVideoDownloader != nullptr ){
        delete m_pVideoDownloader;
        m_pVideoDownloader = nullptr;
    }
}

void CServerCommunication::initNetWork()
{
    if( nullptr == m_pRequestMgr ){
        m_pRequestMgr = CHttpRequestMgr::getInstance();
    }

    connect( m_pRequestMgr, &CHttpRequestMgr::sigTouristUsageRecordFinished, this, &CServerCommunication::onTouristUsageRecord, Qt::QueuedConnection );
}

void CServerCommunication::dealThreadLogic()
{
    //外部有需要更新的线程资源才会进来这里处理
    short shUpdateType = -1;
    bool bRet = false;

    m_mutexSource.lock();
    if(m_vecUpdateList.size() > 0)
    {
        shUpdateType = m_vecUpdateList[0];
    }
    m_mutexSource.unlock();

    auto itFindProcess = m_mapUpdateProcess.find(shUpdateType);
    if(itFindProcess != m_mapUpdateProcess.end())
    {
        bRet = itFindProcess->second();
    }

    //当前更新未成功,需要一直等到成功，若服务器不在线程频繁运行会极大的占用资源
    //一般情况下失败的原因是服务器不存在，将睡眠时间设置为服务器时间1/3
    if( !bRet )
    {
        if(shUpdateType != -1)
        {
            //更新资源失败的因素可能不止服务器未连接，所以为避免线程一直处理错误资源，将当前更新类型放到最后一个即可
            m_mutexSource.lock();
            auto itFind = std::find(m_vecUpdateList.begin(), m_vecUpdateList.end(), shUpdateType);
            if(itFind != m_vecUpdateList.end())
            {
                //将本次使用的资源更新到变量中
                m_semaphoreThread.release();
                m_vecUpdateList.erase(itFind);
                m_vecUpdateList.push_back(shUpdateType);
            }
            m_mutexSource.unlock();
        }
    }
    //成功则删除对应的类型
    else
    {
        m_mutexSource.lock();
        auto itFind = std::find(m_vecUpdateList.begin(), m_vecUpdateList.end(), shUpdateType);
        if(itFind != m_vecUpdateList.end())
        {
            m_vecUpdateList.erase(m_vecUpdateList.begin());
        }
        m_mutexSource.unlock();
    }
}

//更新测试结果
bool CServerCommunication::updateTestResult()
{
    bool bRet = false;

    //1.生产查询信息，只上传未同步结果,生成的信息结构为将没有同步的正式测试和非正式测试都上传到服务器
    CTestResultInfo infoSearchInfo;
    infoSearchInfo.m_bSyncServer = false;
    infoSearchInfo.m_bUseSyncServerToSelect = true;
    infoSearchInfo.m_nNature = CTestResultInfo::TEST_FREE;
    infoSearchInfo.m_bUseUnequalNature = true;

    //2.获取位同步测试结果
    vector<CTestResultInfo> vecTestResult;
    auto pTableTestResult = CDatabaseManage::GetInstance().getTabTestItemResult();
    if(nullptr == pTableTestResult)
    {
        LOGERROR("TableTestItemResult is nullptr");
        return bRet;
    }
    pTableTestResult->getResultByInfo( infoSearchInfo, vecTestResult );

    //3.上传测试结果
    bRet = true;
    short shDevType;

    QVector<QMap<QString, QVariant> > vecInfosBody, vecInfosCommon, vecEyesight;

    size_t nIndex = 0; //变量表示 testResInfo之后的索引
    vector<CTestResultInfo> vecResTemp; //临时保存身高体重数据

    vector<vector<CTestResultInfo>> vecTestResults;
    vector<size_t> vecResIndex;
    set<size_t> setBodyExistIndex;

    for( auto & testResInfo : vecTestResult)
    {
        nIndex++;

        //函数中包含更新本地同步状态函数
        shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testResInfo.m_nItem));
        if(shDevType == DEV_TYPE_UNKNOW)
            continue;

        if(testResInfo.m_bSyncServer)
            continue;

        vecResTemp.clear();

        if(shDevType == DEV_TYPE_BODY)
        {
            if(setBodyExistIndex.find(nIndex - 1) != setBodyExistIndex.end())
            {
                continue;
            }

            //身高体重中的每个已经使用的索引记录，避免重复查找,最后一个身高体重单个数据，删除即可
            setBodyExistIndex.insert(nIndex - 1);
            if(nIndex >= vecTestResult.size())
            {
                //pTableTestResult->deleteResultInfo(testResInfo);
                break;
            }

            if(setBodyExistIndex.find(nIndex) != setBodyExistIndex.end())
            {
                continue;
            }

            //找到一次身高体重测量的测试结果，根据测试时间和项目可以找到，身高体重3个测试项
            //可以使用时间进行相关项目的查找
            vecResIndex = findBodyData(vecTestResult, nIndex, testResInfo.m_strStartTime);

            if(vecResIndex.size() > 0)
            {
                vecResTemp.push_back(testResInfo);

                //倒序遍历同时删除对应数据源中的数据
                for(auto it = vecResIndex.rbegin(); it != vecResIndex.rend(); it++)
                {

                }

                for(auto &index : vecResIndex)
                {
                    vecResTemp.push_back(vecTestResult[index]);
                    setBodyExistIndex.insert(index);
                }

                //找到之后删除数据源中对应的数据
            }
            else
            {
                //当前数据不合法，在数据库中进行删除,身高体重仪器至少需要身高和体重，根据数据数量可以进行判断
                //pTableTestResult->deleteResultInfo(testResInfo);
                continue;
            }

            vecTestResults.push_back(vecResTemp);
        }
        else
        {
            vecResTemp.push_back(testResInfo);
            vecTestResults.push_back(vecResTemp);
        }
    }

    CTestGuideManager::uploadTestResult(vecTestResults, CHttpRequestMgr::getInstance(), DATA_SRC::DataBase );

    //处理上传成功或者失败的测试项
    return bRet;
}

//更新得分规则
bool CServerCommunication::updateScoreRule()
{
    return true;
//    bool bRet = false;

//    //0.安全保护
//    if(m_pHttp == nullptr)
//        return bRet;

//    //1.获取服务器得分规则
//    bool ok = m_pHttp->scoreStanderdSync( true );

//    if( !ok )
//    {
//        LOGERROR("Get scorestanderd rules fail");
//        return bRet;
//    }

//    vector<CScoreRule> vecScoreRole;
//    vecScoreRole = m_pHttp->parseScoreStanderdSync().toStdVector();

//    if( vecScoreRole.size() <= 0 )
//    {
//        LOGERROR( "The scorestanderd rules are empty" );
//        return true;
//    }

//    //2.获取成功后，删除本地得分规则
//    CDatabaseManage *pDatabaseManage = CDatabaseManage::GetInstance();
//    if(nullptr == pDatabaseManage)
//    {
//        LOGERROR("DatabaseManage is nullptr");
//        return bRet;
//    }
//    auto pTableScoreRules = pDatabaseManage->getTabScoreRules();
//    if(nullptr == pTableScoreRules)
//    {
//        LOGERROR("Tablescorerules is nullptr");
//        return bRet;
//    }
//    CScoreRule role;
//    pTableScoreRules->deleteRule(role, true);

//    //3.存入服务器中的得分规则
//    bool bUpdate = true;

//    //分批次传入，一次100条
//    vector<CScoreRule> vecTemp;
//    for(size_t i = 0; i < vecScoreRole.size(); i++)
//    {
//        vecTemp.push_back(vecScoreRole[i]);

//        if(vecTemp.size() == 100 || i == vecScoreRole.size() - 1)
//        {
//            bUpdate &= pTableScoreRules->updateRules(vecTemp);
//            vecTemp.clear();
//        }
//    }

//    if(!bUpdate)
//    {
//        LOGERROR("Database update scorerules fail");
//        return bRet;
//    }

//    //4.通知外部得分规则发生了变化
//    emit signalScoreRuleUpdateCompleted();

//    bRet = true;

//    return bRet;
}

//更新指导视频
bool CServerCommunication::updateVedios()
{
    return true;
//    bool bRet = false;
//    if(m_pHttp == nullptr)
//        return bRet;
//    CDatabaseManage *pDatabaseManage = CDatabaseManage::GetInstance();
//    if(nullptr == pDatabaseManage)
//    {
//        LOGERROR( "DatabaseManage is nullptr" );
//        return bRet;
//    }
//    auto pTableSyncTime = pDatabaseManage->getTabDataSyncTime();

//    QDateTime dateTimeLast;

//    m_mutexSource.lock();
//    short shUpdateType;

//    if( m_vecUpdateList.size() <= 0 )
//    {
//        return false;
//    }

//    shUpdateType = m_vecUpdateList[ 0 ];
//    m_mutexSource.unlock();
//    bool bGet = pTableSyncTime->getSyncTime( shUpdateType, dateTimeLast );
//    if( !bGet )
//    {
//        LOGERROR("Get synchronization time fail");
//        return bRet;
//    }
//    //向服务器发送同步视频链接请求
//    bRet = m_pHttp->sampleVideoSync( QString::number( dateTimeLast.toMSecsSinceEpoch() ), true );
//    if( !bRet )
//    {
//        LOGERROR( "sync sample videos error" );
//        return bRet;
//    }

//    //提取视频链接信息
//    QVector< structSampleVideoSync > vecSampleVideoInfos = m_pHttp->parseSampleVideoSync();
//    if( vecSampleVideoInfos.size() < 0 )
//    {
//        LOGERROR( "parse sample video urls error" );
//        return false;
//    }

//    //下载视频
//    qDebug() << "statring download video";
//    startDownloadSampleVideo( vecSampleVideoInfos );

//    return false;
}

bool CServerCommunication::updateTouristUsageCount()
{
    if(m_pRequestMgr == nullptr)
        return false;

    CTestResultInfo infoSearchInfo;

    infoSearchInfo.m_bSyncServer = false;
    infoSearchInfo.m_bUseSyncServerToSelect = true;
    infoSearchInfo.m_llUniqueID = TOURIST_MODE_ID;

    //2.获取位同步测试结果

    CDatabaseManage &dataBase = CDatabaseManage::GetInstance();

    auto pTableTestResult = dataBase.getTabTestItemResult();
    if( nullptr == pTableTestResult ){
        LOGERROR("TableTestItemResult is nullptr");
        return false;
    }

    vector<CTestResultInfo> vecTestResult;
    pTableTestResult->getResultByInfo(infoSearchInfo, vecTestResult, false);

    if(vecTestResult.size() == 0)
        return true;

    //上传游客使用次数每次仅可上传一次，每次上传一次使用次数，上传成功之后进行下一次上传，若失败，等待外部服务器状态变更进行上传
    CTestResultInfo &testResInfo = vecTestResult[0];
    short shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(testResInfo.m_nItem));

    m_bServerReturn = false;
    m_pRequestMgr->touristUsageRecord( QString::fromStdString(testResInfo.m_sDeviceID), shDevType, testResInfo.getTimeStamp(testResInfo.m_strStartTime));

    //等待服务器返回
    QTime time = QTime::currentTime().addMSecs(5000);
    while(!m_bServerReturn && QTime::currentTime() < time)
    {
        QThread::msleep(1000);
    }

    bool bRet(m_bServerReturn && m_bTouristReturnValue);
    //上传成功将同步标志改为true
    if(bRet)
    {
        pTableTestResult->updateSync(testResInfo.m_llUniqueID, testResInfo.m_nItem, testResInfo.m_strStartTime, true);

        if(vecTestResult.size() - 1 > 0)
        {
            setUpdateType(UPDATE_TYPE_TOURIST_USAGE);
        }
    }

    return bRet;
}

void CServerCommunication::onTouristUsageRecord(bool ok, bool netWorkState, int requestCode)
{
    m_bServerReturn = true;
    m_bTouristReturnValue = ok;
}

vector<size_t> CServerCommunication::findBodyData(const vector<CTestResultInfo> &vecSrc, size_t nIndex, string &strTestTime)
{
    size_t szSize = vecSrc.size();
    short shDevType;
    vector<size_t> vecResIndex;

    for(size_t i = nIndex; i < szSize; i++)
    {
        shDevType = CDevDataCfg::getDevTypeByTestItem(static_cast<short>(vecSrc[i].m_nItem));

        if(shDevType == DEV_TYPE_BODY && vecSrc[i].m_strStartTime == strTestTime)
        {
            vecResIndex.push_back(i);
        }
    }

    return vecResIndex;
}
