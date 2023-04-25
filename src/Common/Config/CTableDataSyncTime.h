#ifndef CFGDATASYNCTIME_H
#define CFGDATASYNCTIME_H
#include "CAbstractConfigInfo.h"
#include <QDateTime>


class COMMONSHARED_EXPORT CTableDataSyncTime: public CAbstractConfigInfo
{
public:
    enum emTableType{
        TABLE_TYPE_USER_INFO, //用户信息表格
        TABLE_TYPE_CLASS_INFO, //班级信息
        TABLE_TYPE_TEST_RESULT, //测试结果
        TABLE_TYPE_SCORE_RULE, //得分标准
        TYPE_VEDIO_VC, //肺活量指导视频
        TYPE_VEDIO_SBJ, //立定跳远指导视频
        TYPE_VEDIO_SFB, //坐位体前屈指导视频
        TYPE_VEDIO_PULLUP, //引体向上指导视频
        TYPE_VEDIO_SITUP, //仰卧起坐指导视频
        TYPE_VEDIO_RUN, //跑步指导视频
    };
    
    CTableDataSyncTime();

    virtual bool Check_Config_Table() override;
    
    
    bool updateSyncTime(short shTableType, const QDateTime &datetime);

    //获取时可能不存在某个类型的表格返回false，或者查询出错也是
    bool getSyncTime(short shType, QDateTime &datetime);

    static QString getDateTimeFormat();
};

#endif // CFGDATASYNCTIME_H
