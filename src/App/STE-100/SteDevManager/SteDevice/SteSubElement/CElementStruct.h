//暂时将所有具体子设备需要的属性放在此处

//1.跑道基本信息(中长跑、短跑、往返跑)
typedef struct Dev_Runway_Info
{
    //身份码
    unsigned int   uiIDCode;

    //时间戳信息
    unsigned short usYear;
    unsigned char  ucMonth;
    unsigned char  ucDay;
    unsigned char  ucHour;
    unsigned char  ucMinute;
    unsigned char  ucSecond;

    //通过时间
    float fRunTime;

    //标志
    bool  bEndFlag;

    Dev_Runway_Info& operator=(const Dev_Runway_Info& DevRunwayInfo)
    {
        this->uiIDCode = DevRunwayInfo.uiIDCode;
        this->usYear   = DevRunwayInfo.usYear;
        this->ucMonth  = DevRunwayInfo.ucMonth;
        this->ucDay    = DevRunwayInfo.ucDay;
        this->ucHour   = DevRunwayInfo.ucHour;
        this->ucMinute = DevRunwayInfo.ucMinute;
        this->ucSecond = DevRunwayInfo.ucSecond;
        this->fRunTime = DevRunwayInfo.fRunTime;
        this->bEndFlag = DevRunwayInfo.bEndFlag;

        return *this;
    }
}DEV_RUNWAY_INFO, *PDEV_RUNWAY_INFO;

//2.跳绳基本信息
typedef struct Dev_Rope_Info
{
    //身份码
    unsigned int   uiIDCode;
    //开始次数
    unsigned short usStartNum;
    //跳绳次数
    unsigned short usSkipNum;
    //充电状态
    unsigned char  ucChargeStatus;
    //电量
    unsigned char  ucPowerNum;
    //数据状态(0-无效 1-测试中 2-测试结束)
    unsigned char  ucDataStatus;

    Dev_Rope_Info& operator=(const Dev_Rope_Info& DevRopeInfo)
    {
        this->uiIDCode       = DevRopeInfo.uiIDCode;
        this->usStartNum     = DevRopeInfo.usStartNum;
        this->usSkipNum      = DevRopeInfo.usSkipNum;
        this->ucChargeStatus = DevRopeInfo.ucChargeStatus;
        this->ucPowerNum     = DevRopeInfo.ucPowerNum;
        this->ucDataStatus   = DevRopeInfo.ucDataStatus;

        return *this;
    }
}DEV_ROPE_INFO, *PDEV_ROPE_INFO;
