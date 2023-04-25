#include "CTableTestItemInfo.h"
#include "commondefin.h"
#include "Logger/CLogger.h"

static TEST_ITEM_INFO defaultTestItemInfo[] = {
    {TEST_ITEM_HEIGHT,          "身高",            "cm",      "厘米"},
    {TEST_ITEM_WEIGHT,          "体重",            "kg",      "千克"},
    {TEST_ITEM_HBC,             "人体成分",         "",       ""},  //人体成分包含多个测试项不含单位
    {TEST_ITEM_VC,              "肺活量",           "ml",     "毫升"},
    {TEST_ITEM_SBJ,             "立定跳远",         "cm",      "厘米"},
    {TEST_ITEM_SFB,             "坐位体前屈",       "cm",     "厘米"},
    {TEST_ITEM_PULL_UP,         "引体向上",         "",        "个"},
    {TEST_ITEM_SIT_UP,          "仰卧起坐",         "",        "个"},
    {TEST_ITEM_RUN_1000,        "1000m跑",         "s",       "秒"},
    {TEST_ITEM_RUN_800,         "800m跑",          "s",       "秒"},
    {TEST_ITEM_RUN_50,          "50m跑",           "s",       "秒"},
    {TEST_ITEM_FOOTBALL,        "足球",            "s",       "秒"},
    {TEST_ITEM_BASKETBALL,      "篮球",            "s",       "秒"},
    {TEST_ITEM_VOLLEYBALL,      "排球",            "",        "个"},
    {TEST_ITEM_PUO,             "斜身引体向上",     "",        "个"},
    {TEST_ITEM_SOLIDBALL,       "实心球",          "",        "个"},
    {TEST_ITEM_VJUMP,           "纵跳摸高",        "cm",      "厘米"}
};

CTableTestItemInfo::CTableTestItemInfo()
{

}

CTableTestItemInfo::~CTableTestItemInfo()
{

}

bool CTableTestItemInfo::Check_Config_Table()
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    bool bRet = true;

    try
    {
        //获取默认实验个数
        int nDefaultItemSize = sizeof(defaultTestItemInfo) / sizeof (TEST_ITEM_INFO);
        bool bExit;
        for(int i = 0; i < nDefaultItemSize; i++)
        {
            TEST_ITEM_INFO &element = defaultTestItemInfo[i];
            bExit = select<TestItemInfo>(*m_pDatabase, TestItemInfo::TestItem == element.nItem).count() > 0;

            if(!bExit)
            {
                updateInfo(element.nItem, GBK2UTF8(element.strItemDescribe), GBK2UTF8(element.strUnit), GBK2UTF8(element.strUnitDes));
            }
        }
    }
    catch (Except e)
    {
        bRet = false;
    }

    return bRet;
}

bool CTableTestItemInfo::updateInfo(int nItem, string strItemDescribe, string strUnit, string strUnitDes)
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
        vector<TestItemInfo> vecInfo = select<TestItemInfo>(*m_pDatabase, TestItemInfo::TestItem == nItem).all();

        bool bExist = false;
        if(vecInfo.size() > 0)
        {
            auto element = *(vecInfo.begin());
            element.testItemDescribe = strItemDescribe;
            element.testItemUnit = strUnit;
            element.testItemUntiDescribe = strUnitDes;
            element.update();
            bExist = true;
        }

        if(!bExist)
        {
            TestItemInfo info(*m_pDatabase);
            info.testItem = nItem;
            info.testItemDescribe = strItemDescribe;
            info.testItemUnit = strUnit;
            info.testItemUntiDescribe = strUnitDes;
            info.update();
        }

    }
    catch (Except e)
    {
        bRet = false;
    }

    unlock();
    return bRet;
}

bool CTableTestItemInfo::getInfo(int nItem, string &strItemDescribe, string &strUnit, string &strUnitDes)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRet = false;

    try
    {
        vector<TestItemInfo> vecInfo = select<TestItemInfo>(*m_pDatabase, TestItemInfo::TestItem == nItem).all();
        if(vecInfo.size() > 0)
        {
            strItemDescribe = vecInfo[0].testItemDescribe;
            strUnit         = vecInfo[0].testItemUnit;
            strUnitDes      = vecInfo[0].testItemUntiDescribe;
            bRet = true;
        }
    }
    catch (Except e)
    {

    }

    unlock();
    return  bRet;
}

bool CTableTestItemInfo::deleteInfo(int nItem)
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
        m_pDatabase->delete_(TestItemInfo::type__, TestItemInfo::TestItem == nItem);
    }
    catch(Except e)
    {
        bRet = false;
    }
    unlock();
    return bRet;
}
