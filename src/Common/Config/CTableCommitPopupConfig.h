#ifndef CTABLECOMMITPOPUPCONFIG_H
#define CTABLECOMMITPOPUPCONFIG_H

#include "CAbstractConfigInfo.h"
#include "CCommitPopupConfig.h"
#include "common_global.h"

//弹窗配置只有一份, 如果不存在，则创建默认的

using namespace Database;

class COMMONSHARED_EXPORT CTableCommitPopupConfig : public CAbstractConfigInfo
{
public:
    enum DELBTN_ENABLE{
        Disable = 0,
        Enable
    };

    enum COMMIT_TYPE{
        Manul_Commit = 0,
        Auto_Commit,
    };

    enum SIGNATURE_EXIST{
        NO_Exist = 0,
        Exist
    };

    CTableCommitPopupConfig();

    //查询配置
    bool queryPopupConfig( CCommitPopupConfig &config );

    bool updatePopupConfig( CCommitPopupConfig &config);

    bool insertDefaultConfig( DELBTN_ENABLE btnType = Enable, COMMIT_TYPE commitType = Manul_Commit,
                              SIGNATURE_EXIST signatureExist = Exist, int countTime = 5,
                              int userInfoPageExisted = CommitPopupConfig::UserInfoPageExistedType::Exixted );

    //是否自动提交
    bool isAutoCommit();

    static void equal( CCommitPopupConfig &target, CommitPopupConfig &src );
    static void equal( CommitPopupConfig &target, CCommitPopupConfig &src );
};

#endif // CTABLECOMMITPOPUPCONFIG_H
