#include "CAnalysisHBC.h"
#include "CoefFactor.h"
#include "CLogger.h"
#include <QDebug>

CAnalysisHBC::CAnalysisHBC()
{
    m_mapDescribes[HBC_HEAD_WEIGHT]      = QString::fromLocal8Bit( "头部重量" );
    m_mapDescribes[HBC_HEAD_FAT]         = QString::fromLocal8Bit( "头部脂肪" );
    m_mapDescribes[HBC_HEAD_MUSCLE]      = QString::fromLocal8Bit( "头部肌肉" );
    m_mapDescribes[HBC_BODY_FAT_PERCENT] = QString::fromLocal8Bit( "全身体脂百分比" );
    m_mapDescribes[HBC_BFLW]             = QString::fromLocal8Bit( "全身去脂体重" );
    m_mapDescribes[HBC_WBF]              = QString::fromLocal8Bit( "全身体脂肪" );
    m_mapDescribes[HBC_TBMM]             = QString::fromLocal8Bit( "全身肌肉量" );
    m_mapDescribes[HBC_WBIS_]            = QString::fromLocal8Bit( "全身无机盐" );
    m_mapDescribes[HBC_TBW]              = QString::fromLocal8Bit( "全身身体总水分TBW" );
    m_mapDescribes[HBC_WHR]              = QString::fromLocal8Bit( "全身腰臀比WHR" );
    m_mapDescribes[HBC_VFA]              = QString::fromLocal8Bit( "内脏脂肪指数VFA" );
    m_mapDescribes[HBC_BMI]              = QString::fromLocal8Bit( "BMI" );

    m_mapUnit[HBC_HEAD_WEIGHT]      = QString::fromLocal8Bit( "Kg" );
    m_mapUnit[HBC_HEAD_FAT]         = QString::fromLocal8Bit( "Kg" );
    m_mapUnit[HBC_HEAD_MUSCLE]      = QString::fromLocal8Bit( "Kg" );
    m_mapUnit[HBC_BODY_FAT_PERCENT] = QString::fromLocal8Bit( "%" );
    m_mapUnit[HBC_BFLW]             = QString::fromLocal8Bit( "Kg" );
    m_mapUnit[HBC_WBF]              = QString::fromLocal8Bit( "Kg" );
    m_mapUnit[HBC_TBMM]             = QString::fromLocal8Bit( "Kg" );
    m_mapUnit[HBC_WBIS_]            = QString::fromLocal8Bit( "Kg" );
    m_mapUnit[HBC_TBW]              = QString::fromLocal8Bit( "Kg" );
    m_mapUnit[HBC_WHR]              = QString::fromLocal8Bit( "" );
    m_mapUnit[HBC_VFA]              = QString::fromLocal8Bit( "面积/10cm" );
    m_mapUnit[HBC_BMI]              = QString::fromLocal8Bit( "" );
}

double CAnalysisHBC::Analysis(short shAnalysisItem, double fHeight, bool bSex, short shAge, double fWeight, vector<double> vImpedance)
{
    double fResult = 0.0;
    int nCount = IMPEDANCE_COEFFICIENT_ROW * IMPEDANCE_COEFFICIENT_COL;
    if (vImpedance.size() != nCount)
    {
        return fResult;
    }


    /* 根据传入的阻抗系数值转换成使用的阻抗系数
     * fArrImpedance:1K、1K的[RA、LA、TT、RL、LL]、	5K、5K的[RA、LA、TT、RL、LL]...
     * 使用到的阻抗系数公式：阻抗系数 = (身高(cm))的平方 / 接收到的阻抗系数
    */

    int nRow, nCol, nIndex;
    nRow = nCol = nIndex =0;

    double fIC[IMPEDANCE_COEFFICIENT_ROW * IMPEDANCE_COEFFICIENT_COL] = {0.0};
    double fCoefficient = 0.0;
    vector<double>::iterator iter = vImpedance.begin();
    for (; iter != vImpedance.end(); iter++, nIndex++)
    {
        nCol = nIndex / IMPEDANCE_COEFFICIENT_ROW;
        nRow = nIndex % IMPEDANCE_COEFFICIENT_ROW;

        fCoefficient = *iter;
        fIC[nRow*IMPEDANCE_COEFFICIENT_COL + nCol] = fabs(fCoefficient) < 0.00001 ? 0 : (fHeight * fHeight / fCoefficient);
    }


    //计算k4*z1 + ....+k33*z30
    double fICSum = 0.0;
    for (int i = 0; i < nCount; i++)
    {
        fICSum += g_CoefFactorCode[shAnalysisItem][i + 4] * fIC[i];
    }

    short shGender = bSex ? 1 : 0;
    double fK0 = g_CoefFactorCode[shAnalysisItem][0];
    double fK1 = g_CoefFactorCode[shAnalysisItem][1];
    double fK2 = g_CoefFactorCode[shAnalysisItem][2];
    double fK3 = g_CoefFactorCode[shAnalysisItem][3];
    double fB = g_CoefFactorCode[shAnalysisItem][COL_NUM - 1];
    fResult = fK0 * fHeight + fK1 * shGender + fK2 * shAge + fK3 * fWeight + fICSum + fB;

    return fResult;
}

bool CAnalysisHBC::getAnalysisResult(const vector<short> &vecAnalysisItem, vector<CAnalysisHBC::HBCComponentDes> &vecResult,
                                     double fHeight, bool bSex, short shAge, double fWeight, const vector<double> &vImpedance)
{
    vecResult.clear();

    LOGERROR("height = " + to_string(fHeight) +
             "weight = " + to_string(fWeight) +
             "gender = " + to_string(bSex) +
             "age = " + to_string(shAge));

    map<short, QString>::iterator itFind;
    for(auto &shAnalysisItem : vecAnalysisItem)
    {
        vecResult.push_back(HBCComponentDes());
        HBCComponentDes &result = vecResult[vecResult.size() - 1];

        if(shAnalysisItem == HBC_BMI)
        {
            getBMI(fHeight, fWeight, result);
        }
        else
        {
            result.shAnalysisItem = shAnalysisItem;

            itFind = m_mapDescribes.find(shAnalysisItem);
            if(itFind != m_mapDescribes.end())
            {
                result.qsDescribe = itFind->second;
            }

            itFind = m_mapUnit.find(shAnalysisItem);
            if(itFind != m_mapUnit.end())
            {
                result.qsUnit = itFind->second;
            }

            result.lfResult = Analysis(shAnalysisItem, fHeight, bSex, shAge, fWeight, vImpedance);
        }
    }

    return vecAnalysisItem.size() == vecResult.size();
}

void CAnalysisHBC::getBMI(const double fHeight, const double fWeight, HBCComponentDes &BMI)
{
    //体质指数 BMI = fWeight(kg) / (fHeight^2(m))
    double lfHeightTemp = fHeight / 100;

    BMI.shAnalysisItem = HBC_BMI;
    BMI.lfResult = fWeight / (lfHeightTemp * lfHeightTemp);
    BMI.qsDescribe = getBodilyFormValue(BMI.lfResult);
}

QString CAnalysisHBC::getBodilyFormValue(const double &fBMI)
{
    QString qsBMI;
    if (fBMI < 18.5)
    {
        qsBMI = QString::fromLocal8Bit("偏瘦");
    }
    else if (18.5 <= fBMI && fBMI < 24.9)
    {
        qsBMI = QString::fromLocal8Bit("正常");
    }
    else if (24.9 <= fBMI && fBMI < 29.9)
    {
        qsBMI = QString::fromLocal8Bit("偏胖");
    }
    else if (29.9 <= fBMI && fBMI < 34.9)
    {
        qsBMI = QString::fromLocal8Bit("肥胖");
    }
    else if (34.9 <= fBMI && fBMI < 39.9)
    {
        qsBMI = QString::fromLocal8Bit("重度肥胖");
    }
    else if (39.9 <= fBMI)
    {
        qsBMI = QString::fromLocal8Bit("极重度肥胖");
    }
    return qsBMI;
}



void CAnalysisHBC::init()
{
    //0
    m_mapDescribes[HBC_HEAD_WEIGHT] = ("头部重量");
    m_mapUnit[HBC_HEAD_WEIGHT] = "kg";

    //1
    m_mapDescribes[HBC_HEAD_FAT] = "头部脂肪";
    m_mapUnit[HBC_HEAD_FAT] = "kg";

    //2
    m_mapDescribes[HBC_HEAD_MUSCLE] = "头部肌肉";
    m_mapUnit[HBC_HEAD_WEIGHT] = "kg";

    //3
    m_mapDescribes[HBC_BODY_FAT_PERCENT] = "全身体脂百分比";
    m_mapUnit[HBC_BODY_FAT_PERCENT] = "%";

    //5
    m_mapDescribes[HBC_BFLW] = "全身去脂体重";
    m_mapUnit[HBC_BFLW] = "kg";

    //6
    m_mapDescribes[HBC_WBF] = "全身体脂肪";
    m_mapUnit[HBC_WBF] = "kg";

    //13
    m_mapDescribes[HBC_HEAD_WEIGHT] = "全身肌肉量";
    m_mapUnit[HBC_HEAD_WEIGHT] = "kg";

    //15
    m_mapDescribes[HBC_WBIS_] = "全身无机盐";
    m_mapUnit[HBC_WBIS_] = "kg";

    //31
    m_mapDescribes[HBC_TBW] = "全身身体总水分";
    m_mapUnit[HBC_TBW] = "kg";

    //50
    m_mapDescribes[HBC_WHR] = "全身腰臀比";
    m_mapUnit[HBC_WHR] = "";

    //51
    m_mapDescribes[HBC_VFA] = "内脏脂肪指数VFA";
    m_mapUnit[HBC_VFA] = "cm²";
}
