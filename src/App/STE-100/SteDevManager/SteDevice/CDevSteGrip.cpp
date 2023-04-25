#include "CDevSteGrip.h"
#include "SteSubElement/CSubElementGrip.h"

CDevSteGrip::CDevSteGrip(unsigned int iDevID, ICommunicate &communicate)
    :CDevHardwareBase (iDevID, communicate)
    , m_shDataLen(sizeof (CElementDataGrip))
{
    m_shDevType = DEV_TYPE_GRIP;

    initSubElement<CSubElementGrip>(SubElementPropertye::ELEMENT_NUM_Grip);
    initDevInfo();
}

CDevSteGrip::~CDevSteGrip()
{

}

bool CDevSteGrip::calibrateGrip(const float &fValue)
{
    QByteArray arr;
    m_shLastCommand = CGenerationCommand::getCmdCalibrate(arr, DATA_FRAME_TYPE_GRIP, m_cHardwareCode, m_iDevID, fValue);
    return sendData(arr, true, 500, COMMAND_LEVEL_1);
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
int CDevSteGrip::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
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

    static CElementDataGrip elementData;
    //获取状态位
    getTestStateFromPackgeData(elementData, pData);

    //获取数据
    memcpy(&elementData.m_fData, pData + PACK_DATA_MARK + 2, 4);

    //将数据设置到对应测试单元
    pElement->setElementInfo(&elementData, m_shDataLen);

    return m_iPackgeLen;
}
