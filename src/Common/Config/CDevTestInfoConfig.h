#ifndef CDEVTESTINFOCONFIG_H
#define CDEVTESTINFOCONFIG_H
#include "CAbstractConfigInfo.h"

class COMMONSHARED_EXPORT CDevTestInfoConfig : public CAbstractConfigInfo
{
public:
    CDevTestInfoConfig();
    ~CDevTestInfoConfig();

    virtual bool Check_Config_Table();

    bool updateDevTestMode(int testMode);

    bool updateDevDisplayScore(bool bDisplayScore);

    bool updateUseRunShortStartFlag(bool bUseRunShortStartFlag);

    bool updateDevRunMidLength(int length);

    bool updateRopeDelayTime(int iTime);

    bool getDevTestMode(int &testMode);

    bool getDevIsDisplayScore();

    bool getUseRunShortStartFlag();

    bool getDevRunMidLength(int &length);

    bool getRopeDelayTime(int &iTime);

    //设备连接模式（组网直连）
    bool updateDevConnectMode(bool bDirectConnect);
    bool getDevConnectMode();

    //设备连接模式自动切换
    bool updateDevConnectModeAuto(bool bAutoSwitch);
    bool getDevConnectModeAuto();

    //立定跳远起跳点
    bool getStandingJumpStartPos( int &startPos );
    bool setStadingJumpStartPos( const int &taregtStartPos );
};
#endif // CDEVTESTINFOCONFIG_H
