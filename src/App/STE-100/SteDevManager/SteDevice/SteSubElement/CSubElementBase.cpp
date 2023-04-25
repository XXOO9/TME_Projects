///////////////////////////////////////////////////////////
//  CSteDevTestUnitBaisc.cpp
//  Implementation of the Class CSteDevTestUnitBaisc
//  Created on:      08-1月-2022 11:19:51
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CSubElementBase.h"
#include "CDevDataCfg.h"

bool CSubElementBase::s_bNotifyTimeOut = false;

CSubElementBase::CSubElementBase(unsigned short shUnitID, unsigned int iElementID, IDevExcept* const pDevExp)
    : m_shUnitID(shUnitID)
    , m_iElementID(iElementID)
    , m_pDevExcept(pDevExp)
{
    connect(&m_objTestStateManage, &CTestStateManage::signalTestStateChange, this, &CSubElementBase::onTestStateChange);
}

CSubElementBase::~CSubElementBase()
{
    deleteAntiCheating();
}

void CSubElementBase::setConnectStatus(unsigned short usStatus)
{
    m_shConnect_Status = usStatus;

    //保证状态属于枚举
    if(m_shConnect_Status < STE_NO_CONNECT || m_shConnect_Status >= STE_CONNECT_COUNT)
    {
        m_shConnect_Status = STE_NO_CONNECT;
    }
}

unsigned short CSubElementBase::getConnectStatus()
{
    return m_shConnect_Status;
}

//外部统一调用此函数，判断此元素是否可用
bool CSubElementBase::getAvailableFlag()
{
    //绑定用户后，无法使用
    if(m_bBindFlag)
    {
        return false;
    }

    return true;
}

bool CSubElementBase::getConnectFlag()
{
    if(m_shConnect_Status == STE_NORMAL_CONNECT || m_shConnect_Status == STE_UNSTABLE_CONNECT)
    {
        return true;
    }

    return false;
}

//根据老版本体测软件对函数进行变更
uint32_t CSubElementBase::checkErr()
{
    //当前是低位两字节进行错误判定
    auto value = 0x0000FFFF & m_tLastTestState;

    if (nullptr == m_pDevExcept)
    {
        static short shTimes = 0;
        if(shTimes == 0)
        {
            LOGERROR("Invalid DevExcept Obj");
            shTimes++;
        }

        return DEVICE_NORMAL;
    }
    return m_pDevExcept->checkErr(value);
}

uint32_t CSubElementBase::getErrCode() const
{
    if (nullptr == m_pDevExcept)
    {
        static short shTimes = 0;
        if(shTimes == 0)
        {
            LOGERROR("Invalid DevExcept Obj");
            shTimes++;
        }

        return DEVICE_NORMAL;
    }
    return m_pDevExcept->getErrCode();
}

uint32_t CSubElementBase::getErrType() const
{
    if (nullptr == m_pDevExcept)
    {
        static short shTimes = 0;
        if(shTimes == 0)
        {
            LOGERROR("Invalid DevExcept Obj");
            shTimes++;
        }

        return TYPE_NORMAL;
    }
    return m_pDevExcept->getErrType();
}

uint32_t CSubElementBase::getLastErrCode() const
{
    if (nullptr == m_pDevExcept)
    {
        static short shTimes = 0;
        if(shTimes == 0)
        {
            LOGERROR("Invalid DevExcept Obj");
            shTimes++;
        }

        return DEVICE_NORMAL;
    }
    return m_pDevExcept->getLastErrCode();
}

uint32_t CSubElementBase::getLastErrType() const
{
    if (nullptr == m_pDevExcept)
    {
        static short shTimes = 0;
        if(shTimes == 0)
        {
            LOGERROR("Invalid DevExcept Obj");
            shTimes++;
        }

        return TYPE_NORMAL;
    }

    return m_pDevExcept->getLastErrType();
}

void CSubElementBase::clearErr()
{
    if (nullptr != m_pDevExcept)
    {
        m_pDevExcept->clearErr();
    }
}

void CSubElementBase::setElementInfo(void* pData, unsigned short shLength)
{
    if(pData == nullptr && shLength == 0)
        return;

    //对应通用设备来说，若在测试状态，但是重连之后，底层设备的状态为未测试状态，那么为超时结束
    if(getTestState() == CTestStateManage::TEST_ERR)
    {
        unsigned char cState;
        memcpy(&cState, &m_tLastTestState, 1);
        //将状态位转为测试状态
        CDevDataCfg::getTestState(cState, cState);
        if(cState == DEV_TEST_STATE_INVALID && m_bStartFlag)
        {
            setTestState(CTestStateManage::TEST_TIMEOUT);
        }
        else
        {
            //恢复上次正常状态
            restoreLastNormalState();
            //清除异常之后通知外部
            clearErr();
            notifyAbnormal();
        }
    }

    checkErr();

    /*
    //设备不在正常状态同时错误码不一致才进行通知外部处理
    if(getErrCode() != DEVICE_NORMAL && getLastErrCode() != getErrCode())
    {
        emit signalErr(m_shUnitID);
    }
    */

    //错误状态变更也通知外部
    if(getLastErrCode() != getErrCode())
    {
        emit signalErr(m_shUnitID);
    }

    m_bDataFrameUpdate = true;
}

string CSubElementBase::getErrDesc() const
{
    if (nullptr == m_pDevExcept)
    {
        static short shTimes = 0;
        if(shTimes == 0)
        {
            LOGERROR("Invalid DevExcept Obj");
            shTimes++;
        }

        return "";
    }
    return m_pDevExcept->getErrDesc(m_pDevExcept->getErrCode());
}

void CSubElementBase::onTestStateChange(short shTestStatus)
{
    //这里的状态是传到测试流程中进行处理，只包含4种，未测试 测试中 测试完成 测试出错
    //主要是测试人员对应的设备状态
    if(shTestStatus != CTestStateManage::TEST_ERR && m_shLastNormalTestState != shTestStatus)
        m_shLastNormalTestState = shTestStatus;

    switch (shTestStatus) {
    //设备全部处于这三种状态才变更灯光
    case CTestStateManage::TEST_NOT_START:
    case CTestStateManage::TEST_PREPARE:
    case CTestStateManage::TEST_CHECKED:
    {
        shTestStatus = CTestStateManage::TEST_NOT_START;
    }
        break;
    //需要第一次变更测试中才进行灯光状态重设
    case CTestStateManage::TEST_IN_PROCESS:
    {
        shTestStatus = CTestStateManage::TEST_IN_PROCESS;
    }
        break;
    //全部可用测试单元在未测状态
    case CTestStateManage::TEST_ERR:
    {
        shTestStatus = CTestStateManage::TEST_ERR;
    }
        break;
    case CTestStateManage::TEST_TIMEOUT:
    {
        shTestStatus = CTestStateManage::TEST_TIMEOUT;
    }
        break;
    case CTestStateManage::TEST_COMPLETED:
    {
        shTestStatus = CTestStateManage::TEST_COMPLETED;
    }
        break;
    default:
        shTestStatus = CTestStateManage::TEST_NOT_START;
        break;
    }

    //变更为测试中那么开启测试单元
    if(shTestStatus == CTestStateManage::TEST_IN_PROCESS)
    {
        startTest();
    }

    emit signalTestStateChange(m_shUnitID, shTestStatus);
}

CTestStateManage &CSubElementBase::getTestStateManage()
{
    return m_objTestStateManage;
}

short CSubElementBase::getCurTestState() const
{
    return m_objTestStateManage.getTestState();
}

void CSubElementBase::startTest()
{
    m_bStartFlag = true;

    //需要初始化变量信息

}

void CSubElementBase::stopTest()
{
    m_bStartFlag = false;

    m_bDataFrameUpdate = false;
}

void CSubElementBase::setAntiCheatingAvailable(bool bAvailable)
{
    m_bAntiCheatingAvailable = bAvailable;

    if(!bAvailable)
    {
        deleteAntiCheating();
    }
    else
    {
        createAntiCheating();
    }
}

void CSubElementBase::createAntiCheating()
{
    //子类创建之前需要先删除操作对象 使用函数 deleteAntiCheating

    //创建对应的防作弊对象

    if(!m_bAntiCheatingAvailable)
        return;

    //统一的防作弊处理，重写函数时先经过上面两步，再调用当前
    if(m_pAntiCheating != nullptr && m_pThreadAntiCheating == nullptr)
    {
        m_pThreadAntiCheating = new QThread();
        connect(m_pThreadAntiCheating, &QThread::finished, m_pThreadAntiCheating, &QObject::deleteLater);
        connect(m_pThreadAntiCheating, &QThread::finished, m_pAntiCheating, &QObject::deleteLater);

        m_pAntiCheating->moveToThread(m_pThreadAntiCheating);
        m_pThreadAntiCheating->start();
    }
}

CTestProcAntiCheatingBasic *CSubElementBase::getAntiCheating()
{
    return m_pAntiCheating;
}

void CSubElementBase::deleteAntiCheating()
{
    stopAntiCheating(false);

    if(m_pThreadAntiCheating != nullptr)
    {
        m_pThreadAntiCheating->quit();
    }

    m_pAntiCheating = nullptr;
    m_pThreadAntiCheating = nullptr;
}

void CSubElementBase::startAntiCheating()
{
    if(m_pAntiCheating == nullptr)
        return;

    m_pAntiCheating->start_();
}

void CSubElementBase::stopAntiCheating(bool bNormalStop)
{
    if(m_pAntiCheating == nullptr)
        return;

    m_pAntiCheating->stop_(bNormalStop);
}

void CSubElementBase::setCurTestItem(short shTestItem)
{
    m_shTestItem = shTestItem;

   m_shDevFrameType = CDevDataCfg::getFrameTypeByTestItem(m_shTestItem);
    if(m_shDevFrameType == DATA_FRAME_TYPE_NULL)
    {
        LOGERROR("setCurTestItem error TestItem is:" + to_string(shTestItem));
    }
}

void CSubElementBase::setElementID(const unsigned int &nID)
{
    m_iElementID = nID;
}

bool CSubElementBase::isCheckErrCode()
{
    short shCurTestState = getTestState();
    return shCurTestState != CTestStateManage::TEST_NOT_START;
}

void CSubElementBase::notifyAbnormal()
{
    //若存在异常那么通知一次
    if(getErrCode() != DEVICE_NORMAL)
    {
        emit signalErr(m_shUnitID);
    }
}

bool CSubElementBase::isCompleteFlagDif(const short &shTestItem)
{
    if(shTestItem == TEST_ITEM_UNKNOW || m_pElementDataBase == nullptr)
    {
        return false;
    }

    return m_pElementDataBase->isCompleteFlagDif();
}

bool CSubElementBase::isCompleteFlagSame(const short &shTestItem)
{
    if(shTestItem == TEST_ITEM_UNKNOW || m_pElementDataBase == nullptr)
    {
        return false;
    }

    return m_pElementDataBase->isCompleteFlagSame();
}

void CSubElementBase::setComplete(const short &shTestItem)
{
    if(shTestItem == TEST_ITEM_UNKNOW || m_pElementDataBase == nullptr)
    {
        return;
    }

    m_pElementDataBase->m_bCompleteExternal = true;
}
