#include "CSerialPortHelper.h"

CSerialPortHelper::CSerialPortHelper()
{
    //不同的产品根据所用串口的描述信息进行设置
	//如果默认不提供描述信息则进行全部枚举
    m_strTargetName = "";
}

CSerialPortHelper::~CSerialPortHelper()
{

}

QStringList CSerialPortHelper::enumComUnits()
{
	//搜索需要的目标串口
	searchTargetPort();

    return  m_strComNameList;
}

void CSerialPortHelper::setComUnitDescription(const QString &strTarget)
{
    m_strTargetName = strTarget;
}

/*************************************************
  <函数名称>    searchPort
  <功能描述>    查找目标串口,注意Windows平台上要删掉蓝牙链接上的标准串行
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   cpc
  <时    间>   2021-09-10
  **************************************************/
void CSerialPortHelper::searchTargetPort()
{
    //枚举可用的串口
    QList<QSerialPortInfo> m_allPortInfo = QSerialPortInfo::availablePorts();

    m_usablePortInfo.clear();
    m_strComNameList.clear();

    if(m_allPortInfo.isEmpty())
        return;

    //读取可用串口信息
    for(auto &info : m_allPortInfo)
    {
        //串口消息需要跳过 虚拟串口 及 蓝牙链接
        if(info.description().indexOf("Virtual") >= 0 ||
           info.description().indexOf(QStringLiteral("蓝牙")) >= 0)
        {
            //qDebug()<<"Name: "<<info.portName();
            //qDebug()<<"systemLocation: "<<info.systemLocation() ;
            //qDebug()<<"description: "<<info.description();
            continue;
        }

        if (m_strTargetName.length() == 0)
        {
            m_strComNameList << info.portName();
            m_usablePortInfo.insert(make_pair(info.portName(), info));
        }
        else
        {
            //查找特定的串口设备
            if (info.portName().contains(m_strTargetName))
            {
                m_strComNameList << info.portName();
                m_usablePortInfo.insert(make_pair(info.portName(), info));
            }
        }
    }
}

/*************************************************
  <函数名称>    getAllTargetPort
  <功能描述>    返回查询到的目标串口
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-08-27
  **************************************************/
map<QString, QSerialPortInfo> CSerialPortHelper::getAllTargetPort()
{
    return  m_usablePortInfo;
}

QSerialPortInfo CSerialPortHelper::getOneSerialPortInfo(QString strUnitsName)
{
	QSerialPortInfo info;

	auto it = m_usablePortInfo.find(strUnitsName);
	if (it != m_usablePortInfo.end())
	{
		info = it->second;
	}

	return info;
}

void CSerialPortHelper::clearTargetName()
{
    m_strComNameList.clear();
    m_usablePortInfo.clear();
}
