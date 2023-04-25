///////////////////////////////////////////////////////////
//  CTestStateManage.h
//  Implementation of the Class CTestStateManage
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_E64F0DFB_6480_4fca_B0EB_FE34F7991530_INCLUDED_)
#define EA_E64F0DFB_6480_4fca_B0EB_FE34F7991530_INCLUDED_
#include <atomic>
#include <QObject>

class CTestStateManage :public QObject
{
    Q_OBJECT
public:
    enum emTest_State{
          TEST_NOT_START    //测试未开始
        , TEST_PREPARE      //准备测试，中间可以设置等待时间
        , TEST_CHECKED      //自检完成 设备调用开始命令，提示开始测试
        , TEST_IN_PROCESS   //测试中
        , TEST_COMPLETED    //测试完成
        , TEST_ERR          //测试出错，一般是设备出现问题
        , TEST_TIMEOUT      //超时结束（算测试完成， 出现的场景是设备重连之后设备内部重置状态为未测状态）
    };

    enum emTest_Model{
          USER_MODE   // 用户模式
        , VISITORS    // 游客模式
    };

signals:
    void signalTestStateChange(short shTestStatus);
public:
    CTestStateManage();
    virtual ~CTestStateManage();
    CTestStateManage(const CTestStateManage &obj);
    CTestStateManage &operator = (const CTestStateManage &obj);
public:
    inline short getTestState() const{return m_shTestState;}

    void setTestState(const short &shTestStatus);

    inline short getTestModel() const{return  m_shTestModel;}
    inline void  setTestModel(short shTestModel){m_shTestModel = shTestModel;}

    inline bool isVisitorMode() const {return m_shTestModel == VISITORS;}
private:
    std::atomic<short> m_shTestState;
    std::atomic<short> m_shTestModel;
};
#endif // !defined(EA_E64F0DFB_6480_4fca_B0EB_FE34F7991530_INCLUDED_)
