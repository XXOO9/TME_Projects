#include "CDataSynchronize.h"
#include "CHttpRequestMgr.h"
#include "Logger/CLogger.h"
#include <QVector>

CDataSynchronize::CDataSynchronize(QObject *parent)
    :QObject (parent)
{

}

CDataSynchronize::~CDataSynchronize()
{

}

void CDataSynchronize::equalUserInfo(CUserInfo &targetUserInfo, const QMap<QString, QString> &srcUserInfo)
{
    targetUserInfo.m_llUniqueID       = srcUserInfo.find(USERIDENTIFY_KEY_ID).value().toLongLong();
    targetUserInfo.m_nRoleType        = srcUserInfo.find(USERIDENTIFY_KEY_ROLE).value().toInt();
    targetUserInfo.m_strName          = srcUserInfo.find(USERIDENTIFY_KEY_NAME).value();
    targetUserInfo.m_strBrithday      = srcUserInfo.find(USERIDENTIFY_KEY_BIRTHDAY).value();
    targetUserInfo.m_bGender          = srcUserInfo.find(USERIDENTIFY_KEY_GENDER).value().toInt() >= 1;
    targetUserInfo.m_strStudentId     = srcUserInfo.find(USERIDENTIFY_KEY_CODE).value();
    targetUserInfo.m_nGrade           = srcUserInfo.find(USERIDENTIFY_KEY_YEAR).value().toInt();
    targetUserInfo.m_nClass           = srcUserInfo.find(USERIDENTIFY_KEY_CLASSID).value().toInt();
    targetUserInfo.m_strNation        = srcUserInfo.find(USERIDENTIFY_KEY_NATION).value();
    targetUserInfo.m_strFaceFeature   = srcUserInfo.find(USERIDENTIFY_KEY_FACEFEATURE).value();
    targetUserInfo.m_strCampusMagcard = srcUserInfo.find(USERIDENTIFY_KEY_CARDNO).value();
    targetUserInfo.longlongTimeStamp  = srcUserInfo.find(USERIDENTIFY_KEY_UPDATETIME).value().toLongLong();

    auto itFind = srcUserInfo.find( USERIDENTIFY_KEY_STAGE );
    if(itFind != srcUserInfo.end())
    {
        targetUserInfo.m_nStage           = itFind.value().toInt();
    }
}

void CDataSynchronize::equalClassInfo(CClassInfo &targetClassInfo, const QMap<QString, QString> &srcClassInfo)
{
    targetClassInfo.m_nUniqueID         = srcClassInfo.find(SYNCCLASSINFO_KEY_ID).value().toInt();
    targetClassInfo.m_strClassDescribe  = srcClassInfo.find(SYNCCLASSINFO_KEY_DESCRIPTION).value().toStdString();
    targetClassInfo.m_timeStamp         = srcClassInfo.find(SYNCCLASSINFO_KEY_UPDATETIME).value().toStdString();
}

void CDataSynchronize::equalScoreInfo(CScoreRule &targetScoreInfo, const QMap<QString, QString> &srcScoreInfo)
{
    targetScoreInfo.m_bGender   = srcScoreInfo.find(SYNCSCORE_GENDER).value().toInt() >= 1;
    targetScoreInfo.m_nItem     = srcScoreInfo.find(SYNCSCORE_TESTITEM).value().toInt();
    targetScoreInfo.m_fScore    = srcScoreInfo.find(SYNCSCORE_SCORE).value().toFloat();
    targetScoreInfo.m_nGrade    = srcScoreInfo.find(SYNCSCORE_GRADE).value().toInt();
    targetScoreInfo.m_fValLeft  = srcScoreInfo.find(SYNCSCORE_RESULTSTART).value().toFloat();
    targetScoreInfo.m_fValRight = srcScoreInfo.find(SYNCSCORE_RESULTEND).value().toFloat();

    if(srcScoreInfo.find(SYNCSCORE_AGESTART) != srcScoreInfo.end())
    {
        targetScoreInfo.m_fAgeS     = srcScoreInfo.find(SYNCSCORE_AGESTART).value().toFloat();
    }

    if(srcScoreInfo.find(SYNCSCORE_AGEEND) != srcScoreInfo.end())
    {
        targetScoreInfo.m_fAgeE     = srcScoreInfo.find(SYNCSCORE_AGEEND).value().toFloat();
    }

    if(srcScoreInfo.find(SYNCSCORE_VERSION) != srcScoreInfo.end())
    {
        targetScoreInfo.m_iVersion = srcScoreInfo.find(SYNCSCORE_VERSION).value().toInt();
    }
}

//更新用户信息表
void CDataSynchronize::onUpdateUserInfo()
{
    LOGINFO("+++++ Update UserInfo");
    m_bUserFlag = true;

    //只有在系统空闲状态才更新
    if(!m_bAvailableFlag)
        return;

    /// 0.通知外部正在同步用户信息
    emit sigSyncStatusChanged(SYNC_PROCESS_USER);

    /// 1.同步用户信息
    //1.0 安全保护(理论上一定能通过)
    CHttpRequestMgr *pHttp = CHttpRequestMgr::getInstance();
    if(nullptr == pHttp)
    {
        emit sigSyncStatusChanged(SYNC_FAILED_USER);
        LOGERROR("http is nullptr");
        return;
    }

    auto pTableUserInfo = CDatabaseManage::GetInstance().getTableUserIdentityInfo();
    if(nullptr == pTableUserInfo)
    {
        emit sigSyncStatusChanged(SYNC_FAILED_USER);
        LOGERROR("tableuserinfo is nullptr");
        return;
    }
    auto pTableSyncTime = CDatabaseManage::GetInstance().getTabDataSyncTime();
    if(nullptr == pTableSyncTime)
    {
        emit sigSyncStatusChanged(SYNC_FAILED_USER);
        LOGERROR("tablesynctime is nullptr");
        return;
    }

    bool bUpdate;

    //1.1 根据时间戳信息，删除小于时间戳信息的用户，若为空则不需要操作
    QString qsSplitTime = pHttp->getUserSpliTime();
    if(qsSplitTime != "")
    {
        bUpdate = pTableUserInfo->DeleteInfoByTime(qsSplitTime.toLongLong());

        //删除失败
        if(!bUpdate)
        {
            emit sigSyncStatusChanged(SYNC_FAILED_USER);
            LOGERROR("delete userinfo by time failed");
            return;
        }
    }

    //1.2 根据删除用户表，删除指定需要删除的用户信息
    QVector<QString> qvDelUser = pHttp->getDelUsers();
    if(qvDelUser.size() > 0)
    {
        vector<int> vecDel;
        for(int i = 0; i < qvDelUser.size(); i++)
        {
            vecDel.push_back(qvDelUser[i].toInt());
        }

        bUpdate = pTableUserInfo->DeleteInfo(vecDel);

        //删除失败
        if(!bUpdate)
        {
            emit sigSyncStatusChanged(SYNC_FAILED_USER);
            LOGERROR("delete userinfo failed");
            return;
        }
    }

    //1.3 根据用户信息表，增加需要额外添加的用户信息
    QVector<QMap< QString, QString >> userInfos = pHttp->getVecUserInfo();
    if(userInfos.size() > 0)
    {
        vector<CUserInfo> users;
        CUserInfo user;
        long long llTimeStamp = 0;

        //将信息变更为每1000条数据进行一次数据插入
        unsigned int nSize = 1000;
        unsigned int i = 0;
        users.resize(nSize);

        for(auto &element : userInfos)
        {
            equalUserInfo(user, element);

            users[i++] = user;
            if(llTimeStamp < user.longlongTimeStamp)
            {
                llTimeStamp = user.longlongTimeStamp;
            }

            if(i >= nSize || &element == &userInfos.back())
            {
                if(i != nSize)
                {
                    users.resize(i);
                }

                bUpdate = pTableUserInfo->updateInfos(users);

                i = 0;

                //更新失败
                if(!bUpdate)
                {
                    emit sigSyncStatusChanged(SYNC_FAILED_USER);
                    LOGERROR("update userinfo failed");
                    return;
                }

                bUpdate = pTableSyncTime->updateSyncTime(CTableDataSyncTime::TABLE_TYPE_USER_INFO, QDateTime::fromMSecsSinceEpoch(llTimeStamp));
                //更新最新时间戳失败
                if(!bUpdate)
                {
                    emit sigSyncStatusChanged(SYNC_FAILED_USER);
                    LOGERROR("update synctime failed");
                    return;
                }
            }
        }
    }
    // 2022.12.14， 如果新增同步下来的学生数量为0， 则把当前学生表里面的最大时间戳改为心跳返回的最大时间戳
    //else{
        QString serverMaxTimeStamp = pHttp->getPMultiHeartBeatInfos()->lastSyncTime();
        serverMaxTimeStamp = QString::number( QDateTime::fromString( serverMaxTimeStamp, "yyyyMMddhhmmss" ).toMSecsSinceEpoch() );
        CDatabaseManage::GetInstance().getTableUserIdentityInfo()->updateMaxTimeStamp( serverMaxTimeStamp );
    //}

    //2.通知外部同步完成(不论成功或失败)
    emit sigSyncStatusChanged(SYNC_COMPLETE_USER);

    /// 3.修改完成标志
    m_bUserFlag = false;

    /// 4.是否结束线程
    m_bIsUpdateUser = true;

    if(m_bIsUpdateUser && m_bIsUpdateClass && m_bIsUpdateScore)
    {
        emit sigStopWork();
    }
}

//更新班级信息表
void CDataSynchronize::onUpdateClassInfo()
{
    LOGINFO("+++++ Update ClassInfo");
    m_bClassFlag = true;

    //只有在系统空闲状态才更新
    if(m_bAvailableFlag)
    {
        /// 0.通知外部正在同步用户信息
        emit sigSyncStatusChanged(SYNC_PROCESS_CLASS);

        /// 1.同步用户信息
        //1.0 安全保护(理论上一定能通过)
        CHttpRequestMgr *pHttp = CHttpRequestMgr::getInstance();
        if(nullptr == pHttp)
        {
            emit sigSyncStatusChanged(SYNC_FAILED_CLASS);
            LOGERROR("phttp is nullptr");
            return;
        }

        auto pTabClassInfo = CDatabaseManage::GetInstance().getTabClassInfo();
        if(nullptr == pTabClassInfo)
        {
            emit sigSyncStatusChanged(SYNC_FAILED_CLASS);
            LOGERROR("tableclassinfo is nullptr");
            return;
        }
        auto pTableSyncTime = CDatabaseManage::GetInstance().getTabDataSyncTime();
        if(nullptr == pTableSyncTime)
        {
            emit sigSyncStatusChanged(SYNC_FAILED_CLASS);
            LOGERROR("tablesynctime is nullptr");
            return;
        }

        bool bUpdate;

        //1.1 根据时间戳信息，删除小于时间戳信息的班级，若为空则不需要操作
        QString qsSplitTime = pHttp->getClassSpliTime();
        if(qsSplitTime != "")
        {
            bUpdate = pTabClassInfo->DeleteInfoByTime(qsSplitTime.toLongLong());

            //删除失败
            if(!bUpdate)
            {
                emit sigSyncStatusChanged(SYNC_FAILED_CLASS);
                LOGERROR("delete classinfo by time failed");
                return;
            }
        }

        //1.2 根据删除班级表，删除指定需要删除的班级信息
        QVector<QString> qvDelClass = pHttp->getDelClass();
        if(qvDelClass.size() > 0)
        {
            vector<int> vecDel;
            for(int i = 0; i < qvDelClass.size(); i++)
            {
                vecDel.push_back(qvDelClass[i].toInt());
            }

            bool bUpdate = false;

            bUpdate = pTabClassInfo->DeleteInfo(vecDel);

            //删除失败
            if(!bUpdate)
            {
                emit sigSyncStatusChanged(SYNC_FAILED_CLASS);
                LOGERROR("delete classinfo failed");
                return;
            }
        }

        //1.3 根据班级信息表，增加需要额外添加的班级信息
        QVector<QMap< QString, QString >> classInfos = pHttp->getVecClassInfo();
        if(classInfos.size() > 0)
        {
            vector<CClassInfo> classes;
            CClassInfo classInfo;
            long long llTimeStamp = 0;

            for(auto &element : classInfos)
            {
                equalClassInfo(classInfo, element);

                classes.push_back(classInfo);

                if(llTimeStamp < element.find(SYNCCLASSINFO_KEY_UPDATETIME).value().toLongLong())
                {
                    llTimeStamp = element.find(SYNCCLASSINFO_KEY_UPDATETIME).value().toLongLong();
                }
            }

            bUpdate = true;
            bUpdate &= pTabClassInfo->updateClassInfos(classes);
            //更新失败
            if(!bUpdate)
            {
                emit sigSyncStatusChanged(SYNC_FAILED_CLASS);
                LOGERROR("update classinfo failed");
                return;
            }

            //1.4 更新用户表对应的同步时间戳(取上述信息中的时间戳最大值)
            bUpdate = false;
            bUpdate = pTableSyncTime->updateSyncTime(CTableDataSyncTime::TABLE_TYPE_CLASS_INFO, QDateTime::fromMSecsSinceEpoch(llTimeStamp));
            //更新失败
            if(!bUpdate)
            {
                emit sigSyncStatusChanged(SYNC_FAILED_CLASS);
                LOGERROR("update synctime failed");
                return;
            }
        }

        //2.通知外部同步完成(不论成功或失败)
        emit sigSyncStatusChanged(SYNC_COMPLETE_CLASS);

        /// 3.修改完成标志
        m_bClassFlag = false;

        /// 4.是否结束线程
        m_bIsUpdateClass = true;

        if(m_bIsUpdateUser && m_bIsUpdateClass && m_bIsUpdateScore)
        {
            emit sigStopWork();
        }
    }
}

//更新成绩标准表
void CDataSynchronize::onUpdateScoreInfo()
{
    LOGINFO("+++++ Update ScoreInfo");
    m_bScoreFlag = true;

    //只有在系统空闲状态才更新
    if(m_bAvailableFlag)
    {
        /// 0.通知外部正在同步成绩信息
        emit sigSyncStatusChanged(SYNC_PROCESS_SCORE);

        /// 1.同步成绩信息
        //1.0 安全保护(理论上一定能通过)
        CHttpRequestMgr *pHttp = CHttpRequestMgr::getInstance();
        if(nullptr == pHttp)
        {
            emit sigSyncStatusChanged(SYNC_FAILED_SCORE);
            LOGERROR("phttp is nullptr");
            return;
        }

        auto pTableScoreRules = CDatabaseManage::GetInstance().getTabScoreRules();
        if(nullptr == pTableScoreRules)
        {
            emit sigSyncStatusChanged(SYNC_FAILED_SCORE);
            LOGERROR("tablescore is nullptr");
            return;
        }

        //1.1 获取得分规则
        QVector<QMap< QString, QString >> scoreStd = pHttp->getVecScoreStd();

        //1.2 判断得分规则是否为空
        if(scoreStd.size() > 0)
        {
            //1.3 删除本地得分规则
            CScoreRule role;
            pTableScoreRules->deleteRule(role, true);

            //1.4 存入服务器得分规则
            vector<CScoreRule> rules;
            for (auto &element : scoreStd)
            {
                equalScoreInfo(role, element);

                rules.push_back(role);
            }
            bool bUpdate = true;
            bUpdate &= pTableScoreRules->updateRules(rules);

            //更新失败
            if(!bUpdate)
            {
                emit sigSyncStatusChanged(SYNC_FAILED_SCORE);
                LOGERROR("update score failed");
                return;
            }
        }

        /// 2.通知外部成绩同步成功
        emit sigSyncStatusChanged(SYNC_COMPLETE_SCORE);

        /// 3.修改完成标志
        m_bScoreFlag = false;

        /// 4.是否结束线程
        m_bIsUpdateScore = true;

        if(m_bIsUpdateUser && m_bIsUpdateClass && m_bIsUpdateScore)
        {
            emit sigStopWork();
        }
    }
}

//状态更新消息
void CDataSynchronize::onDevTestStateChange(short shTestStatus)
{
    //1.当状态变为未测试时，允许更新
    if(shTestStatus == 0)
    {
        m_bAvailableFlag = true;

        if(m_bUserFlag)
        {
            onUpdateUserInfo();
        }

        if(m_bClassFlag)
        {
            onUpdateClassInfo();
        }

        if(m_bScoreFlag)
        {
            onUpdateScoreInfo();
        }
    }
    else
    {
        m_bAvailableFlag = false;
    }
}
