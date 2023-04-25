#ifndef CSCORERULEINFO_H
#define CSCORERULEINFO_H
#include "CThreadResourceCtrlBase.h"
#include "CUserInfo.h"
#include <QMutex>
#include "CScoreRule.h"
#include "commondefin.h"
using namespace std;

//用于加载得分以获取实际得分
//成绩规则解析，由外部触发，在服务器线程中更新信息之后更新
class CScoreRuleInfo : public CThreadResourceCtrlBase
{
    //定义得分规则
    struct scoreRule_Grad{
        float fScore;
        float fValueLeft;
        float fValueRight;
        int nGrade;

        //排序，在使用set时仅会使用到得分进行排序,需要重载小于符号
        bool operator<(const scoreRule_Grad &scoreRule) const
        {
            //年级小于或者的分小于都算小于
            if(nGrade < scoreRule.nGrade)
            {
                return true;
            }

            if(nGrade == scoreRule.nGrade)
            {
                if(fScore < scoreRule.fScore)
                {
                    return true;
                }
            }

            return  false;
        }
    };

    struct scoreRule_Age{
        float fScore;
        float fValueLeft;
        float fValueRight;
        short shAge_s;
        short shAge_e;

        //排序，在使用set时仅会使用到得分进行排序,需要重载小于符号,年龄段存在最小和最大
        //男女在年龄段划分时年龄段存在唯一性，只需要区分各自性别的最小年龄最大年龄之和即可
        bool operator<(const scoreRule_Age &scoreRule) const
        {
            //年龄相加小于即可
            bool bRet = false;

            if(shAge_s + shAge_e < scoreRule.shAge_s + scoreRule.shAge_e)
            {
                bRet = true;
            }
            else
            {
                if(shAge_s + shAge_e == scoreRule.shAge_s + scoreRule.shAge_e)
                {
                   bRet = compareFloating(fScore, scoreRule.fScore) < 0;
                }
            }

            return bRet;
        }
    };

    Q_OBJECT
public:
    //保存得分规则，得分规则结构体

    CScoreRuleInfo();
    virtual ~CScoreRuleInfo() override;
public slots:
    //由服务器更新发送消息调用当前函数，只在存在更新时发送或者第一次更新发送
    void onReloadScoreRule();

    float getScore(short shTestItem, const CUserInfo &userinfo, const float &fValue);

    //获取当前当前加载状态
    inline bool getLoadScoreState(){return !m_bLoadScoreCompleted;}
protected:
    //加载是在线程中加载
    void reloadScoreRule();

    virtual void dealThreadLogic() override;

    //按照成绩和年纪获取得分
    float getScoreGrad(short shTestItem, const CUserInfo &userinfo, const float &fValue);
    float getScoreAge(short shTestItem, const CUserInfo &userinfo, const float &fValue);
private:
    std::atomic<bool> m_bLoadScoreCompleted;

    QMutex m_mutexSource;

    //这里的得分规则按照多套存储格式进行划分（目前有两种，一种是按照年龄段，一种是按照年级）
    //存放得分规则的结构比较复杂，需要划分男女，划分项目 进行存放，以便在查找时更快速
    map<bool, map<short, set<scoreRule_Grad>>> m_mapScoreRule_Grad;
    map<bool, map<short, set<scoreRule_Age>>> m_mapScoreRule_Age;
};

#endif // CSCORERULEINFO_H
