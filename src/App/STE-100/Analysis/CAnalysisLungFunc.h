#ifndef CANALYSISLUNGFUNC_H
#define CANALYSISLUNGFUNC_H
#include "CUserInfo.h"

//描述呼吸波形的起始位置
typedef struct rw {
    unsigned long spos;					// wave start point position(relative position).
    float		svalue;					// wave start point value(represente by sample value).
    unsigned long tpos;					// wave top point position.
    float		tvalue;					// wave top point value.
    unsigned long epos;					// wave end point position.
    float		evalue;					// wave end point value;
} R_WAVE;

//肺活量分析结果
class CAnlsResultVC
{
public:
    double VC;			/// 肺活量(Vital capacity)，				         单位：ml
    double VCP;			/// 预计肺活量，							         单位：ml
    double VC_VCP;		/// VC/VCP，                                      单位：%
    double IRV;			/// 补吸气量(Inspiratory reserve volume，	         单位：ml
    double ERV;			/// 补呼气量(Expiratory reserve volume)，            单位：ml
    double TV;			/// 潮气量(Tidal volume)，                          单位：ml
    double TVM;			/// 每分静息通气量，TVM=TV*RR;
    double IC;			/// 吸气量(Inspiratory capacity))，IC=TV+IRV        单位：ml  实际是深吸气量
    double RV;			/// 残气量(Residual volume)，                       单位：ml
    double FRC;			/// 功能残气量(Functional residual capacity)，FRC=RV+ERV
    double TLC;			/// 肺总量(Total lung capacity)，TLC=IRV+TV+ERV+RV
    double VE;			/// 每分钟呼气量
    double EC;			/// 呼气量	实际是深吸气量
    double VA;			/// 肺泡通气量（alveolar ventilation）， VA=(VT-2.2*体重（kg）)*RR
    double RR;			/// 呼吸率，								          单位：次/分
};

//用力肺活量分析结果
class CAnlsResultFVC
{
public:
    double FVC;		/// 用力肺活量，						 单位：升
    double FEV05;	/// 0.5秒后用力呼气量，				 单位：升
    double FEV1;	/// 1秒后用力呼气量，                  单位：升
    double FEV2;	/// 2秒后用力呼气量，                  单位：升
    double FEV3;	/// 3秒后用力呼气量，                   单位：升
    double FEV05R;	/// 0.5秒率，FEV05R	= FEV05/FVC,   	单位：%
    double FEV1R;	/// 1秒率，	FEV1R	= FEV1/FVC,	   	单位：%
    double FEV2R;	/// 2秒率，	FEV2R	= FEV2/FVC,	   	单位：%
    double FEV3R;	/// 3秒率，	FEV3R	= FEV3/FVC,	   	单位：%
    double MEFR;	/// 最大呼气流量率，					单位：L/s
    double MMEF;	/// 最大呼气中间流量，                 单位：L/s
    double EXTIME;	/// 呼气时间，						单位：s
};

//最大通气量分析结果
class CAnlsResultMPV
{
public:
    double      VE;                             /// 每分平静通气量
    double		MVV;							/// 实测最大通气量，                                单位：升
    double		MVVP;							/// 预计最大通气量，                                单位：升
    // MVVP = [86.5-(0.522*age)]*BSA	(男性)
    // MVVP = [71.3-(0.47*age)]*BSA		(女性)
    double		MVV_MVVP;						/// MVV/MVVP，                                   单位：%	单位：%
    double		RR;								/// 呼吸率，                                      单位：次/分
    double		VSP;							/// 通气储备百分比(Ventilatory reserve percent),   单位：% // VSP = (每分最大通气量-每分静息通气量)/每分最大通气量=(MVV-TV*RR)/MVV
    double		BMC;
    float		AVI;							/// 气速指数 = (最大通气量实测 / 预测) / (肺活量 / 预计);
};


class CAnalysisLungFunc
{
public:
    enum DataType{
        Data_Velocity, //流速数据
        Data_Flow      //流量数据
    };
    CAnalysisLungFunc();

    //每次在分析之前需要设置基本信息参数，分析时需要
    void setUserInfo(const CUserInfo &userInfo);
    void setCurDataType(short shDataType);

    //潮气量相关指标分析
    bool anlsTidalVolume(float *pSrcData, const unsigned long &lSrcDataLen, const float &fSampleRate, CAnlsResultVC &TargetRes);

    //用力肺活量相关指标分析
    bool anlsForceVitalCapacity(float *pSrcData, const unsigned long &lSrcDataLen, const float &fSampleRate, CAnlsResultFVC &TargetRes);
    //获取用力肺活量测量结果,流量数据都是ml，流速数据都是L/min
    static void getFvcResult(ulong lStart, ulong lEnd, float *pVelocityData, float *pFlowData, const float fSampleRate, CAnlsResultFVC &targetRes);

    //最大通气量分析
    bool anlsMaxPulmonaryVentilation(float *pSrcData, const unsigned long &lSrcDataLen, const float &fSampleRate, CAnlsResultMPV &TargetRes);

    //获取流量和流速数据时，若没有上一个点的数据，那么第一个数据会被赋值为0,添加函数只进行新数据的分析, fLastData和数据类型正好相反
    static void getDataVelocityAndFlow(float *pVelocityData, float *pFlowData, short shDataType, const unsigned long &lSrcDataLen, float fSampleRate, bool bHaveLastData = false, float fLastVelocity = 0.0f, float fLastFlow = 0.0f);

    //滤掉波形中的连续等值数据
    static unsigned long fliterEqualData(float *pInDataBuffer, float *pOutDataBuffer, unsigned long lSrcDataLen, map<unsigned long, unsigned long> &mapMapping);

    //获取用力肺活量波形信息
    static void getFVCWavePos(const ulong &lNum, float *pVelocityData, float *pFlowData, float fSampleRate, vector<ulong> &vecWavePos);

    //获取流速波形所有零点位置
    static void getVelocityDataZeroPos(float *pVelocityData, ulong lNum, vector<ulong> &vecZeroPos);

    //删除某个位置点
    static unsigned int deleteZeroPosByIndex(vector<ulong> &vecZeroPos, ulong nIndex);

    static bool isZeroValue(const float &fValue);

    static void dealWave(float *pVelocityData, float *pFlowData, const ulong &lNum, float fSampleRate, vector<ulong> &vecZeroPos);

    //获取呼吸波形
    static int getWave(vector<R_WAVE> &vecWave, const ulong &num, float *pData, float fMinAmplitude);
    //最新的获取波形函数，通过一个阈值来删除无用的波形
    static int getWave(const ulong &lSrcLen, float *pData, float fSampleRate, map<ulong, vector<R_WAVE>> &mapContinueWave, const map<ulong, ulong> &mapMapping);

    template<class T> //差值 减数 被减数
    static T getAbsValueOfSubtraction(const T &dataMinuend , const T &datasubtrahend)
    {
        return dataMinuend > datasubtrahend ? dataMinuend - datasubtrahend : datasubtrahend - dataMinuend;
    }
private:

    //获取除法绝对值
    template<class T, class A> //T 被除数  A除数
    static double getAbsValueOfDivision(const T &dataDivdend, const A &dataDivisor)
    {
        return abs(static_cast<double>(dataDivdend) / static_cast<double>(dataDivisor));
    }

    template<class T, class A>
    static double getValueOfDivision(const T &dataDivdend, const A &dataDivisor)
    {
        return static_cast<double>(dataDivdend) / static_cast<double>(dataDivisor);
    }

    //对特殊结构的数据结构（上面Get_Wave里面的存储结构）进行降序处理，大于某个值的值进行降序
    static void descendingOrderProc(map<ulong, vector<ulong>> &mapData, const ulong &lDeleteIndex, const ulong &lMinuend);

    //获取标准差
    static float getStandardDeviation(const vector<ulong> &vecValue, unsigned int nIndex);
    static float getStandardDeviation(const vector<ulong> &vecValue);

    //获取指定位置之前的两个极值(可能为最大值可能为最小值)
    template<class T> //T 被除数  A除数
    bool getExtremeValue(T &First, ulong &lFirst_pos, T &Second, ulong &lSecond_pos, ulong lNum, float *pData);
private:
    //分析数据类型
    short m_shDataType{Data_Velocity};
};

#endif // CANALYSISLUNGFUNC_H
