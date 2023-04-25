#ifndef CSCORERULE_H
#define CSCORERULE_H
#include <vector>
#include "common_global.h"
using namespace std;

class COMMONSHARED_EXPORT CScoreRule
{
public:
    enum emQuery_Condition{
        Query_Type_Item, Query_Type_Grade};
public:
    CScoreRule();
    ~CScoreRule();

    void reset();

    //判断类型符合添加条件(数据库使用)
    bool isAddCorrect() const;

    //获取查询项类型(数据库查询和删除)
    static vector<short> getQueryItem(const CScoreRule &info);

    int m_nItem;
    int m_nGrade;
    bool m_bGender;
    float m_fValLeft;
    float m_fValRight;
    float m_fScore;
    float m_fAgeS, m_fAgeE;
    int   m_iVersion = 1;
};

#endif // CSCORERULE_H
