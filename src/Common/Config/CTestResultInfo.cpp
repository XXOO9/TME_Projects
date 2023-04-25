///////////////////////////////////////////////////////////
//  CTestResultInfo.cpp
//  Implementation of the Class CTestResultInfo
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CTestResultInfo.h"

#include <QString>
#include <QStringList>
#include "CFilePath.h"

//当前实验可能一次测试中存在多个结果，若是正式测试，可作为正常成绩的测试，需要进行防作弊
//现在的防作弊是提取图像，每一次实验当中的一次测试随机提取3张图像，因为是存取路径所以采用不可见字符同时用相同的字符进行成绩分隔
//每一次测试使用 ; 进行间隔，一次测试中的多个行为使用,进行间隔
const char g_separateCharEachTime = ';';
const char g_separateCharPath = ',';


static QString s_dateTimeFormat("yyyyMMddhhmmss");

CTestResultInfo::CTestResultInfo()
{

}

void CTestResultInfo::reset()
{
    m_llUniqueID         = UNKNOWN_ID;
    m_nItem              = TEST_ITEM_UNKNOW;
    m_nNature            = TEST_UNKONW;
    m_strMulitResult     = "";
    m_strProcessDataPath = "";
    m_strConfidence      = "";
    m_strResSignature    = "";
    m_bSyncServer        = false;
    m_bUseSyncServerToSelect = false;
    m_sDeviceID          = "";
    m_nRunNfcCardID      = 0;
    m_nRunShortWay       = 0;
    m_strPathResourceRoot.clear();
    m_bUseUnequalNature  = false;
}

void CTestResultInfo::clearResult(bool bRetentionTimes)
{
    auto shCount = getTestTimes();

    m_strMulitResult.clear();
    m_strConfidence.clear();
    m_strProcessDataPath.clear();

    if(bRetentionTimes)
    {
        //对结果赋空值
        for(short i = 0; i < shCount; i++)
        {
            addTestResult("", false);
            addProcSrc("", true);
        }
    }
    else
    {
        m_strStartTime.clear();
    }
}

bool CTestResultInfo::isResultCorrect() const
{
    //自由测试时数据不进行保存，仅仅是作为使用次数进行保存
    //这里判断对信息的有无和格式进行判断
    //if(m_llUniqueID == TOURIST_MODE_ID)
    //    return false;

    if(m_nItem == TEST_ITEM_UNKNOW)
        return false;

    if(m_strMulitResult.empty())
        return false;

    //若是正式的成绩，需要判断成绩和路径信息是否一致,暂不判断文件是否存在
//    if(m_nNature == TEST_FORMAL)
//    {
//        if(m_strProcessDataPath.empty() && m_nItem != TEST_ITEM_ROPE_SKIP && m_nItem != TEST_ITEM_RUN_50 && m_nItem != TEST_ITEM_RUN_800 && m_nItem != TEST_ITEM_RUN_1000)
//            return false;

//        if(m_strResSignature.empty())
//            return  false;

//        QString qsMulitRes, qsPath;
//        qsMulitRes.fromStdString(m_strMulitResult);
//        qsPath.fromStdString(m_strProcessDataPath);

//        QStringList lsMultRes = qsMulitRes.split(g_separateCharEachTime);
//        QStringList lsPath = qsPath.split(g_separateCharEachTime);

//        if(lsMultRes.size() != lsPath.size())
//            return false;
//    }

    return true;
}

//根据测试项目 用户id 时间就可找出一个 确定的测试结果
vector<short> CTestResultInfo::getQueryItem(const CTestResultInfo &info)
{
    vector<short> vecType;
    if(info.m_llUniqueID != UNKNOWN_ID)
        vecType.push_back(emQuery_Condition::Query_Type_UniqueID);

    if(info.m_nItem != TEST_ITEM_UNKNOW)
        vecType.push_back(Query_Type_Item);

    if(!info.m_strStartTime.empty())
        vecType.push_back(Query_Type_Time);

    if(info.m_nNature != TEST_UNKONW && info.m_bQueryWithNature)
    {
        if(!info.m_bUseUnequalNature )
        {
            vecType.push_back(Query_Type_Nature);
        }
        else
        {
            vecType.push_back(Query_Type_Nature_Unequal);
        }
    }


    if(info.m_bUseSyncServerToSelect)
        vecType.push_back(Query_Type_SyncServer);

    return  vecType;
}

void CTestResultInfo::setResourceRootPath(string strUserID, string strRootPath, string strDateTime, string strDevID, string strDevDescribe)
{
    m_strPathResourceRoot = strRootPath + strDateTime + '-' + strDevDescribe + '-' + strUserID + '-' + strDevID + '/';
}

void CTestResultInfo::addTestResult(const string &strResult, bool bCredible, const uint &nIllegalTimes)
{
    string strConfidence = bCredible ? "1" : "0";
    m_strConfidence += (strConfidence + g_separateCharEachTime);

    string strResTemp;

    //引体向上
    //strResTemp值为 5(2);
    if(nIllegalTimes > 0)
    {
        strResTemp = strResult + g_separateCharIllegal + to_string(nIllegalTimes) + g_separateCharEachTime;
    }
    //strResTemp值为 5;
    else
    {
        strResTemp = strResult + g_separateCharEachTime;
    }

    m_strMulitResult += strResTemp;
}

void CTestResultInfo::updateTestResult(uint nIndex, const string &strResult, bool bCredible, const uint &nIllegalTimes)
{
    if(m_strMulitResult.empty())
    {
        return;
    }

    vector<string> vecConfidence;
    vector<string> vecTestResult;

    string strSplit(&g_separateCharEachTime, 1);
    splitString(m_strConfidence, strSplit, vecConfidence);
    splitString(m_strMulitResult, strSplit, vecTestResult);

    bool bRet = vecConfidence.size() == vecTestResult.size();

    if(!bRet)
        return;

    if(nIndex >= static_cast<uint>(vecTestResult.size()))
    {
        return;
    }

    string strRes;

    //清空当前结果
    m_strMulitResult.clear();
    m_strConfidence.clear();
    uint nIllegalTimesTemp;

    for(uint i = 0; i < vecConfidence.size(); i++)
    {
        getDetailedRes(vecTestResult[i], strRes, nIllegalTimesTemp);

        if(i != nIndex)
        {
            addTestResult(strRes, vecConfidence[i] == "1", nIllegalTimesTemp);
        }
        else
        {
            addTestResult(strResult, bCredible, nIllegalTimes);
        }
    }
}

int CTestResultInfo::getValidMaxTestResult(string &strMax) const
{  
    uint nIllegalTimes;
    return getValidMaxTestResult(strMax, nIllegalTimes);
}

bool CTestResultInfo::getLastTestResult(string &strRes) const
{
    uint nIllegalTimes;
    return getLastTestResult(strRes, nIllegalTimes);
}

int CTestResultInfo::getValidMaxTestResult(string &strMax, uint &nIllegalTimes) const
{
    if(m_strMulitResult.empty())
    {
        return -1;
    }

    vector<string> vecConfidence;
    vector<string> vecTestResult;

    string strSplit(&g_separateCharEachTime, 1);
    splitString(m_strConfidence, strSplit, vecConfidence);
    splitString(m_strMulitResult, strSplit, vecTestResult);

    bool bRet = vecConfidence.size() == vecTestResult.size();
    bool bValid = false;

    if(!bRet)
        return -1;

    bool bFirstValidData = true;
    unsigned int nMaxIndex = 0;
    double lfMax = 0.0, lfMaxTemp = 0.0;
    bool bReplace;
    string strRes;

    for(unsigned int i = 0; i < vecConfidence.size(); i++)
    {
        if(vecConfidence[i] == "1" && !vecTestResult[i].empty())
        {
            bValid = true;
            strRes = vecTestResult[i];

            getDetailedRes(vecTestResult[i], strRes, nIllegalTimes);

            lfMaxTemp = atof(strRes.c_str());

            //排球的最大数据判断需要最大数据减去违例次数之后再进行判断
            if(m_nItem == TEST_ITEM_VOLLEYBALL)
            {
                lfMaxTemp = atof(strRes.c_str()) - static_cast<double>(nIllegalTimes);
            }

            //不是第一次进入需要根据项目进行判断
            bReplace = bFirstValidData;
            if(!bReplace)
            {
                //计时越短越好
                if(m_nItem == TEST_ITEM_RUN_50 || m_nItem == TEST_ITEM_RUN_800 || m_nItem == TEST_ITEM_RUN_1000
                        || m_nItem == TEST_ITEM_BASKETBALL || m_nItem == TEST_ITEM_FOOTBALL)
                {
                    bReplace = lfMaxTemp < lfMax;
                }
                else
                {
                    bReplace = lfMaxTemp > lfMax;
                }
            }

            if(bReplace)
            {
                lfMax = lfMaxTemp;
                nMaxIndex = i;
            }

            bFirstValidData = false;
        }
    }

    if(bValid)
    {
        strMax = vecTestResult[nMaxIndex];
        getDetailedRes(vecTestResult[nMaxIndex], strMax, nIllegalTimes);
    }

    return bValid ? static_cast<int>(nMaxIndex) : -1;
}

bool CTestResultInfo::getLastTestResult(string &strRes, uint &nIllegalTimes) const
{
    if(m_strMulitResult.empty())
    {
        return false;
    }

    vector<string> vecConfidence;
    vector<string> vecTestResult;

    string strSplit(&g_separateCharEachTime, 1);
    splitString(m_strConfidence, strSplit, vecConfidence);
    splitString(m_strMulitResult, strSplit, vecTestResult);

    strRes.clear();
    if(vecTestResult.size() > 0)
    {
        getDetailedRes(*vecTestResult.rbegin(), strRes, nIllegalTimes);
    }

    bool bRet = false;
    if(vecConfidence.size() == vecTestResult.size() && !strRes.empty())
    {
        if(*vecConfidence.rbegin() == "1")
        {
            bRet = true;
        }
    }

    return bRet;
}

bool CTestResultInfo::getTestResult(uint nIndex, string &strRes, uint &nIllegalTimes) const
{
    if(m_strMulitResult.empty())
    {
        return false;
    }

    vector<string> vecConfidence;
    vector<string> vecTestResult;

    string strSplit(&g_separateCharEachTime, 1);
    splitString(m_strConfidence, strSplit, vecConfidence);
    splitString(m_strMulitResult, strSplit, vecTestResult);

    strRes.clear();
    if(nIndex <= vecTestResult.size())
    {
        getDetailedRes(vecTestResult[nIndex], strRes, nIllegalTimes);
    }
    else {
        return false;
    }

    return true;
}

bool CTestResultInfo::getDetailedRes(const string &src, string &strRes, uint &nIllegalTimes)
{
    static string strSplitIllegalTimes(&g_separateCharIllegal, 1);

    nIllegalTimes = 0;
    strRes = src;
    //证明存在违例次数
    if(src.find(strSplitIllegalTimes) != string::npos)
    {
        vector<string> vecTemp;
        splitString(src, strSplitIllegalTimes, vecTemp);
        if(vecTemp.size() >= 2)
        {
            strRes = vecTemp[0];
            nIllegalTimes = static_cast<uint>(atoi(vecTemp[1].c_str()));
        }
    }

    return true;
}

string CTestResultInfo::getSaveTimeStamp(const QString &qsTime)
{
    QDateTime datetime = QDateTime::fromMSecsSinceEpoch(qsTime.toLongLong());
    qDebug() << "original ret = " << QString::fromStdString( getSaveTimeStamp(datetime) );
    return getSaveTimeStamp(datetime);
}

string CTestResultInfo::getSaveTimeStamp(const QDateTime &datetime)
{
    string strTemp;
    strTemp = datetime.toString(s_dateTimeFormat).toStdString();
    return strTemp;
}

QString CTestResultInfo::getTimeStamp(const string &strSaveTime)
{
    QDateTime datetime = QDateTime::fromString(QString::fromStdString(strSaveTime), s_dateTimeFormat);
    long long llTimeStamp = datetime.toMSecsSinceEpoch();
    return QString::number(llTimeStamp);
}

long long CTestResultInfo::changeSaveTimeToStamp(const string &strSaveTime)
{
    long long llTimeStamp = 0;

    QDateTime datetime = QDateTime::fromString(QString::fromStdString(strSaveTime), s_dateTimeFormat);
    llTimeStamp = datetime.toMSecsSinceEpoch();

    return llTimeStamp;
}

short CTestResultInfo::compare(const CTestResultInfo &one, const CTestResultInfo &two)
{
    string strOne, strTwo;
    int nOne = one.getValidMaxTestResult(strOne);
    int nTwo = two.getValidMaxTestResult(strTwo);

    short shRet = 0;
    //两者都无有效成绩，根据时间戳判断（新的替换旧的）
    if(nOne < 0 && nTwo < 0)
    {
        if(one.m_strStartTime != two.m_strStartTime)
            shRet = one.m_strStartTime > two.m_strStartTime ? 1 : -1;
    }
    else if ( nOne > 0 && nTwo < 0)
    {
        shRet = 1;
    }
    else if ( nOne < 0 && nTwo > 0)
    {
        shRet = -1;
    }
    else
    {
        //两个值相同使用时间戳判断
        if(strOne == strTwo)
        {
            if(one.m_strStartTime != two.m_strStartTime)
                shRet = one.m_strStartTime > two.m_strStartTime ? 1 : -1;
        }
        else
        {
            CTestResultInfo resTemp;
            resTemp.addTestResult(strOne, 1);
            resTemp.addTestResult(strTwo, 1);
            nOne = resTemp.getValidMaxTestResult(strOne);

            shRet = strOne == strTwo ? -1 : 1;
        }
    }

    return shRet;
}

bool CTestResultInfo::mergeToCurrent(const CTestResultInfo &src)
{
    if(src.m_nItem != m_nItem)
        return false;
    
    m_strMulitResult     += src.m_strMulitResult;
    m_strConfidence      += src.m_strConfidence;
    m_strProcessDataPath += src.m_strProcessDataPath;
    
    return true;
}

short CTestResultInfo::getTestTimes() const
{
    vector<string> vecTestResult;
    string strSplit(&g_separateCharEachTime, 1);
    splitString(m_strMulitResult, strSplit, vecTestResult);

    return static_cast<short>(vecTestResult.size());
}

void CTestResultInfo::addProcSrc(const string &src, bool bEnd)
{
    m_strProcessDataPath.append(src);
    if(!bEnd)
    {
        m_strProcessDataPath += g_separateCharPath;
    }
    else
    {
        m_strProcessDataPath += g_separateCharEachTime;
    }
}

vector<string> CTestResultInfo::getProcSrc(int nIndex) const
{
    vector<string> vecRet, vecTemp, vecTempEachTime;

    string strTemp(&g_separateCharPath, 1), strTempEachTime(&g_separateCharEachTime, 1);

    splitString(m_strProcessDataPath, strTempEachTime, vecTempEachTime);
    //获取指定的某一次的测试资源
    if(nIndex >= 0)
    {
        if(nIndex < static_cast<int>(vecTempEachTime.size()))
        {
            auto &strSrc = vecTempEachTime[static_cast<unsigned int>(nIndex)];
            splitString(strSrc, strTemp, vecRet);
        }
    }
    //获取全部测试资源
    else
    {
        for(auto &element : vecTempEachTime)
        {
            splitString(element, strTemp, vecTemp);

            for(auto &path : vecTemp)
            {
                vecRet.push_back(path);
            }
        }
    }

    return vecRet;
}

void CTestResultInfo::splitString(const string &strSrc, const string &delimit, vector<string> &result)
{
    string str(strSrc);
    size_t pos = str.find(delimit);
    str += delimit;//将分隔符加入到最后一个位置，方便分割最后一位

    result.clear();

    string strTemp;
    while (pos != string::npos) {
        result.push_back(str.substr(0, pos));
        str = str.substr(pos + 1);//substr的第一个参数为起始位置，第二个参数为复制长度，默认为string::npos到最后一个位置
        pos = str.find(delimit);
    }

    //删除最后一个为空字符的内容
    size_t szSize = result.size();
    if(szSize > 0)
    {
        if(result[szSize - 1].empty())
        {
            result.erase(result.begin() + static_cast<int>(szSize) - 1);
        }
    }
    //不存在则表明只有一个数据
    else
    {
        result.push_back(strSrc);
    }
}

vector<string> CTestResultInfo::splitString(const string &strSrc, string delimit, bool bKeepEmptyElement)
{
    vector<string> result;
    if(strSrc.empty())
        return result;

    string strTemp, str(strSrc);
    size_t pos = str.find(delimit);
    str += delimit;//将分隔符加入到最后一个位置，方便分割最后一位

    while (pos != string::npos) {
        strTemp = str.substr(0, pos);

        //不保存空元素的情况下，资源为空不进行保存
        if(!(!bKeepEmptyElement && strTemp.empty()))
        {
            result.push_back(strTemp);
        }

        str = str.substr(pos + 1);//substr的第一个参数为起始位置，第二个参数为复制长度，默认为string::npos到最后一个位置
        pos = str.find(delimit);
    }

    //删除最后一个为空字符的内容
    size_t szSize = result.size();
    if(szSize > 0)
    {
        if(result[szSize - 1].empty())
        {
            result.erase(result.begin() + static_cast<int>(szSize) - 1);
        }
    }
    //不存在则表明只有一个数据
    else
    {
        if(strSrc.find(delimit) == string::npos)
            result.push_back(strSrc);
    }

    return result;
}

QString CTestResultInfo::getPrintRes(QString unit) const
{
    QString qsRet = QStringLiteral("犯规");
    int nTemp;
    uint nIllegalTimes;
    string strValidRes;

    if(getValidMaxTestResult(strValidRes, nIllegalTimes) >= 0)
    {
        switch (m_nItem) {
        //当前结果等于有效值减去犯规次数
        case TEST_ITEM_PUO:
        case TEST_ITEM_PULL_UP:
        case TEST_ITEM_VOLLEYBALL:
        {
            nTemp = atoi(strValidRes.c_str());
            if(static_cast<uint>(nTemp) > nIllegalTimes)
            {
                qsRet = QString::number(static_cast<uint>(nTemp) - nIllegalTimes);
            }
            else
            {
                qsRet = "0";
            }
            break;
        }
        //打印对应的时间格式, 中长跑 变更为分秒显示 03'26''
        case TEST_ITEM_RUN_800:
        case TEST_ITEM_RUN_1000:
        {
            double lfValue = atof(strValidRes.c_str());
            int nMin = static_cast<int>(lfValue / 60);
            //保留秒之后的2位小数，外部存储是保留2位小数即可
            double lfSes = lfValue - nMin * 60;

            //最终展示在面板上的值 01:07.22;
            char arrTemp[10] = {0};
            sprintf_s(arrTemp, 10, "%02d", nMin);

            char arrTemp1[10] = {0};
            sprintf_s(arrTemp1, 10, ":%05.02f", lfSes);

            QByteArray arr1(arrTemp, 2);
            QByteArray arr2(arrTemp1, 6);

            arr1 += arr2;
            qsRet = QString(arr1);

            if(strValidRes == TEST_RES_ABORT)
            {
                qsRet = QStringLiteral("终止考试");
            }

            break;
        }
        default:
        {
            qsRet = QString::fromStdString(strValidRes);
            break;
        }
        }

        qsRet += unit;
    }

    return qsRet;
}

QString CTestResultInfo::getUploadRes() const
{
    QString qsRet(TEST_RES_FOUL);
    int nTemp;
    uint nIllegalTimes;
    string strValidRes;

    if(getValidMaxTestResult(strValidRes, nIllegalTimes) >= 0)
    {
        switch (m_nItem) {
        //当前结果等于有效值减去犯规次数
        case TEST_ITEM_PUO:
        case TEST_ITEM_PULL_UP:
        case TEST_ITEM_VOLLEYBALL:
        {
            nTemp = atoi(strValidRes.c_str()); 

            if(static_cast<uint>(nTemp) > nIllegalTimes)
            {
                qsRet = QString::number(static_cast<uint>(nTemp) - nIllegalTimes);
            }
            else
            {
                qsRet = "0";
            }
            break;
        }
        default:
        {
            qsRet = QString::fromStdString(strValidRes);
            break;
        }
        }
    }

    return qsRet;
}

bool CTestResultInfo::getScoreRes(bool bMaxValid, float &fValue) const
{
    string strTemp;
    bool bRet;
    short shTestItem = static_cast<short>(m_nItem);
    uint nIllegalTimes;

    if(bMaxValid)
    {
        bRet = getValidMaxTestResult(strTemp, nIllegalTimes) >= 0;
    }
    else
    {
        bRet = getLastTestResult(strTemp);
    }

    if(bRet)
    {
        //排球的得分指标需要减去结果进行获取
        if(shTestItem == TEST_ITEM_VOLLEYBALL || shTestItem == TEST_ITEM_PUO || shTestItem == TEST_ITEM_PULL_UP)
        {
            fValue = QString::fromStdString(strTemp).toFloat() - static_cast<float>(nIllegalTimes);
            if(fValue < 0)
            {
                fValue = 0.0f;
            }
        }
        else
        {
            fValue = QString::fromStdString(strTemp).toFloat();
        }
    }

    return bRet;
}

bool CTestResultInfo::isLogData() const
{
    string strTemp(&g_separateCharEachTime, 1);
    auto vecTemp = splitString(m_strMulitResult, strTemp, true);
    return vecTemp.size() == 0;
}
