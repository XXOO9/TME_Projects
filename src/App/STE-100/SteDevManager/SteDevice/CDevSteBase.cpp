#include "CDevSteBase.h"

#include <CSerialPortCom.h>
#include <CSerialPortHelper.h>
#include <CGenerationCommand.h>
#include "CDevDataCfg.h"
using namespace HardwarePropertye;
using namespace DataFrameInfo;

unsigned int CDevSteBase::s_ilastFrameNo(0);
unsigned int CDevSteBase::s_iCurCmdFrameNo(0);
unsigned int CDevSteBase::s_iCurFrameNo(0);

CDevSteBase::CDevSteBase()
{
    if(m_arrCmdHeartbeat.length() == 0)
    {
        unsigned char command[] = {0xFF, 0xFF, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x01, 0x01, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xA9, 0xD0, 0xDB, 0xA5,
                                   0x01, 0x28, 0x00, 0x00, 0xE6, 0x07, 0x04, 0x13, 0x11, 0x0D,
                                   0x32, 0x00, 0x00, 0x00, 0xBC, 0x49, 0x00, 0x08, 0x00, 0x00};

        int nSize = sizeof (command) / sizeof (char);
        m_arrCmdHeartbeat = QByteArray(reinterpret_cast<char *>(command), nSize);
    }
}

CDevSteBase::~CDevSteBase()
{

}

unsigned char CDevSteBase::convertDevType2HardwareCode(const short &shDevType)
{
    unsigned char cHardwareCode = HARDWARE_CODE_SIZE;

    auto &mapHardwareTypeMapping = CDevDataCfg::getHardwareTypeMapping();
    for(auto &element : mapHardwareTypeMapping)
    {
        if(element.second == shDevType)
        {
            cHardwareCode = element.first;
            break;
        }
    }

    return cHardwareCode;
}

short CDevSteBase::convertHardwareCode2DevType(const unsigned char &cHardwareCode)
{
    short shDevType = DEV_TYPE_UNKNOW;

    auto &mapHardwareTypeMapping = CDevDataCfg::getHardwareTypeMapping();
    auto itFind = mapHardwareTypeMapping.find(cHardwareCode);
    if(itFind != mapHardwareTypeMapping.end())
    {
        shDevType = itFind->second;
    }

    return shDevType;
}

/*************************************************
  <函数名称>    queryDevType()
  <功能描述>    查询设备类型
  <参数说明>
  <返 回 值>

  <函数说明>    向设备发送查询类型命令，在解析数据过程中，根据命令
              响应来判定该设备是否合法

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
void CDevSteBase::queryDevType()
{
    QByteArray cmd;
    CGenerationCommand::getCmdGetInfo(cmd);
    sendData(cmd);
}

void CDevSteBase::dataParse(QByteArray &array)
{
    if(array.size() == 0)
        return;
}

/*************************************************
  <函数名称>    devOpen
  <功能描述>    打开这边
  <参数说明>

  <返 回 值>

  <函数说明>    对于有独立通讯的设备，打开设备构建数据解析过程

  <作    者>   zhy
  <时    间>   2022-02-22
  **************************************************/
bool CDevSteBase::devOpen()
{
    //只有在拥有独立通讯机制的情况下进行打开设备,此时不进行设备有效赋值，需要解析的数据正确才进行赋值
    if(m_bIndependent)
    {
        if(CDevice::devOpen())
        {
            CSerialPortCom *pCom = reinterpret_cast<CSerialPortCom *>(m_pCom);
            if(pCom != nullptr)
            {
                pCom->setDTR(true);
            }
        }
    }
    //非独立设备也需要打开对应的定时器
    else
    {
        m_bOpened = true;

        setUpdateDataState(true);
        emit signalDevStatus(true, this);
        m_bValidDev = true;
    }

    return true;
}

unsigned short CDevSteBase::getNormalCmdFromOrigCmd(const unsigned short &shOrigCmd)
{
    bool bDataFrameType = false;

    auto &setDataTypeCmd = CDevDataCfg::getDataCmdcontainer();

    //底层数据命令存在两种形式（一种是正常指令 一种是最高位置1） 控制命令 只能是最高位 置1
    unsigned short shTemp = shOrigCmd & ~(1 << 15);
    auto itFindDataFrameType = setDataTypeCmd.find(shOrigCmd);
    if(itFindDataFrameType == setDataTypeCmd.end())
    {
        itFindDataFrameType = setDataTypeCmd.find(shTemp);

        //最高位数据置1数据命令
        if(itFindDataFrameType != setDataTypeCmd.end())
        {
            bDataFrameType = true;
        }
    }
    //原始数据命令
    else
    {
        bDataFrameType = true;
    }

    //将获取数据命名统一为获取数据指令,命令指令统一范围最高位置1之后的原始指令
    return  bDataFrameType ? DATA_FRAME_TYPE_GET_DATA : shTemp;
}

unsigned short CDevSteBase::getCmdFromOrigCmd(const unsigned short &shOrigCmd)
{
    auto &setDataTypeCmd = CDevDataCfg::getDataCmdcontainer();

    //底层数据命令存在两种形式（一种是正常指令 一种是最高位置1） 控制命令 只能是最高位 置1
    unsigned short shTemp = shOrigCmd & ~(1 << 15);
    auto itFindDataFrameType = setDataTypeCmd.find(shOrigCmd);
    if(itFindDataFrameType == setDataTypeCmd.end())
    {
        itFindDataFrameType = setDataTypeCmd.find(shTemp);
    }

    //将获取数据命名统一为获取数据指令,命令指令统一范围最高位置1之后的原始指令
    return shTemp;
}

void CDevSteBase::devClose()
{
    m_bValidDev = false;

    //只有在拥有独立通讯机制的情况下进行关闭设备
    if(m_bOpened)
    {
        if(m_bIndependent)
        {
            CDevice::devClose();
        }
        else
        {
            m_bOpened = false;
            setUpdateDataState(false);
        }
    }

    //避免设备发送信息阻塞
    m_bSendCommandSucess = true;
}
