///////////////////////////////////////////////////////////
//  CTestResultInfo.h
//  Implementation of the Class CTestResultInfo
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_B1E7033D_FF0C_4a00_993B_E54DDFE37C31_INCLUDED_)
#define EA_B1E7033D_FF0C_4a00_993B_E54DDFE37C31_INCLUDED_
#include "../common_global.h"

#include "CUserInfo.h"
#include "ste100database.hpp"
#include "commondefin.h"
#include "CStdString.h"
#include <QDebug>

#include <string>
#include <QString>
#include <QDateTime>

//表示犯规
#define TEST_RES_FOUL "-50"
//表示中止
#define TEST_RES_ABORT "-51"


//每次成绩中存在的违例成绩使用这个标志进行隔开 引体向上 结果 5-2;6-4;
//结果
const char g_separateCharIllegal = '*';

using namespace Database;
using namespace std;

class COMMONSHARED_EXPORT CTestResultInfo
{
public:
    enum emQuery_Condition{
        Query_Type_UniqueID, Query_Type_Item, Query_Type_Nature, Query_Type_Nature_Unequal , Query_Type_SyncServer, Query_Type_Time};

    //测试有两种情况，自由（游客模式）和非自由测试，非自由测试分两种正式测试和非正式测试
    enum emTestNature{
        TEST_UNKONW = 0, TEST_FORMAL, TEST_INFORMAL, TEST_FREE};

public:
    CTestResultInfo();

    //所有内容重置，包含测试信息
    void reset();
    //清空测试结果，只清空测试成绩(时间、结果、结果可信度，过程照片) 保留结果次数的意思是
    //比如 跳远结果(1.7;1.6;) 时间：(202007211435) 结果可信度(0;1;) 过程照片(1.png,2.png;3.png;)
    //若不保留测试次数，那么将对应的结果全部清空
    //保留之后  结果(;;)      时间: (202007211435) 结果可信度(0;0;) 过程照片(;;)保留的意思是根据结果能看出来测试了几次
    void clearResult(bool bRetentionTimes);

    //判断体测结果是否符合要求
    bool isResultCorrect() const;

    //获取查询条件
    static vector<short> getQueryItem(const CTestResultInfo &info);

    //设置资源存储路径
    void setResourceRootPath(string strUserID, string strRootPath, string strDateTime, string strDevID, string strDevDescribe);

    //添加成绩，按照固定的程序进行成绩添加,传入本次是否可信（后续设备存在违例次数，将违例次数也保存到数据库中）
    //成绩是以 如 引体向上 5;2; 表示两次测试一次是5一次是2
    //添加违例次数之后是 5*2;2*1; 表示两次测试 一次是5其中存在2次违例 另一次是 2 其中存在一次违例
    //变更符号为 * 要不会影响 坐位体前屈的 负数成绩
    void addTestResult(const string &strResult, bool bCredible, const uint &nIllegalTimes = 0);

    void updateTestResult(uint nIndex, const string &strResult, bool bCredible, const uint &nIllegalTimes = 0);

    ///------ 下面两个函数暂时分开并且功能有所区别，后续需求确定下来之后进行统一
    //获取有效最大结果，返回有效值的次数索引，小于0为无效
    int getValidMaxTestResult(string &strMax) const;
    //获取最后一次结果，返回值返回是否有效
    bool getLastTestResult(string &strRes) const;

    //最新的函数，获取违例次数
    int getValidMaxTestResult(string &strMax, uint &nIllegalTimes) const;
    //获取最后一次结果，返回值返回是否有效
    bool getLastTestResult(string &strRes, uint &nIllegalTimes) const;
    //返回值返回是否成功
    bool getTestResult(uint nIndex, string &strRes, uint &nIllegalTimes) const;

    //获取结果 传入的数据为 6-3;5-2; 中的 6-3（暂未进行传入资源的格式校验）
    static bool getDetailedRes(const string &src, string &strRes, uint &nIllegalTimes);

    //保存时间与时间戳转换
    static string getSaveTimeStamp(const QString &qsTime);
    static string getSaveTimeStamp(const QDateTime &datetime);
    static QString getTimeStamp(const string &strSaveTime);
    static long long changeSaveTimeToStamp(const string &strSaveTime);

    //返回 > 0 第一个更好 返回等于 0 两者相同，返回 < 0 第二个更好
    static short compare(const CTestResultInfo& one, const CTestResultInfo& two);
    
    //合并两次测试结果，中考项目添加合并两次测试结果选项，将传入的测试结果与当前测试结果合并
    bool mergeToCurrent(const CTestResultInfo& src);

    //获取测试次数
    short getTestTimes() const;

    //添加过程资源(图像资源) 添加的资源内部为 1.png,2.png;3.png,4.png; 表示的意思诶第一次测试的资源为 1 2两张图
    void addProcSrc(const string &src, bool bEnd = false);

    //传入对应哪一次的测试，若是小于零那么返回全部资源
    vector<string> getProcSrc(int nIndex) const;

    //分割字符串
    static void splitString(const string &strSrc, const string &delimit, vector<string>&result);
    static vector<string> splitString(const string &strSrc, string delimit, bool bKeepEmptyElement = true);

    /// -------------------- 获取结果函数在进行变更时需要每个获取函数都进行变更 ---------------
    //获取有效结果打印格式（在打印时调用,内部打印有效成绩）
    QString getPrintRes(QString unit) const;
    //获取上传结果(上传也是获取有效成绩)
    QString getUploadRes() const;
    //获取得分计算值
    bool getScoreRes(bool bMaxValid, float &fValue) const;


    //判断数据是否仅仅是记录，没有任何数据，仅有测试记录
    bool isLogData() const;

    long long m_llUniqueID{UNKNOWN_ID};   //用户唯一id
    int       m_nItem{TEST_ITEM_UNKNOW};  //项目
    int       m_nNature{TEST_UNKONW};     //测试性质 正式测试或者非正式测试
    string    m_strStartTime{""};         //测试时间,存的时间戳
    string    m_strConfidence{""};        //多次结果对应的多次结果置信度 取值范围为 0~1之间 取一位小数
    string    m_strMulitResult{""};       //测试多次结果
    string    m_strProcessDataPath{""};   //相关测试资源
    string    m_strResSignature{""};      //结果签名
    bool      m_bSyncServer{false};       //和服务器同步状态
    string    m_sDeviceID;                //设备号


    //临时添加一项 针对实际测试时用于记录中长跑的NFC卡号,仅在测试是中长跑时使用
    //在识别用户信息之后，进行跑步设备的刷卡时进行赋值
    unsigned int m_nRunNfcCardID;

    //针对不同的跑道赋值,从 0 开始,在测试是短跑时使用
    unsigned int m_nRunShortWay;

    //下面是查询选项
    bool m_bUseSyncServerToSelect{false};

    //使用不等号
    bool m_bUseUnequalNature{false};

    //是否增加按类型查询
    bool m_bQueryWithNature{true};

    //临时添加一项针对测试过程当中的资源存放根目录，上面的相关测试资源是详细的文件存储地址,
    //这里只是暂时进行保存的位置
    //例如身高体重进行测试的存放项目为 时间(年月日时分秒)-设备类型(vc)-设备号，内部文件包含名称为各个
    //测试项目如 身高 height1.png 或者其他格式jpg之类的 体重为 weight1.png，其他测试项如肺活量
    //为 vc1.png vc2.png vc3.png 以此类推,这里存放的是根目录地址
    string m_strPathResourceRoot;
};
#endif // !defined(EA_B1E7033D_FF0C_4a00_993B_E54DDFE37C31_INCLUDED_)
