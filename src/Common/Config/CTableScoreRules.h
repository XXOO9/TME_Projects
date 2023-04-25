#ifndef CFGSCORERULES_H
#define CFGSCORERULES_H
#include "CAbstractConfigInfo.h"
#include "CScoreRule.h"

class COMMONSHARED_EXPORT CTableScoreRules: public CAbstractConfigInfo
{
public:
    CTableScoreRules();

    virtual bool Check_Config_Table() override;
public:

    //进行添加和更新信息时以项目代号、年级、分数作为唯一标识符
    bool updateRule(CScoreRule &rule);

    bool updateRules(vector<CScoreRule> &vecRules);

    //进行获取时，以条件组合作为获取依据，条件包含项目、年级
    bool getRule(const CScoreRule &queryCondition, vector<CScoreRule> &vecRule, bool bGetAll = false);

    //进行删除时，以条件组合作为删除依据 条件包含 项目 年级 分数
    bool deleteRule(const CScoreRule &queryCondition, bool bDeleteAll = false);

    //删除全部判分规则
    bool deleteAllRule();

    bool gendrateQueryCriteria(const CScoreRule &queryCondition, string &query);

    bool getQurty(short shType, const CScoreRule &queryCondition, string &query);

    void equal(CScoreRule &target, const ScoreRules &src);
    void equal(ScoreRules &target, const CScoreRule &src);
};

#endif // CFGSCORERULES_H
