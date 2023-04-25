#include "CDevDataCfg.h"
#include "SteDevManager/SteDevice/CSteCommonDefin.h"
#include "commondefin.h"
#include "SteTestGuide/TestMode.h"
#include "SteTestGuide/CTestGuideRunShort.h"
#include "SteTestGuide/CTestGuideEyesight.h"

#include "SteDevManager/SteDevice/CDevSteBody.h"
#include "SteDevManager/SteDevice/CDevSteVC.h"
#include "SteDevManager/SteDevice/CDevSteSBJ.h"
#include "SteDevManager/SteDevice/CDevSteSFB.h"
#include "SteDevManager/SteDevice/CDevStePullUp.h"
#include "SteDevManager/SteDevice/CDevSteSitUp.h"
#include "SteDevManager/SteDevice/CDevSteRunShort.h"
#include "SteDevManager/SteDevice/CDevSteRunMid.h"
#include "SteDevManager/SteDevice/CDevSteSkip.h"
#include "SteDevManager/SteDevice/CDevSteGrip.h"
#include "SteDevManager/SteDevice/CDevSteEyesight.h"
#include "SteDevManager/SteDevice/CDevStePushup.h"
#include "SteDevManager/SteDevice/CDevStePVV.h"
#include "SteDevManager/SteDevice/CDevSteFootball.h"
#include "SteDevManager/SteDevice/CDevSteBasketball.h"
#include "SteDevManager/SteDevice/CDevSteVolleyball.h"
#include "SteDevManager/SteDevice/CDevStePUO.h"
#include "SteDevManager/SteDevice/CDevSteVertivalJump.h"
#include "SteDevManager/SteDevice/CDevSteSolidball.h"

#include "CInterAction.h"

#include "HttpClient/CHttpConfigCommonDefine.h"
#include "Config/CDatabaseManage.h"

using namespace DataFrameInfo;
using namespace HardwarePropertye;
using namespace TestTableCfg;

const map<short, unsigned short> CDevDataCfg::s_mapTestItemAndDataFrameTypeMapping(initTestItemAndDataFrameTypeMapping());
const set<unsigned short> CDevDataCfg::s_setDataTypeCmd(initDataTypeCmd());
const map<unsigned char, short> CDevDataCfg::s_mapHardwareTypeMapping(initHardwareTypeMapping());
const map<short, short> CDevDataCfg::s_mapTestItemAndDevTypeTypeMapping(initTestItemAndDevTypeTypeMapping());
const set<short> CDevDataCfg::s_seHomePage(initHomePage());
const set<short> CDevDataCfg::s_setSpecialDev(initSpeciaDev());
const set<short> CDevDataCfg::s_setViolationDev(initViolationDev());
const set<short> CDevDataCfg::s_setViolationItem(initViolationTestItem());
const map<short, set<unsigned int>> CDevDataCfg::s_mapChaInfo(initChaInfo());
const set<unsigned char> CDevDataCfg::s_setHardwareCompleteFlag(initsHardwareCompleteFlag());
const map<short, string> CDevDataCfg::s_mapTestItemDescribe(initTestItemDescribe());
const map<short, string> CDevDataCfg::s_mapDevDescribe(initDevTypeDescribe());
const map<short, int> CDevDataCfg::s_mapTestItemAndServerItemMapping(initTestItemAndServerItem());
const map<short, int> CDevDataCfg::s_mapDevTypeAndServerTypeMapping(initDevTypeAndServerType());
const map<short, int> CDevDataCfg::s_mapItemPackgeLen(initItemPackgeLen());

atomic<uint> CDevDataCfg::s_nFrameNo(0);

CDevDataCfg::CDevDataCfg()
{

}

void CDevDataCfg::delayMSec(int msec)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

unsigned short CDevDataCfg::getFrameTypeByTestItem(short shTestItem)
{
    unsigned short shFrameType = DATA_FRAME_TYPE_NULL;

    auto itFind = s_mapTestItemAndDataFrameTypeMapping.find(shTestItem);
    if(itFind != s_mapTestItemAndDataFrameTypeMapping.end())
    {
        shFrameType = itFind->second;
    }

    return shFrameType;
}

short CDevDataCfg::getTestItemByFrameType(unsigned short shDataFrameType)
{
    short shTestItem = TEST_ITEM_UNKNOW;

    for(auto &element : s_mapTestItemAndDataFrameTypeMapping)
    {
        if(element.second == shDataFrameType)
        {
            shTestItem = element.first;
        }
    }

    return shTestItem;
}

short CDevDataCfg::getDevTypeByTestItem(const short &shTestItem)
{
    auto itFind = s_mapTestItemAndDevTypeTypeMapping.find(shTestItem);
    short shDevType = DEV_TYPE_UNKNOW;
    if(itFind != s_mapTestItemAndDevTypeTypeMapping.end())
    {
        shDevType = itFind->second;
    }

    return shDevType;
}

short CDevDataCfg::getTestItemByDevType(const short &shDevType)
{
    short shTestItem = TEST_ITEM_UNKNOW;
    for(auto &element : s_mapTestItemAndDevTypeTypeMapping)
    {
        if(element.second == shDevType)
        {
            shTestItem = element.first;
            break;
        }
    }

    return shTestItem;
}

short CDevDataCfg::getDevTypeByChaNo(const unsigned int &nChaNo)
{
    short shDevType = DEV_TYPE_UNKNOW;
    for(auto &element : s_mapChaInfo)
    {
        if(element.second.find(nChaNo) != element.second.end())
        {
            shDevType = element.first;
        }
    }

    return shDevType;
}

bool CDevDataCfg::isSpecialDev(const short &shDevType)
{
    return s_setSpecialDev.find(shDevType) != s_setSpecialDev.end();
}

bool CDevDataCfg::isNeedViolationInfoDev(const short &shDevType)
{
    return s_setViolationDev.find(shDevType) != s_setViolationDev.end();
}

bool CDevDataCfg::isNeedViolationInfoItem(const short &shItem)
{
    return s_setViolationItem.find(shItem) != s_setViolationItem.end();
}

bool CDevDataCfg::isRunTimeDev(const short &shDevType)
{
    return shDevType == DEV_TYPE_RUN_MID || shDevType == DEV_TYPE_RUN_SHORT;
}

QString CDevDataCfg::convertStatusToDescrip(const short &shStatus)
{
    QString qsDescrip;

    //测试中
    switch (shStatus) {
    case CTestStateManage::TEST_IN_PROCESS:
        qsDescrip = "测试中";
        break;
    case CTestStateManage::TEST_COMPLETED:
        qsDescrip = "测试完成";
        break;
    case CTestStateManage::TEST_ERR:
        qsDescrip = "设备断连";
        break;
    case CTestStateManage::TEST_TIMEOUT:
        qsDescrip = "超时结束";
        break;
    default:
        qsDescrip = "未测试";
        break;
    }

    return qsDescrip;
}

void CDevDataCfg::getDevTestTable(vector<short> &vecTableCol, const short &shDevType, const short &shTestMode, bool bComplete)
{
    vecTableCol.clear();

    if(shTestMode >= TEST_MODE_COUNT)
        return;

    short shType = COL_DEV_STATE;

    //异步测试需要将设备状态变更为成绩

    switch (shDevType) {
    case DEV_TYPE_BODY:
        //异步 设备号 姓名 准考证号 身高 体重 设备状态
        //同步 设备号 姓名 准考证号          设备状态
        vecTableCol.push_back(COL_DEV_ID);
        vecTableCol.push_back(COL_USER_NAME);
        vecTableCol.push_back(COL_USER_ID);

        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_HEIGHT);
            vecTableCol.push_back(COL_WEIGHT);

            if(bComplete)
                shType = COL_TEST_SCORE;
        }

        vecTableCol.push_back(shType);

        //异步最后一列为取消绑定
        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_UNBIND_ENABLE);
        }
        break;
    //视力
    case DEV_TYPE_EYESIGHT:
    //异步 设备号 姓名 准考证号 左眼 右眼 设备状态 取消绑定
    //同步 设备号 姓名 准考证号          设备状态
        vecTableCol.push_back(COL_DEV_ID);
        vecTableCol.push_back(COL_USER_NAME);
        vecTableCol.push_back(COL_USER_ID);
        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_LEFT_EYE);
            vecTableCol.push_back(COL_RIGHT_EYE);

            if(bComplete)
                shType = COL_TEST_SCORE;
        }

        vecTableCol.push_back(shType);

        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_UNBIND_ENABLE);
        }
        break;
    //异步 设备号 姓名 准考证号 次数 结果 设备状态 取消绑定
    //同步 设备号 姓名 准考证号          设备状态
    case DEV_TYPE_VC:
    case DEV_TYPE_SBJ:
    case DEV_TYPE_SFB:
    case DEV_TYPE_GRIP:
    case DEV_TYPE_PVV:
    case DEV_TYPE_BASKETBALL:
    case DEV_TYPE_FOOTBALL:
    case DEV_TYPE_VOLLEYBALL:
    case DEV_TYPE_SOLIDBALL:
    case DEV_TYPE_PUO:
    case DEV_TYPE_VJUMP:
    {
        vecTableCol.push_back(COL_DEV_ID);
        vecTableCol.push_back(COL_USER_NAME);
        vecTableCol.push_back(COL_USER_ID);
        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_TEST_TIMES);
            vecTableCol.push_back(COL_TEST_RESULT);

            if(bComplete)
                shType = COL_TEST_SCORE;
        }

        vecTableCol.push_back(shType);

        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_UNBIND_ENABLE);
        }
    }
        break;
    //异步 设备号 姓名 准考证号  结果 设备状态 取消绑定
    //同步 设备号 姓名 准考证号      设备状态
    case DEV_TYPE_PULL_UP:
    case DEV_TYPE_SIT_UP:
    case DEV_TYPE_PUSH_UP:
    {
        vecTableCol.push_back(COL_DEV_ID);
        vecTableCol.push_back(COL_USER_NAME);
        vecTableCol.push_back(COL_USER_ID);
        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_TEST_RESULT);
        }
        vecTableCol.push_back(COL_DEV_STATE);
        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_UNBIND_ENABLE);
        }
    }
        break;
    //同步 设备号 姓名 准考证号          设备状态
    //异步 设备号 姓名 准考证号 圈数 结果 设备状态 取消绑定
    case DEV_TYPE_RUN_MID:
    {
        vecTableCol.push_back(COL_DEV_ID);
        vecTableCol.push_back(COL_USER_NAME);
        vecTableCol.push_back(COL_USER_ID);
        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_THROUGH_TIMES);
            vecTableCol.push_back(COL_TEST_RESULT);

            if(bComplete)
                shType = COL_TEST_SCORE;
        }

        vecTableCol.push_back(shType);

        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_UNBIND_ENABLE);
        }
    }
        break;
    // 同步短跑 设备 姓名 准考 状态
    // 异步短跑 设备 姓名 准考 结果 状态 取消绑定
    // 同步往返 设备 姓名 准考 状态
    // 异步往返 设备 姓名 准考 圈数 结果 状态 取消绑定
    case DEV_TYPE_RUN_SHORT:
        vecTableCol.push_back(COL_DEV_ID);
        vecTableCol.push_back(COL_USER_NAME);
        vecTableCol.push_back(COL_USER_ID);
        if(shTestMode == TEST_ASYNC)
        {
            if(CTestGuideRunShort::getTestItem() == TEST_ITEM_RUN_BACK_8_50 || CTestGuideRunShort::getTestItem() == TEST_ITEM_RUN_BACK_4_10)
            {
                vecTableCol.push_back(COL_THROUGH_TIMES);
            }
            vecTableCol.push_back(COL_TEST_RESULT);

            if(bComplete)
                shType = COL_TEST_SCORE;
        }

        vecTableCol.push_back(shType);

        if(shTestMode == TEST_ASYNC)
        {
            vecTableCol.push_back(COL_UNBIND_ENABLE);
        }
        break;
    case DEV_TYPE_ROPE_SKIP:
    //异步 设备号 姓名 准考证号 结果 设备状态 电量 取消绑定
    //跳绳异步模式统一使用带有电量的内容，代码中进行变更
    {
        vecTableCol.push_back(COL_DEV_ID);
        vecTableCol.push_back(COL_USER_NAME);
        vecTableCol.push_back(COL_USER_ID);
        vecTableCol.push_back(COL_TEST_RESULT);

        if(shTestMode == TEST_ASYNC)
        {
            if(bComplete)
                shType = COL_TEST_SCORE;
            vecTableCol.push_back(shType);
            vecTableCol.push_back(COL_DEV_POWER);
            vecTableCol.push_back(COL_UNBIND_ENABLE);
            //vecTableCol.push_back(COL_TIME_COUNT_DOWN);
        }
    }

        break;
    default:
        LOGERROR("Unrecognized Device Type");
        break;
    }
}

//成绩提交界面为：序号 姓名 准考 结果 成绩
void CDevDataCfg::getDevScoreTable(vector<short> &vecTableCfg, const short &shDevType)
{
    vecTableCfg.clear();

    vecTableCfg.push_back(COL_TEST_NO);
    vecTableCfg.push_back(COL_USER_NAME);
    vecTableCfg.push_back(COL_USER_ID);

    if(shDevType == DEV_TYPE_BODY)
    {
        vecTableCfg.push_back(COL_HEIGHT);
        vecTableCfg.push_back(COL_WEIGHT);
    }
    else if(shDevType == DEV_TYPE_EYESIGHT)
    {
        vecTableCfg.push_back(COL_LEFT_EYE);
        vecTableCfg.push_back(COL_RIGHT_EYE);
    }
    else
    {
        vecTableCfg.push_back(COL_TEST_RESULT);
    }

    vecTableCfg.push_back(COL_TEST_SCORE);
}

short CDevDataCfg::getColTypeIndex(const short &shColType, const short &shDevType, const short &shTestMode, const bool bScoreTable)
{
    //根据尾部参数，区分两个表格
    vector<short> vecColCfg;
    //成绩表格
    if(bScoreTable)
    {
        getDevScoreTable(vecColCfg, shDevType);
    }
    //测试表格
    else
    {
        getDevTestTable(vecColCfg, shDevType, shTestMode);
    }

    if(vecColCfg.size() == 0)
        return -1;

    short shRet = -1;
    for(auto &element : vecColCfg)
    {
        shRet++;
        if(element == shColType)
        {
            return shRet;
        }
    }

    return -1;
}

QString CDevDataCfg::getContentByColDataType(const short &shColDataType, const CLocation &location, const CUserInfo &userInfo, const vector<CTestResultInfo> &vecTestResultInfo)
{
    QString qsTemp("");

    switch (shColDataType) {
    case COL_DEV_ID:
        qsTemp = QString::number(userInfo.m_nExtraInfoID);
        break;
    case COL_USER_NAME:
        qsTemp = userInfo.m_strName;
        break;
    case COL_USER_ID:
        qsTemp = userInfo.m_strStudentId;
        break;
    case COL_TEST_TIMES: //在统一的获取中不需要填充内容
        if(vecTestResultInfo.size() != 0)
            qsTemp = QString::number(vecTestResultInfo[0].getTestTimes());
        break;
    case COL_DEV_STATE: //在统一的获取中不需要填充内容
    {
        CDevHardwareBase *pDev = CSteDeviceManager::Get_Instance().getDevSteByID(location.m_iSteDevMacID);
        if(pDev != nullptr)
        {
            CSubElementBase *pElement = pDev->getElement(location.m_shElementIndex);
            if(pElement != nullptr)
            {
                short shTestState = pElement->getTestState();
                qsTemp = convertStatusToDescrip(shTestState);
            }
        }
    }
        break;
    case COL_THROUGH_TIMES: //在统一的获取中不需要填充内容
        break;
    case COL_DEV_POWER: //在统一的获取中不需要填充内容
        break;
    case COL_TIME_COUNT_DOWN: //在统一的获取中不需要填充内容
        qsTemp = "60";
        break;
    case COL_UNBIND_ENABLE: //在统一的获取中填充默认值：1 - 可以取消绑定
    {
        if(vecTestResultInfo.size() != 0)
        {
            qsTemp = "false";
        }
        else
        {
            qsTemp = "true";
        }
    }
        break;
    case COL_UNKNOWN: //在统一的获取中不需要填充内容
        break;
   ///--------------------- 和测试结果相关 ------------------------
    //序号需要在外部进行获取
    case COL_TEST_NO:
        break;
    case COL_TEST_RESULT:
        if(vecTestResultInfo.size() == 1)
        {
            qsTemp = CTestGuideBase::getTestResult(true, vecTestResultInfo[0]);
        }
        break;
    case COL_HEIGHT:
        qsTemp = DEFAULT_DISP_VALUE;
        for(auto &element : vecTestResultInfo)
        {
            if(element.m_nItem == TEST_ITEM_HEIGHT)
            {
                qsTemp = CTestGuideBase::getTestResult(true, element);
            }
        }
        break;
    case COL_WEIGHT:
        qsTemp = DEFAULT_DISP_VALUE;
        for(auto &element : vecTestResultInfo)
        {
            if(element.m_nItem == TEST_ITEM_WEIGHT)
            {
                qsTemp = CTestGuideBase::getTestResult(true, element);
            }
        }
        break;
    case COL_LEFT_EYE:
    case COL_RIGHT_EYE:
        qsTemp = DEFAULT_DISP_VALUE;
        if(vecTestResultInfo.size() == 1)
        {
            string strResult;
            QString qsLeftEyeSight, qsRightEyeSight;
            vecTestResultInfo[0].getLastTestResult(strResult);
            CTestGuideEyesight::getDeserializationEyeightData(qsLeftEyeSight, qsRightEyeSight, strResult);
            qsTemp = shColDataType == COL_LEFT_EYE ? qsLeftEyeSight : qsRightEyeSight;
        }
        break;
    case COL_TEST_SCORE: //在统一的获取中不需要填充内容
        if(vecTestResultInfo.size() == 1)
        {
            qsTemp = CTestGuideBase::getTestScore(true, vecTestResultInfo[0], userInfo);

            //2023.02.22 济南新增需求，不显示具体得分，只显示结果
            int nSoftVersion = CCommonCfg::GetInstance().getCurSoftwareVersion();
            if(nSoftVersion == soft::VERSION_SDJN)
            {
                qsTemp = "-";
            }
        }
        break;
    default:
        break;
    }

    return qsTemp;
}

QStringList CDevDataCfg::getTableContent(const CLocation &location, const short &shTestMode, const CUserInfo &userInfo, const vector<CTestResultInfo> &vecTestResultInfo, bool bComplete)
{
    vector<short> vecColCfg;
    short shDevType = CSteDeviceManager::Get_Instance().getType();
    CDevDataCfg::getDevTestTable(vecColCfg, shDevType, shTestMode, bComplete);

    QStringList listContent;
    for(auto &element : vecColCfg)
    {
        listContent.append(getContentByColDataType(element, location, userInfo, vecTestResultInfo));
    }

    return listContent;
}

QStringList CDevDataCfg::getScoreTableContent(const CUserInfo &userInfo, const vector<CTestResultInfo> &vecTestResult, const short &shDevType)
{
    vector<short> vecColCfg;
    CDevDataCfg::getDevScoreTable(vecColCfg, shDevType);

    QStringList listContent;
    static CLocation locition;

    for(auto &element : vecColCfg)
    {
        listContent.append(getContentByColDataType(element, locition, userInfo, vecTestResult));
    }

    return listContent;
}

CDevHardwareBase *CDevDataCfg::createSteDevcice(unsigned int iDevID, ICommunicate &comunicate, const short &shDevType)
{
    //创建具体设备，如肺活量、跳绳等等，此处需要传入硬件通讯接口，其目的是具体设备可能存在发送一个指令
    CDevHardwareBase *pDev = nullptr;
    switch (shDevType) {
    case DEV_TYPE_BODY:
        pDev = new CDevSteBody(iDevID, comunicate);
        break;
    case DEV_TYPE_VC:
        pDev = new CDevSteVC(iDevID, comunicate);
        break;
    case DEV_TYPE_SBJ:
        pDev = new CDevSteSBJ(iDevID, comunicate);
        break;
    case DEV_TYPE_SFB:
        pDev = new CDevSteSFB(iDevID, comunicate);
        break;
    case DEV_TYPE_PULL_UP:
        pDev = new CDevStePullUp(iDevID, comunicate);
        break;
    case DEV_TYPE_SIT_UP:
        pDev = new CDevSteSitUp(iDevID, comunicate);
        break;
    case DEV_TYPE_RUN_SHORT:
        pDev = new CDevSteRunShort(iDevID, comunicate);
        break;
    case DEV_TYPE_RUN_MID:
        pDev = new CDevSteRunMid(iDevID, comunicate);
        break;
    case DEV_TYPE_ROPE_SKIP:
        pDev = new CDevSteSkip(iDevID, comunicate);
        break;
    case DEV_TYPE_GRIP:
        pDev = new CDevSteGrip(iDevID, comunicate);
        break;
    case DEV_TYPE_EYESIGHT:
        pDev = new CDevSteEyesight(iDevID, comunicate);
        break;
    case DEV_TYPE_PUSH_UP:
        pDev = new CDevStePushup(iDevID, comunicate);
        break;
    case DEV_TYPE_PVV:
        pDev = new CDevStePVV(iDevID, comunicate);
        break;
    case DEV_TYPE_FOOTBALL:
        pDev = new CDevSteFootball(iDevID, comunicate);
        break;
    case DEV_TYPE_BASKETBALL:
        pDev = new CDevSteBasketball(iDevID, comunicate);
        break;
    case DEV_TYPE_VOLLEYBALL:
        pDev = new CDevSteVolleyball(iDevID, comunicate);
        break;
    case DEV_TYPE_PUO:
        pDev = new CDevStePUO(iDevID, comunicate);
        break;
    case DEV_TYPE_SOLIDBALL:
        pDev = new CDevSteSolidball(iDevID, comunicate);
        break;
    case DEV_TYPE_VJUMP:
        pDev = new CDevSteVertivalJump(iDevID, comunicate);
        break;
    default:
        pDev = nullptr;
        break;
    }

    return pDev;
}

int CDevDataCfg::getSinglePackgeLenByDataType(const unsigned short &shDataFrameType)
{
    //针对命令帧，通常的单包数据中的数据长度为 4
    int nCommonLen = 4;

    switch (shDataFrameType) {
    case DATA_FRAME_TYPE_GET_INFO:
        nCommonLen = 192;
        break;
    case DATA_FRAME_TYPE_SET_CHANNEL:
        nCommonLen = 8;
        break;
    default:
        break;
    }

    return Packet_Commond_Head + nCommonLen;
}

bool CDevDataCfg::isHomePage(const short &shPage)
{
    return s_seHomePage.find(shPage) != s_seHomePage.end();
}

bool CDevDataCfg::isTestPage(const short &shPage, const short &shTestMode)
{
    set<short> setTemp;
    setTemp.insert(CProjectEnums::PAGE_TOURIST_TEST);
    setTemp.insert(CProjectEnums::PAGE_USER_TEST);
    setTemp.insert(CProjectEnums::PAGE_GROUP_TEST);

    //异步测试主页也是测试界面
    if(shTestMode == TEST_ASYNC)
    {
        setTemp.insert(CProjectEnums::PAGE_GROUP_HOME);
    }

    return setTemp.find(shPage) != setTemp.end();
}

string CDevDataCfg::getDevDescribe(const short &shDevType)
{
    string strTemp("");

    auto itFind = s_mapDevDescribe.find(shDevType);
    if(itFind != s_mapDevDescribe.end())
    {
        strTemp = itFind->second;
    }

    return strTemp;
}

string CDevDataCfg::getTestItemDescribe(const short &shTestItem)
{
    string strTemp("");

    auto itFind = s_mapTestItemDescribe.find(shTestItem);
    if(itFind != s_mapTestItemDescribe.end())
    {
        strTemp = itFind->second;
    }

    return strTemp;
}

bool CDevDataCfg::isValidExtensionNum(const int &nDevExtensionNo)
{
    return nDevExtensionNo >= NumberOfChannelAndextension::ExtensionNoMin && nDevExtensionNo <= NumberOfChannelAndextension::ExtensionNoMax;
}

set<unsigned short> CDevDataCfg::initDataTypeCmd()
{
    set<unsigned short> set;
    set.insert(DATA_FRAME_TYPE_HEIGHT);
    set.insert(DATA_FRAME_TYPE_WEIGHT);
    set.insert(DATA_FRAME_TYPE_VC);
    set.insert(DATA_FRAME_TYPE_GRIP);
    set.insert(DATA_FRAME_TYPE_SFB);
    set.insert(DATA_FRAME_TYPE_PULL_UP);
    set.insert(DATA_FRAME_TYPE_SIT_UP_1);
    set.insert(DATA_FRAME_TYPE_STEP_TEST);
    set.insert(DATA_FRAME_TYPE_SBJ);
    set.insert(DATA_FRAME_TYPE_RUN_SHORT);
    set.insert(DATA_FRAME_TYPE_RUN_MID);
    set.insert(DATA_FRAME_TYPE_BODY_IMPEDANCE);
    set.insert(DATA_FRAME_TYPE_BODY_IMPEDANCE1);
    set.insert(DATA_FRAME_TYPE_BODY_IMPEDANCE2);
    set.insert(DATA_FRAME_TYPE_BODY_IMPEDANCE3);
    set.insert(DATA_FRAME_TYPE_BODY_IMPEDANCE4);
    set.insert(DATA_FRAME_TYPE_BODY_IMPEDANCE5);
    set.insert(DATA_FRAME_TYPE_GRIP);
    set.insert(DATA_FRAME_TYPE_EYESIGHT);
    set.insert(DATA_FRAME_TYPE_PUSHUP);
    set.insert(DATA_FRAME_TYPE_PVV);
    set.insert(DATA_FRAME_TYPE_SKIPPING_ROPE);
    set.insert(DATA_FRAME_TYPE_RUN_SHORT_DATA);
    set.insert(DATA_FRAME_TYPE_FOOTBALL);
    set.insert(DATA_FRAME_TYPE_BASKETBALL);
    set.insert(DATA_FRAME_TYPE_VOLLEYBALL);
    set.insert(DATA_FRAME_TYPE_PUO);
    set.insert(DATA_FRAME_SOLIDBALL);
    set.insert(DATA_FRAME_VJUMP);
    return set;
}

map<unsigned char, short> CDevDataCfg::initHardwareTypeMapping()
{
    map<unsigned char, short> mapMapping;

    mapMapping[HARDWARE_CODE_NETWORK]    = DEV_TYPE_NETWORK;
    mapMapping[HARDWARE_CODE_BODY]       = DEV_TYPE_BODY;
    mapMapping[HARDWARE_CODE_VC]         = DEV_TYPE_VC;
    mapMapping[HARDWARE_CODE_SBJ]        = DEV_TYPE_SBJ;
    mapMapping[HARDWARE_CODE_SFB]        = DEV_TYPE_SFB;
    mapMapping[HARDWARE_CODE_PULL_UP]    = DEV_TYPE_PULL_UP;
    mapMapping[HARDWARE_CODE_SIT_UP]     = DEV_TYPE_SIT_UP;
    mapMapping[HARDWARE_CODE_RUN_SHORT]  = DEV_TYPE_RUN_SHORT;
    mapMapping[HARDWARE_CODE_RUN_MID]    = DEV_TYPE_RUN_MID;
    mapMapping[HARDWARE_CODE_ROPE_SKING] = DEV_TYPE_ROPE_SKIP;
    mapMapping[HARDWARE_CODE_GRIP]       = DEV_TYPE_GRIP;
    mapMapping[HARDWARE_CODE_EYESIGHT]   = DEV_TYPE_EYESIGHT;
    mapMapping[HARDWARE_CODE_PUSHUP]     = DEV_TYPE_PUSH_UP;
    mapMapping[HARDWARE_CODE_PVV]        = DEV_TYPE_PVV;
    mapMapping[HARDWARE_CODE_FOOTBALL]   = DEV_TYPE_FOOTBALL;
    mapMapping[HARDWARE_CODE_BASKETBALL] = DEV_TYPE_BASKETBALL;
    mapMapping[HARDWARE_CODE_VOLLEYBALL] = DEV_TYPE_VOLLEYBALL;
    mapMapping[HARDWARE_CODE_SOLIDBALL]  = DEV_TYPE_SOLIDBALL;
    mapMapping[HARDWARE_CODE_PUO]        = DEV_TYPE_PUO;
    mapMapping[HARDWARE_CODE_VJUMP]      = DEV_TYPE_VJUMP;
    return mapMapping;
}

map<short, unsigned short> CDevDataCfg::initTestItemAndDataFrameTypeMapping()
{
    map<short, unsigned short> mapping;
    mapping[TEST_ITEM_HEIGHT]          = DATA_FRAME_TYPE_HEIGHT;
    mapping[TEST_ITEM_WEIGHT]          = DATA_FRAME_TYPE_WEIGHT;
    mapping[TEST_ITEM_HBC]             = DATA_FRAME_TYPE_BODY_IMPEDANCE;
    mapping[TEST_ITEM_VC]              = DATA_FRAME_TYPE_VC;
    mapping[TEST_ITEM_SBJ]             = DATA_FRAME_TYPE_SBJ;
    mapping[TEST_ITEM_SFB]             = DATA_FRAME_TYPE_SFB;
    mapping[TEST_ITEM_PULL_UP]         = DATA_FRAME_TYPE_PULL_UP;
    mapping[TEST_ITEM_SIT_UP]          = DATA_FRAME_TYPE_SIT_UP_1;
    mapping[TEST_ITEM_RUN_1500]        = DATA_FRAME_TYPE_RUN_MID;
    mapping[TEST_ITEM_RUN_1000]        = DATA_FRAME_TYPE_RUN_MID;
    mapping[TEST_ITEM_RUN_800]         = DATA_FRAME_TYPE_RUN_MID;
    mapping[TEST_ITEM_RUN_2000]         = DATA_FRAME_TYPE_RUN_MID;
    mapping[TEST_ITEM_RUN_50]          = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_BACK_8_50]   = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_BACK_4_10]   = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_60]          = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_100]         = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_200]         = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_400]         = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_4_100]       = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_HURDLES_100] = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_HURDLES_110] = DATA_FRAME_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_GRIP]            = DATA_FRAME_TYPE_GRIP;
    mapping[TEST_ITEM_EYESIGHT]        = DATA_FRAME_TYPE_EYESIGHT;
    mapping[TEST_ITEM_PUSHUP]          = DATA_FRAME_TYPE_PUSHUP;
    mapping[TEST_ITEM_PVV]             = DATA_FRAME_TYPE_PVV;
    mapping[TEST_ITEM_ROPE_SKIP]       = BASE_STATION_FRAME_TYPE_SKIPPING_ROPE;
    mapping[TEST_ITEM_FOOTBALL]        = DATA_FRAME_TYPE_FOOTBALL;
    mapping[TEST_ITEM_BASKETBALL]      = DATA_FRAME_TYPE_BASKETBALL;
    mapping[TEST_ITEM_VOLLEYBALL]      = DATA_FRAME_TYPE_VOLLEYBALL;
    mapping[TEST_ITEM_PUO]             = DATA_FRAME_TYPE_PUO;
    mapping[TEST_ITEM_SOLIDBALL]       = DATA_FRAME_SOLIDBALL;
    mapping[TEST_ITEM_VJUMP]           = DATA_FRAME_VJUMP;
    return mapping;
}

map<short, short> CDevDataCfg::initTestItemAndDevTypeTypeMapping()
{
    map<short, short> mapping;
    mapping[TEST_ITEM_HEIGHT]          = DEV_TYPE_BODY;
    mapping[TEST_ITEM_WEIGHT]          = DEV_TYPE_BODY;
    mapping[TEST_ITEM_HBC]             = DEV_TYPE_BODY;
    mapping[TEST_ITEM_VC]              = DEV_TYPE_VC;
    mapping[TEST_ITEM_SBJ]             = DEV_TYPE_SBJ;
    mapping[TEST_ITEM_SFB]             = DEV_TYPE_SFB;
    mapping[TEST_ITEM_PULL_UP]         = DEV_TYPE_PULL_UP;
    mapping[TEST_ITEM_SIT_UP]          = DEV_TYPE_SIT_UP;
    mapping[TEST_ITEM_RUN_1500]        = DEV_TYPE_RUN_MID;
    mapping[TEST_ITEM_RUN_1000]        = DEV_TYPE_RUN_MID;
    mapping[TEST_ITEM_RUN_800]         = DEV_TYPE_RUN_MID;
    mapping[TEST_ITEM_RUN_2000]         = DEV_TYPE_RUN_MID;
    mapping[TEST_ITEM_RUN_50]          = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_BACK_8_50]   = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_60]          = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_100]         = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_200]         = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_400]         = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_4_100]       = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_BACK_4_10]   = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_HURDLES_100] = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_RUN_HURDLES_110] = DEV_TYPE_RUN_SHORT;
    mapping[TEST_ITEM_GRIP]            = DEV_TYPE_GRIP;
    mapping[TEST_ITEM_EYESIGHT]        = DEV_TYPE_EYESIGHT;
    mapping[TEST_ITEM_PUSHUP]          = DEV_TYPE_PUSH_UP;
    mapping[TEST_ITEM_PVV]             = DEV_TYPE_PVV;
    mapping[TEST_ITEM_ROPE_SKIP]       = DEV_TYPE_ROPE_SKIP;
    mapping[TEST_ITEM_FOOTBALL]        = DEV_TYPE_FOOTBALL;
    mapping[TEST_ITEM_BASKETBALL]      = DEV_TYPE_BASKETBALL;
    mapping[TEST_ITEM_VOLLEYBALL]      = DEV_TYPE_VOLLEYBALL;
    mapping[TEST_ITEM_PUO]             = DEV_TYPE_PUO;
    mapping[TEST_ITEM_SOLIDBALL]       = DEV_TYPE_SOLIDBALL;
    mapping[TEST_ITEM_VJUMP]           = DEV_TYPE_VJUMP;
    return mapping;
}

set<short> CDevDataCfg::initHomePage()
{
    set<short> setTemp;
    setTemp.insert(CProjectEnums::PAGE_DIRECT_HOME);
    setTemp.insert(CProjectEnums::PAGE_GROUP_HOME);

    return setTemp;
}

set<short> CDevDataCfg::initSpeciaDev()
{
    set<short> setTemp;
    setTemp.insert(DEV_TYPE_RUN_MID);
    setTemp.insert(DEV_TYPE_RUN_SHORT);
    setTemp.insert(DEV_TYPE_ROPE_SKIP);
    return setTemp;
}

set<short> CDevDataCfg::initViolationDev()
{
    set<short> setTemp;
    setTemp.insert(DEV_TYPE_FOOTBALL);
    setTemp.insert(DEV_TYPE_BASKETBALL);
    setTemp.insert(DEV_TYPE_VOLLEYBALL);
    setTemp.insert(DEV_TYPE_PUO);
    //setTemp.insert(DEV_TYPE_SOLIDBALL);
    setTemp.insert(DEV_TYPE_VJUMP);
    return setTemp;
}

set<short> CDevDataCfg::initViolationTestItem()
{
    set<short> setTemp;
    setTemp.insert(TEST_ITEM_FOOTBALL);
    setTemp.insert(TEST_ITEM_BASKETBALL);
    setTemp.insert(TEST_ITEM_VOLLEYBALL);
    //setTemp.insert(TEST_ITEM_PUO);
    //setTemp.insert(TEST_ITEM_SOLIDBALL);
    //setTemp.insert(TEST_ITEM_VJUMP);
    return setTemp;
}

map<short, set<unsigned int>> CDevDataCfg::initChaInfo()
{
    map<short, set<unsigned int>> mapTemp;

    set<unsigned int> setTemp;

    //测试信道
    //setTemp.clear();
    //setTemp.insert(29);
    //setTemp.insert(209);
    //mapTemp[DEV_TYPE_UNKNOW] = setTemp;

    setTemp.insert(45);
    setTemp.insert(193);
    mapTemp[DEV_TYPE_SBJ] = setTemp;

    setTemp.clear();
    setTemp.insert(73);
    setTemp.insert(165);
    mapTemp[DEV_TYPE_SFB] = setTemp;

    setTemp.clear();
    setTemp.insert(37);
    setTemp.insert(201);
    mapTemp[DEV_TYPE_VC] = setTemp;

    setTemp.clear();
    setTemp.insert(33);
    setTemp.insert(205);
    mapTemp[DEV_TYPE_BODY] = setTemp;

    setTemp.clear();
    setTemp.insert(57);
    setTemp.insert(181);
    mapTemp[DEV_TYPE_SIT_UP] = setTemp;

    setTemp.clear();
    setTemp.insert(61);
    setTemp.insert(177);
    mapTemp[DEV_TYPE_PULL_UP] = setTemp;

    setTemp.clear();
    setTemp.insert(69);
    setTemp.insert(169);
    mapTemp[DEV_TYPE_RUN_SHORT] = setTemp;

    setTemp.clear();
    setTemp.insert(53);
    setTemp.insert(185);
    mapTemp[DEV_TYPE_RUN_MID] = setTemp;

    setTemp.clear();
    setTemp.insert(77);
    setTemp.insert(161);
    mapTemp[DEV_TYPE_ROPE_SKIP] = setTemp;

    setTemp.clear();
    setTemp.insert(41);
    setTemp.insert(197);
    mapTemp[DEV_TYPE_EYESIGHT] = setTemp;

    setTemp.clear();
    setTemp.insert(49);
    setTemp.insert(189);
    mapTemp[DEV_TYPE_GRIP] = setTemp;

    setTemp.clear();
    setTemp.insert(65);
    setTemp.insert(173);
    mapTemp[DEV_TYPE_PUSH_UP] = setTemp;

    setTemp.clear();
    setTemp.insert(81);
    setTemp.insert(157);
    mapTemp[DEV_TYPE_FOOTBALL] = setTemp;

    setTemp.clear();
    setTemp.insert(85);
    setTemp.insert(153);
    mapTemp[DEV_TYPE_BASKETBALL] = setTemp;

    setTemp.clear();
    setTemp.insert(89);
    setTemp.insert(149);
    mapTemp[DEV_TYPE_VOLLEYBALL] = setTemp;

    setTemp.clear();
    setTemp.insert(105);
    setTemp.insert(141);
    mapTemp[DEV_TYPE_PUO] = setTemp;

    setTemp.clear();
    setTemp.insert(101);
    setTemp.insert(145);
    mapTemp[DEV_TYPE_SOLIDBALL] = setTemp;

    setTemp.clear();
    setTemp.insert(109);
    setTemp.insert(137);
    mapTemp[DEV_TYPE_VJUMP] = setTemp;

    return mapTemp;
}

set<unsigned char> CDevDataCfg::initsHardwareCompleteFlag()
{
    set<unsigned char> setTemp;
    setTemp.insert(DEV_TEST_STATE_EOT_VALID);
    setTemp.insert(DEV_TEST_STATE_EOT_INVALID);
    return setTemp;
}

map<short, string> CDevDataCfg::initTestItemDescribe()
{
    map<short, string> mapping;
    mapping[TEST_ITEM_HEIGHT]          = "HEIGHT";
    mapping[TEST_ITEM_WEIGHT]          = "WEIGHT";
    mapping[TEST_ITEM_HBC]             = "BCI";
    mapping[TEST_ITEM_ROPE_SKIP]       = "SKIP";
    mapping[TEST_ITEM_VC]              = "VC";
    mapping[TEST_ITEM_SBJ]             = "SBJ";
    mapping[TEST_ITEM_SFB]             = "SFB";
    mapping[TEST_ITEM_PULL_UP]         = "PULL_UP";
    mapping[TEST_ITEM_SIT_UP]          = "SIT_UP";
    mapping[TEST_ITEM_RUN_1500]        = "RUN1500";
    mapping[TEST_ITEM_RUN_1000]        = "RUN1000";
    mapping[TEST_ITEM_RUN_800]         = "RUN800";
    mapping[TEST_ITEM_RUN_2000]         = "RUN2000";
    mapping[TEST_ITEM_RUN_50]          = "RUN50";
    mapping[TEST_ITEM_RUN_BACK_8_50]   = "RUN8*50";
    mapping[TEST_ITEM_RUN_BACK_4_10]   = "RUN4*10";
    mapping[TEST_ITEM_RUN_60]          = "RUN60";
    mapping[TEST_ITEM_RUN_100]         = "RUN100";
    mapping[TEST_ITEM_RUN_200]         = "RUN200";
    mapping[TEST_ITEM_RUN_400]         = "RUN400";
    mapping[TEST_ITEM_RUN_4_100]       = "RUN4*100";
    mapping[TEST_ITEM_RUN_HURDLES_100] = "RUN_HURDLES100";
    mapping[TEST_ITEM_RUN_HURDLES_110] = "RUN_HURDLES110";
    mapping[TEST_ITEM_GRIP]            = "GRIP";
    mapping[TEST_ITEM_EYESIGHT]        = "EYESIGHT";
    mapping[TEST_ITEM_PUSHUP]          = "PUSHUP";
    mapping[TEST_ITEM_PVV]             = "PVV";
    mapping[TEST_ITEM_FOOTBALL]        = "FOOT";
    mapping[TEST_ITEM_BASKETBALL]      = "BASKET";
    mapping[TEST_ITEM_VOLLEYBALL]      = "VOLLEY";
    mapping[TEST_ITEM_PUO]             = "PUO";
    mapping[TEST_ITEM_SOLIDBALL]       = "SOLID";
    mapping[TEST_ITEM_VJUMP]           = "VJUMP";

    return mapping;
}

map<short, string> CDevDataCfg::initDevTypeDescribe()
{
    map<short, string> mapMapping;
    mapMapping[DEV_TYPE_NETWORK]       = "NETWORK";
    mapMapping[DEV_TYPE_BODY]          = "BODY";
    mapMapping[DEV_TYPE_VC]            = "VC";
    mapMapping[DEV_TYPE_SBJ]           = "SBJ";
    mapMapping[DEV_TYPE_SFB]           = "SFB";
    mapMapping[DEV_TYPE_PULL_UP]       = "PULL_UP";
    mapMapping[DEV_TYPE_SIT_UP]        = "SIT_UP_1";
    mapMapping[DEV_TYPE_RUN_SHORT]     = "RUN_SHORT";
    mapMapping[DEV_TYPE_RUN_MID]       = "RUN_MID";
    mapMapping[DEV_TYPE_ROPE_SKIP]     = "SKIPPING_ROPE";
    mapMapping[DEV_TYPE_GRIP]          = "GRIP";
    mapMapping[DEV_TYPE_EYESIGHT]      = "EYESIGHT";
    mapMapping[DEV_TYPE_PUSH_UP]       = "PUSHUP";
    mapMapping[DEV_TYPE_PVV]           = "PVV";
    mapMapping[DEV_TYPE_FOOTBALL]      = "FOOT";
    mapMapping[DEV_TYPE_BASKETBALL]    = "BASKET";
    mapMapping[DEV_TYPE_VOLLEYBALL]    = "VOLLEY";
    mapMapping[DEV_TYPE_PUO]           = "PUO";
    mapMapping[DEV_TYPE_SOLIDBALL]     = "SOLID";
    mapMapping[DEV_TYPE_VJUMP] = "VJUMP";
    return mapMapping;
}

map<short, int> CDevDataCfg::initTestItemAndServerItem()
{
    map<short, int> mapping;
    mapping[TEST_ITEM_HEIGHT]          = TEST_ITEM::HTTP_TEST_ITEM_HEIGHT;
    mapping[TEST_ITEM_WEIGHT]          = TEST_ITEM::HTTP_TEST_ITEM_WEIGHT;
    mapping[TEST_ITEM_HBC]             = TEST_ITEM::HTTP_TEST_ITEM_HBC;
    mapping[TEST_ITEM_ROPE_SKIP]       = TEST_ITEM::HTTP_TEST_ITEM_ROPE_SKIP;
    mapping[TEST_ITEM_VC]              = TEST_ITEM::HTTP_TEST_ITEM_VC;
    mapping[TEST_ITEM_SBJ]             = TEST_ITEM::HTTP_TEST_ITEM_SBJ;
    mapping[TEST_ITEM_SFB]             = TEST_ITEM::HTTP_TEST_ITEM_SFB;
    mapping[TEST_ITEM_PULL_UP]         = TEST_ITEM::HTTP_TEST_ITEM_PULL_UP;
    mapping[TEST_ITEM_SIT_UP]          = TEST_ITEM::HTTP_TEST_ITEM_SIT_UP;
    mapping[TEST_ITEM_RUN_1500]        = TEST_ITEM::HTTP_TEST_ITEM_RUN_1500;
    mapping[TEST_ITEM_RUN_1000]        = TEST_ITEM::HTTP_TEST_ITEM_RUN_1000;
    mapping[TEST_ITEM_RUN_800]         = TEST_ITEM::HTTP_TEST_ITEM_RUN_800;
    mapping[TEST_ITEM_RUN_2000]        = TEST_ITEM::HTTP_TEST_ITEM_RUN_2000;
    mapping[TEST_ITEM_RUN_50]          = TEST_ITEM::HTTP_TEST_ITEM_RUN_50;
    mapping[TEST_ITEM_RUN_BACK_8_50]   = TEST_ITEM::HTTP_TEST_ITEM_RUN_BACK_8_50;
    mapping[TEST_ITEM_RUN_BACK_4_10]   = TEST_ITEM::HTTP_TEST_ITEM_RUN_BACK_4_10;
    mapping[TEST_ITEM_RUN_60]          = TEST_ITEM::HTTP_TEST_ITEM_RUN_60;
    mapping[TEST_ITEM_RUN_100]         = TEST_ITEM::HTTP_TEST_ITEM_RUN_100;
    mapping[TEST_ITEM_RUN_200]         = TEST_ITEM::HTTP_TEST_ITEM_RUN_200;
    mapping[TEST_ITEM_RUN_400]         = TEST_ITEM::HTTP_TEST_ITEM_RUN_400;
    mapping[TEST_ITEM_RUN_4_100]       = TEST_ITEM::HTTP_TEST_ITEM_RUN_4_100;
    mapping[TEST_ITEM_RUN_HURDLES_100] = TEST_ITEM::HTTP_TEST_ITEM_RUN_HURDLES_100;
    mapping[TEST_ITEM_RUN_HURDLES_110] = TEST_ITEM::HTTP_TEST_ITEM_RUN_HURDLES_110;
    mapping[TEST_ITEM_GRIP]            = TEST_ITEM::HTTP_TEST_ITEM_GRIP;
    mapping[TEST_ITEM_PUSHUP]          = TEST_ITEM::HTTP_TEST_ITEM_PUSH_UP;

    mapping[TEST_ITEM_FOOTBALL]        = TEST_ITEM::HTTP_TEST_ITEM_FOOTBALL;
    mapping[TEST_ITEM_BASKETBALL]      = TEST_ITEM::HTTP_TEST_ITEM_BASKETBALL;
    mapping[TEST_ITEM_VOLLEYBALL]      = TEST_ITEM::HTTP_TEST_ITEM_VOLLEYBALL;
    mapping[TEST_ITEM_PUO]             = TEST_ITEM::HTTP_TEST_ITEM_PUO;
    mapping[TEST_ITEM_SOLIDBALL]       = TEST_ITEM::HTTP_TEST_ITEM_SOLDBALL;
    mapping[TEST_ITEM_VJUMP]           = TEST_ITEM::HTTP_TEST_ITEM_VERTICALBALL;

    return mapping;
}

map<short, int> CDevDataCfg::initDevTypeAndServerType()
{
    map<short, int> mapMapping;
    mapMapping[DEV_TYPE_BODY]       = HTTP_DEV_TYPE_BODY;
    mapMapping[DEV_TYPE_VC]         = HTTP_DEV_TYPE_VC;
    mapMapping[DEV_TYPE_SBJ]        = HTTP_DEV_TYPE_SBJ;
    mapMapping[DEV_TYPE_SFB]        = HTTP_DEV_TYPE_SFB;
    mapMapping[DEV_TYPE_PULL_UP]    = HTTP_DEV_TYPE_PULL_UP;
    mapMapping[DEV_TYPE_SIT_UP]     = HTTP_DEV_TYPE_SIT_UP;
    mapMapping[DEV_TYPE_RUN_SHORT]  = HTTP_DEV_TYPE_RUN_SHORT;
    mapMapping[DEV_TYPE_RUN_MID]    = HTTP_DEV_TYPE_RUN_MID;
    mapMapping[DEV_TYPE_ROPE_SKIP]  = HTTP_DEV_TYPE_ROPE_SKIP;
    mapMapping[DEV_TYPE_GRIP]       = HTTP_DEV_TYPE_ROPE_GRIP;
    mapMapping[DEV_TYPE_EYESIGHT]   = HTTP_DEV_TYPE_VISION;
    mapMapping[DEV_TYPE_PUSH_UP]    = HTTP_DEV_TYPE_PUSH_UP;

    mapMapping[DEV_TYPE_FOOTBALL]    = HTTE_DEV_TYPE_FOOTBALL;
    mapMapping[DEV_TYPE_BASKETBALL]  = HTTE_DEV_TYPE_BASKETBALL;
    mapMapping[DEV_TYPE_VOLLEYBALL]  = HTTE_DEV_TYPE_VOLLEYBALL;
    mapMapping[DEV_TYPE_PUO]         = HTTE_DEV_TYPE_PUO;
    mapMapping[DEV_TYPE_SOLIDBALL]   = HTTE_DEV_TYPE_SOLIDBALL;
    mapMapping[DEV_TYPE_VJUMP]       = HTTE_DEV_TYPE_VERTICALJUMP;

    return mapMapping;
}

map<short, int> CDevDataCfg::initItemPackgeLen()
{
    map<short, int> mapping;
    mapping[TEST_ITEM_HEIGHT]          = Packet_Len_Height;
    mapping[TEST_ITEM_WEIGHT]          = Packet_Len_Weight;
    mapping[TEST_ITEM_HBC]             = Packet_Len_Impedance;
    mapping[TEST_ITEM_ROPE_SKIP]       = Packet_Len_Jump_Rope;
    mapping[TEST_ITEM_VC]              = Packet_Len_VC;
    mapping[TEST_ITEM_SBJ]             = Packet_Len_SBJ;
    mapping[TEST_ITEM_SFB]             = Packet_Len_SFB;
    mapping[TEST_ITEM_PULL_UP]         = Packet_Len_PULL_UP;
    mapping[TEST_ITEM_SIT_UP]          = Packet_Len_SIT_UP;
    mapping[TEST_ITEM_RUN_1000]        = Packet_Len_Run_Mid;
    mapping[TEST_ITEM_RUN_800]         = Packet_Len_Run_Mid;
    mapping[TEST_ITEM_RUN_1500]        = Packet_Len_Run_Mid;
    mapping[TEST_ITEM_RUN_2000]        = Packet_Len_Run_Mid;
    mapping[TEST_ITEM_RUN_50]          = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_BACK_8_50]   = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_BACK_4_10]   = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_60]          = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_100]         = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_200]         = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_400]         = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_4_100]       = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_HURDLES_100] = Packet_Len_Run_Short;
    mapping[TEST_ITEM_RUN_HURDLES_110] = Packet_Len_Run_Short;
    mapping[TEST_ITEM_GRIP]            = Packet_Len_GRIP;
    mapping[TEST_ITEM_EYESIGHT]        = Packet_Len_EYEIGHT;
    mapping[TEST_ITEM_PUSHUP]          = Packet_Len_PUSHUP;
    mapping[TEST_ITEM_PVV]             = Packet_Len_PVV;
    mapping[TEST_ITEM_FOOTBALL]        = Packet_Len_Footbal;
    mapping[TEST_ITEM_BASKETBALL]      = Packet_Len_Basketball;
    mapping[TEST_ITEM_VOLLEYBALL]      = Packet_Len_Volleyball;
    mapping[TEST_ITEM_PUO]             = Packet_Len_PUO;
    mapping[TEST_ITEM_SOLIDBALL]       = Packet_Len_Solidball;
    mapping[TEST_ITEM_VJUMP]           = Packet_Len_Vertival_Jump;
    return mapping;
}

QString CDevDataCfg::getS_mapChaInfo()
{
    QString dataToString = "";

    QJsonArray channelArray;
    QString strDevType("devType");
    QString strChannel("channel");
    for(auto item : s_mapChaInfo)
    {
        QJsonObject item_data;
        item_data.insert(strDevType, QJsonValue(item.first));
        QString allChannel = "";
        for(auto channel : item.second)
        {
            allChannel += QString::number(channel) + "-";
            item_data.insert(strChannel, QJsonValue(allChannel));
        }
        channelArray.push_back(QJsonValue(item_data));
    }

    QJsonObject final_object;
    final_object.insert(QString("Channels"), QJsonValue(channelArray));

    QJsonDocument doc = QJsonDocument(final_object);
    dataToString = QString(doc.toJson());

    return dataToString;
}

QStringList CDevDataCfg::getDevSignalChannelList( const short &devType )
{
    QStringList retList;
    for( auto &tmpDevType : s_mapChaInfo ){
        if( tmpDevType.first == devType ){
            for( auto &tmpChan : tmpDevType.second ){
                retList << QString::number( tmpChan );
            }
            break;
        }
    }

    // 如果 参数 devType 为 DEV_TYPE_UNKONWON， 信道号列表为空
    if( retList.isEmpty() ){
        retList << "";
    }

    return retList;
}

int CDevDataCfg::getHttpTestItemFromItem(const short &shTestItem)
{
    int nTestItem = HTTP_TEST_ITEM_UNKNOW;

    auto itFind = s_mapTestItemAndServerItemMapping.find(shTestItem);
    if(itFind != s_mapTestItemAndServerItemMapping.end())
    {
        nTestItem = itFind->second;
    }

    return nTestItem;
}

int CDevDataCfg::getHttpDevTypeFromType(const short &shDevType)
{
    int nDevType = HTTP_DEV_TYPE_UNKNOW;

    auto itFind = s_mapDevTypeAndServerTypeMapping.find(shDevType);
    if(itFind != s_mapDevTypeAndServerTypeMapping.end())
    {
        nDevType = itFind->second;
    }

    return nDevType;
}

short CDevDataCfg::getTestItemFromHttpItem(const int &iHttpTestItem)
{
    short shDevType = TEST_ITEM_UNKNOW;

    for(auto &element : s_mapTestItemAndServerItemMapping)
    {
        if(element.second == iHttpTestItem)
        {
            shDevType = element.first;
            break;
        }
    }

    return shDevType;
}

short CDevDataCfg::getScoreTypeFromView(const int &viewScoreType)
{
    if( CProjectEnums::TestNature::TEST_INFORMAL == viewScoreType ){
        return CTestResultInfo::emTestNature::TEST_INFORMAL;
    }

    if( CProjectEnums::TestNature::TEST_FORMAL == viewScoreType ){
        return CTestResultInfo::emTestNature::TEST_FORMAL;
    }

    return CTestResultInfo::emTestNature::TEST_FORMAL;
}

void CDevDataCfg::getTestState(const unsigned char &cStateByte, unsigned char &cTestState)
{
    //提取状态在资源的第七位和第八位,将资源右移6位即可
    auto cTemp = cStateByte;
    cTestState = cTemp >> 6;
}

bool CDevDataCfg::getDevDirectConnectFlag()
{
    CSteDeviceManager &objSteDevMgr(CSteDeviceManager::Get_Instance());
    //false代表直连
    bool bDirect = !objSteDevMgr.isStaionSte();
    short shDevType = objSteDevMgr.getType();

    //根据数据库配置进行，若是非自动切换，根据模式强制设置连接模式(后续通过变量进行控制，长久访问数据库可能会造成一定磁盘压力)
    CDatabaseManage &devDatabase = CDatabaseManage::GetInstance();
    CDevTestInfoConfig *pTableDevTestInfo = devDatabase.getDevTestInfoConfig();
    if(pTableDevTestInfo != nullptr && !pTableDevTestInfo->getDevConnectModeAuto())
    {
        bDirect = pTableDevTestInfo->getDevConnectMode();
    }

    //项目为足球排球篮球仅有直连模式一种展现形式 斜实跳
    if(shDevType == DEV_TYPE_FOOTBALL || shDevType == DEV_TYPE_BASKETBALL || shDevType == DEV_TYPE_VOLLEYBALL
            || shDevType == DEV_TYPE_PUO || shDevType == DEV_TYPE_VOLLEYBALL || shDevType == DEV_TYPE_VJUMP)
        bDirect = true;

    //只有组网一种展现模式
    if(shDevType == DEV_TYPE_RUN_SHORT || shDevType == DEV_TYPE_RUN_MID || shDevType == DEV_TYPE_ROPE_SKIP)
        bDirect = false;

    return bDirect;
}

int CDevDataCfg::getItemSignalPackgeLen(const short &shTestItem)
{
    int bRet = -1;

    auto itFind = s_mapItemPackgeLen.find(shTestItem);
    if(itFind != s_mapItemPackgeLen.end())
    {
        bRet = itFind->second;
    }

    return bRet;
}

bool CDevDataCfg::is433DevType(const short &shDevType)
{
    static set<short> s_set433SupportDev = {DEV_TYPE_FOOTBALL, DEV_TYPE_BASKETBALL, DEV_TYPE_SOLIDBALL, \
                                           DEV_TYPE_PUO, DEV_TYPE_PULL_UP};

    return s_set433SupportDev.find(shDevType) != s_set433SupportDev.end();
}
