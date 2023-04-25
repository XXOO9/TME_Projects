#include "CAnalysisLungFunc.h"
#include "CAlgorithm.h"
#include "commondefin.h"
#include <numeric>

//定义呼吸阈值，呼吸包含呼和吸过程, 单位 ml
const float NORMAL_BREATHE_THRESHOLD = 150; //正常呼吸
const float VC_BREATHE_THRESHOLD = 2000;	//肺活量（包含用力肺活量）
const float WRONG_BREATHE_THRESHOLD = 50;	//不正常过程

CAnalysisLungFunc::CAnalysisLungFunc()
{

}

bool CAnalysisLungFunc::anlsTidalVolume(float *pSrcData, const unsigned long &lSrcDataLen, const float &fSampleRate, CAnlsResultVC &targetRes)
{
    float *pVelocity = nullptr, *pFlow = nullptr;
    float *pTempNewFlowOrVelocity = new float[lSrcDataLen];

    if(m_shDataType == Data_Velocity)
    {
        pVelocity = pSrcData;
        pFlow = pTempNewFlowOrVelocity;
    }
    else
    {
        pFlow = pSrcData;
        pVelocity = pTempNewFlowOrVelocity;
    }

    if(pVelocity == nullptr || pFlow == nullptr)
    {
        delete [] pTempNewFlowOrVelocity;
        return false;
    }

    //找到数据结尾点，通过流量数据的最大小值进行获取
    float max, min;
    ULONG_ALGORITHM	max_pos, min_pos;
    if(!CAlgorithm::getMaxMin(max, max_pos, min, min_pos, lSrcDataLen, pFlow))
    {
        delete [] pTempNewFlowOrVelocity;
        return false;
    }

    ULONG_ALGORITHM lEndBreathPos = max > min ? max_pos : min_pos;

    //获取简单进行等值滤波之后的数据
    float *pOutDataBuffer = new float[lEndBreathPos + 1];
    memset(pOutDataBuffer, 0, (lEndBreathPos + 1) * sizeof(float));

    //滤波之后数据与原始数据之间长度不一致，在其中添加了滤波数据与原始数据间的对应关系
    map<ULONG_ALGORITHM, ULONG_ALGORITHM> mapMapping;
    ULONG_ALGORITHM lOutDataLength = fliterEqualData(pFlow, pOutDataBuffer, lEndBreathPos + 1, mapMapping);

    ///2.2 ---------------- 获取该段数据的所有完整（最大25个）. ---------------
    //////////////////////////////////////////////////////////////////////////
    /// update by sw 由于以前未对下降高度进行限制导致获取到的Wave个数偏多导致不正确，
    /// 这里设置下降高度不得小于最大下降高度的0.01
    float	judge_fu = abs(((max-min)*0.01f));	/// 设置最小下降高度

    ///2.2 ---------------- 获取该段数据的所有完整 波形. ---------------
    vector<R_WAVE> vecWave;
    int waveCount = getWave(vecWave, lOutDataLength, pOutDataBuffer, judge_fu);

    //////////////////////////////////////////////////////////////////////////

    ///2.4 ---------------- 计算所有自然波形的顶部时间差之和得到呼吸频率. -----
    float  fTemp = 0;
    double s1	 = 0.0;     //潮气量
    double s2    = 0.0;     //补吸气量
    double s3    = 0.0;     //补呼气量

    int     nTVCount = 0;
    float   fSubMax = 0.0, fSubMin = 0.0;

    int fSumPeriod = 0; // 正常呼吸波形经历总时间
    unsigned long lPrevious, lCur;
    auto itFind = mapMapping.begin();
    float fLastSvalue = 0;

    if(waveCount <= 0)
    {
        delete [] pTempNewFlowOrVelocity;
        delete [] pOutDataBuffer;
        return false;
    }

    for(unsigned int i = 0; i < static_cast<unsigned int>(waveCount); i++)
    {
        if ((vecWave[i].tpos != max_pos) && (compareFloating(vecWave[i].tvalue, max) != 0) &&
            (vecWave[i].spos != min_pos) && (compareFloating(vecWave[i].svalue, min) != 0))  //去除肺活量波形
        {
            fLastSvalue = vecWave[i].svalue;
            fSubMin += vecWave[i].svalue;
            //当前测量出来的波形是以波峰为起点，以波峰为终点，所以实际波形数量少于测量出来的点数
            //测算潮气量时第一个波形不取波峰最后一个波形不取波谷,在数量和理论上都能满足

            if (i > 0)
            {
                fSubMax += vecWave[i].tvalue;
                //这里计算总周期需要将滤波之后的数据对照成原始数据再进行处理，要不
                //计算的实际长度会短与手动测量的长度，测试出来数据偏高
                itFind = mapMapping.find(vecWave[i].tpos);
                if(itFind == mapMapping.end())
                    continue;

                lCur = itFind->second;
                itFind = mapMapping.find(vecWave[i - 1].tpos);
                if(itFind == mapMapping.end())
                    continue;

                lPrevious = itFind->second;

                fTemp = lCur - lPrevious;

                fSumPeriod += fTemp;

                //实际波形数量总是少于测试出来的点数
                nTVCount++;
            }
        }
    }

    if (nTVCount <= 0)
    {
        delete [] pTempNewFlowOrVelocity;
        delete [] pOutDataBuffer;
        return false;
    }

    //最后一个波形的波谷不需要
    fSubMin -= fLastSvalue;

    //平均每个正常波形的时间
    float fAvePeriod = fSumPeriod / nTVCount;

    //倒数第一,倒数第二
    float fLastOne, fLastButOne;

    //潮气量（等于当前所有测量的波峰减去波谷）
    s1 = static_cast<double>(fabs(fSubMax - fSubMin)) / static_cast<double>(nTVCount);

    if( max_pos > min_pos)
    {
        fLastOne = max;
        fLastButOne = min;
    }
    else
    {
        fLastOne = min;
        fLastButOne = max;
    }
     //补吸气量(根据波形是倒数第二个绝对值最大的量(若当前量是波峰，则减去 波峰累加和/波形个数，反之))
    //s2 = abs(max - fSubMax/nTVCount);
    //倒数第二大于最后一个即为波峰，反之
    fTemp = fLastButOne > fLastOne ? fSubMax : fSubMin;
    s2 = abs(static_cast<double>(fLastButOne) - getValueOfDivision(fTemp, nTVCount));

    //补呼气量（根据波形是倒数第一个绝对值最大的量(若当前量是波峰，则减去 波峰累加和/波形个数，反之)）
    //s3 = abs(min - fSubMin/nTVCount);
    fTemp = fLastOne > fLastButOne ? fSubMax : fSubMin;
    s3 = abs(static_cast<double>(fLastOne) - getValueOfDivision(fTemp, nTVCount));

    ///2.6 ---------------- 通过lung结果传出所有肺活量计算的数值. -------------
    //预测肺活量

    //肺活量
    targetRes.VC = s1 + s2 + s3;
    //潮气量
    targetRes.TV = s1;
    targetRes.IRV = s2;
    targetRes.ERV = s3;

    //计算出肺活量之后，将肺活量设置到实验者信息当中,在最大通气量中计算气速指数时会用到
    ulong lFirstPos, lSecondPos;
    float fFirstValue, fSecondValue;

    //默认采用平均值测量的结果
    if(!getExtremeValue(fFirstValue, lFirstPos, fSecondValue, lSecondPos, lOutDataLength, pOutDataBuffer))
    {
        //这里是按照最后一次进行计算，s深吸气量为倒数第二个最大值减去上一个极值(第二个值)
        targetRes.IC		= static_cast<double>(abs(fLastButOne - fSecondValue));
        //深呼气量为最后一个最大值减去，倒数第二个最大值的上两个极值
        targetRes.EC		= static_cast<double>(abs(fLastOne - fFirstValue));
    }
    else
    {
        //呼气量、吸气量（深呼气量、深吸气量），按照公式进行计算时由与补吸气量和补呼气量采用的是平均值进行计算，数据
        //可能是不准的，现在采用最后一次正常呼吸，和最后一次深呼吸深吐气之间进行计算结果，这里是按照公式运算
        targetRes.IC		= s1 + s2;
        targetRes.EC		= s1 + s3;
    }

    //由于rv（残气量）预测公式没有确定，这三个量暂时不进行计算和显示
    //targetRes.TLC		= (s1 + s2 + s3) + targetRes.RV;
    //targetRes.FRC		= s3 + targetRes.RV;

    if(compareFloating(fAvePeriod, 0.0f) == 0)
    {
        delete [] pTempNewFlowOrVelocity;
        delete [] pOutDataBuffer;
        return false;
    }

    //targetRes.RR		= 60.0 * (1 / fAvePeriod);	/// 次/分
    targetRes.RR		= getAbsValueOfDivision(60.0, fAvePeriod / fSampleRate); //cpm

    targetRes.TVM		= targetRes.TV * targetRes.RR;

    targetRes.VE		= s1 * targetRes.RR;

    //va没有体重不能计算
    //targetRes.VA		= (targetRes.TV - 2.2 * targetRest.weight) * targetRes.RR;

    delete [] pTempNewFlowOrVelocity;
    delete [] pOutDataBuffer;
    return true;
}

bool CAnalysisLungFunc::anlsForceVitalCapacity(float *pSrcData, const unsigned long &lSrcDataLen, const float &fSampleRate, CAnlsResultFVC &TargetRes)
{
    float *pVelocity = nullptr, *pFlow = nullptr;
    float *pTempNewFlowOrVelocity = new float[lSrcDataLen];

    if(m_shDataType == Data_Velocity)
    {
        pVelocity = pSrcData;
        pFlow = pTempNewFlowOrVelocity;
    }
    else
    {
        pFlow = pSrcData;
        pVelocity = pTempNewFlowOrVelocity;
    }

    if(pVelocity == nullptr || pFlow == nullptr)
    {
        delete [] pTempNewFlowOrVelocity;
        return false;
    }

    //获取流量和流速数据
    getDataVelocityAndFlow(pVelocity, pFlow, m_shDataType, lSrcDataLen, fSampleRate);

    //获取满足用力肺活量的波形
    vector<ulong> vecWavePos;
    getFVCWavePos(lSrcDataLen, pVelocity, pFlow, fSampleRate, vecWavePos);

    unsigned int nCount = static_cast<unsigned int>(vecWavePos.size() / 2);

    if(nCount < 1)
        return false;

    for(unsigned int i = 0; i < nCount * 2 - 1; i += 2)
    {
        getFvcResult(vecWavePos[i], vecWavePos[i + 1], pVelocity, pFlow, fSampleRate, TargetRes);
        //将此结果存放到所有结果当中,并且更改头信息
    }

    delete [] pTempNewFlowOrVelocity;

    return vecWavePos.size() > 1;
}

void CAnalysisLungFunc::getFvcResult(ulong lStartPos, ulong lEndPos, float *pVelocityData, float *pFlowData, const float fSampleRate, CAnlsResultFVC &targetRes)
{
    //获取当前流速段最大值
    float fdmax_dt = 0;
    for(ulong i = lStartPos; i <= lEndPos; i++)
    {
        if(abs(pVelocityData[i]) > fdmax_dt)
        {
            fdmax_dt = abs(pVelocityData[i]);
        }
    }
    fdmax_dt = fdmax_dt / 60; //转为L/s

    float *p = pFlowData;

    ///2.4 ----------------  1秒率. -----------------------------------------
    float fCurPosValue,s,s1		= 0.0;
    unsigned int iSampePoints = static_cast<unsigned int>(fSampleRate*1.0f);	/// 1s的采样数据
    float fStartValue = p[lStartPos];

    s = abs(pFlowData[lEndPos] - pFlowData[lStartPos]);
    if( lStartPos + iSampePoints >= lEndPos )					/// 1秒已经呼气完毕
    {
        targetRes.FEV1 = static_cast<double>(s);
        targetRes.FEV1R = 100;
    }
    else
    {
        fCurPosValue = *(p + lStartPos + iSampePoints);
        s1 = abs(fCurPosValue - fStartValue);

        targetRes.FEV1 = static_cast<double>(s1);
        targetRes.FEV1R = getValueOfDivision(targetRes.FEV1 * 100, s);;
    }

    ///2.5 -----------------  2秒率. -----------------------------------------
    s1		= 0.0;
    iSampePoints = static_cast<unsigned int>(fSampleRate * 2.0f);
    if(lStartPos + iSampePoints >= lEndPos)	// 2秒已经呼气完毕。
    {
        targetRes.FEV2 = static_cast<double>(s);
        targetRes.FEV2R = 100;
    }
    else
    {
        fCurPosValue = *(p + lStartPos + iSampePoints);
        s1=	abs(fCurPosValue - fStartValue);

        targetRes.FEV2 = static_cast<double>(s1);
        targetRes.FEV2R = getValueOfDivision(targetRes.FEV2 * 100, s);
    }
    ///2.6 -----------------  3秒率. -----------------------------------------
    s1		= 0.0;
    iSampePoints = static_cast<unsigned int>(fSampleRate * 3.0f);
    if(lStartPos + iSampePoints >= lEndPos)	// 3秒已经呼气完毕。
    {
        targetRes.FEV3 = static_cast<double>(s);
        targetRes.FEV3R = 100;
    }
    else
    {
        fCurPosValue = *(p + lStartPos + iSampePoints);
        s1=	abs(fCurPosValue - fStartValue);

        targetRes.FEV3 = static_cast<double>(s1);
        targetRes.FEV3R = getValueOfDivision(targetRes.FEV3 * 100, s);
    }

    targetRes.FVC	= static_cast<double>(s);
    targetRes.MEFR = static_cast<double>(fdmax_dt * fSampleRate);// 最大呼气流量率，单位：L/s
    targetRes.EXTIME = getAbsValueOfDivision((lEndPos - lStartPos), fSampleRate);  	// 呼气时间

    /// 最大呼气中间流量取整体正常波形中间一般波形进行计算
    int nQuarter = ( lEndPos - lStartPos )/4;
    s1   = 0.0;

    //取端点值还是平均值
    fCurPosValue = *(p + lStartPos + nQuarter);          //  1/4处的值
    s1           = *(p + lStartPos + 3 * nQuarter);      //  3/4处的值
    s1 = abs(fCurPosValue - s1) - fStartValue;

    /*float fValue = Average(nQuarter * 2, p + lStartPos + nQuarter);
    s1 = fValue - fStartValue;*/

    //中间两段的容积 / 所占用的时间
    targetRes.MMEF	= getValueOfDivision(s1, targetRes.EXTIME / 2);
}

bool CAnalysisLungFunc::anlsMaxPulmonaryVentilation(float *pSrcData, const unsigned long &lSrcDataLen, const float &fSampleRate, CAnlsResultMPV &TargetRes)
{
    float *pVelocity = nullptr, *pFlow = nullptr;
    float *pTempNewFlowOrVelocity = new float[lSrcDataLen];

    if(m_shDataType == Data_Velocity)
    {
        pVelocity = pSrcData;
        pFlow = pTempNewFlowOrVelocity;
    }
    else
    {
        pFlow = pSrcData;
        pVelocity = pTempNewFlowOrVelocity;
    }

    if(pVelocity == nullptr || pFlow == nullptr)
    {
        delete [] pTempNewFlowOrVelocity;
        return false;
    }

    getDataVelocityAndFlow(pVelocity, pFlow, m_shDataType, lSrcDataLen, fSampleRate);

    ///2.------------------ 开始进行肺活量分析. -------------------------------
    ///2.1 ---------------- 准备分析数据. -------------------------------------
    float max, min;
    ulong max_pos, min_pos;
    CAlgorithm::getMaxMin(max, max_pos, min, min_pos, lSrcDataLen, pFlow);

    ///2.2 ---------------- 获取该段数据的所有完整（最大25个）. ---------------
    float  fTemp = 0;
    float *pOutDataBuffer = new float[lSrcDataLen];
    memset(pOutDataBuffer, 0, lSrcDataLen * sizeof(float));

    map<ulong, ulong> mapMapping;
    ulong lOutDataLength = fliterEqualData(pFlow, pOutDataBuffer, lSrcDataLen, mapMapping);
    auto itFind = mapMapping.begin();
    vector<ulong> vecTime;
    vector<float> vecStand;

    ///2.2 ---------------- 获取该段数据的所有完整连续波形. ---------------
    //新的获取波形函数一个顶点方向在下的余弦波为一个完整波形(总共波形包含正常呼吸和最大通气量呼吸)
    map<ulong, vector<R_WAVE> >mapContinueWave;
    getWave(lOutDataLength, pOutDataBuffer, fSampleRate, mapContinueWave, mapMapping);

    if(mapContinueWave.size() == 0)
    {
        delete [] pOutDataBuffer;
        delete [] pTempNewFlowOrVelocity;
        return false;
    }

    //选出其中最长的一个波形，或者是取其中最后一个波形，或者是将波形进行平均值计算
    //现在默认取出其中最后一个波形
    vector<R_WAVE> &vecValideWave = mapContinueWave.rbegin()->second;
    unsigned int waveCount = static_cast<unsigned int>(vecValideWave.size());

    if( waveCount == 0)
    {
        delete [] pOutDataBuffer;
        delete [] pTempNewFlowOrVelocity;
        return false;
    }


    //这里通过波形时长进行分类，做到刚好将连续波形分为两类
    //一类是正常呼吸，一类是快速呼吸，通过标准差进行划分，体现数据离散程度
    vecTime.clear();
    for(unsigned int i = 0; i < vecValideWave.size(); i++)
    {
        vecTime.push_back(vecValideWave[i].epos - vecValideWave[i].spos);
    }

    unsigned int nTemp = 0;
    bool bReplace;
    float fMin = getStandardDeviation(vecTime, 0);

    for(unsigned int i = 1; i < vecValideWave.size() - 1; i++)
    {
        fTemp = getStandardDeviation(vecTime, i);
        vecStand.push_back(fTemp);

        bReplace = fTemp < fMin;
        if(bReplace)
        {
            fMin = fTemp;
            nTemp = i;
        }
    }
    //由于计算标准差函数内部计算，这个分割点+1才是真正的另一个点起始位置
    nTemp++;

    //表示临界波形位置, 获取方差最小值位置，不在首位置或者末尾位置则代表是待测波形
    unsigned int nCriticalindex = nTemp;
    ///--- 这里计算的最佳结果时掐头去尾之后的结果，若数量满足需求则去头去尾，不满足则全部进行计算 ---
    unsigned int nStart, nEnd;

    //先计算正常波形
    unsigned int nNormalWaveCount = 0;
    float fSubTidalVolumeOfNormal = 0.0;

    if(nCriticalindex > 2)
    {
        nNormalWaveCount = nCriticalindex - 2;
        nStart = 1;
        nEnd = nCriticalindex - 1;
    }
    else
    {
        nNormalWaveCount = nCriticalindex;
        nStart = 0;
        nEnd = nCriticalindex;
    }

    for(unsigned int i = nStart; i < nEnd; i++)
    {
        //当前总潮气量 每一次的潮气量之差
        fSubTidalVolumeOfNormal += abs(vecValideWave[i].evalue - vecValideWave[i].tvalue);
    }

    //正常呼吸时长
    itFind = mapMapping.find(vecValideWave[nStart].spos);
    nStart = itFind->second;
    itFind = mapMapping.find(vecValideWave[nEnd - 1].epos);
    nEnd   = itFind->second;

    float fSumPeriod = nEnd - nStart;
    //每个周期所占的平均采样点数
    float fAvePeriodNormal = fSumPeriod / nNormalWaveCount;

    //再计算最大通气量的波形
    unsigned int nMaxWaveCount = 0;
    float fSubTidalVolumeOfMax = 0.0;

    if( waveCount - nCriticalindex > 2 )
    {
        nMaxWaveCount = waveCount - nCriticalindex - 2;
        nStart = nCriticalindex + 1;
        nEnd = waveCount - 1;
    }
    else
    {
        nMaxWaveCount = nCriticalindex;
        nStart = nCriticalindex;
        nEnd = waveCount;
    }

    for(unsigned int i = nStart; i < nEnd; i++)
    {
        //当前总潮气量 每一次的潮气量之差
        fSubTidalVolumeOfMax += abs(vecValideWave[i].evalue - vecValideWave[i].tvalue) /*+ abs(vecValideWave[i].s - vecValideWave[i].tvalue) / 2*/;
    }

    //正常呼吸时长
    itFind = mapMapping.find(vecValideWave[nStart].spos);
    nStart = itFind->second;
    itFind = mapMapping.find(vecValideWave[nEnd - 1].epos);
    nEnd   = itFind->second;

    fSumPeriod = nEnd - nStart;
    //每个周期所占的平均采样点数
    float fAvePeriodMax = fSumPeriod / nMaxWaveCount;

    /////2.5 ---------------- 计算最大肺通气量. ---------------------------------	/// MVV 测量计数
    //呼吸频率 单位 cpm 每分钟呼吸次数
    float fRRNormal = 60.0f / (fAvePeriodNormal / fSampleRate);
    float fRRMax = 60.0f / (fAvePeriodMax / fSampleRate);
    TargetRes.RR = static_cast<double>(fRRMax); //cpm

    //最大通气量 = 每次潮气量 * RR
    TargetRes.MVV = static_cast<double>(fRRMax) * getValueOfDivision(fSubTidalVolumeOfMax, nMaxWaveCount);

    //每分平静通气量 = 每次潮气量 * RR
    TargetRes.VE = static_cast<double>(fRRNormal) * getValueOfDivision(fSubTidalVolumeOfNormal, nNormalWaveCount);

    //通气贮量百分比 = （最大通气量 - 没分平静通气量）/ 最大通气量 * 100%
    TargetRes.VSP = (TargetRes.MVV - TargetRes.VE) / TargetRes.MVV * 100;

    ///------------- 气速指数 -----------------------------------------------------------
    //将本次计算的肺活量和预计肺活量 记录下来
    //TargetRes.VCP = CExperimenterBaseInfo::Get_Instance().Get_VCP();
    //TargetRes.VC  = CExperimenterBaseInfo::Get_Instance().Get_VC();

    //TargetRes.BSA = CExperimenterBaseInfo::Get_Instance().Get_BSA();
    //TargetRes.MVVP = CExperimenterBaseInfo::Get_Instance().Get_MVVP();

    //气速指数需要预计最大肺通气量
//    if(TargetRes.VC != 0 && TargetRes.VCP != 0)
//    {
//        TargetRes.AVI = (TargetRes.MVV / TargetRes.MVVP) / (TargetRes.VC / TargetRes.VCP);
//    }
//    else
//    {
//        TargetRes.AVI = 0;
//    }

    delete []pOutDataBuffer;
    delete []pTempNewFlowOrVelocity;
    return true;
}

void CAnalysisLungFunc::getDataVelocityAndFlow(float *pVelocityData, float *pFlowData, short shDataType, const unsigned long &lSrcDataLen, float fSampleRate, bool bHaveLastData, float fLastVelocity, float fLastFlow)
{
    if(pVelocityData == nullptr || pFlowData == nullptr || lSrcDataLen == 0)
        return;

    //若没有上一次数据，那么将第一个数据置为0，同时从第二个数据开始进行获取数据

    float fTemp;
    unsigned long i = bHaveLastData ? 0 : 1;

    switch(shDataType){
    case Data_Velocity:
    {
        float fSum = bHaveLastData ? fLastFlow : 0.0f;
        pFlowData[0] = 0.0f;
        //将数据处理为流量数据 这里流量单位为 ml
        for(; i < lSrcDataLen; i++)
        {
            //两点积分面积 保证x值差值为1个数据点 即可
            if(i != 0)
            {
                fTemp = CAlgorithm::integralTwoPt(static_cast<float>((i - 1)), static_cast<float>(i), pVelocityData[i - 1], pVelocityData[i]);
            }
            else
            {
                fTemp = CAlgorithm::integralTwoPt(static_cast<float>((i)), static_cast<float>(i + 1), fLastVelocity, pVelocityData[i]);
            }


            //将面积转为ml
            fTemp = fTemp / 60 / fSampleRate * 1000;
            fSum += fTemp;
            pFlowData[i] = fSum;
        }
    }
        break;
    case Data_Flow:
    {
        //将数据处理为流速数据,原始数据单位为 ml，这里统一单位为 L / min
        pVelocityData[0] = 0.0f;
        for(; i < lSrcDataLen; i++)
        {
            //计算每个点的流量差别 / 时间即为流速，然后换算为对应单位即可
            if(i != 0)
            {
                fTemp = pFlowData[i] - pFlowData[i - 1];
            }
            else
            {
                fTemp = pFlowData[i] - fLastFlow;
            }

            //转为 ml / s  改变量 / 改变时间
            fTemp = fTemp / (1 / fSampleRate);

            //转为 ml / min
            fTemp *= 60;

            //转为 L / min
            fTemp /= 1000;

            pVelocityData[i] = fTemp;
        }
    }
        break;
    default:
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// <函    数>	Fliter_Equal_Raw_Data
/// <功    能>
///				除去波形中的等值数据，便于提取有效波形
///	</功   能>
/// <参    数>
///				pInDataBuffer	-> 传入数据
///				pOutDataBuffer	-> 输出到的位置，需要提前就申请好空间
///				lSrcDataLen		-> 传入数据长度
///             mapMapping		-> 保存原始数据与滤除相同值之后的数据间的映射关系
///	</参   数>
/// <返 回 值>
///				返回有效数据数量
///</返 回 值>
/// <说    明>
///				若有很多相同的值，则取最后一个值，（以前取得前面的值）要不在处理最大呼气流量容积曲线时会出错
///</说   明>
/// <作  者> 		chenlei 	 				</作	者>
/// <创建日期>		2020-06-11  </创建日期>
/// <修	   改>                              </修     改>
/// </函   数>
///////////////////////////////////////////////////////////////////////////////
unsigned long CAnalysisLungFunc::fliterEqualData(float *pInDataBuffer, float *pOutDataBuffer, unsigned long lSrcDataLen, map<unsigned long, unsigned long> &mapMapping)
{
    unsigned long lFilterNum = 0;

    //在滤波之后数据长度整体比原始数据短了很多，这里建立一个对应关系，在后续实际进行测量时
    //能准确获取对应的数据长度
    mapMapping.clear();
    float fEqualData = 0.00001f;

    for ( unsigned long i = 0, z = 0; i < lSrcDataLen - 1; i++, z++)
    {
        if (fabs(pInDataBuffer[i] - pInDataBuffer[i + 1]) > fEqualData)
        {
            pOutDataBuffer[z] = pInDataBuffer[i];
            mapMapping[z] = i;
        }
        else
        {
            for (unsigned long  j = i + 1; j < lSrcDataLen - 1; j++)
            {
                if (fabs(pInDataBuffer[j] - pInDataBuffer[j + 1]) > fEqualData)
                {
                    pOutDataBuffer[z] = pInDataBuffer[j];
                    mapMapping[z] = i;

                    i = j;

                    break;
                }
            }
        }
    }

    lFilterNum = static_cast<unsigned long>(mapMapping.size());
    return lFilterNum;
}

void CAnalysisLungFunc::getFVCWavePos(const ulong &lNum, float *pVelocityData, float *pFlowData, float fSampleRate, vector<ulong> &vecWavePos)
{
    vecWavePos.clear();

    //这里找波形根据呼吸流量和呼吸流速共同进行查找,这里不考虑过多变化，只要流速变化为零就说明结束
    vector<ulong> vecTurningPt; //存储拐点,流量波形拐点即流速波形零点

    //获取零点位置
    getVelocityDataZeroPos(pVelocityData, lNum, vecTurningPt);

    //处理波形
    dealWave(pVelocityData, pFlowData, lNum, fSampleRate, vecTurningPt);

    // 用力肺活量界限值
    float fFVC_Threshold_Value = VC_BREATHE_THRESHOLD;

    //获取当前半个呼吸过程的个数
    ulong lCount = static_cast<ulong>(vecTurningPt.size());
    if(lCount < 2)
        return;

    unsigned int nMinValueIndex = 0;
    float fTemp;
    ulong lTemp;

    //若两个拐点间的面积大于某个值，同时持续时间应该大于0.8s，则说明这一段是可以进行测量时间肺活量波形
    unsigned int n1SptNum = static_cast<unsigned int>(0.8f * fSampleRate);
    float fMinValue = abs(pFlowData[vecTurningPt[1]] - pFlowData[vecTurningPt[0]]); //肺活量不会超过 19999ml

    bool bReplaceMinInfo;
    ulong i;
    for(i = 2; i < lCount; i += 2)
    {
        fTemp = abs(pFlowData[vecTurningPt[i + 1]] - pFlowData[vecTurningPt[i]]);
        lTemp = vecTurningPt[i + 1] - vecTurningPt[i];
        //若当前半个呼吸过程大于阈值，则说明可以用来当做分析用力肺活量
        if(fTemp >= fFVC_Threshold_Value && lTemp >= n1SptNum)
        {
            //记录下最小值位置(第一个值作为基准)
            bReplaceMinInfo = fTemp < fMinValue;
            if(bReplaceMinInfo)
            {
                fMinValue = fTemp;
                nMinValueIndex = i;
            }

            //记录下符合要求的位置
            vecWavePos.push_back(vecTurningPt[i]);
            vecWavePos.push_back(vecTurningPt[i + 1]);
        }
    }

    //对于用力肺活量来说或者肺活量测量来说，呼气流量总是比吸气流量大排除其中的吸气流量，排除掉其中的吸气流程
    if(vecWavePos.size() == 0)
        return;

    lCount = static_cast<ulong>(vecWavePos.size());
    fMinValue = pFlowData[vecTurningPt[nMinValueIndex + 1]] - pFlowData[vecTurningPt[nMinValueIndex]];
    vecTurningPt.clear();
    for(i = 0; i < lCount; i += 2)
    {
        fTemp = pFlowData[vecWavePos[i + 1]] - pFlowData[vecWavePos[i]];

        //呼气流程
        if(fTemp * fMinValue < 0)
        {
            vecTurningPt.push_back(vecWavePos[i]);
            vecTurningPt.push_back(vecWavePos[i + 1]);
        }
    }

    //若找到最后一个，还未发现有波形，那么就全部是可以用于用力肺活量测量的波形
    if(vecTurningPt.size() != 0)
    {
        vecWavePos.clear();
        vecWavePos = vecTurningPt;
        vecTurningPt.clear();
    }


    //此时 vecWavePos 中存储的就是用力肺活量的全部波形，但是由于用力肺活量进行实验时拿动呼吸传感器，会导致波形前端
    //有一段无效波形，这里进行处理，重新确定起点
    lCount = static_cast<ulong>(vecWavePos.size());

    ulong lStartPos, lEndPos;
    ulong lSize;
    ulong j = 0;

    for(i = 0; i < lCount; i += 2)
    {
        lStartPos = vecWavePos[i];
        lEndPos = vecWavePos[i + 1];

        //再一次找到零点，去掉前面所有不满足要求的 呼气过程或者吸气过程
        getVelocityDataZeroPos(pVelocityData + lStartPos, lEndPos - lStartPos + 1, vecTurningPt);

        lSize = static_cast<ulong>(vecTurningPt.size());
        for(j = 0; j < lSize; j += 2)
        {
            fTemp = pFlowData[vecTurningPt[j + 1]  + lStartPos ] - pFlowData[vecTurningPt[j]  + lStartPos ];

            //不正常过程
            if(abs(fTemp) >= WRONG_BREATHE_THRESHOLD)
            {
                fTemp = pFlowData[vecTurningPt[j]  + lStartPos];
                break;
            }

            deleteZeroPosByIndex(vecTurningPt, j + 1);
            lSize = deleteZeroPosByIndex(vecTurningPt, j);
            j -= 2;
        }

        if(lSize == 0)
        {
            //说明当前点不满足需求，去掉即可
            deleteZeroPosByIndex(vecWavePos, i + 1);
            lCount = deleteZeroPosByIndex(vecWavePos, i);
        }
        else
        {
            vecWavePos[i] = vecTurningPt[0]  + lStartPos;
        }
    }
}

void CAnalysisLungFunc::getVelocityDataZeroPos(float *pVelocityData, ulong lNum, vector<ulong> &vecZeroPos)
{
    vecZeroPos.clear();

    if(lNum <= 0 || pVelocityData == nullptr)
        return;

    float fTemp;
    float fLastValue = pVelocityData[0];
    ulong lTemp, lLastZero;

    //是否是零点，是否是奇数个数
    bool bZero, bOddNum;

    //每两个点是一个呼吸过程，吸气或者呼气
    for(ulong i = 0; i < lNum; i++)
    {
        fTemp = pVelocityData[i];
        bZero = isZeroValue(fTemp);

        bOddNum = vecZeroPos.size() % 2 != 0;

        //若当前数据是零点或者当前数据和上一个数据之间存在零点
        if(fLastValue * pVelocityData[i] < 0 || bZero)
        {
            //将靠零近的点作为拐点
            if(i != 0)
            {
                lTemp = abs(fLastValue) > abs(pVelocityData[i]) ? i : i - 1;
            }
            else
            {
                lTemp = 0;
            }


            //若数据是奇数个数则存入当前零点位置，若是偶数则更新当前零点位置
            if(bOddNum)
            {
                //半个呼吸过程结束点
                vecZeroPos.push_back(lTemp);
            }

            //更新零点位置
            lLastZero = lTemp;
        }
        else
        {
            if(!bOddNum)
            {
                //当前数据是起点
                vecZeroPos.push_back(i != 0 ? lLastZero : i);
            }
        }

        fLastValue = pVelocityData[i];
    }

    //若点个数为奇数，则将最后一个点作为结尾点,保证是偶数个数
    bOddNum = vecZeroPos.size() % 2 != 0;

    if(bOddNum)
    {
        //半个呼吸过程结束点
        vecZeroPos.push_back(lNum - 1);
    }
}

unsigned int CAnalysisLungFunc::deleteZeroPosByIndex(vector<ulong> &vecZeroPos, ulong nIndex)
{
    unsigned int nSize = static_cast<unsigned int>(vecZeroPos.size());
    auto itBegin = vecZeroPos.begin();
    auto itEnd = vecZeroPos.end();

    auto it = itBegin + nIndex;

    if(it >= itBegin && it < itEnd)
    {
        vecZeroPos.erase(it);
        nSize -= 1;
    }

    return nSize;
}

bool CAnalysisLungFunc::isZeroValue(const float &fValue)
{
    //一般float型只能精确到小数到后六位
    float fMinRange = -0.000001f;
    float fMaxRange = 0.000001f;

    return (fValue >= fMinRange && fValue <= fMaxRange);
}

void CAnalysisLungFunc::dealWave(float *pVelocityData, float *pFlowData, const ulong &lNum, float fSampleRate, vector<ulong> &vecZeroPos)
{
    if(pVelocityData == nullptr || pFlowData == nullptr || lNum == 0)
        return;

    //结束点和起点位置相差的点数即可
    vector<ulong> vecWavePos;
    ulong lSize = static_cast<ulong>(vecZeroPos.size());
    int nThresholdPtNum = static_cast<int>(fSampleRate * 0.1f);
    ulong lCurEnd, lNexStart;
    float fTemp1, fTemp2;

    //先合并连续的同方向的过程,使下一步波形能更好的进行处理，处理完后全是正反过程相间
    ulong i;
    for(i = 0; i < lSize; i += 2)
    {
        //当前过程结尾位置
        lCurEnd = (vecZeroPos[i + 1]);

        //下一过程起始位置
        if(i + 2 < lSize)
        {
            lNexStart = ( vecZeroPos[i + 2] );
        }
        else
        {
            //当前位置已经是结尾位置
            continue;
        }

        if(lNexStart - lCurEnd <= static_cast<ulong>(nThresholdPtNum))
        {
            //代表和下一个连续，判断方向一致就进行合并
            fTemp1 = pFlowData[vecZeroPos[i + 1]] - pFlowData[vecZeroPos[i]]; //当前流量
            fTemp2 = pFlowData[vecZeroPos[i + 3]] - pFlowData[vecZeroPos[i + 2]]; //下一个流量

            if(fTemp2 * fTemp1 > 0)
            {
                //方向一致，进行合并

                //将下一过程结束点作为当前结束点
                vecZeroPos[i + 1] = vecZeroPos[i + 3];

                deleteZeroPosByIndex(vecZeroPos, i + 3);
                lSize = deleteZeroPosByIndex(vecZeroPos, i + 2);

                //保持当前位置不变
                i -= 2;
            }
            else
            {
                //方向不一致
                continue;
            }
        }
        else
        {
            //和下一个不连续
            continue;
        }
    }


    //获取一个完整的呼吸流程呼气或者吸气过程（这里的过程指呼气或者吸气，呼吸过程才是一个完整的呼吸波）：
    //1.首先取起始波，判断下一个是否和当前波形连续 这里规定相邻0.1s的范围的波形为连续波形，若不连续，则当前起止位置为完整的过程；
    //2.若连续 则判断下一个 波形方向是否和当前一致若一致，直接融合下一个点，
    //3.若不一致，则需要判断当前过程的 下一个过程和下下个过程是否是连续的，若不连续，则这次就是一个单独过程完结
    //4.若连续，首先判断 下一个过程和下下个过程是否能组成一个正常的 呼吸流程（两个过程有一个 气量差少于 200即为不正常），若波形
    //正常，则当前是一个完整波，若不正常情况下，判断下下过程结尾位置是否绝对值比当前结尾大 将下下个过程终点作为当前终点，以此循环下去找到最后一个过程
    //一个过程两个点
    lSize = static_cast<unsigned long>(vecZeroPos.size());
    for(i = 0; i < lSize; i += 2)
    {
        //这里i进来之后就是一个过程的起点

        //当前过程结尾位置
        lCurEnd = (vecZeroPos[i + 1]);

        //下一过程起始位置
        if(i + 2 > lSize - 1)
        {
            //当前位置已经是结尾位置
            continue;

        }

        lNexStart = ( vecZeroPos[i + 2] );

        if(lNexStart - lCurEnd <= static_cast<ulong>(nThresholdPtNum))
        {
            //和下一过程连续
            fTemp1 = pFlowData[vecZeroPos[i + 1]] - pFlowData[vecZeroPos[i]]; //获取当前过程方向
            fTemp2 = pFlowData[vecZeroPos[i + 3]] - pFlowData[vecZeroPos[i + 2]];//获取下一个过程方向,

            if(fTemp1 * fTemp2 > 0)
            {
                //和下一过程方向相同，直接和下一过程合并，上一部分进行了合并不需要处理
                //不知道为什么还存在漏网之鱼，继续处理

                //将下一过程结束点作为当前结束点
                vecZeroPos[i + 1] = vecZeroPos[i + 3];

                deleteZeroPosByIndex(vecZeroPos, i + 3);
                lSize = deleteZeroPosByIndex(vecZeroPos, i + 2);

                //保持当前位置不变
                i -= 2;
                continue;
            }
            else
            {
                //这里和下一过程方向相反

                //判断下下过程在不在，不在的话当前就是一个完整过程
                if(i + 4 > lSize - 1)
                {
                    continue;
                }

                //和下一过程方向相反，需要判断当前过程的下一个呼吸过程 当前呼吸过程为 0， 1 下一个为 2 3 下下个为 4， 5
                fTemp1 = fTemp2; //下一个过程值
                fTemp2 = pFlowData[vecZeroPos[i + 5]] - pFlowData[vecZeroPos[i + 4]]; //下下过程值

                if( abs(fTemp1) >= 200 && abs(fTemp2) >= 200)
                {
                    //呼吸过程正常,是正常呼吸波
                    //continue;
                }
                else
                {
                    bool bContainBreathingProcess = false;
                    //呼吸过程异常
                    fTemp1 = pFlowData[vecZeroPos[i + 1]] - pFlowData[vecZeroPos[i]]; // 当前过程气体量
                    fTemp2 = pFlowData[vecZeroPos[i + 5]] - pFlowData[vecZeroPos[i]]; // 合并起来之后的气体量

                    //若下一个呼吸过程合并到当前过程之后，并不能对当前过程的值有所增益，则当前过程是一个完整的过程、
                    if(fTemp1 * fTemp2 >= 0)
                    {
                        //同号判断是否有增益
                        if(abs(fTemp1) < abs(fTemp2))
                        {
                            bContainBreathingProcess = true;
                        }
                    }
                    else
                    {
                        bContainBreathingProcess = false;
                    }

                    if(bContainBreathingProcess)
                    {
                        //证明是一个过程，进行合并
                        vecZeroPos[i + 1] = vecZeroPos[i + 5];

                        //删除下下过程和下个过程
                        deleteZeroPosByIndex(vecZeroPos, i + 5);
                        deleteZeroPosByIndex(vecZeroPos, i + 4);
                        deleteZeroPosByIndex(vecZeroPos, i + 3);
                        lSize = deleteZeroPosByIndex(vecZeroPos, i + 2);

                        //保持当前波形位置
                        i -= 2;
                        continue;
                    }
                    else
                    {
                        //当前过程完成
                        continue;
                    }
                }

            }
        }
        else
        {
            //和下一过程不连续
            continue;
        }
    }
}

int CAnalysisLungFunc::getWave(const ulong &lSrcLen, float *pData, float fSampleRate, map<ulong, vector<R_WAVE> > &mapContinueWave, const map<ulong, ulong> &mapMapping)
{
    vector<R_WAVE> vecWave;
    R_WAVE waveR;

    size_t szWaveSize = 0;
    bool bUp_Flag = false;
    float fDifStart = 0.0f, fDifMid = 0.0f, fDifEnd = 0.0f;
    ulong lTPos = 0;
    float fTValue = 0.0;

    //无效呼吸波形，其中吸气或者呼气只要有不大于当前量的都视为无效呼吸波
    float fMinimumGasVolume = NORMAL_BREATHE_THRESHOLD;

    //找到所有波形
    ulong i = 0;
    for(i = 1; i < lSrcLen; i++)
    {
        fDifStart = *(pData + (i - 1));
        fDifMid = *(pData + i);
        fDifEnd = *(pData + (i + 1));

        if ((fDifMid >= fDifStart) && (fDifMid >= fDifEnd))   //波峰
        {
            bUp_Flag = true;
            lTPos = i;
            fTValue = fDifMid;
            continue;
        }

        if (bUp_Flag && (fDifMid <= fDifStart) && (fDifMid <= fDifEnd)) //波谷
        {
            bUp_Flag = false;

            waveR.spos = lTPos;
            waveR.svalue = fTValue;
            //将波谷认为是R顶点
            waveR.tpos = i;
            waveR.tvalue = fDifMid;

            vecWave.push_back(waveR);

            //保存一个完整的波形
            szWaveSize = vecWave.size();
            if(szWaveSize >= 2)
            {
                vecWave[szWaveSize - 2].epos = waveR.spos - 1;
                vecWave[szWaveSize - 2].evalue = *(pData + vecWave[szWaveSize - 2].epos);
            }
        }
    }

    //去掉最后一个不完整的波形，这里按照 波峰 波谷 波峰为一个完整的波形,可能会去掉前后一个波，取平均值影响不大
    if(szWaveSize > 0)
    {
        auto it = vecWave.end();
        it--;
        vecWave.erase(it);
        szWaveSize--;
    }

    //去掉其中不连续的少数波形，最终只会取其中一部分连续波形较多的波形
    map<ulong, vector<ulong>> mapContinueWaveIndex;
    vector<ulong> vecTemp;

    //当前波形是经过一次相等值滤除之后的波形，所以这里的查找是一定存在的
    auto itFind = mapMapping.begin();
    auto itFindContinueWave = mapContinueWaveIndex.begin();
    ulong lTemp;

    //规定连续波形的时间 s
    float fContinueWaveTime = 0.3f;
    unsigned int nCount = static_cast<unsigned int>(fSampleRate * fContinueWaveTime);

    for(i = 1; i < szWaveSize; i++)
    {
        vecTemp.clear();
        //波形连续，需要判断在原始波形数据中的位置是连续的(正常一个呼吸波形在2秒以上,现在认为在0.2秒中内的波形都是连续的)
        itFind = mapMapping.find(vecWave[i].spos);
        lTemp = itFind->second;

        itFind = mapMapping.find(vecWave[i - 1].epos);

        vecTemp.clear();

        if(getAbsValueOfSubtraction(lTemp, itFind->second) <= nCount)
        {
            itFindContinueWave = mapContinueWaveIndex.find(static_cast<ulong>(mapContinueWaveIndex.size() - 1));

            if(itFindContinueWave == mapContinueWaveIndex.end())
            {
                vecTemp.push_back(i -1);
                vecTemp.push_back(i);
                mapContinueWaveIndex[static_cast<ulong>(mapContinueWaveIndex.size())] = vecTemp;
            }
            else
            {
                itFindContinueWave->second.push_back(i);
            }
        }
        else
        {
            if(mapContinueWaveIndex.size() == 0)
            {
                vecTemp.push_back(i -1);
                mapContinueWaveIndex[static_cast<ulong>(mapContinueWaveIndex.size())] = vecTemp;
                vecTemp.clear();
                vecTemp.push_back(i);
                mapContinueWaveIndex[static_cast<ulong>(mapContinueWaveIndex.size())] = vecTemp;
            }
            else
            {
                vecTemp.push_back(i);
                mapContinueWaveIndex[static_cast<ulong>(mapContinueWaveIndex.size())] = vecTemp;
            }
        }
    }

    //先去掉一部分不符合需求的连续波形，需要将当前波形整个删除，通过峰峰值进行判断
    //正常人每次呼出吸入的气体量为500ml，这里定义平均呼出吸入的最低量为150ml，若连续的平均值少于这个值，则认为当前波形不满足需求
    float fTemp, fAverage, fSumValue = 0.0f;
    lTemp = static_cast<ulong>(mapContinueWaveIndex.size());

    for(i = 0; i < lTemp; i++)
    {
        itFindContinueWave = mapContinueWaveIndex.find(i);

        //一个连续波形索引，这里指向的是所有 vecWave 的下标索引
        vector<ulong> &vecContinueWaveIndex = itFindContinueWave->second;

        fSumValue = 0.0f;
        for(unsigned int j = 0; j < vecContinueWaveIndex.size(); j++)
        {
            R_WAVE &waveTemp = vecWave[vecContinueWaveIndex[j]];
            fTemp = abs(waveTemp.tvalue - waveTemp.svalue);
            fSumValue += fTemp;
            fTemp = abs(waveTemp.tvalue - waveTemp.evalue);
            fSumValue += fTemp;
        }

        fAverage = fSumValue / (vecContinueWaveIndex.size() * 2);

        //杂波混入有效波形之后影响并不大
        if(!(fAverage > fMinimumGasVolume))
        {
            mapContinueWaveIndex.erase(itFindContinueWave);
        }
    }

    //在清除掉无效的连续波形之后，需要将有效连续波形中的无效波形进行合并和删除（删除只能删除前面和后面的连续无效波形）
    //无效波形按照不大于  fEffectiveWave 进行划分
    bool fEffectiveWave;
    vector<ulong> vecDelete;
    for(auto it = mapContinueWaveIndex.begin(); it != mapContinueWaveIndex.end(); it++)
    {
        vector<ulong> &vecContinueWaveIndex = it->second;

        //删除无效波形
        lTemp = 0;
        vecTemp.clear();
        vecDelete.clear();

        for(ulong j = 0; j < vecContinueWaveIndex.size(); j++)
        {
            fEffectiveWave = true;
            R_WAVE &waveTemp = vecWave[vecContinueWaveIndex[j]];
            fTemp = abs(waveTemp.tvalue - waveTemp.svalue);

            //判断呼吸两个过程，其中一个无效就代表现在的波形无效
            if(!(fTemp > fMinimumGasVolume))
            {
                fEffectiveWave = false;
            }

            fSumValue += fTemp;
            fTemp = abs(waveTemp.tvalue - waveTemp.evalue);

            if(!(fTemp > fMinimumGasVolume))
            {
                fEffectiveWave = false;
            }

            if(!fEffectiveWave)
            {
                //从零开始的连续无效波形
                if(j - lTemp == 0)
                {
                    vecDelete.push_back(j);
                    lTemp = j;
                }
                //后续所有无效波形
                else
                {
                    //已经找到从头开始的无效波形这里进行删除，减去开头连续波形数量即为删除之后的正确索引
                    vecTemp.push_back(j - static_cast<ulong>(vecDelete.size()));
                    //break;
                }
            }
        }

        //这里删除的是指向原始波形的索引
        //vecDelete指向 的是 指向原始波形下标容器 的索引，直接删除即可
        for(unsigned int i = 0; i < vecDelete.size(); i++)
        {
            auto itDeleteContent = vecContinueWaveIndex.begin();
            vecContinueWaveIndex.erase(itDeleteContent);
        }

        //vecTemp和vecDelete指向的下标意义相同 删除从尾部开始的连续无效波形,倒置删除
        if(vecTemp.size() != 0)
        {
            if(vecTemp[vecTemp.size() - 1] == vecContinueWaveIndex.size() - 1)
            {
                lTemp = 1;
                for(unsigned int i = static_cast<unsigned int>(vecTemp.size()) - 1; i > 1 && i < vecTemp.size(); i--)
                {
                    if(vecTemp[i] - vecTemp[i - 1] == 1)
                    {
                        lTemp++;
                    }
                    else
                    {
                        break;
                    }
                }

                for(ulong i = 0; i < lTemp; i++)
                {
                    auto itDeleteContent = vecContinueWaveIndex.end();
                    itDeleteContent--;
                    vecContinueWaveIndex.erase(itDeleteContent);

                    auto itDeleteIndex = vecTemp.end();
                    itDeleteIndex--;
                    vecTemp.erase(itDeleteIndex);
                }
            }
        }

        //将剩余无效波形中的波形挑选出来，相邻的无效波形放到一起
        map<ulong, vector<ulong>> mapValue;
        vector<ulong> vecValue;
        for(unsigned int i = 0; i < vecTemp.size(); i++)
        {
            //向后查询即可
            vecValue.clear();
            for(unsigned int j = i; j < vecTemp.size(); j++)
            {
                vecValue.push_back(vecTemp[j]);

                if(j < (vecTemp.size()) - 1)
                {
                    if(getAbsValueOfSubtraction(vecContinueWaveIndex[vecTemp[j]], vecContinueWaveIndex[vecTemp[j + 1]]) != 1)
                    {
                        break;
                    }

                    vecValue.push_back(vecTemp[j + 1]);
                    j++;
                }
            }

            mapValue[static_cast<ulong>(mapValue.size())] = vecValue;

            i += static_cast<ulong>(vecValue.size()) - 1;
        }

        //将相邻 无效波形合为一个波形（认为多个无效波形合并之后是有效波形），将单独的波形按照起止点值大小融到附近波形当中,
        for(auto itrBegin = mapValue.begin(); itrBegin != mapValue.end(); itrBegin++)
        {
            //这里的指向代表指向 vecContinueWaveIndex 下标
            vector<ulong> &vecValue = itrBegin->second;

            if(vecValue.size() > 1)
            {
                //获取连续波形下标 vecContinueWaveIndex
                lTemp = vecValue[0];
                //获取原始波形下标 vecWave
                lTemp = vecContinueWaveIndex[lTemp];
                //获取原始波形
                R_WAVE &wave1 = vecWave[lTemp];

                R_WAVE &wave2 = vecWave[vecContinueWaveIndex[vecValue[vecValue.size() - 1]]];

                //--- 暂时将第一个波形和最后一个波形进行合并 ---
                wave1.epos = wave2.epos;
                wave1.evalue = wave2.evalue;
                //同时将中间值绝对值最大的作为中间点值

                //删除原始波形中后续的小波形，删除个数为 vecValue.size() - 1个
                lTemp = static_cast<ulong>(vecValue.size() - 1);

                while(lTemp)
                {
                    auto itDelete = vecWave.begin() + vecContinueWaveIndex[vecValue[lTemp]];
                    vecWave.erase(itDelete);
                    lTemp--;
                }

                //将所有连续波形从删除的序号开始进行降序处理
                descendingOrderProc(mapContinueWaveIndex, vecContinueWaveIndex[vecValue[vecValue.size() - 1]], static_cast<ulong>(vecValue.size() - 1));

                //同时将连续波形中的 无效连续波形也进行降序处理
                descendingOrderProc(mapValue, *(vecValue.begin()), static_cast<ulong>(vecValue.size()) - 1);
            }
            else
            {
                //判断当前波形的方向进行合并，这里已经剔除掉了首尾的波形，剩下的都是中间的无效波形
                lTemp = vecValue[0];
                R_WAVE &waveTempCur = vecWave[vecContinueWaveIndex[lTemp]];

                //合并到前一个波形
                if(waveTempCur.svalue < waveTempCur.evalue)
                {
                    R_WAVE &waveTempPre = vecWave[vecContinueWaveIndex[lTemp] - 1];

                    waveTempPre.epos = waveTempCur.epos;
                    waveTempPre.evalue = waveTempCur.evalue;
                    //采用中间值大的波形
                    if(abs(waveTempPre.tvalue) < abs(waveTempCur.tvalue))
                    {
                        waveTempPre.tpos = waveTempCur.tpos;
                        waveTempPre.tvalue = waveTempCur.tvalue;
                    }

                    //删除当前波形
                    auto itBegin = vecWave.begin();
                    itBegin += vecContinueWaveIndex[lTemp];
                    vecWave.erase(itBegin);

                    //将后续的波形进行降序处理，使对应的原始波形不会错位波形不会出错
                    descendingOrderProc(mapContinueWaveIndex, vecContinueWaveIndex[lTemp], 1);

                    //同时将连续波形中的 无效连续波形也进行降序处理
                    descendingOrderProc(mapValue, *(vecValue.begin()), static_cast<ulong>(vecValue.size()));
                }
                //（需要合并到后一个波形的情况）
                else
                {
                    R_WAVE &waveTempNext = vecWave[vecContinueWaveIndex[lTemp] + 1];

                    waveTempCur.epos = waveTempNext.epos;
                    waveTempCur.evalue = waveTempNext.evalue;

                    //采用中间值大的波形
                    if(abs(waveTempCur.tvalue) < abs(waveTempNext.tvalue))
                    {
                        waveTempCur.tpos = waveTempNext.tpos;
                        waveTempCur.tvalue = waveTempNext.tvalue;
                    }

                    //删除当前波形后一个波形
                    auto itBegin = vecWave.begin();
                    itBegin += vecContinueWaveIndex[lTemp] + 1;
                    vecWave.erase(itBegin);

                    //将后续的波形进行降序处理，使对应的原始波形不会错位波形不会出错
                    descendingOrderProc(mapContinueWaveIndex, vecContinueWaveIndex[lTemp] + 1, 1);

                    //同时将连续波形中的 无效连续波形也进行降序处理
                    descendingOrderProc(mapValue, *(vecValue.begin()), static_cast<ulong>(vecValue.size()));
                }
            }
        }
    }

    if(mapContinueWaveIndex.size() == 0)
    {
        return 0;
    }

    for(auto it = mapContinueWaveIndex.begin(); it != mapContinueWaveIndex.end(); it++)
    {
        vector<ulong> &ValideWaveIndex = it->second;
        vector<R_WAVE> vecValideWave;

        for(unsigned int i = 0; i < ValideWaveIndex.size(); i++)
        {
            vecValideWave.push_back(vecWave[ValideWaveIndex[i]]);
        }

        mapContinueWave[it->first] = vecValideWave;
    }

    //选出其中最长的一个波形，或者是取其中最后一个波形，或者是将波形进行平均值计算
    //现在默认取出其中最后一个波形
    vector<ulong> &ValideWaveIndex = mapContinueWaveIndex.rbegin()->second;

    //波形挑选完成，这里面按照最大肺活量的波形就还有两类波形，现在对波形进行分类，
    //这里按照角度还是波形时长进行分类还存在不确定
    return static_cast<int>(ValideWaveIndex.size());
}

void CAnalysisLungFunc::descendingOrderProc(map<ulong, vector<ulong> > &mapData, const ulong &lDeleteIndex, const ulong &lMinuend)
{
    ulong lTemp;
    for(auto itBegin = mapData.begin(); itBegin != mapData.end(); itBegin++)
    {
        lTemp = lMinuend;
        vector<ulong> &vecContent = itBegin->second;

        if(vecContent.size() < 1)
            continue;

        //若删除的索引在当前分类中，只需要将当前分类中删除对应数量即可
        if(vecContent[0] <= lDeleteIndex && lDeleteIndex <= vecContent[vecContent.size() - 1])
        {
            //若存在，从尾部
            while(lTemp)
            {
                auto it = vecContent.begin();
                it += static_cast<int>((vecContent.size() - 1));
                //删除掉最后一个波形即可
                vecContent.erase(it);
                lTemp--;
            }
        }
        //若当前起始点大于删除的索引，全部进行减去对应数量
        else if(vecContent[0] > lDeleteIndex)
        {
            //若存在，从尾部
            for(unsigned int i = 0; i < vecContent.size(); i++)
            {
                vecContent[i] -= lMinuend;
            }
        }
        //若当前删除点大于末尾点，不进行处理
        else if(lDeleteIndex > vecContent[vecContent.size() - 1])
        {

        }
    }
}

float CAnalysisLungFunc::getStandardDeviation(const vector<ulong> &vecValue, unsigned int nIndex)
{
    //获取标准差，返回数组的标准差左右之和
    size_t szSize = vecValue.size();
    if(szSize == 0)
        return 0.0f;

    vector<ulong> vec1, vec2;
    for(unsigned int i = 0; i < szSize; i++)
    {
        if(i <= nIndex)
        {
            vec1.push_back(vecValue[i]);
        }
        else
        {
            vec2.push_back(vecValue[i]);
        }
    }

    //分别计算两者的标准差
    float fStandardDeviation1, fStandardDeviation2;
    fStandardDeviation1 = getStandardDeviation(vec1);
    fStandardDeviation2 = getStandardDeviation(vec2);

    return fStandardDeviation1 + fStandardDeviation2;
}

float CAnalysisLungFunc::getStandardDeviation(const vector<ulong> &vecValue)
{
    float sum = static_cast<float>(std::accumulate(std::begin(vecValue), std::end(vecValue), 0.0));
    float mean =  sum / vecValue.size(); //均值

    float accum  = 0.0f;
    for_each (std::begin(vecValue), std::end(vecValue), [&](const int d) {
        accum  += (d-mean)*(d-mean);
    });

    float stdev;
    if(isZeroValue(accum))
    {
        stdev = 0;
    }
    else
    {
        stdev = sqrt(accum/(vecValue.size()-1)); //方差
    }

    return stdev;
}

int CAnalysisLungFunc::getWave(vector<R_WAVE> &vecWave, const unsigned long &num, float *pData, float fMinAmplitude)
{
    bool bUpFlag = false;
    float fDifStart = 0.0f, fDifMid = 0.0f, fDifEnd = 0.0f;
    unsigned long lTPos = 0;
    float fTValue = 0.0;

    vecWave.clear();
    R_WAVE waveTemp;

    /*.--------------------  先找到第一个下降点(从后往前推)。----------------------------*/
    for(unsigned long i = 1; i < num; i++)			// 寻找所有的波形，包括干扰波形。
    {
        fDifStart = *(pData + (i - 1));
        fDifMid = *(pData + i);
        fDifEnd = *(pData + (i + 1));

        if ((fDifMid >= fDifStart) && (fDifMid >= fDifEnd))   //波峰
        {
            bUpFlag = false;
            lTPos = i;
            fTValue = fDifMid;
            continue;
        }

        if (bUpFlag && (fDifMid <= fDifStart) && (fDifMid <= fDifEnd)) //波谷
        {
            bUpFlag = false;

            //若斜率不满足需求，继续进行下一个
            if(fTValue -  fDifMid <= fMinAmplitude)
            {
                continue;
            }

            waveTemp.tpos = lTPos;
            waveTemp.tvalue = fTValue;
            waveTemp.spos = i;
            waveTemp.svalue = fDifMid;
            vecWave.push_back(waveTemp);
        }
    }

    return static_cast<int>(vecWave.size());
}

template<class T>
bool CAnalysisLungFunc::getExtremeValue(T &First, ulong &lFirst_pos, T &Second, ulong &lSecond_pos, ulong lNum, float *pData)
{
    T tDifStart, tDifMid, tDifEnd;

    size_t szLen = sizeof (T);
    memset(&tDifStart, 0, szLen);
    memset(&tDifMid, 0, szLen);
    memset(&tDifEnd, 0, szLen);

    bool bFindTop = false, bFindBottom = false;

    //从后往前进行遍历(最后一个位置为 lnum - 1 ，最后一个位置前一个为 lnum - 2)
    for(ulong i = lNum - 2; i > 1; i--)
    {
        tDifStart = *(pData + (i + 1));
        tDifMid = *(pData + i);
        tDifEnd = *(pData + (i - 1));

        //找到第一个波峰
        if ((tDifMid >= tDifStart) && (tDifMid >= tDifEnd))
        {
            lFirst_pos = i;
            First = tDifMid;
            bFindTop = true;
            continue;
        }

        //找到第一个波谷
        if((tDifMid <= tDifStart) && (tDifMid <= tDifEnd))
        {
            lSecond_pos = i;
            Second = tDifMid;
            bFindBottom = true;
            continue;
        }

        if(bFindTop && bFindBottom)
        {
            //将顺序进行交换，小的放在第一个
            if(lFirst_pos > lSecond_pos)
            {
                swap(lFirst_pos, lSecond_pos);
                swap(First, Second);
            }

            break;
        }
    }

    return (bFindTop && bFindBottom);
}
