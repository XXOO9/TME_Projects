#ifndef CTestProcAntiCheatingBasic_H
#define CTestProcAntiCheatingBasic_H

#include "CDevCamera.h"
#include "commondefin.h"

#include <QTimer>
#include <string>
#include <vector>
#include <QDateTime>

using namespace std;

//测试过程防作弊处理，主要是进行图像分析，这里使用一种简单的方式进行分析
//每隔2s进行一次图像的抓取，抓取过程需要进行人脸图像的简单判断，如人脸大小判断和人脸存在判断
//若符合要求那么进行图像保存，在结束时，选三张进行图像显示，同时进行其余额外图像的删除

//分不同的测试项目进行照片的提取

//肺活量，有数据就进行提取，每隔1s进行一次抓取

//仰卧起做，第一次数据变更进行抓取，之后每隔1s进行一次数据抓取

//坐位体前屈,数据变更就进行抓取，每隔 0.5s抓取一次

//立定跳远，采用录制视屏的方式进行

//引体向上，数据变更一次就进行一次抓取

class CGrabImage{
public:
    CGrabImage(const CGrabImage &image){ m_matImage = image.m_matImage; m_datetimeGrabTime = image.m_datetimeGrabTime;}
    explicit CGrabImage(const cv::Mat &mat){ m_matImage = mat; m_datetimeGrabTime = QDateTime::currentDateTime();}

    const QDateTime& getGrabTime()const{return m_datetimeGrabTime;}
    cv::Mat &getImage() {return m_matImage;}
//mat
private:
    cv::Mat m_matImage;
    QDateTime m_datetimeGrabTime;
};

class CTestProcAntiCheatingBasic : public QObject
{
    Q_OBJECT
public:
    CTestProcAntiCheatingBasic();
    ~CTestProcAntiCheatingBasic();

    //测试开始之前需要全部进行设置
    void setCamer(CDevCamera *pCamer);
    void setRootPath(const string &strRootPath);
    void setTestItemDescribe(const string &strTestDescribe);

    //测试完成之后调用
    void getResourceList(vector<string> &vecResourece, int &nExitFileNum);

    //这里提供的三个函数是等待函数，等待数据处理完成
    void start_();
    void stop_(bool bNormalStop);
    void record_();
//这里的信号选择方式全部选择线程等待直连
signals:
    /**
     *  根据实际情况进行响应函数的编写，一般不在外部进行调用
     */
    //开始
    void signalStart();
    //结束
    void signalStop(bool bNormalStop);
    //记录
    void signalRecord();
protected slots:
    virtual void onStart();
    virtual void onStop(bool bNormalStop);
    //记录过程每个不同一定要进行处理
    virtual void onRecord() = 0;
    //定时器有两种作用，一种是定时抓拍，一种是视屏录制超出某个时长就重新进行录制
    virtual void onTimeOut();
protected:
    //获取随机值
    static int getRandom(int min,int max);

    //当前类不正常停止的处理都一致，正常处理的逻辑取决于实际项目
    virtual void stopNormal();

    //立马抓取一次
    virtual void grab();

    //创建定时器
    void createTimer();
    void startTimerC(const int &nMsec);
    void stopTimer();
    bool isTimerStart();

    void clear() {m_vecGrabImage .clear(); m_vecPath.clear();}
protected:
    //在线程中创建，不进行多次创建
    QTimer *m_pTimer{nullptr};
    //是否执行定时器功能
    bool m_bExecuteTimerFunc{false};
    int m_itimerInterval{1000};

    //存储位置根目录
    string m_strRootPath;

    //当前存储项目描述
    string m_strTestItemDescribe;

    //存储位置，这个是实际使用的变量, 由 m_strRootPath + m_strTestItemDescribe,不包含文件类型
    //和对应的序号
    string m_strFileStorageName;

    CDevCamera *m_pCamer{nullptr};

    vector<string> m_vecPath;
    vector<CGrabImage> m_vecGrabImage;

    //测试项目类型
    short m_shTestItem;

    //每一次再开始之前获取当前文件夹中的相同类型文件数，将数量作为当前索引
    int m_nExitFileNum{0};

    //用于保存图像的递增选项
    int m_nExitFileNumTemp;

    //文件保存格式
    string m_strFileSaveFormat{".png"};

    //文件名称
    static string s_strVideoName;
};

#endif // CTestProcAntiCheatingBasic_H
