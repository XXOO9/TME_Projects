#include "CTestProcAntiCheatingBasic.h"
#include "../Common/CFilePath.h"
#include "CLogger.h"

#include <QTime>

string CTestProcAntiCheatingBasic::s_strVideoName("proc.mp4");
CTestProcAntiCheatingBasic::CTestProcAntiCheatingBasic()
{
    //在实际使用时，当前类处于另一个线程，所以使用等待直连方式
    connect(this, &CTestProcAntiCheatingBasic::signalStart, this, &CTestProcAntiCheatingBasic::onStart, Qt::BlockingQueuedConnection);
    connect(this, &CTestProcAntiCheatingBasic::signalStop, this, &CTestProcAntiCheatingBasic::onStop, Qt::BlockingQueuedConnection);
    connect(this, &CTestProcAntiCheatingBasic::signalRecord, this, &CTestProcAntiCheatingBasic::onRecord, Qt::BlockingQueuedConnection);
}

CTestProcAntiCheatingBasic::~CTestProcAntiCheatingBasic()
{
    if(m_pTimer != nullptr)
    {
        m_pTimer->stop();
        delete m_pTimer;
    }

    disconnect(this, &CTestProcAntiCheatingBasic::signalStart, this, &CTestProcAntiCheatingBasic::onStart);
    disconnect(this, &CTestProcAntiCheatingBasic::signalStop, this, &CTestProcAntiCheatingBasic::onStop);
    disconnect(this, &CTestProcAntiCheatingBasic::signalRecord, this, &CTestProcAntiCheatingBasic::onRecord);
}

void CTestProcAntiCheatingBasic::setCamer(CDevCamera *pCamer)
{
    m_pCamer = pCamer;

    clear();
}

void CTestProcAntiCheatingBasic::setRootPath(const string &strRootPath)
{
    m_strRootPath = strRootPath;

    clear();

    if(!m_strRootPath.empty() && !m_strTestItemDescribe.empty())
    {
        m_strFileStorageName = m_strRootPath + m_strTestItemDescribe;
    }
}

void CTestProcAntiCheatingBasic::setTestItemDescribe(const string &strTestDescribe)
{
    m_strTestItemDescribe = strTestDescribe;

    clear();

    if(!m_strRootPath.empty() && !m_strTestItemDescribe.empty())
    {
        m_strFileStorageName = m_strRootPath + m_strTestItemDescribe;
    }
}

void CTestProcAntiCheatingBasic::grab()
{
    if(m_pCamer != nullptr && m_pCamer->isOpenExternal() && m_pCamer->isOpenInterior())
    {
        bool bClocwiseRotation = false;
        if(m_shTestItem == TEST_ITEM_VC)
        {
            bClocwiseRotation = true;
        }

        CGrabImage image(m_pCamer->Grab_Image(bClocwiseRotation));
        if(!image.getImage().empty())
        {
            m_vecGrabImage.push_back(image);
        }
    }
}

void CTestProcAntiCheatingBasic::createTimer()
{
    if(m_pTimer != nullptr)
        return;

    m_pTimer = new QTimer;
    m_pTimer->setTimerType(Qt::PreciseTimer);
    connect(m_pTimer, &QTimer::timeout, this, &CTestProcAntiCheatingBasic::onTimeOut);
}

void CTestProcAntiCheatingBasic::startTimerC(const int &nMsec)
{
    if(m_pTimer == nullptr)
    {
        return;
    }

    if(!m_pTimer->isActive())
    {
        m_pTimer->start(nMsec);
    }
}

void CTestProcAntiCheatingBasic::stopTimer()
{
    if(m_pTimer == nullptr)
    {
        return;
    }

    if(m_pTimer->isActive())
    {
        m_pTimer->stop();
    }
}

bool CTestProcAntiCheatingBasic::isTimerStart()
{
    bool bRet = false;
    if(m_pTimer != nullptr)
    {
        bRet = m_pTimer->isActive();
    }

    return bRet;
}

void CTestProcAntiCheatingBasic::getResourceList(vector<string> &vecResourece, int &nExitFileNum)
{
    vecResourece = m_vecPath;
    clear();

    nExitFileNum = m_nExitFileNum;
}

void CTestProcAntiCheatingBasic::start_()
{
    if(m_pCamer == nullptr)
    {
        LOGERROR("No camera is configured.");
        return;
    }

    //开启摄像头
    if(m_pCamer->isOpenInterior())
    {
        m_pCamer->openCamer();
    }
    m_pCamer->setState(true);

    emit signalStart();
}

void CTestProcAntiCheatingBasic::stop_(bool bNormalStop)
{
    emit signalStop(bNormalStop);
}

void CTestProcAntiCheatingBasic::record_()
{
    emit signalRecord();
}

void CTestProcAntiCheatingBasic::onStart()
{
    clear();
    //开始之前获取相同类型文件数量
    QString qsFilter = "*" + QString::fromStdString(m_strTestItemDescribe) + "*";
    QStringList listFileName;
    CFilePath::getFileInfoInDir(QString::fromStdString(m_strRootPath), listFileName, qsFilter);

    //若存在视屏，删除一个，主要是获取大小
    QString qsFind = QString::fromStdString(s_strVideoName);
    for(auto &element : listFileName)
    {
        if(element.contains( qsFind ))
        {
            listFileName.removeAt(0);
            break;
        }
    }

    m_nExitFileNumTemp = m_nExitFileNum = listFileName.size();

    createTimer();
}

//
void CTestProcAntiCheatingBasic::onStop(bool bNormalStop)
{
    if(m_pCamer != nullptr)
    {
        m_pCamer->pause();
        m_pCamer->setState(false);
    }

    //只是暂停内部定时器
    stopTimer();

    //正常结束需要根据每个实验的逻辑选取图像，同时进行额外图像文件的删除和重新命名
    if(bNormalStop)
    {
        stopNormal();
    }
    //非正常结束只需删除本次所有过程资源即可
    else
    {
        clear();
    }
}

void CTestProcAntiCheatingBasic::onTimeOut()
{
    grab();
}

int CTestProcAntiCheatingBasic::getRandom(int min, int max)
{
    int num = qrand()%(max-min);
    return num;
}

//进行图像的选取，这里是选取3张，若多余三张才进行工作
void CTestProcAntiCheatingBasic::stopNormal()
{
    size_t szExitImage = m_vecGrabImage.size();
    if(szExitImage > 3)
    {
        //一般的处理情况是选取其中的首尾,同时随机进行选取中间的图像
        vector<CGrabImage> vecTemp;
        size_t szIndex = static_cast<size_t>(getRandom(1, static_cast<int>(szExitImage) - 2));

        for(size_t i = 0; i < szExitImage; i++)
        {
            if(i == 0 || i == szExitImage - 1 || i == szIndex)
            {
                vecTemp.push_back(m_vecGrabImage[i]);
            }
        }

        m_vecGrabImage = vecTemp;
    }

    //初始文件索引,现存的文件数量是索引从 0 开始
    int nIndexOfStart = m_nExitFileNum;

    //对剩下的进行重命名
    string strNewFileName;
    for(auto &element : m_vecGrabImage)
    {
        strNewFileName = m_strFileStorageName + to_string(nIndexOfStart++) + m_strFileSaveFormat;
        bool success = imwrite(strNewFileName, element.getImage());
        if(success)
        {
            m_vecPath.push_back(strNewFileName);
            m_nExitFileNumTemp++;
        }
    }
}
