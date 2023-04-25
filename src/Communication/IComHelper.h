#ifndef ICOMHELPER_H
#define ICOMHELPER_H

#include <QObject>
#include <QString>

class IComHelper
{
public:
    virtual ~IComHelper(){}

    /**
     * @brief 枚举全部可用的通讯单元
     * @return
     */
    virtual QStringList enumComUnits() = 0;

protected:

    /**
     * @brief 通讯单元名称列表
     */
    QStringList m_strComNameList;
};

#endif // ICOMHELPER_H
