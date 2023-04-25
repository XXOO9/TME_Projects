#include "CFindUserBasic.h"
#include "CLogger.h"
#include "CDataSynchronize.h"

CFindUserBasic::CFindUserBasic()
{

}

CFindUserBasic::~CFindUserBasic()
{
    if( nullptr != m_pUserIndentify ){
        delete m_pUserIndentify;
        m_pUserIndentify = nullptr;
    }
}

void CFindUserBasic::setFindUserInfo(const CUserInfo &userInfo)
{
    if(!isAvailable())
        return;

    m_objUserInfo = userInfo;

    m_semaphoreThread.release();
}

void CFindUserBasic::startFind()
{
    startThread();
}

void CFindUserBasic::stopFind()
{
    stopThread();
}

bool CFindUserBasic::isAvailable()
{
    bool bRet = false;

    //线程在运行
    if(m_bThreadRunFlag)
    {
        //线程循环标志
        if(m_bThreadLoopFlag)
        {
            if(!m_bProcessState)
            {
                bRet = true;
            }
        }
    }

    return bRet;
}

void CFindUserBasic::setCurFindValid(bool bValid)
{
    m_bCurFindValid = bValid;
}

void CFindUserBasic::dealThreadLogic()
{
    LOGINFO( "start user identify..." );
    initServer();

    m_vecFindRes.clear();

    //优先在本地识别用户信息
    findOnLocal(m_objUserInfo, m_vecFindRes);

    //如果本地识别失败，从服务器去查询
    bool bFindOnServer = false;
    m_bServerReturn = true;

    //本地没查找到 同时服务器存在
    if(m_vecFindRes.size() == 0 && m_bServerConnect)
    {
        bFindOnServer = true;
        m_bServerReturn = false;

        //识别的结果会从服务器返回,并且进行结果赋值，统一从线程中进行服务器结果的查询
        findOnServer(m_objUserInfo);
    }

    //等待服务器资源返回,最多等待5s
    if(bFindOnServer)
    {
        qint64 llTime = QDateTime::currentDateTime().toMSecsSinceEpoch() + 5000;
        while (!m_bServerReturn && QDateTime::currentDateTime().toMSecsSinceEpoch() < llTime)
        {
//            msleep(50);
        }

        //超时取消本次请求
        if(!m_bServerReturn && m_pUserIndentify != nullptr)
        {
            m_pUserIndentify->abortRequest();
        }
    }

    if(m_vecFindRes.size() > 0)
    {
        //判断是从 本地数据库 还是 服务器 得到的用户信息
        if(bFindOnServer)
        {
            LOGINFO("Mode is : Face Server, Name is : " + m_vecFindRes[0].m_strName.toStdString() +
                    " Gender is : " + to_string(m_vecFindRes[0].m_bGender));
        }
        else
        {
            LOGINFO("Mode is : Face Local, Name is : " + m_vecFindRes[0].m_strName.toStdString() +
                    " Gender is : " + to_string(m_vecFindRes[0].m_bGender));
        }

        emit signalFindSucess(m_vecFindRes.at(0), this);
    }
    else
    {
        emit signalFindFail(this);
    }
}

void CFindUserBasic::initServer()
{
//    qDebug() << "init Server thread ID = " << QThread::currentThreadId();
    if( nullptr == m_pUserIndentify ){
        m_pUserIndentify = new CHttpUserIdentify();
    }
    connect( m_pUserIndentify, &CHttpUserIdentify::sigRequestRetAvailable, this, &CFindUserBasic::onUserIdentifyFinished, Qt::QueuedConnection );
}

void CFindUserBasic::onServerConnectState(bool bConnect)
{
    m_bServerConnect = bConnect;
}

CHttpUserIdentify *CFindUserBasic::pUserIndentify() const
{
    return m_pUserIndentify;
}

void CFindUserBasic::userIndentify(QString faceFeature, QString cardNo, FACE_ENGINE facegine, QString code)
{
//    qDebug() << "send request thread ID = " << QThread::currentThreadId();
    if( nullptr == m_pUserIndentify ){
        initServer();
    }

    m_pUserIndentify->setIndentifyCondition( faceFeature, cardNo, facegine );
    m_pUserIndentify->httpRequest();
}

void CFindUserBasic::onUserIdentifyFinished(bool ok)
{
    if(ok)
    {
        if(m_pUserIndentify == nullptr)
        {
            LOGERROR("m_pUserIndentify == nullptr");
            return;
        }

        QMap<QString, QString> result = m_pUserIndentify->getMapUserInfo();
        CUserInfo userInfo;

        CDataSynchronize::equalUserInfo(userInfo, result);
        m_vecFindRes.push_back(userInfo);

        //每次将识别的用户信息更新到数据库同步
        auto *pTableUserInfo = CDatabaseManage::GetInstance().getTableUserIdentityInfo();
        if(pTableUserInfo != nullptr)
        {
            pTableUserInfo->updateInfo(userInfo);
        }
    }

    //服务器返回之后再进行下一次识别返回
    m_bServerReturn = true;
}

void CFindUserBasic::onHttp(QString face, QString cardNo, int faceengine)
{
//    qDebug() <<"onHttp ID =" << QThread::currentThreadId();
    FACE_ENGINE engine     = static_cast<FACE_ENGINE>(faceengine);

    if(m_pUserIndentify == nullptr)
        return;

    m_pUserIndentify->setIndentifyCondition( face, cardNo, engine );
    m_pUserIndentify->httpRequest();
}
