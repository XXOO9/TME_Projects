#include "CTestProcManage.h"
#include "CTestProcHeight.h"
#include "CTestProcWeight.h"
#include "CTestProcBCI.h"
#include "CTestProcVC.h"
#include "CTestProcPullUp.h"
#include "CTestProcRun1000.h"
#include "CTestProcRun50.h"
#include "CTestProcRun800.h"
#include "CTestProcSBJ.h"
#include "CTestProcSFB.h"
#include "CTestProcSitUp.h"
#include "CTestProcEyesight.h"
#include "CTestProcGrip.h"
#include "CTestProcPushup.h"
#include "CTestProcPVV.h"
#include "CTestProcSkip.h"
#include "CTestProcBasketball.h"
#include "CTestProcFootball.h"
#include "CTestProcVolleyball.h"
#include "CTestProcVerticalJump.h"
#include "CTestProcPUO.h"
#include "CTestProcSolidball.h"
#include "ProductRegistrar.h"
#include "commondefin.h"
#include "CTestStateManage.h"
#include "CLogger.h"

bool CTestProcManage::s_bInitProduct = false;

CTestProcManage::CTestProcManage()
{
}

CTestProcManage::~CTestProcManage()
{
    if(m_pCurTestProc != nullptr)
        delete m_pCurTestProc;

    uninitProduct();
}

void CTestProcManage::setCurTestItem(short shTestItem)
{
    if(m_pCurTestProc != nullptr)
    {
        if(m_pCurTestProc->getTestItem() != shTestItem)
            safeDeleteClassPoint(&m_pCurTestProc);
    }

    //暂不清楚放在构造函数中会存在问题
    initProduct();

    if(m_pCurTestProc == nullptr)
    {
        m_pCurTestProc = CProductFactory<CTestProcBasic>::Instance().GetProduct(shTestItem);
        if(m_pCurTestProc != nullptr)
        {
            m_pCurTestProc->setTestStateManage(m_pTestStateMange);
        }
    }
    else
    {
        m_pCurTestProc->reset();
    }
}

short CTestProcManage::getCurTestItem()
{
    short shTestItem = TEST_ITEM_UNKNOW;
    if(m_pCurTestProc != nullptr)
    {
        shTestItem = m_pCurTestProc->getTestItem();
    }

    return shTestItem;
}

short CTestProcManage::getCurTestItemTimesIndex() const
{
    short shTestItemTimesIndex = -1;
    if(m_pCurTestProc != nullptr)
    {
        shTestItemTimesIndex = m_pCurTestProc->getCurTestTimesIndex();
    }
    return  shTestItemTimesIndex;
}

string CTestProcManage::getcurTestItemDescribe() const
{
    string strTemp("");
    if(m_pCurTestProc != nullptr)
    {
        strTemp = m_pCurTestProc->getTestItemDescribe();
    }

    return strTemp;
}

void CTestProcManage::setCurTestState(short shTestState)
{
    if(m_pCurTestProc == nullptr)
    {
        LOGERROR("m_pCurTestProc invalid");
        return;
    }

    m_pCurTestProc->setTestState(shTestState);
}

short CTestProcManage::getCurTestState() const
{
    short shRet = CTestStateManage::TEST_NOT_START;

    if(m_pCurTestProc != nullptr)
    {
        shRet = m_pCurTestProc->getCurTestState();
    }

    return shRet;
}

QString CTestProcManage::getTestProcContent()
{
    QString qsTemp;
    if(m_pCurTestProc != nullptr)
    {
        qsTemp = m_pCurTestProc->getTestProcDispContent();
    }

    return qsTemp;
}

int CTestProcManage::getTestStartWaitTime()
{
    int nTimeMsec = CTestProcBasic::TEST_START_WAIT_TIME;

    if(m_pCurTestProc != nullptr)
    {
        nTimeMsec = m_pCurTestProc->getTestStartWaitTime();
    }

    return nTimeMsec;
}

void CTestProcManage::setTestStateManage(CTestStateManage *pManage)
{
    m_pTestStateMange = pManage;
    if(m_pCurTestProc != nullptr)
    {
        m_pCurTestProc->setTestStateManage(m_pTestStateMange);
    }
}

bool CTestProcManage::getTestCompleteFlag()
{
    bool bComplete = true;
    if(m_pCurTestProc != nullptr)
    {
        bComplete = m_pCurTestProc->getTestCompleteFlag();
    }

    return bComplete;
}

void CTestProcManage::loadTestCfg()
{
    if(m_pCurTestProc != nullptr)
    {
        m_pCurTestProc->loadTestCfg();
    }
}

void CTestProcManage::initProduct()
{
    //注册测试过程
    if(!s_bInitProduct)
    {
        new CProductRegistrar<CTestProcBasic,CTestProcHeight>(TEST_ITEM_HEIGHT);
        new CProductRegistrar<CTestProcBasic,CTestProcWeight>(TEST_ITEM_WEIGHT);
        new CProductRegistrar<CTestProcBasic,CTestProcBCI>(TEST_ITEM_HBC);
        new CProductRegistrar<CTestProcBasic,CTestProcVC>(TEST_ITEM_VC);
        new CProductRegistrar<CTestProcBasic,CTestProcPullUp>(TEST_ITEM_PULL_UP);

        //800 1000 1500使用的流程一致
        new CProductRegistrar<CTestProcBasic,CTestProcRun1000>(TEST_ITEM_RUN_1500);
        new CProductRegistrar<CTestProcBasic,CTestProcRun1000>(TEST_ITEM_RUN_1000);
        new CProductRegistrar<CTestProcBasic,CTestProcRun800>(TEST_ITEM_RUN_800);
        new CProductRegistrar<CTestProcBasic,CTestProcRun50>(TEST_ITEM_RUN_50);
        new CProductRegistrar<CTestProcBasic,CTestProcSBJ>(TEST_ITEM_SBJ);
        new CProductRegistrar<CTestProcBasic,CTestProcSFB>(TEST_ITEM_SFB);
        new CProductRegistrar<CTestProcBasic,CTestProcSitUp>(TEST_ITEM_SIT_UP);
        new CProductRegistrar<CTestProcBasic,CTestProcGrip>(TEST_ITEM_GRIP);
        new CProductRegistrar<CTestProcBasic,CTestProcEyesight>(TEST_ITEM_EYESIGHT);
        new CProductRegistrar<CTestProcBasic,CTestProcPushup>(TEST_ITEM_PUSHUP);
        new CProductRegistrar<CTestProcBasic,CTestProcPVV>(TEST_ITEM_PVV);
        new CProductRegistrar<CTestProcBasic,CTestProcSkip>(TEST_ITEM_ROPE_SKIP);
        new CProductRegistrar<CTestProcBasic,CTestProcBasketball>(TEST_ITEM_BASKETBALL);
        new CProductRegistrar<CTestProcBasic,CTestProcFootball>(TEST_ITEM_FOOTBALL);
        new CProductRegistrar<CTestProcBasic,CTestProcVolleyball>(TEST_ITEM_VOLLEYBALL);
        new CProductRegistrar<CTestProcBasic,CTestProcVerticalJump>(TEST_ITEM_VJUMP);
        new CProductRegistrar<CTestProcBasic,CTestProcPUO>(TEST_ITEM_PUO);
        new CProductRegistrar<CTestProcBasic,CTestProcSolidball>(TEST_ITEM_SOLIDBALL);
        s_bInitProduct = true;
    }
}

void CTestProcManage::uninitProduct()
{
    if(s_bInitProduct)
    {
        CProductFactory<CTestProcBasic>::Instance().Clear(true);

        s_bInitProduct = false;
    }
}
