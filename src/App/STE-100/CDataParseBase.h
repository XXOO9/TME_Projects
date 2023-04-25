#ifndef CDATAPARSEBASE_H
#define CDATAPARSEBASE_H
#include "IDataParse.h"
#include <functional>
using namespace std;
typedef function <void(void *, unsigned int)> NotifyFunc;

//解析类基类，添加统一的缓存资源管理

//统一的内部缓存资源，不允许子类随意修改
class CDataParseBase :public IDataParse
{
public:
    CDataParseBase();
    virtual ~CDataParseBase() override;
    /**
   * 清空数据缓存
   */
    virtual  void clearDataCache();

    /**
      * 数据解析对于当前来说无用
      */
    virtual  void dataParse(QByteArray &array) override{if(array.size())return;}

    //子类继承时最好不变更当前函数（若子类不需要自己管理数据直接重载这个函数）
    virtual void dataParse(unsigned int iLen,void *data);

    /**
   * 设置回调函数，需要在dataParse当中解析成功之后调用，通知外部时数据参数为 pData：指针地址 ， nDataLen：指针字节数
   */
    template <class T>
    void setNotifyFunc(void(T::* p)(void *pData, unsigned int nDataLen), T& obj)
    {
        m_pFunc = std::bind(p, &obj, std::placeholders::_1, std::placeholders::_2);
    }
protected:
    //若子类需要进行数据数据管理，重载当前函数即可
    //详细解析 iSrcLen：传入pData长度 pData：数据首地址 nParsedData：记录当前函数中解析的数据长度
    virtual void dataParseCustomer(unsigned int iSrcLen, unsigned char *pData, unsigned int &nParsedData);

protected:
    NotifyFunc m_pFunc{nullptr};
private:
    //附加数据
    void appendData(void *src, unsigned long lSrcLen);

    //删除已存在数据
    void delExistData(unsigned long lDelLen);

    unsigned long &getExistDataSize();
    unsigned char *getCacheData();
private:
    unsigned char *m_pCacheData{nullptr};
    unsigned long m_lCacheDataSize{1024};
    unsigned long m_lExistLenth{0};
};

#endif // CDATAPARSEBASE_H
