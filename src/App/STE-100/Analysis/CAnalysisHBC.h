#ifndef CANALYSISHBC_H
#define CANALYSISHBC_H
#include <vector>
#include <map>
#include <QString>

using namespace std;

class CAnalysisHBC
{
public:
    CAnalysisHBC();

	enum HBC{
        HBC_HEAD_WEIGHT = 0,    // 0:头部重量(可能推定值)(单位:Kg)
		HBC_HEAD_FAT,           // 1:头部脂肪(可能推定值)(单位:Kg)
		HBC_HEAD_MUSCLE,        // 2:头部肌肉(可能推定值)(单位:Kg)
        HBC_BODY_FAT_PERCENT,   // 3:全身体脂百分比(单位:%) PBF
        HBC_BFLW = 5,           // 5.全身去脂体重(瘦组织+无机盐)(单位:Kg) Body fat loss weight
        HBC_WBF = 6,            // 6:全身体脂肪(单位:Kg) Whole body fat
        HBC_TBMM = 13,          // 13:全身肌肉量(单位:Kg) Total body muscle mass
        HBC_WBIS_= 15,          // 15:全身无机盐(单位kg） Whole body inorganic salt
        HBC_TBW = 31,           // 31:全身身体总水分TBW(单位:Kg)
        HBC_WHR = 50,           // 50:全身腰臀比 WHR
        HBC_VFA = 51,            // 51:内脏脂肪指数VFA(单位:面积/10cm)
        HBC_ITEM_COUNT,
        HBC_BMI                 //额外测试项目 BMI
    };
	
    enum IMPEDANCE_COEFFICIENT
    {
        IMPEDANCE_COEFFICIENT_ROW = 5,
        IMPEDANCE_COEFFICIENT_COL = 6
    };

    //成分描述
    struct HBCComponentDes
    {
        short shAnalysisItem;
        double lfResult;
        QString qsDescribe;
        QString qsUnit;
    };

    double Analysis(short shAnalysisItem, double fHeight, bool bSex, short shAge, double fWeight, vector<double> vImpedance);

    //一层封装的接口用于获取多个人体成分
    bool getAnalysisResult(const vector<short> &vecAnalysisItem, vector<HBCComponentDes> &vecResult,
                           double fHeight, bool bSex, short shAge, double fWeight,
                           const vector<double> &vImpedance);

    //传入单位 cm kg
    void getBMI(const double fHeight, const double fWeight, HBCComponentDes &BMI);

    //默认采用国际标准
    QString getBodilyFormValue(const double& fBMI);
protected:
    void init();

private:
    map<short, QString> m_mapDescribes;
    map<short, QString> m_mapUnit;
};

#endif // CANALYSISBCI_H
