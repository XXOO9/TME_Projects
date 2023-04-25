#include "CScoreRule.h"
#include "commondefin.h"

CScoreRule::CScoreRule()
    : m_nItem(TEST_ITEM_UNKNOW)
    , m_nGrade(-1)
    , m_bGender(false)
    , m_fValLeft(0.0f)
    , m_fValRight(0.0f)
    , m_fScore(0.0f)
{

}

CScoreRule::~CScoreRule()
{

}

void CScoreRule::reset()
{
    m_nItem = TEST_ITEM_UNKNOW;
    m_nGrade = -1;
    m_bGender = false;
    m_fValLeft = 0.0f;
    m_fValRight = 0.0f;
    m_fScore = 0.0f;
}

bool CScoreRule::isAddCorrect() const
{
    if(m_nItem == TEST_ITEM_UNKNOW)
        return false;

    if(m_nGrade < 0)
        return false;

    //目前认为不是0~100得分则格式不正确
    if(m_fScore < 0 || m_fScore > 100)
        return  false;

    return  true;
}

vector<short> CScoreRule::getQueryItem(const CScoreRule &info)
{
    vector<short> vecType;

    if(info.m_nItem != TEST_ITEM_UNKNOW)
        vecType.push_back(Query_Type_Item);

    if(info.m_nGrade > 0)
        vecType.push_back(Query_Type_Grade);

    return  vecType;
}
