#ifndef IDATAPARSE_H
#define IDATAPARSE_H

#include <QByteArray>
#include <QObject>

/*************************************************
  <类    名>     IDataParse
  <功能描述>     数据解析类的抽象类，通过具体设备从指定通讯接口取数据

  <作    者>     zhy
  <时    间>     2021/9/22
**************************************************/
class IDataParse : public QObject
{
	Q_OBJECT

public:
    IDataParse(){}
    virtual ~IDataParse(){}
protected:
    /**
      * 数据解析
      */
    virtual  void dataParse(QByteArray &array) = 0 ;
};

#endif // IDATAPARSE_H
