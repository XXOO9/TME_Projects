﻿#include "CDevStePushup.h"
#include "SteSubElement/CSubElementPushup.h"

CDevStePushup::CDevStePushup(unsigned int iDevID, ICommunicate &communicate)
    : CDevHardwareBase (iDevID, communicate)
    , m_shDataLen(sizeof (CElementDataPushup))
{
    m_shDevType = DEV_TYPE_PUSH_UP;

    initSubElement<CSubElementPushup>(SubElementPropertye::ELEMENT_NUM_Pushup);
    initDevInfo();
}

CDevStePushup::~CDevStePushup()
{

}

bool CDevStePushup::calibrateScale(const float &fValue)
{
    //通过校准体测命令参数进行设置
    QByteArray arr;
    CGenerationCommand::getCmdNew(arr, DATA_FRAME_DEV_PARAM_SET, DATA_FRAME_TYPE_SBJ, m_cHardwareCode, m_iDevID, static_cast<unsigned int>(fValue));
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
int CDevStePushup::parseSinglePackage(const unsigned int &iFrameNo, unsigned char *pData, int iDatalen)
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

    static CElementDataPushup elementData;
    static float fData;
    //获取状态位
    getTestStateFromPackgeData(elementData, pData);

    //获取原始数据
    memcpy(&fData, pData + PACK_DATA_MARK + 2, 4);

    //原始数据为float 将float转为int数据单位为（个数）
    elementData.m_nData = static_cast<int>(fData);

    //将数据设置到对应测试单元
    pElement->setElementInfo(&elementData, m_shDataLen);

    return m_iPackgeLen;
}
