#ifndef CEFV_H
#define CEFV_H
#include "CThreadResourceCtrlBase.h"
#include "Analysis/CAnalysisLungFunc.h"
#include "CInterAction.h"
#include <QReadWriteLock>
#include <QPointF>

class CEFV  : public CThreadResourceCtrlBase
{
public:
    CEFV();
    ~CEFV();

    //设置当前通气量数据类型
    void setCurDataType(short shDataType);
    void setSampleRate(float fSampleRate);

    //添加数据
    void addNewData(const float *pData, ulong dataLen);
    void addNewData(const vector<float> &vecData);

    //清空数据
    void clearData();

    //删除连续空间
    template <class T>
    void deleteContinueData(T *pData)
    {
        if(pData != nullptr)
            delete [] pData;
    }

protected:
    //处理线程逻辑，只需重载这个变量即可
    virtual void dealThreadLogic();

    //将数据填充到位置
    template <class T>
    void updateData(T *pDest,const ulong &lMaxLenth, ulong &lExitLent, const T *pSrc, const ulong &lSrcLen);
    
    bool getWaveInfo(R_WAVE &lastWave, float *pFlow, float *pVelocity, const ulong &lDataLen);
    
    //传入流速数据进行判断
    bool isStopBrathing(short shDataType, float *pData, const ulong &lDataLen, const float &fSampleRate);

    //处理数据
    bool processData(float *pFlow, float *pVelocity, const ulong &lDataLen, const float &fSampleRate, R_WAVE &lastWave);

    //最新的获取波形函数，通过一个阈值来删除无用的波形
    bool getWave(const ulong &lDataLen, float *pFlow, const float &fSampleRate, R_WAVE &lastWave, const map<ulong, ulong> &mapMapping);

    int getMinOrMaxWaveIndex(vector<R_WAVE> &vec, const ulong &lStart, ulong lNum, short shType);
private:
    //分析的数据类型
    atomic<short> m_shDataType{CAnalysisLungFunc::Data_Velocity};
    atomic<float> m_fSampleRate{0.0f};

    //当前数据
    float *m_pData{nullptr};
    ulong m_lDataLen{0};
    ulong m_lDataLenExist{0};
    
    //进行分析时需要流量和流速数据，这个数据用于中间转换变量使用
    float *m_pDataFlowOrVelocity{nullptr};

    //简单滤波变量
    float *m_pDataFliter{nullptr};
    
    //默认为10s的数据
    unsigned long m_lDataTimeSc{10};
    
    QReadWriteLock  m_mutex;

    vector<QPointF> m_vecBreatheOut;
    vector<QPointF> m_vecBreatheIn;

    CInterAction &m_ui{CInterAction::Get_Instance()};
};

#endif // CEFV_H
