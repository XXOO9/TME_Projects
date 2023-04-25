#include "CCycleQueue.h"
#include <QDebug>

template<class T>
CCycleQueue<T>::CCycleQueue(void)
{
    // 按默认值初始化参数
    initialize();

    // 创建队列时即按默认值分配了内存
    allocateBuffer();
}

template<class T>
CCycleQueue<T>::CCycleQueue(unsigned long lSize)
{
    initialize( lSize );
    allocateBuffer( lSize );
}

template<class T>
CCycleQueue<T>::~CCycleQueue(void)
{
    freeBuffer();
}

/*************************************************
  <函数名称>    initialize(unsigned long lSize)
  <功能描述>    该函数初始化循环队列
  <参数说明>    lSize	-> 初始化循环队列的字节数

  <返 回 值>    void

  <函数说明>    初始化循环队列的相关变量

  <作    者>   zhy
  <时    间>   2021-11-22
  **************************************************/
template<class T>
void CCycleQueue<T>::initialize(unsigned long lSize)
{
    wp					= 0;
    rp					= 0;
    m_bReverse_Flag		= false;
    m_lTotal_Queue_Size	= lSize;
    m_ucBuffer			= nullptr;
}

/*************************************************
<函数名称>    write(unsigned int iLength, const T* buffer)
<功能描述>    该函数向循环队列中写入指定长度的数据
<参数说明>    lWrite_Number 需要写入的字节数
             buffer 存放写入数据的缓冲区

<返 回 值>    void

<函数说明>    初始化循环队列的相关变量

<作    者>   zhy
<时    间>   2021-11-22
**************************************************/
template<class T>
unsigned int CCycleQueue<T>::write(unsigned int iLength, const T* buffer)
{
    m_mutexReadWrite.lock();
    unsigned int iWriteSize = 0;
    if(m_ucBuffer != nullptr && iLength > 0 && buffer != nullptr)
    {
        if(m_lTotal_Queue_Size > getSizeInQueue())
        {
            unsigned int iAvaliableSize = m_lTotal_Queue_Size - getSizeInQueue();

            iWriteSize = iLength;
            if(iAvaliableSize < iLength)
            {
                iWriteSize = iAvaliableSize;
            }

            unsigned long lSize = sizeof (T) * iWriteSize;
            memcpy(m_ucBuffer + wp, buffer, lSize);
            wp += iWriteSize;
        }
    }

    m_mutexReadWrite.unlock();
    return iWriteSize;
}

/*************************************************
<函数名称>    read(unsigned int iLength, T* buffer)
<功能描述>    该函数向从循环队列中读出指定长度的数据
<参数说明>    iLength	 需要读出的字节数
             buffer	 存放读出数据的缓冲区

<返 回 值>    void

<函数说明>    初始化循环队列的相关变量

<作    者>   zhy
<时    间>   2021-11-22
**************************************************/
template<class T>
unsigned int CCycleQueue<T>::read(unsigned int iLength, T* buffer)
{
    //保护：如果没有分配内存，或读取数据长度错误，返回0.
    m_mutexReadWrite.lock();
    unsigned int iReadLength = 0;

    if(m_ucBuffer != nullptr && iLength > 0 && buffer != nullptr)
    {
        unsigned int iDataSize = getSizeInQueue();
        if(iDataSize > 0)
        {
            iReadLength = iLength;
            if(iDataSize < iLength)
            {
                iReadLength = iDataSize;
            }

            unsigned long lTypeLen = sizeof (T);
            unsigned long lSize = iReadLength * lTypeLen;

            //从内存中读取指定长度数据
            memcpy(buffer, m_ucBuffer, lSize);

            //将剩余数据移动到地址头
            if(wp >= iReadLength)
            {
                wp = wp - iReadLength;

                if(wp > 0)
                {
                    memcpy(m_ucBuffer, m_ucBuffer + iReadLength, wp * lTypeLen);
                }
            }
        }
    }

    m_mutexReadWrite.unlock();
    return iReadLength;
}

/*************************************************
<函数名称>    getSizeInQueue(void)
<功能描述>    该函数得到当前循环队列中存放数据的字节数
<参数说明>

<返 回 值>    循环队列中的有效字节数

<函数说明>

<作    者>   zhy
<时    间>   2021-11-22
**************************************************/
template<class T>
unsigned long CCycleQueue<T>::getSizeInQueue(void)
{
    //如果没有分配内存，返回0
    if ( !m_ucBuffer )
    {
        return ( 0 );
    }

    return wp;
}

/*************************************************
<函数名称>    allocateBuffer(unsigned long lSize)
<功能描述>    该函数为当前队列分配指定字节数
<参数说明>
            lSize	-> 指定分配的字节数
<返 回 值>
             bool false或true
<函数说明>

<作    者>   zhy
<时    间>   2021-11-22
**************************************************/
template<class T>
bool CCycleQueue<T>::allocateBuffer(unsigned long lSize)
{
    //如果已经分配内存，则先释放内存后再分配
    if( m_ucBuffer )
    {
        delete	[]m_ucBuffer;
        m_ucBuffer = nullptr;
    }

    //按指定字节大小分配内存
    m_ucBuffer = new T[lSize];

    //分配成功返回true
    if( m_ucBuffer )
    {
        m_lTotal_Queue_Size	= lSize;
        return ( true );
    }
    //分配失败返回false
    else
    {
        return ( false );
    }
}

/*************************************************
<函数名称>    allocateBuffer(unsigned long lSize)
<功能描述>    该函数释放队列已分配内存
<参数说明>
            void
<返 回 值>
            void
<函数说明>

<作    者>   zhy
<时    间>   2021-11-22
**************************************************/
template<class T>
void CCycleQueue<T>::freeBuffer(void)
{
    if( m_ucBuffer )
    {
        delete []m_ucBuffer;
        m_ucBuffer = nullptr;
    }
}

/*************************************************
  <函数名称>    clearQueue
  <功能描述>    该函数清空循环队列中的所有数据
  <参数说明>    无

  <返 回 值>    无

  <函数说明>    无

  <作    者>   zhy
  <时    间>   2021-11-22
  **************************************************/
template<class T>
void CCycleQueue<T>::clearQueue(void)
{
    wp				= 0;
    rp				= 0;					/// 读写指针都设置为0
    m_bReverse_Flag	= false;				/// 翻转标记为false
}
