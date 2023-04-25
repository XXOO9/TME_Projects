#include "CTableScoreRules.h"
#include "Logger/CLogger.h"

CTableScoreRules::CTableScoreRules()
{

}

bool CTableScoreRules::Check_Config_Table()
{
    return true;
}

bool CTableScoreRules::updateRule(CScoreRule &rule)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();

    if(!rule.isAddCorrect())
    {
        m_mutex.unlock();
        return false;
    }

    bool bRes = true;

    try
    {
        //float值不能作为判断条件只能通过查询之后的值进行判断
        vector<ScoreRules> vecSearch;
        string query;

        //生成条件出错标识rule不符合插入或者更新条件
        if(gendrateQueryCriteria(rule, query))
        {
            vecSearch = select<ScoreRules>(*m_pDatabase, RawExpr(query)).all();
            bool bExist = false;

            //更新
            for(auto &element : vecSearch)
            {
                if(isEqual(rule.m_fScore, element.score))
                {
                    bExist = true;
                    equal(element, rule);
                    element.update();
                    continue;
                }
            }

            //新增
            if(!bExist)
            {
                ScoreRules scoreRule(*m_pDatabase);
                equal(scoreRule, rule);
                scoreRule.update();
            }
        }
        else
        {
            bRes = false;
        }
    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

bool CTableScoreRules::updateRules(vector<CScoreRule> &vecRules)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try
    {
       m_pDatabase->begin();
       for(auto &element : vecRules)
       {
           ScoreRules scoreRule(*m_pDatabase);
           equal(scoreRule, element);
           scoreRule.update();
       }
       m_pDatabase->commit();
    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

bool CTableScoreRules::getRule(const CScoreRule &queryCondition, vector<CScoreRule> &vecRule, bool bGetAll)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();

    bool bRet = true;
    vecRule.clear();

    try
    {
        vector<ScoreRules> vecSearch;

        if(bGetAll)
        {
            vecSearch = select<ScoreRules>(*m_pDatabase).all();
        }
        else
        {
            string query;
            if(gendrateQueryCriteria(queryCondition, query))
            {
                vecSearch = select<ScoreRules>(*m_pDatabase, RawExpr(query)).all();
            }
            else
            {
                bRet = false;
            }
        }

        CScoreRule ruleTemp;
        for(auto &element : vecSearch)
        {
            equal(ruleTemp, element);
            vecRule.push_back(ruleTemp);
        }
    }
    catch (Except e)
    {
        bRet = false;
    }

    unlock();
    return bRet;
}

bool CTableScoreRules::deleteRule(const CScoreRule &queryCondition, bool bDeleteAll)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRet = true;

    try
    {
        vector<ScoreRules> vecSearch;

        if(bDeleteAll)
        {
            m_pDatabase->delete_(ScoreRules::table__, ScoreRules::Id != -1);
        }
        else
        {
            string query;
            if(gendrateQueryCriteria(queryCondition, query))
            {
                vecSearch = select<ScoreRules>(*m_pDatabase, RawExpr(query)).all();
                for(auto &element : vecSearch)
                {
                    element.del();
                }
            }
            else
            {
                bRet = false;
            }
        }

    }
    catch (Except e)
    {
        bRet = false;
    }
    unlock();
    return bRet;
}

bool CTableScoreRules::deleteAllRule()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();

    bool bRet = true;

    try
    {
        m_pDatabase->begin();

         m_pDatabase->delete_(ScoreRules::table__);

         m_pDatabase->commit();
    }
    catch (exception e)
    {
        bRet = false;
    }

     unlock();

     return bRet;
}

bool CTableScoreRules::gendrateQueryCriteria(const CScoreRule &queryCondition, string &query)
{
    //查询合法的条件
    vector<short> vecType = queryCondition.getQueryItem(queryCondition);

    size_t szType = vecType.size();
    if(szType == 0)
    {
        return false;
    }

    //清除查询值
    string strQueryTemp;

    bool bFirst = true, bSearch = false;
    for(auto &type : vecType)
    {
        if(bFirst)
        {
            getQurty(type, queryCondition, query);
            bFirst = false;
            bSearch = true;
        }
        else
        {
            if(getQurty(type, queryCondition, strQueryTemp))
            {
                query = (RawExpr(query) && RawExpr(strQueryTemp)).asString();
            }
        }
    }

    return bSearch;
}

bool CTableScoreRules::getQurty(short shType, const CScoreRule &queryCondition, string &query)
{
    bool bRet = true;
    switch (shType)
       {
           case CScoreRule::Query_Type_Item:
               query = (ScoreRules::TestItem == queryCondition.m_nItem).asString();
               break;
           case CScoreRule::Query_Type_Grade:
               query = (ScoreRules::Grade == queryCondition.m_nGrade).asString();
               break;
           default:
               bRet = false;
               break;
       }

    return bRet;
}

void CTableScoreRules::equal(CScoreRule &target, const ScoreRules &src)
{
    target.m_nItem       = src.testItem;
    target.m_nGrade      = src.grade;
    target.m_bGender     = src.gender;
    target.m_fValLeft    = src.valueLeft;
    target.m_fValRight   = src.valueRight;
    target.m_fScore      = src.score;
    target.m_fAgeS       = src.ageS;
    target.m_fAgeE       = src.ageE;
    target.m_iVersion    = src.version;
}

void CTableScoreRules::equal(ScoreRules &target, const CScoreRule &src)
{
    target.testItem         = src.m_nItem;
    target.grade            = src.m_nGrade;
    target.gender           = src.m_bGender;
    target.valueLeft        = src.m_fValLeft;
    target.valueRight       = src.m_fValRight;
    target.score            = src.m_fScore;
    target.ageS             = src.m_fAgeS;
    target.ageE             = src.m_fAgeE;
    target.version          = src.m_iVersion;
}


