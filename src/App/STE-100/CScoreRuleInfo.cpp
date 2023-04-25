#include "CScoreRuleInfo.h"
#include "commondefin.h"
#include "CDatabaseManage.h"
#include "CInterAction.h"
#include "CDevDataCfg.h"

CScoreRuleInfo::CScoreRuleInfo()
{

}

CScoreRuleInfo::~CScoreRuleInfo()
{

}

void CScoreRuleInfo::onReloadScoreRule()
{
    reloadScoreRule();
}

//小于零的得分说明获取失败，大于0的得分则说明没问题
float CScoreRuleInfo::getScore(short shTestItem, const CUserInfo &userinfo, const float &fValue)
{
    float fScore = -1;

    int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();
    if(nSoftVersion == soft::VERSION_CD_PSB)
    {
        fScore = getScoreAge(shTestItem, userinfo, fValue);
    }
    else
    {
        fScore = getScoreGrad(shTestItem, userinfo, fValue);
    }

    return fScore;
}

void CScoreRuleInfo::reloadScoreRule()
{
    //将加载标志设置为未加载
    m_bLoadScoreCompleted = false;

    //提示正在加载得分规则
    CInterAction::Get_Instance().setViewErorrString(MINFO_LOAD_SCORE, !m_bLoadScoreCompleted);

    //增加查询资源
    m_semaphoreThread.release();

    //内部判断不会多次重启
    startThread();
}

void CScoreRuleInfo::dealThreadLogic()
{
    //从数据库查询数据，将数据根据规则放到容器中
    CTableScoreRules *pScoreRule = nullptr;

    pScoreRule = CDatabaseManage::GetInstance().getTabScoreRules();

    if(pScoreRule != nullptr)
    {

        CScoreRule rule;
        vector<CScoreRule> vecScoreRule;
        pScoreRule->getRule(rule, vecScoreRule, true);

        m_mutexSource.lock();

        int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();
        if(nSoftVersion == soft::VERSION_CD_PSB)
        {
            map<short, set<scoreRule_Age>> mapItemTemp;
            scoreRule_Age structScoreRule;

            m_mapScoreRule_Age.clear();
            m_mapScoreRule_Age[true] = mapItemTemp;
            m_mapScoreRule_Age[false] = mapItemTemp;

            //将实际得分放入对应的存放队列即可
            for(auto &scoreRule : vecScoreRule)
            {
                auto &mapItem = m_mapScoreRule_Age[scoreRule.m_bGender];
                structScoreRule.fScore = scoreRule.m_fScore;
                structScoreRule.fValueLeft = scoreRule.m_fValLeft;
                structScoreRule.fValueRight = scoreRule.m_fValRight;
                structScoreRule.shAge_s = static_cast<short>(scoreRule.m_fAgeS);
                structScoreRule.shAge_e = static_cast<short>(scoreRule.m_fAgeE);

                mapItem[static_cast<short>(scoreRule.m_nItem)].insert(structScoreRule);
            }
        }

        map<short, set<scoreRule_Grad>> mapItemTemp;
        scoreRule_Grad structScoreRule;

        m_mapScoreRule_Grad.clear();
        m_mapScoreRule_Grad[true] = mapItemTemp;
        m_mapScoreRule_Grad[false] = mapItemTemp;

        //将实际得分放入对应的存放队列即可
        for(auto &scoreRule : vecScoreRule)
        {
            auto &mapItem = m_mapScoreRule_Grad[scoreRule.m_bGender];
            structScoreRule.fScore = scoreRule.m_fScore;
            structScoreRule.fValueLeft = scoreRule.m_fValLeft;
            structScoreRule.fValueRight = scoreRule.m_fValRight;
            structScoreRule.nGrade = scoreRule.m_nGrade;
            mapItem[static_cast<short>(scoreRule.m_nItem)].insert(structScoreRule);
        }

        m_mutexSource.unlock();
    }

    m_bLoadScoreCompleted = true;

    //提示正在加载得分规则
    CInterAction::Get_Instance().setViewErorrString(MINFO_LOAD_SCORE, !m_bLoadScoreCompleted);
}

float CScoreRuleInfo::getScoreGrad(short shTestItem, const CUserInfo &userinfo, const float &fValue)
{
    //userinfo中存放m_nGrade为届别，真实的年级为 当前年限（2021） - m_nGrade + 1
    //得分规则中的年级就是指年级
    int nGrade = userinfo.getGrade();

    int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();
    if(nSoftVersion == soft::VERSION_QLNJXX)
    {
        nGrade = userinfo.getGradeNew();
    }

    float fScore = -1;

    short shDevType = CDevDataCfg::getDevTypeByTestItem(shTestItem);

    if(m_bLoadScoreCompleted)
    {
        m_mutexSource.lock();

        auto itFindGenderRule = m_mapScoreRule_Grad.find(userinfo.m_bGender);

        if(itFindGenderRule != m_mapScoreRule_Grad.end())
        {
            auto itFindItemRule = itFindGenderRule->second.find(shTestItem);

            if(itFindItemRule != itFindGenderRule->second.end())
            {
                //在对应项目对应的规则中找寻实际得分
                auto &setRule = itFindItemRule->second;

                //默认为 Valueleft <= fValue < valueRight,正常判断
                //跑步时间越短越好使用符号 Valueleft >= fValue > valueRight
                bool bUseNormal = !CDevDataCfg::isRunTimeDev(shDevType);

                bool bFind = false;
                for(auto &element: setRule)
                {
                    if(element.nGrade != nGrade)
                        continue;

                    if(bUseNormal)
                    {
                        if(element.fValueLeft <= fValue && fValue < element.fValueRight)
                            bFind = true;
                    }
                    else
                    {
                        if(element.fValueLeft >= fValue && fValue > element.fValueRight)
                            bFind = true;
                    }

                    if(bFind)
                    {
                        fScore = element.fScore;
                        break;
                    }
                }
            }
        }

        m_mutexSource.unlock();
    }

    return fScore;
}

float CScoreRuleInfo::getScoreAge(short shTestItem, const CUserInfo &userinfo, const float &fValue)
{
    float fScore = -1;

    short shAge = userinfo.getAge();
    short shDevType = CDevDataCfg::getDevTypeByTestItem(shTestItem);

    if(m_bLoadScoreCompleted)
    {
        m_mutexSource.lock();

        auto itFindGenderRule = m_mapScoreRule_Age.find(userinfo.m_bGender);

        if(itFindGenderRule != m_mapScoreRule_Age.end())
        {
            auto itFindItemRule = itFindGenderRule->second.find(shTestItem);

            if(itFindItemRule != itFindGenderRule->second.end())
            {
                //在对应项目对应的规则中找寻实际得分
                auto &setRule = itFindItemRule->second;

                //默认为 Valueleft <= fValue < valueRight,正常判断
                //跑步时间越短越好使用符号 Valueleft >= fValue > valueRight
                bool bUseNormal = !CDevDataCfg::isRunTimeDev(shDevType);

                bool bFind = false;
                for(auto &element: setRule)
                {
                    if(!(element.shAge_s <= shAge && shAge <= element.shAge_e))
                        continue;

                    if(bUseNormal)
                    {
                        if(element.fValueLeft <= fValue && fValue < element.fValueRight)
                            bFind = true;
                    }
                    else
                    {
                        if(element.fValueLeft >= fValue && fValue > element.fValueRight)
                            bFind = true;
                    }

                    if(bFind)
                    {
                        fScore = element.fScore;
                        break;
                    }
                }
            }
        }

        m_mutexSource.unlock();
    }

    return fScore;
}
