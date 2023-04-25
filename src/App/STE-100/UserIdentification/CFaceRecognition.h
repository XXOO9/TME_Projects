///////////////////////////////////////////////////////////
//  CFaceRecognition.h
//  Implementation of the Class CFaceRecognition
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_3322E0DA_ECD5_4a79_A9E4_1412891ADE17_INCLUDED_)
#define EA_3322E0DA_ECD5_4a79_A9E4_1412891ADE17_INCLUDED_

#include <atomic>
#include <string>

#include "CCamerRegister.h"
#include "commondefin.h"
#include "CThreadResourceCtrlBase.h"

#include "IDetector.h"

using namespace std;

//将符合要求的人脸发送到实际的识别类中进行识别
class CFaceRecognition : public CThreadResourceCtrlBase, public CCamerRegister
{
    Q_OBJECT
public:
    CFaceRecognition();
    virtual ~CFaceRecognition() override;

    //添加识别图像
    void addRecognitionImage(const cv::Mat &image);

    //外部调用开启、关闭识别，这里进行识别才能进行特征值提取进一步进行识别
    void startFaceRecognition();
    void stopFaceRecognition();

    //中间暂停人脸识别
    void pauseFaceRecognition();
    void resumeFaceRecognition();

    bool initArcFaceEngine();

    //重新初始化人脸识别引擎
    void reinitFaceEngine();

    //获取图像识别开始时间
    static const long long& getStartTime() {return s_llStartTime;}
public:
    static void translateTransform(cv::Mat const& src, cv::Mat& dst, int dx, int dy);

    static void translateTransformSize(const cv::Mat &src, cv::Mat &dst, int dx, int dy);
protected:

    //摄像头对象通知新图像触发函数
    virtual void setCamerData(const cv::Mat &matImage) override;

    virtual void dealThreadLogic() override;

signals:
    void signalFindFaceFeature(QString pFeature);
private:
    //当前进行深拷贝的数据，深拷贝，需要进行释放
    cv::Mat m_matCurRecognition;

    std::atomic<bool> m_bFaceEngineRun{false};

    //人脸探测器
    IDetector *m_pFaceDetector{nullptr};

    //人脸特征长度512（轻量级模型）
    int m_nMaxFeatureSize = 1024;
    float m_fFeature[1024]{0.0};

    double m_lfTime{0.0};

    //变量用于保存用于识别的图像完成整个人脸识别流程时的耗时时间
    //仅测试进行人脸识别时间用到，没有其他作用
    static long long s_llStartTime;
};
#endif // !defined(EA_3322E0DA_ECD5_4a79_A9E4_1412891ADE17_INCLUDED_)
