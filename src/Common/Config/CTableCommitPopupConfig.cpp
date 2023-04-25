#include "CTableCommitPopupConfig.h"
#include "Logger/CLogger.h"

CTableCommitPopupConfig::CTableCommitPopupConfig()
{

}

bool CTableCommitPopupConfig::queryPopupConfig(CCommitPopupConfig &config)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool ok = true;
    bool isEmpty = false;
    try {
        vector<CommitPopupConfig> vecRets;
        vecRets = select<CommitPopupConfig>( *m_pDatabase ).all();

        for( auto &eachEle : vecRets ){
            equal( config, eachEle );
        }

        if( 0 == vecRets.size() ){
            isEmpty = true;
        }

    } catch (Except e) {
        ok = false;
    }
    unlock();

    if( isEmpty ){
        ok = insertDefaultConfig();
        if( ok ){
            ok = queryPopupConfig( config );
        }
    }

    return ok;
}

bool CTableCommitPopupConfig::updatePopupConfig(CCommitPopupConfig &config)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool ok = true;
    lock();

    try {
        vector<CommitPopupConfig> vecRets = select<CommitPopupConfig>( *m_pDatabase ).all();
        CommitPopupConfig infos(*m_pDatabase);
        if( vecRets.size() <= 0 ){
            equal( infos, config );
        }else {
            infos = vecRets[0];
            equal( infos, config );
        }

        infos.update();

    } catch (Except e) {
        ok = false;
    }
    unlock();

    return ok;
}

bool CTableCommitPopupConfig::insertDefaultConfig( DELBTN_ENABLE btnType, COMMIT_TYPE commitType,
                                                   SIGNATURE_EXIST signatureExist, int countTime, int userInfoPageExisted )
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool ok = true;
    lock();
    try {
        vector<CommitPopupConfig> vecRets = select<CommitPopupConfig>( *m_pDatabase ).all();
        if( vecRets.size() <= 0 ){
            CommitPopupConfig defaultConfig(*m_pDatabase);
            defaultConfig.delScoreType = static_cast<int>( btnType );
            defaultConfig.commitType = static_cast<int>( commitType );
            defaultConfig.signatureExisted = static_cast<int>( signatureExist );
            defaultConfig.autoCommitCountTime = countTime;

            defaultConfig.update();
        }
    } catch (Except e) {
        ok = false;
    }
    unlock();
    return ok;
}

bool CTableCommitPopupConfig::isAutoCommit()
{
    //2022/12.1 修改，  签字版存在，则为手动提交，签字版不存在，则是自动提交
    bool bRet = false;

    CCommitPopupConfig commitPopCfg;
    queryPopupConfig(commitPopCfg);

    if(commitPopCfg.m_commitType == Auto_Commit)
    {
        bRet = true;
    }

    return bRet;
}

void CTableCommitPopupConfig::equal(CCommitPopupConfig &target, CommitPopupConfig &src)
{
    target.m_delBtnEnable = src.delScoreType;
    target.m_commitType = src.commitType;
    target.m_signatureExisted = src.signatureExisted;
    target.m_autoCommitCountTime = src.autoCommitCountTime;
    target.m_userInfoPageExisted = src.userInfoPageExisted;
}

void CTableCommitPopupConfig::equal(CommitPopupConfig &target, CCommitPopupConfig &src)
{
    target.delScoreType = src.m_delBtnEnable;
    target.commitType = src.m_commitType;
    target.signatureExisted = src.m_signatureExisted;
    target.autoCommitCountTime = src.m_autoCommitCountTime;
    target.userInfoPageExisted = src.m_userInfoPageExisted;
}
