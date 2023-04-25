#ifndef CTESTRUNCONFIG_H
#define CTESTRUNCONFIG_H

#include <string>
#include<QSettings>
#include "../common_global.h"
using namespace std;

class COMMONSHARED_EXPORT CTestRunConfig
{
public:
    CTestRunConfig();

    void Init();

    // 发令枪com口名称
    QString getStartingGunComName() const {return m_qsStartingGunComName;}

    //无起点短跑需要增加延时时间
    QString getRunShortTime() const { return m_qsRunShortTime; }
private:

    void      Default(QSettings& settings);

    void      Read(const QSettings& settings);

    void      Check_Field(const QSettings& settings, const string& strField);

    QString   m_qsStartingGunComName;

    QString   m_qsRunShortTime;
};

#endif // CTESTRUNCONFIG_H
