#ifndef CDevDataCfg_H
#define CDevDataCfg_H

// 设备数据配置，添加新设备时，在这里添加所有相关即可
#include "ICommunicate.h"
#include "SteDevManager/SteDevice/CDevHardwareBase.h"
#include "CProjectEnums.h"
#include "CUserInfo.h"
#include "CTestResultInfo.h"

#include <map>
#include <set>
#include <vector>
using namespace std;

//测试表格配置
namespace  TestTableCfg{
    //定义统一的类型码,收集现有所有的类型，表格可能变更，但是列类型不会变更,在外部进行操作的是 ColDataType，其他的表格只是在这里进行统一的类型转换
    enum ColDataType{
        COL_DEV_ID,          //设备号
        COL_USER_NAME,       //姓名
        COL_USER_ID,         //身份ID
        COL_TEST_TIMES,      //测试次数
        COL_TEST_RESULT,     //结果
        COL_DEV_STATE,       //设备状态
        COL_THROUGH_TIMES,   //经过次数(跑步)
        COL_DEV_POWER,       //设备电量
        COL_TIME_COUNT_DOWN, //倒计时
        COL_TEST_NO,         //序号
        COL_TEST_SCORE,      //成绩
        COL_HEIGHT,          //身高
        COL_WEIGHT,          //体重
        COL_LEFT_EYE,        //左眼
        COL_RIGHT_EYE,       //右眼
        COL_UNBIND_ENABLE,   //是否可取消绑定
        COL_ILLEGAL_TIMES,   //违例次数
        COL_UNKNOWN
    };
}

//底层设备分机编号范围
namespace NumberOfChannelAndextension
{
    enum ExtensionNoRange{
        ExtensionNoMin = 1,
        ExtensionNoMax = 20
    };
}


class CDevDataCfg
{
public:
    CDevDataCfg();

    //实际测试会阻塞类中的定时器
    static void delayMSec(int msec);
    static void delayMsecLoop(int msec);


    //测试项目和帧数据类型转换
    static unsigned short getFrameTypeByTestItem(short shTestItem);
    static short getTestItemByFrameType(unsigned short shDataFrameType);
    //测试项目和设备的类型转换
    static short getDevTypeByTestItem(const short &shTestItem);
    static short getTestItemByDevType(const short &shDevType); //通过设备类型获取最常用的测试项目

    //根据信号号获取设备类型
    static short getDevTypeByChaNo(const unsigned int &nChaNo);

    //是否为数据类型命令码
    static const set<unsigned short> &getDataCmdcontainer() {return s_setDataTypeCmd;}
    static const map<unsigned char, short> &getHardwareTypeMapping(){return s_mapHardwareTypeMapping;}

    //获取特殊类型设备 中长跑 短跑 跳绳
    static const set<short> &getSpecialDev(){return s_setSpecialDev;}
    static bool isSpecialDev(const short &shDevType);

    //判断是否是需要 违规信息 的设备
    static const set<short> &getViolationDev(){return s_setViolationDev;}
    static bool isNeedViolationInfoDev(const short &shDevType);
    static bool isNeedViolationInfoItem(const short &shItem);

    //获取跑步类型设备 时间的比较方式与正常相反
    static bool isRunTimeDev(const short &shDevType);

    //工具函数：将状态标志转换为描述
    static QString convertStatusToDescrip(const short &shStatus);

    //-------------------------------- 相关表格操作 -----------------------------------

    ///表格分为(测试表格 与 成绩表格)
    //测试表格：根据 设备类型 及 同步异步 构建默认数值, 异步表格测试表格和完成测试合为一张表，根据后续完成状态，获取的表格类型也不一致（设备状态显示为得分）
    static void getDevTestTable(vector<short> &vecTableCfg, const short &shDevType, const short &shTestMode, bool bComplete = false);
    //成绩表格：目前所有成绩表格统一
    static void getDevScoreTable(vector<short> &vecTableCfg, const short &shDevType);

    //获取某列所在位置(最后默认参数兼容上述第二表格)，每次调用函数都需要判断下标是否正确
    static short getColTypeIndex(const short &shColType, const short &shDevType, const short &shTestMode, const bool bScoreTable = false);

    //获取表格内容,针对一对多测试过程中的表格数据类型获取
    static QString getContentByColDataType(const short &shColDataType, const CLocation &location, const CUserInfo &userInfo, const vector<CTestResultInfo> &vecTestResultInfo = vector<CTestResultInfo>());
    //获取公共表格信息(需要获取的结果和测试结果相关需要传入测试结果)
    static QStringList getTableContent(const CLocation &location, const short &shTestMode, const CUserInfo &userInfo, const vector<CTestResultInfo> &vecTestResultInfo = vector<CTestResultInfo>(), bool bComplete = false);
    //获取得分表格内容,针对同步测试中的界面跳转(异步也存在)
    static QStringList getScoreTableContent(const CUserInfo &userInfo, const vector<CTestResultInfo> &vecTestResult, const short &shDevType);

    //-------------------------------- 相关表格操作 -----------------------------------


    //根据类型创建设备
    static CDevHardwareBase *createSteDevcice(unsigned int iDevID, ICommunicate &comunicate, const short &shDevType);

    //获取帧类型所对应的单包数据长度，在解析时会进行使用
    static int getSinglePackgeLenByDataType(const unsigned short &shDataFrameType);

    //是否显示识别摄像头
    static bool isHomePage(const short &shPage);

    //是否是测试界面
    static bool isTestPage(const short &shPage, const short &shTestMode);

    //
    static bool isHardwareComplete(const unsigned char &cHardwardState){return s_setHardwareCompleteFlag.find(cHardwardState) != s_setHardwareCompleteFlag.end();}

    static string getDevDescribe(const short &shDevType);
    static string getTestItemDescribe(const short &shTestItem);

    static bool isValidExtensionNum(const int &nDevExtensionNo);
    static QString getS_mapChaInfo();
    static QStringList getDevSignalChannelList(const short &devType);

    //转换本地设备类型与服务器设备类型
    static int getHttpTestItemFromItem(const short &shTestItem);
    static int getHttpDevTypeFromType(const short &shDevType);

    static short getTestItemFromHttpItem(const int &iHttpTestItem);
    static short getScoreTypeFromView( const int &viewScoreType );

    //通过底层状态位获取对应的测试状态 cStateByte为资源 cTestState存放测试状态
    static void getTestState(const unsigned char &cStateByte, unsigned char &cTestState);

    //获取当前硬件连接之后的展示模式(根据当前硬件类型和实际连接模式进行判断)
    static bool getDevDirectConnectFlag();

    //单包数据长度，根据测试类型获取(返回负数证明没有这个测试项目信息)
    static int getItemSignalPackgeLen(const short& shTestItem);

    //需要设置433信道的设备类型
    static bool is433DevType(const short &shDevType);

    //保存的帧号，递增
    static atomic<uint> s_nFrameNo;
protected:
    static set<unsigned short> initDataTypeCmd();
    static map<unsigned char, short> initHardwareTypeMapping();
    static map<short, unsigned short> initTestItemAndDataFrameTypeMapping();
    static map<short, short> initTestItemAndDevTypeTypeMapping();
    static set<short> initHomePage();
    static set<short> initSpeciaDev();
    static set<short> initViolationDev();
    static set<short> initViolationTestItem();
    static map<short, set<unsigned int>> initChaInfo();
    static set<unsigned char> initsHardwareCompleteFlag();
    static map<short, string> initTestItemDescribe();
    static map<short, string> initDevTypeDescribe();
    static map<short, int> initTestItemAndServerItem();
    static map<short, int> initDevTypeAndServerType();
    static map<short, int> initItemPackgeLen();
private:
    //测试项目 和 硬件测试项目
    const static map<short, unsigned short> s_mapTestItemAndDataFrameTypeMapping;

    //测试项目 和 服务器测试项目
    const static map<short, int> s_mapTestItemAndServerItemMapping;

    //设备类型 和 服务器设备类型
    const static map<short, int> s_mapDevTypeAndServerTypeMapping;

    //测试项目和对应的设备类型（软件中的类型和软件中的测试项目）
    const static map<short, short> s_mapTestItemAndDevTypeTypeMapping;

    //存储所有数据命令//底层传递的命令和数据类型，若是底层传递的是数据类型，那么直接匹配当前变量中的内容即可
    //若不是，需要将最高位置0，再判断命令
    const static set<unsigned short> s_setDataTypeCmd;

    //保存硬件编码与当前软件类型的映照关系 key：保存底层硬件编码 value:保存对应的类型
    //底层硬件转换为当前设备类型更常用，将底层硬件作为key提高查询速度
    const static map<unsigned char, short> s_mapHardwareTypeMapping;

    //存储软件当前界面是否属于主页，在主页才会进行人脸识别
    const static set<short> s_seHomePage;

    //当前软件中有几个类型的设备，中长跑 跳绳这些特殊设备
    const static set<short> s_setSpecialDev;

    //当前软件中有几个类型的设备，足球、篮球、排球、斜身、跳高、实心
    const static set<short> s_setViolationDev;

    //当前软件中有几个测试项，足球、篮球、排球、斜身、跳高、实心，查询时需特殊处理，加入违规次数
    const static set<short> s_setViolationItem;

    //保存软件中的信道 前面的为设备类型，后面的为对应的信道,方便设备与信道值的查找
    const static map<short, set<unsigned int>> s_mapChaInfo;

    //硬件完成标志
    const static set<unsigned char> s_setHardwareCompleteFlag;

    //测试项目及其字符描述
    const static map<short, string> s_mapTestItemDescribe;

    //设备及其字符描述
    const static map<short, string> s_mapDevDescribe;

    //项目单包数据长度
    const static map<short, int> s_mapItemPackgeLen;
};

#endif // CDevDataCfg_H
