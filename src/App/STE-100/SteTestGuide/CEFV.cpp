#include "CEFV.h"
#include "commondefin.h"

//定义波形朝向
const short UP_WAVE = 0; // V形状
const short DOWN_WAVE = 1; //V倒置

CEFV::CEFV()
{
}

CEFV::~CEFV()
{
    deleteContinueData(m_pData);
    deleteContinueData(m_pDataFlowOrVelocity);
    deleteContinueData(m_pDataFliter);
}

void CEFV::setCurDataType(short shDataType)
{
    if(shDataType >= CAnalysisLungFunc::Data_Velocity && shDataType <= CAnalysisLungFunc::Data_Flow)
    {
        m_shDataType = shDataType;
    }
}

void CEFV::setSampleRate(float fSampleRate)
{
    //根据采样率设置当前数据长度
    float fTempSR = m_fSampleRate;
    if(!(fSampleRate > 0.0f && compareFloating(fSampleRate, fTempSR) != 0))
        return;
    
    //暂时停止线程的逻辑处理
    m_bIntoDealProc = false;
    
    //等待当前逻辑执行完成
    while(m_bProcessState)
    {
    }
    
    //清空资源使不可用
    while(m_semaphoreThread.available())
    {
        m_semaphoreThread.acquire();
    }
    
    //恢复处理
    m_bIntoDealProc = true;
    
    m_fSampleRate = fSampleRate;
    
    //计算当前采样率下的数据点数
    ulong lDataLen = static_cast<ulong>(m_fSampleRate) * m_lDataTimeSc;
    
    m_mutex.lockForWrite();
    if(lDataLen > m_lDataLen)
    {
        float *pTemp = new float[lDataLen];
        float *pTemp1 = new float[lDataLen];
        
        deleteContinueData(m_pData);
        deleteContinueData(m_pDataFlowOrVelocity);
        deleteContinueData(m_pDataFliter);
        
        m_pData = pTemp;
        m_pDataFlowOrVelocity = pTemp1;
        m_lDataLenExist = 0;
        m_lDataLen = lDataLen;
    }
    m_mutex.unlock();
}

void CEFV::addNewData(const float *pData, ulong dataLen)
{
    if(pData == nullptr || dataLen == 0 || m_pData == nullptr || m_lDataLen == 0)
        return;
    
    //将数据添加即可
    m_mutex.lockForWrite();
    updateData(m_pData, m_lDataLen, m_lDataLenExist, pData, dataLen);
    m_mutex.unlock();
    
    //同时添加线程资源用于分析
    m_semaphoreThread.release();
}

void CEFV::addNewData(const vector<float> &vecData)
{
    if(m_pData == nullptr || m_lDataLen == 0)
        return;

    addNewData(vecData.data(), static_cast<ulong>(vecData.size()));
}

void CEFV::clearData()
{
    m_mutex.lockForWrite();
    m_lDataLenExist = 0;
    m_mutex.unlock();
}

void CEFV::dealThreadLogic()
{
    if(m_pData == nullptr || m_pDataFlowOrVelocity == nullptr || m_lDataLenExist == 0)
        return;
    
    //初步在这里直接进行全部数据的值转换，后续优化为新增数据进来一个转换一个
    float *pFlow = nullptr, *pVelocity = nullptr;
    
    if(m_shDataType == CAnalysisLungFunc::Data_Velocity)
    {
        pVelocity = m_pData;
        pFlow = m_pDataFlowOrVelocity;
    }
    else 
    {
        pVelocity = m_pDataFlowOrVelocity;
        pFlow = m_pData;
    }
    
    if(pVelocity == nullptr || pFlow == nullptr)
    {
        return;
    }

    R_WAVE wave;
    memset(&wave, 0, sizeof(R_WAVE));
    
    m_mutex.lockForRead();
    CAnalysisLungFunc::getDataVelocityAndFlow(pVelocity, pFlow, m_shDataType, m_lDataLenExist, m_fSampleRate);

    if(!(getWaveInfo(wave, pFlow, pVelocity, m_lDataLenExist) && wave.spos != wave.tpos &&
        wave.spos < m_lDataLenExist && wave.tpos < m_lDataLenExist && wave.epos < m_lDataLenExist))
    {
        m_mutex.unlock();
        return;
    }

    //判断波形是只有一段还是
    m_vecBreatheIn.clear();
    m_vecBreatheOut.clear();

    //将波形分为两段，呼气 吸气，变量表示是否存在第二段波形
    bool bExistSecondWave = wave.epos != 0;

    vector<QPointF> *pFirst = nullptr, *pSecond = nullptr;

    //变量控制 吸气为正还是为负（指的流速数据），若为正，那么吸气的流量波形呈上升趋势（根据传感器进行调整）
    bool bBreatheInPositiveNum = true;

    //第一段是否是吸气波形
    bool bBreatheIn = bBreatheInPositiveNum ? wave.tvalue > wave.svalue : wave.tvalue < wave.svalue;
    if(bBreatheIn)
    {
        pFirst = &m_vecBreatheIn;
        pSecond = &m_vecBreatheOut;
    }
    else
    {
        pFirst = &m_vecBreatheOut;
        pSecond = &m_vecBreatheIn;
    }

    if(pFirst == nullptr || pSecond == nullptr)
    {
        m_mutex.unlock();
        return;
    }

    //获取气体量
    float fVolume1 = abs(wave.tvalue - wave.svalue);
    float fVolume2 = abs(bExistSecondWave ? wave.evalue - wave.tvalue : 0.0f);

    ulong lDataNum, lStart;

    float fTempX;
    //坐标轴中进行绘制时，按照标准的绘制方法,吸气结束在坐标轴0位置处,呼气起始在坐标轴
    int bTimes = bExistSecondWave ? 1 : 2;
    float fTempVolume;
    float fStartValue;
    vector<QPointF> *pTempVector = nullptr;

    for(int curTime = 0; curTime < bTimes; curTime++)
    {
        if(curTime == 0)
        {
            fTempVolume = fVolume1;
            pTempVector = pFirst;
            lDataNum = wave.tpos - wave.spos;
            lStart = wave.spos;
            fStartValue = wave.svalue;
        }
        else
        {
            fTempVolume = fVolume2;
            pTempVector = pSecond;
            lDataNum = wave.epos - wave.tpos;
            lStart = wave.tpos;
            fStartValue = wave.tvalue;
        }

        for(ulong i = lStart; i < lDataNum; i++)
        {
            //纵轴为当前气体流速，横轴为气体量，起点为整个气体体积位置
            if(bBreatheIn)
            {
                fTempX = fTempVolume - (pFlow[i] - fStartValue);
            }
            //起点为呼气量起始最大值
            else
            {
                fTempX = fStartValue - pFlow[i];
            }

            m_ui.appendPVVChartPoint(CProjectEnums::FV_Chart, fTempX, pVelocity[i]);
        }
    }

    m_mutex.unlock();
}

bool CEFV::getWaveInfo(R_WAVE &lastWave, float *pFlow, float *pVelocity, const ulong &lDataLen)
{
    if(pFlow == nullptr || pVelocity == nullptr || lDataLen == 0)
        return false;

    //判断当前是否处于停止阶段
    if(isStopBrathing(CAnalysisLungFunc::Data_Flow, pFlow, lDataLen, m_fSampleRate))
        return false;

    //处理波形数据
    return processData(pFlow, pVelocity, lDataLen, m_fSampleRate, lastWave);
}

bool CEFV::isStopBrathing(short shDataType, float *pData, const ulong &lDataLen, const float &fSampleRate)
{
    bool bRet = true;

    if(shDataType == CAnalysisLungFunc::Data_Flow)
    {
        //末尾存在连续零点或者连续相同值,或者两者交叉存在
        //取末尾0.2s
        ulong lLen = static_cast<ulong>(fSampleRate * 0.2f);
        if(lLen > lDataLen)
        {
            lLen = lDataLen;
        }

        //将位置移到末尾段起始阶段
        float *pTemp = pData;
        pTemp += (lDataLen - 1);
        pTemp -= lLen;

        //简单判断是否大于50ml
        bRet = abs(pTemp[lLen - 1] - *pTemp) > 50;
    }

    return bRet;
}

bool CEFV::processData(float *pFlow, float *pVelocity, const ulong &lDataLen, const float &fSampleRate, R_WAVE &lastWave)
{
    if(pFlow == nullptr || pVelocity == nullptr || lDataLen == 0)
        return false;

    map<ulong, ulong> mapMapping;
    ulong lOutDataLength = CAnalysisLungFunc::fliterEqualData(pFlow, m_pDataFliter, lDataLen, mapMapping);

    if(lOutDataLength < static_cast<ulong>(fSampleRate * 0.05f))
        return false;

    if(getWave(lDataLen, pFlow, fSampleRate, lastWave, mapMapping))
    {
        //将波形转换为原始点对应位置
        lastWave.spos = mapMapping.find(lastWave.spos)->second;
        lastWave.tpos = mapMapping.find(lastWave.tpos)->second;

        if(lastWave.epos != 0)
        {
            lastWave.epos = mapMapping.find(lastWave.epos)->second;
        }
        return true;
    }

    return false;
}

bool CEFV::getWave(const ulong &lDataLen, float *pData, const float &fSampleRate, R_WAVE &lastWave, const map<ulong, ulong> &mapMapping)
{
    bool bValideWave = false;
    //保存所有波形向向下和向上的波形
    vector<R_WAVE> vecWave;
    R_WAVE waveR;

    float fDifStart = 0.0f, fDifMid = 0.0f, fDifEnd = 0.0f;

    //无效呼吸波形，其中吸气或者呼气只要有不大于当前量的都视为无效呼吸波,正常呼吸波形为500左右
    float fMinimumGasVolume = 200;

    //是否存在转折点，当前转折点是在不存在 向上波形和向下波形时使用
    vector<ulong> vecTurningPt;

    vecTurningPt.push_back(0);
    //找到所有转折点并且保存下来
    for(ulong i = 1; i < lDataLen - 1; i++)
    {
        fDifStart = *(pData + (i - 1));
        fDifMid = *(pData + i);
        fDifEnd = *(pData + (i + 1));

        if ((fDifMid > fDifStart) && (fDifMid > fDifEnd))
        {
            vecTurningPt.push_back(i);
            continue;
        }

        if ((fDifMid < fDifStart) && (fDifMid < fDifEnd))
        {
            vecTurningPt.push_back(i);
            continue;
        }
    }

    if(lDataLen - 1 != vecTurningPt[vecTurningPt.size() - 1])
    {
        vecTurningPt.push_back(lDataLen - 1);
    }

    //表示只有吸气或者呼气过程,只有一半波形
    if(vecTurningPt.size() == 2)
    {
        waveR.spos = vecTurningPt[0];
        waveR.svalue = *(pData + waveR.spos);
        waveR.tpos = vecTurningPt[1];
        waveR.tvalue = *(pData + waveR.tpos);
        waveR.epos = 0;
        lastWave = waveR;
        return true;
    }

    //一般三个转折点为一个完整的波形，同时还要根据阈值去除小波
    size_t szSize = vecTurningPt.size();
    for( size_t j = szSize - 1; j >= 2; )
    {
        waveR.spos = vecTurningPt[j - 2] + 1;
        waveR.svalue = *(pData + waveR.spos);
        waveR.tpos = vecTurningPt[j - 1];
        waveR.tvalue = *(pData + waveR.tpos);
        waveR.epos = vecTurningPt[j];
        waveR.evalue = *(pData + waveR.epos);

        vecWave.push_back(waveR);

        if(j < 2)
        {
            break;
        }

        j -= 2;
    }

    //这里一定会存在波形，首先去掉与最后一个波形不连续的波形
    //规定连续波形的时间 s
    float fContinueWaveTime = 0.2f;
    ulong nCount = static_cast<ulong>((fSampleRate * fContinueWaveTime));
    auto itFind = mapMapping.begin();
    ulong lTemp;

    for(ulong i = 0; i < vecWave.size() - 1; i++)
    {
        itFind = mapMapping.find(vecWave[i].spos);
        lTemp = itFind->second;
        itFind = mapMapping.find(vecWave[i + 1].epos);

        if(CAnalysisLungFunc::getAbsValueOfSubtraction(lTemp, itFind->second) > nCount)
        {
            //删除所有前面不连续的波形，在vec中存储在后面
            size_t j;
            while(true)
            {
                j = vecWave.size();
                if(j <= 1)
                    break;

                if(j - 1 < i +  1)
                    break;


                auto itBegin = vecWave.begin();
                itBegin += (i + 1);
                vecWave.erase(itBegin);
            }
        }
    }

    //将剩余波形中的无效波形挑选出来，然后进行波形合并,第一个波形（即存储在最后只判断右侧）vec中波形倒序,最后一个只判断左侧
    map<ulong, vector<ulong>> mapInvalideWave;
    vector<ulong> vecTemp;
    bool bEffectiveWave;
    float fTemp;
    int nLastInvalideWaveIndex = -2;

    for(int i = 0; i < static_cast<int>(vecWave.size()); i++)
    {
        lTemp = static_cast<ulong>(i);
        bEffectiveWave = true;
        R_WAVE &waveTemp = vecWave[lTemp];

        //判断左侧
        fTemp = abs(waveTemp.tvalue - waveTemp.svalue);
        if( fTemp < fMinimumGasVolume && lTemp != vecWave.size() - 1)
        {
            bEffectiveWave = false;
        }

        //判断右侧
        fTemp = abs(waveTemp.tvalue - waveTemp.evalue);
        if( fTemp < fMinimumGasVolume && i != 0)
        {
            bEffectiveWave = false;
        }

        //无效波形
        if(!bEffectiveWave)
        {
            //证明不是连续波形
            if(i - nLastInvalideWaveIndex != 1)
            {
                vecTemp.clear();
                vecTemp.push_back(lTemp);
                mapInvalideWave[static_cast<ulong>(mapInvalideWave.size())] = vecTemp;
            }
            else
            {
                auto itFindValideWave = mapInvalideWave.find(static_cast<ulong>(mapInvalideWave.size() - 1));
                itFindValideWave->second.push_back(lTemp);
            }

            nLastInvalideWaveIndex = i;
        }
    }

    //将无效波形合并到附近的波形,只需要将最后一个波形合并完成即可，判断第一个即可
    auto itBegin = mapInvalideWave.begin();
    if( itBegin!= mapInvalideWave.end())
    {
        vector<ulong> &vecInvalideWave = itBegin->second;
        if(vecInvalideWave[0] <= 1)//最后一个波形在0位置，这里无效波形索引等于1或者等于0证明最后一个波形不完整
        {
            //找到波形中最大或最小波峰的值（根据第一个波形朝向），然后将首尾相连即可
            short shWaveType;
            if(vecWave[0].tvalue > vecWave[0].evalue)
            {
                shWaveType = DOWN_WAVE;
            }
            else
            {
                shWaveType = UP_WAVE;
            }

            if(vecInvalideWave[0] == 0)
            {
                szSize = vecInvalideWave.size();
            }
            else
            {
                szSize = vecInvalideWave.size() + 1;
            }

            int nMaxOrMinIndex = getMinOrMaxWaveIndex(vecWave, vecInvalideWave[0], static_cast<ulong>(szSize), shWaveType);

            if(nMaxOrMinIndex != -1)
            {
                vecWave[0].tpos = vecWave[static_cast<ulong>(nMaxOrMinIndex)].tpos;
                vecWave[0].tvalue = vecWave[static_cast<ulong>(nMaxOrMinIndex)].tvalue;
                vecWave[0].spos = vecWave[vecInvalideWave[vecInvalideWave.size() - 1]].spos;
                vecWave[0].svalue = vecWave[vecInvalideWave[vecInvalideWave.size() - 1]].svalue;
            }

        }
    }

    if(vecWave.size() >0)
    {
        if(abs(vecWave[0].tvalue - vecWave[0].svalue) > fMinimumGasVolume)
        {
            bValideWave = true;
        }
        lastWave = vecWave[0];
    }

    return bValideWave;
}

int CEFV::getMinOrMaxWaveIndex(vector<R_WAVE> &vec, const ulong &lStart, ulong lNum, short shType)
{
    if(lNum == 0 || vec.size() - 1 < lStart || lStart + lNum > vec.size())
    {
        return -1;
    }

    ulong nMaxIndex = lStart, nMinIndex = lStart;

    for(ulong i = lStart + 1; i < lStart + lNum; i++)
    {
        if(vec[i].tvalue > vec[nMaxIndex].tvalue)
        {
            nMaxIndex = i;
        }

        if(vec[i].tvalue < vec[nMinIndex].tvalue)
        {
            nMinIndex = i;
        }
    }

    int nRet = static_cast<int>(shType == UP_WAVE ? nMinIndex : nMaxIndex);
    return nRet;
}

template<class T>
void CEFV::updateData(T *pDest, const ulong &lMaxLenth, ulong &lExitLent, const T *pSrc, const ulong &lSrcLen)
{
    //若下一次采样的长度加上以前采样的数据长度大于规定的时间数据长度，那么丢弃最前面的老数据
    ulong lTotalLenth = lExitLent + lSrcLen;

    if(lTotalLenth > lMaxLenth)
    {
        const T *pTempData = pSrc;

        //这里会出现一个问题，即最新的数据大于最大数据长度
        if(lSrcLen > lMaxLenth)
        {
            //将最新的数据赋值即可
            memcpy(pDest, pTempData + (lSrcLen - lMaxLenth), lMaxLenth * sizeof(T));
        }
        else
        {
            //丢弃数据量是当前总的数据量减去当前数据长度，这里只存在正值，复制已在上面处理
            ulong lDiscardNum = lTotalLenth - lExitLent;
            pTempData = pDest + lDiscardNum;

            //在已有数据中，丢弃lDiscardNum个数据点
            memcpy(pDest, pTempData, (lMaxLenth - lDiscardNum) * sizeof(T));

            //将新数据加在后面
            memcpy(pDest + lMaxLenth - lSrcLen, pSrc, lSrcLen * sizeof(T));
        }

        lExitLent = lMaxLenth;
    }
    else
    {
        memcpy(pDest + lExitLent, pSrc, lSrcLen * sizeof(T));
        lExitLent += lSrcLen;
    }
}
