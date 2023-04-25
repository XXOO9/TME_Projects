#ifndef COMMON_ERROR_H
#define COMMON_ERROR_H
/***********************************STE-100错误码定义*******************************/
//1、在此处统一所有错误信息，通过日志查询错误码，即可清晰定位软件错误。
//2、每次使用错误码时，需要先将错误码变量赋值，便于查找最后一次错误信息
//3、错误类型分为：硬件错误、软件错误、网络错误
//4、硬件错误：硬件数据错误导致连接异常，硬件数据错误导致结果异常等
//5、软件错误：软件逻辑错误导致结果异常，软件设计错误导致程序崩溃等
//6、网络错误：网络逻辑错误导致同步异常，网络状态错误导致提交失败等
//7、具体细节在下面定义中划分：
/**********************************************************************************/
//1.常用错误定义
#define MOK             0x00       //成功
#define MERR_UNKNOWN    0x01       //错误原因不明

//2.硬件错误定义
//设备连接错误
#define MERR_CAMERA_RECOGNITION    0x2101 //识别摄像头断连
#define MERR_CAMERA_ANTICHEATING   0x2102 //防作弊摄像头断连
#define MERR_USER_RECONGNITION_NFC 0x2103 //用户识别NFC
#define MERR_RUN_MID_NFC           0x2104 //中长跑NFC
#define MERR_POWER                 0x2105 //电源设备
#define MINFO_STARTING_GUN         0x2106 //发令枪设备
#define MINFO_FOUL_CONTROL         0x2107 //犯规控制设备

//3.软件错误定义
//3.1 数据库同步相关
#define MERR_SYNC_USER             0x3101 //正在同步用户信息
#define MERR_SYNC_CLASS            0x3102 //正在同步班级信息
#define MERR_SYNC_SCORE            0x3103 //正在同步成绩标准
#define MINFO_LOAD_SCORE           0x3104 //正在加载成绩标准
#define MINFO_LOAD_FACE_FEATURE    0x3105 //正在加载本地人脸特征

//4.设备相关提示
#define MINFO_RUN_CARD_BIND_FULL   0x4101 //中长跑底层设备绑卡已满

#include "common_global.h"
#include <QString>
#include <map>
using namespace std;

//外部通过此类获取错误描述
class COMMONSHARED_EXPORT CErrorCode
{
public:
    CErrorCode();

    //1.获取错误描述
    static QString getErrorDescribe(int uiErrorCode);

    //2.设置错误码
    static void setErrorCode(int uiErrorCode);

    //3.得到最后出现的错误码
    static int getErrorCode();

protected:
    const static map<int, QString> initCodeToDescribe();

private:
    static int s_Error_Code;

    const static map<int, QString> map_Code_To_Describe;
};
#endif // COMMON_ERROR_H
