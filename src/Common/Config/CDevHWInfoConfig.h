#ifndef CDEVHWINFOCONFIG_H
#define CDEVHWINFOCONFIG_H
#include "CAbstractConfigInfo.h"

class COMMONSHARED_EXPORT CDevHWInfoConfig : public CAbstractConfigInfo
{
public:
    CDevHWInfoConfig();
    ~CDevHWInfoConfig();

    virtual bool Check_Config_Table();

    //信道号、最大链接数
    bool updateDevChannelAndCount(int channel, int maxCount);

    //分机编号
    bool updateDevNumber(int num);

    bool getDevChannel(int &channel);

    bool getDevMaxConnectCount(int &maxCount);

    bool getDevNumber(int &num);
};

#endif // CDEVHWINFOCONFIG_H
