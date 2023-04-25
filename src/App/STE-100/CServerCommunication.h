///////////////////////////////////////////////////////////
//  CServerCommunication.h
//  Implementation of the Class CServerCommunication
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_0246BBC4_5B3C_4005_A519_7D7D6EDB4D74_INCLUDED_)
#define EA_0246BBC4_5B3C_4005_A519_7D7D6EDB4D74_INCLUDED_
#include <QString>
#include <QSemaphore>
#include <QUrl>
#include <vector>
#include <functional>
#include <map>

#include <QTimer>
#include <QMutex>
#include <QNetworkConfigurationManager>
#include <QProcess>

#include "CHttpConnection.h"
#include "CHttpDownloadThread.h"
#include "CThreadResourceCtrlBase.h"
#include "CDatabaseManage.h"
#include "CHttpRequestMgr.h"
#include "../Common/CSingletonPattern.h"

//class MainFrame;

using namespace std;

//和服务器的通讯不单是通过这个类进行沟通，这个类的作用是检测服务器存在和自动更新数据
//线程的作用是从服务器更新数据，定时器用于判断和服务器的连接
//若服务器不存在也向外抛出得分信息

//总体流程为更新学生信息，通知人脸识别类更新人脸信息
//更新得分规则之后，通知  CLookUpFaceFeature 类更新人脸信息
//更新完成之后往外发送信号即可

typedef function<bool()> pUpdateFunc;


class CServerCommunication : public CThreadResourceCtrlBase , public CSingletonPtr<CServerCommunication>
{
    Q_OBJECT
    DECLARE_SINGLETON_CLASS(CServerCommunication)
public:
    enum emUpdateType{
        UPDATE_TYPE_TEST_RESULT, //向服务器更新为上传的学生表格信息，触发是在测量完成上传失败，和开启软件之后
        UPDATE_TYPE_VEDIO_VC, //获取肺活量指导视频
        UPDATE_TYPE_VEDIO_SBJ, //获取立定跳远指导视频
        UPDATE_TYPE_VEDIO_SFB, //获取坐位体前屈指导视频
        UPDATE_TYPE_VEDIO_PULLUP, //获取引体向上指导视频
        UPDATE_TYPE_VEDIO_SITUP, //获取仰卧起坐指导视频
        UPDATE_TYPE_VEDIO_RUN, //获取跑步指导视频
        UPDATE_TYPE_TOURIST_USAGE, //游客模式使用的次数
        UPDATE_TYPE_COUNT
    };

    enum emServerTestItemNature{
        TEST_NATURE_EXERCISE = 1, //练习模式
        TEST_NATURE_TEST = 2   //测试模式
    };

    CServerCommunication();
    virtual ~CServerCommunication() override;

    //开启活动，包含开启更新线程和开启心跳
    void startActive();

    //关闭活动，心跳和更新线程
    void stopActive();

    //设置需要更新的表格类型
    void setUpdateType(short shType);

    //下载视频
    void startDownloadSampleVideo( QVector< structSampleVideoSync > vecSampleVideoInfos );

    //获取下载进度
    void onHttpDownloadProcess( QString downloadPersent );

    //所有下载完成
    void onHttpDownloadFinished();

    /************************新版HTTP接口**********************************/
public slots:

    //响应游客使用记录
    void onTouristUsageRecord(bool ok , bool netWorkState, int requestCode);

private:
    //初始化m_pRequestMgr的信号槽
    void initNetWork();
private:
    CHttpRequestMgr     *m_pRequestMgr{nullptr};
    /********************************HTTP end*******************************************/

protected:
    virtual void dealThreadLogic() override;

    //更新测试成绩信息，更新到服务器
    bool updateTestResult();

    //更新得分规则,从服务器更新
    bool updateScoreRule();

    //更新服务器视频
    bool updateVedios();

    //更新使用次数
    bool updateTouristUsageCount();

    //注册更新处理函数
    template<class T>
    void setUpdateFunc(short shUpdateType, bool(T::*p)(), T &obj)
    {
        pUpdateFunc pFunc = std::bind(p, &obj);
        m_mapUpdateProcess.insert(make_pair(shUpdateType, pFunc));
    }

    //从容器缓存中找到身高体重数据, 参数 vector源数据 nIndex开始往后找寻索引 string ：测试结果时间
    vector<size_t> findBodyData(const vector<CTestResultInfo> &vector, size_t nIndex, string &strTestTime);
public:
    //资源访问锁
    QMutex m_mutexSource;

    //用于保存更新的数据表格内容
    vector<short> m_vecUpdateList;

    //服务器连接状态
    std::atomic<bool> m_bServerConnect{false};

    //更新函数
    map< short, pUpdateFunc > m_mapUpdateProcess;

    //下载视频的http downloader
    HttpDownloadThread *m_pVideoDownloader{nullptr};

    //游客模式上传反馈
    atomic<bool> m_bServerReturn{false};
    atomic<bool> m_bTouristReturnValue{false};
};
#endif // !defined(EA_0246BBC4_5B3C_4005_A519_7D7D6EDB4D74_INCLUDED_)
