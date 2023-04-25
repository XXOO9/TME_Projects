#include "CDevSteSBJ.h"
#include "SteSubElement/CSubElementSBJ.h"
#include "Config/CDatabaseManage.h"
#include "CDevDataCfg.h"

// 立定跳远
CDevSteSBJ::CDevSteSBJ(unsigned int iDevID, ICommunicate &communicate)
    :CDevHardwareBase (iDevID, communicate)
    , m_shDataLen(sizeof (CElementDataSBJ))
{
    m_shDevType = DEV_TYPE_SBJ;

    initSubElement<CSubElementSBJ>(SubElementPropertye::ELEMENT_NUM_SBJ);

    initDevInfo();

    //从数据库加载目标起点位置
    int nPos = 0;
    CDatabaseManage::GetInstance().getDevTestInfoConfig()->getStandingJumpStartPos(nPos);
    setStartPos(nPos);

    m_iGetDataInterval = 1000;
}

CDevSteSBJ::~CDevSteSBJ()
{

}

void CDevSteSBJ::setStartPos(const int &pos)
{
    unsigned short shSetValue = pos == 80 ? Start_Pos_80 : Start_Pos_0;

    cmdInfo info;
    info.shCmdType = DATA_FRAME_DEV_PARAM_SET;
    info.nFrameNo = CDevDataCfg::s_nFrameNo++;
    info.nParam = static_cast<uint>(shSetValue);

    CGenerationCommand::getCmdNew(info.arrCmd, DATA_FRAME_DEV_PARAM_SET, DATA_FRAME_TYPE_SBJ, m_cHardwareCode, m_iDevID, info.nParam);
    updateCmd(info);
}

bool CDevSteSBJ::getCalibrationSPosFlag()
{
    bool bRet = true;
    auto listCmd = getCmd();
    for(auto &cmd : listCmd)
    {
        if(cmd.shCmdType == DATA_FRAME_DEV_PARAM_SET)
        {
            bRet = false;
            break;
        }
    }

    return bRet;
}

/*************************************************
  <函数名称>    parseSinglePackage(int iStartPos, QByteArray &array)
  <功能描述>    在独立通讯模式下的单点数据解析（需要沿用父类的通用处理）
  <参数说明>    int iStartPos 数据开始位置
               QByteArray &array 单点数据内容

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
int CDevSteSBJ::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
{
    int nLen = CDevHardwareBase::parseSinglePackage(iFrameNo, pData, iDatalen);

    if(nLen > 0)
        return nLen;

    //到这一步能保证数据为数据帧中的 单包数据，只进行当前数据解析即可
    //检测数据有效性
    if(!checkSignalPackageValid(pData, iDatalen))
        return iDatalen;


    //解析数据设置到测试单元中
    CSubElementBase *pElement = getElement(0);
    if(pElement == nullptr)
    {
        LOGERROR("pElement Err invalid");
        return m_iPackgeLen;
    }

    static CElementDataSBJ elementData;
    //获取状态位
    getTestStateFromPackgeData(elementData, pData);

    //获取数据
    memcpy(&elementData.m_fData, pData + PACK_DATA_MARK + 2, 4);

    //原始数据转为标准数据
    getNormalData(elementData.m_fData, pElement->getCurTestItem());

    //将数据设置到对应测试单元
    pElement->setElementInfo(&elementData, m_shDataLen);

    return m_iPackgeLen;
}
