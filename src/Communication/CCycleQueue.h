/*************************************************
  <类    名>    CCycleQueue
 <描		述>	CycleQueue.h 是CycleQueue.cpp的头文件。
 <说		明>	该类是工具类，实现一个大小为800K大小的循环队列
 <作		者>	zhy
 <创建 日期>	2021-4-21
**************************************************/

#ifndef CYCLE_QUEUE1_HH_
#define CYCLE_QUEUE1_HH_
#include <QMutex>

template<class T>
class  CCycleQueue
{
private:

    // 默认的循环队列大小为800K个数个数据
    enum { BUF_SIZE = 80000 };

    // 循环队列写指针（write pointer)
    unsigned long wp{0};

    // 循环队列读指针（read pointer)
    unsigned long rp{0};

    // 循环队列翻转标志
    bool m_bReverse_Flag{false};

    // 循环队列的大小
    unsigned long m_lTotal_Queue_Size{0};

    // 循环队列存贮控件指针
    T* m_ucBuffer{nullptr};

public:
    CCycleQueue(void);
    CCycleQueue(unsigned long lSize);
    ~CCycleQueue(void);

    /**
     * @brief 读取一定长度的数据
     * @param iLength
     * @param buffer
     * @return
     */
    unsigned int read(unsigned int iLength, T* buffer);

    /**
     * @brief 写入一定长度的数据
     * @param iLength
     * @param buffer
     * @return
     */
    unsigned int write(unsigned int iLength, const T* buffer);

    /**
     * @brief 获取缓冲区中的数据字节数
     * @return
     */
    unsigned long getSizeInQueue(void);

    /**
     * @brief 清空队列
     */
    void clearQueue(void);

private:
    /**
     * @brief 分配一定大小的缓存区
     * @param lSize
     * @return
     */
    bool allocateBuffer(unsigned long lSize = BUF_SIZE);

    /**
     * @brief 释放缓冲区
     */
    void freeBuffer(void);

    /**
     * @brief 初始化循环队列
     * @param lSize
     */
    void initialize(unsigned long lSize = BUF_SIZE);

    QMutex m_mutexReadWrite;
};

//模板的分离式编程必须包含CPP文件
#include "CCycleQueue.cpp"

#endif
