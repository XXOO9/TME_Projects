#ifndef CDEVSWINFOCONFIG_H
#define CDEVSWINFOCONFIG_H

#include "CAbstractConfigInfo.h"
#include <QStringList>

typedef struct Dev_Sw_Test_Info{
    int iDevType;
    int testTimes;
    int waitStartTimes;
    int pageCountTime;
    string strTestItems;
} DEV_SW_TEST_INFO;

class COMMONSHARED_EXPORT CDevSWInfoConfig : public CAbstractConfigInfo
{
public:
    CDevSWInfoConfig();
    ~CDevSWInfoConfig();

    virtual bool Check_Config_Table();

    //测试次数配置
    bool updateDevSWTestConfig(int devType, int testTimes, string testItems);

    bool getDevTestTimes(int devType, int &times);

    bool getDevTestStartWaitTime(int devType, int &time);

    int getDevTestPageCountTime(int devType);

    bool updateDevTestPageCountTime(int devType, int time);

    bool getDevTestItemsVec(int devType, vector<short> &item);

    bool getDevTestItemsStr(int devType, string &testItem);
};

#endif // CDEVSWINFOCONFIG_H
