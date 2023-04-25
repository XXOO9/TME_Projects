#include "CTestProcAntiCheatingSitSBJ.h"
#include "../Common/CFilePath.h"
#include "CLogger.h"
#include "CFilePath.h"
#include "CInterAction.h"

//图像抓取间隔
CTestProcAntiCheatingSitSBJ::CTestProcAntiCheatingSitSBJ()
    : CTestProcAntiCheatingBasic()
{
    m_shTestItem = TEST_ITEM_SBJ;
    m_itimerInterval = 1000;
}

void CTestProcAntiCheatingSitSBJ::onStart()
{
    double l1;
    l1 = clock();
    CTestProcAntiCheatingBasic::onStart();

    //开启视屏录制
    if(m_pCamer != nullptr)
    {
        m_pCamer->closeRecordVideo();
        m_pCamer->closeVideoFile();
        if(m_pCamer->isOpenInterior())
        {
            if(!m_pCamer->recordVideo(m_strRootPath + s_strVideoName))
            {
                LOGERROR("m_pCamer->recordVideo == false");
            }
        }
        else
        {
            LOGERROR("m_pCamer->isOpenInterior() == false");
        }
    }

    //开启定时器定时
    startTimerC(m_itimerInterval);
    qDebug() << "tttttttttttttttt:" << clock() - l1;
}

void CTestProcAntiCheatingSitSBJ::onTimeOut()
{
    ++m_iRecordVideoTime;

    //重新开启录制
    if(m_iRecordVideoTime > 600 && m_pCamer != nullptr)
    {
        m_pCamer->closeRecordVideo();
        if(!m_pCamer->recordVideo(m_strRootPath + s_strVideoName))
        {
            LOGERROR("m_pCamer->recordVideo == false");
        }
        m_iRecordVideoTime = 0;
    }
}

void CTestProcAntiCheatingSitSBJ::onStop(bool bNormalStop)
{
    if(m_pCamer == nullptr)
    {
        return;
    }

    m_pCamer->setState(false);
    if(m_pCamer != nullptr)
    {
        m_pCamer->closeRecordVideo();
    }

    CTestProcAntiCheatingBasic::onStop(bNormalStop);

    if(m_pCamer != nullptr)
    {
        m_pCamer->closeVideoFile();
    }
}

void CTestProcAntiCheatingSitSBJ::stopNormal()
{
    m_vecGrabImage.clear();
    if(m_pCamer != nullptr)
    {
        m_pCamer->closeRecordVideo();

        if(m_pCamer->openVideo(m_strRootPath + s_strVideoName))
        {
            int nCount = m_pCamer->getFrameCount();
            float fFPS = m_pCamer->getFPS();

            //这里进行视屏分析发现跳远的结束一般在视屏结尾2.91s左右，腾空时间为1.7s
            static int nStopPos = 2910;
            static int nHangTime = 1700;
            int nStopCount = static_cast<int>((static_cast<float>(nStopPos) / fFPS));
            int nHangTimeCount = static_cast<int>((static_cast<float>(nHangTime) / fFPS));

            int nStopIndex = nCount, nStartIndex = 0;

            //总帧数大于结束
            if(nCount >= nStopCount)
            {
                nStopIndex = nCount - nStopCount;
                nStartIndex = nStopIndex - nHangTimeCount;
                if(nStartIndex < 0)
                    nStartIndex = 0;
            }
            else
            {
                nStartIndex = 0;
                nStopIndex = nStartIndex + nHangTimeCount;

                if(nStopIndex > nCount)
                    nStopPos = nCount;
            }

            vector<cv::Mat> vec;
            m_pCamer->getFrameInVideoFile(vec, nStartIndex, nStopIndex);
            m_pCamer->closeVideoFile();

            for(auto &element : vec)
            {
                m_vecGrabImage.push_back(CGrabImage(element));
            }
        }
        else
        {
            LOGERROR("m_pCamer->openVideo == false");
        }
    }

    //非调试模式，删除视屏资源
    if(!CInterAction::Get_Instance().getIsDeveloper())
    {
        CFilePath::deleteFileOrFolder(m_strRootPath + s_strVideoName);
    }

    int nExitNum = static_cast<int>(m_vecGrabImage.size());
    if(nExitNum >= 3)
    {
        vector<CGrabImage> vecTemp = m_vecGrabImage;

        //将m_vecGrabImage分为3段，前1、2段选择最后一张，最后一个 选取其中中间位置的照片
        size_t nImageNum = vecTemp.size();
        size_t szEverySectionNum = (nImageNum + 1) / 3;

        //找到每一段中间的图像即可
        size_t szStart, szMid, szEnd;

        szStart = szEverySectionNum - 1;
        szMid = szEverySectionNum - 1 + szStart;
        szEnd = szMid + szEverySectionNum / 2;
        if(szEnd >= nImageNum)
        {
            szEnd = nImageNum - 1;
        }

        m_vecGrabImage.clear();
        nImageNum = vecTemp.size();
        for(size_t i = 0; i < nImageNum; i++)
        {
            if(i == szStart || i == szMid || i== szEnd)
            {
                m_vecGrabImage.push_back(vecTemp[i]);
            }
            else
            {
                vecTemp[i].getImage().release();
            }
        }
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
        element.getImage().release();
    }
}

void CTestProcAntiCheatingSitSBJ::onRecord()
{

}
