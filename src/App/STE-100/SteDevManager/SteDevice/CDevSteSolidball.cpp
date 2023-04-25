#include "CDevSteSolidball.h"
#include "SteSubElement/CSubElementSolidball.h"
#include "Config/CDatabaseManage.h"
#include "CDevDataCfg.h"

CDevSteSolidball::CDevSteSolidball(unsigned int iDevID, ICommunicate &communicate)
    :CDevHardwareBase(iDevID, communicate)
    , m_shDataLen(sizeof (CElementDataSolidball))
{
    m_shDevType = DEV_TYPE_SOLIDBALL;

    initSubElement<CSubElementSolidball>(SubElementPropertye::ELEMENT_NUM_Solidball);

    initDevInfo();

    int nPos = 0;
    CDatabaseManage::GetInstance().getDevTestInfoConfig()->getStandingJumpStartPos(nPos);
    setStartPos(nPos);
}

CDevSteSolidball::~CDevSteSolidball()
{
}

void CDevSteSolidball::setStartPos(const int &pos)
{
    cmdInfo info;
    info.shCmdType = DATA_FRAME_DEV_PARAM_SET;
    info.nFrameNo = CDevDataCfg::s_nFrameNo++;
    info.nParam = static_cast<uint>(pos);

    CGenerationCommand::getCmdNew(info.arrCmd, DATA_FRAME_DEV_PARAM_SET, DATA_FRAME_SOLIDBALL, m_cHardwareCode, m_iDevID, info.nParam);
    updateCmd(info);
}

bool CDevSteSolidball::getCalibrationSPosFlag()
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
int CDevSteSolidball::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
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

    static CElementDataSolidball elementData;
    //获取状态位
    getTestStateFromPackgeData(elementData, pData);

    //获取数据，原始数据cm转为m
    memcpy(&elementData.m_fData, pData + PACK_DATA_MARK + 2, 4);
    elementData.m_fData /= 100;

    //将数据设置到对应测试单元
    pElement->setElementInfo(&elementData, m_shDataLen);

    return m_iPackgeLen;
}
