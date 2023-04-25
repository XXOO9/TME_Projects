#ifndef CSERIALHLEIPER_H
#define CSERIALHLEIPER_H

#include <QSerialPortInfo>
#include <QDebug>
#include <QString>
#include <map>

using namespace std;

#include "IComHelper.h"

/*************************************************
  <类    名>    CSerialHelper
  <功能描述>     串口助手，主要用于枚举需要用到的通信串口名称

  <作    者>    zhy
  <时    间>    2021-11-22
**************************************************/
class CSerialPortHelper : public IComHelper
{
public:

    explicit CSerialPortHelper();
    virtual ~CSerialPortHelper() override;

    virtual QStringList enumComUnits() override;

    /**
     * @brief 设置需要的目标描述信息，否则枚举全部通讯接口
     * @param strTarget描述信息
     */
    void setComUnitDescription(const QString &strTarget);

    /**
     * @brief 获取全部目标串口，
     * @return QList
     */
	map<QString, QSerialPortInfo> getAllTargetPort();

	/**
	 * @brief 根据通讯单元名称获取串口信息
	 * @return QList
	 */
	QSerialPortInfo getOneSerialPortInfo(QString strUnitsName);

    /**
     * @brief 清理可用串口号列表
     *
     */
     void clearTargetName();

private:
    /**
     * @brief 枚举串口
     */
    void searchTargetPort();

private:

    /**
     * @brief 目标串口描述信息字符串
     */
    QString m_strTargetName;

    /**
     * @brief 可用的串口号链表
     */
    map<QString, QSerialPortInfo>  m_usablePortInfo;
};

#endif // CSERIALHLEIPER_H
