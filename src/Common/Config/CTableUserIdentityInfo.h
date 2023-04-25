#ifndef CTableUserIdentityInfo_H
#define CTableUserIdentityInfo_H
#include "CAbstractConfigInfo.h"
#include "CUserInfo.h"
#include <vector>
using namespace std;
class COMMONSHARED_EXPORT CTableUserIdentityInfo : public CAbstractConfigInfo
{
public:
    CTableUserIdentityInfo();
    ~CTableUserIdentityInfo() override;

    virtual bool Check_Config_Table() override;

public:
    //新增或者更新某个用户信息
    bool updateInfo(CUserInfo &userInfo);

    //批量更新
    bool updateInfos(vector<CUserInfo> &vecInfo);

    //查询某个学生的方法
    bool getUserInfo(const CUserInfo &userInfo, vector<CUserInfo> &vecUserInfo, bool bGetAll = false);
    bool getUserInfo(const CUserInfo &userInfo, vector<UserIdentityInfo> &vecUserInfo, bool bGetAll = false);

    static void equal(UserIdentityInfo &targeInfo, CUserInfo &srcInfo, bool bFlag = true);
    static void equal(CUserInfo &targeInfo, UserIdentityInfo &srcInfo, bool bFlag = true);

    //删除某个学生信息
    bool DeleteInfo(const long long &lUniqueID);
    bool DeleteInfo(vector<int> &vecDeleteinfo);
    bool DeleteInfoByTime(const long long &llTimeStamp);

    //删除全部学生信息
    bool DeleteAllInfo();

    //生成当前信息的条件
    bool gendrateQueryCriteria(const CUserInfo &Info, string &query);

    bool queryStudentByUniqueID(const QString &stuUniqueID , CUserInfo &srcUserInfo);

    //查找用户信息表中的最大时间戳
    QString queryMaxTimeStamp();

    //更新当前最大时间戳用户的时间戳信息( Java端服务器缺陷导致，心跳返回的最近更新时间用于去获取最新的用户信息不一定正确，所以，如果增量更新用户信息时，拉取下来的用户信息为空的话，
    //将该表中的最大时间戳变更为心跳中的最新更新时间 )
    void updateMaxTimeStamp(const QString &newMaxTimeStamp );

private:
    //生成单个查询条件
    static bool getQurty(short shType, const CUserInfo &Info, string &query);
};

#endif // CTableUserIdentityInfo_H
