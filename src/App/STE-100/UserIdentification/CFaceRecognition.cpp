///////////////////////////////////////////////////////////
//  CFaceRecognition.cpp
//  Implementation of the Class CFaceRecognition
//  Created on:      13-9月-2021 18:39:22
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#include "CFaceRecognition.h"
#include "CDatabaseManage.h"
#include "CLogger.h"
#include "CIdentifyUser.h"
#include "CFilePath.h"

#include <fstream>
#include <QDebug>

long long CFaceRecognition::s_llStartTime = 0;

CFaceRecognition::CFaceRecognition()
{
    reinitFaceEngine();
}

CFaceRecognition::~CFaceRecognition()
{
   if(m_pFaceDetector != nullptr)
   {
       m_pFaceDetector->release();

       delete m_pFaceDetector;
   }
}

void CFaceRecognition::addRecognitionImage(const cv::Mat &image)
{
    //若正在识别人脸资源，不进行后续查找动作
    if(CIdentifyUser::getFaceRecognitionState())
    {
        return;
    }

    //暂停识别
    if(!m_bIntoDealProc)
        return;

    //图像无效
    if(image.empty())
       return;

    //正在在识别状态
    if(m_bProcessState)
        return;

    //识别线程未开启
    if(!m_bThreadRunFlag)
        return;

    //释放上一帧资源
    if(!m_matCurRecognition.empty())
    {
       m_matCurRecognition.release();
    }

    m_matCurRecognition = image.clone();

    //添加线程运行资源
    m_semaphoreThread.release();
}

void CFaceRecognition::startFaceRecognition()
{
    startThread();
}

void CFaceRecognition::stopFaceRecognition()
{
    stopThread();
}

void CFaceRecognition::pauseFaceRecognition()
{
    m_bIntoDealProc = false;
}

void CFaceRecognition::resumeFaceRecognition()
{
    //线程在运行状态需要等待一次处理完成再处理图像
    m_bIntoDealProc = true;
}

bool CFaceRecognition::initArcFaceEngine()
{
    //获取人脸探测对象
    m_pFaceDetector = getDetector(false, false, false);

    if(m_pFaceDetector == nullptr)
        return false;

    //设置模式文件路径，如果不设置则会默认去当前运行目录的models文件夹去找,程序自启必须进行设置
    string path = CFilePath::getAppDirPath();
    m_pFaceDetector->setDetectModelPath((path + "/models/face_detector.csta").c_str());
    m_pFaceDetector->setLandMarkerModelPath((path + "/models/face_landmarker_pts5.csta").c_str());
    m_pFaceDetector->setAgePredictorModelPath((path + "/models/age_predictor.csta.csta").c_str());
    m_pFaceDetector->setGenderPredictorModelPath((path + "/models/gender_predictor.csta").c_str());
    m_pFaceDetector->setRecognizerModelPath((path + "/models/face_recognizer.csta").c_str());

    //库写反了
    m_pFaceDetector->setPoseExModelPath((path + "/models/quality_lbn.csta").c_str());
    m_pFaceDetector->setQualtiyLBNModelPath((path + "/models/pose_estimation.csta").c_str());

    //设置最小人脸尺寸
    m_pFaceDetector->setMinFaceSize(80);

    //设置为高精度人脸特征探测，1024个特征点，与服务器冻结存储的特征点值要一一对应
    m_pFaceDetector->setDetectorHighPrecision();

    //设置人脸yaw，pitch，roll的角度，去判定人脸的正面，让探测器进行人脸识别特征提取
    AngleRange range;

    range.fLowYaw = -20;
    range.fHighYaw = 20;
    range.fLowPitch = -20;
    range.fHighPitch = 20;
    range.fLowRoll = -20;
    range.fHighRoll = 20;

    m_pFaceDetector->setFacePoseRange(range);

    //初始化人脸探测对象相关设置
    m_pFaceDetector->initilize();

    m_bFaceEngineRun = true;

    return m_bFaceEngineRun;
}

void CFaceRecognition::reinitFaceEngine()
{
    if(!m_bFaceEngineRun)
    {
        initArcFaceEngine();
    }
}

//平移后不改变图片大小
void CFaceRecognition::translateTransform(const cv::Mat &src, cv::Mat &dst, int dx, int dy)
{
    CV_Assert(src.depth() == CV_8U);

    const int rows = src.rows;
    const int cols = src.cols;

    dst.create(rows, cols, src.type());

    cv::Vec3b *p;
    for (int i = 0; i < rows; i++)
    {
        p = dst.ptr<cv::Vec3b>(i);
        for (int j = 0; j < cols; j++)
        {
            //平移后坐标映射到原图像
            int x = j - dx;
            int y = i - dy;

            //保证映射后的坐标在原图像范围内
            if (x >= 0 && y >= 0 && x < cols && y < rows)
            {
                p[j] = src.ptr<cv::Vec3b>(y)[x];
            }
        }
    }
}

//平移后自适应图片大小
void CFaceRecognition::translateTransformSize(cv::Mat const& src, cv::Mat& dst, int dx, int dy)
{
    CV_Assert(src.depth() == CV_8U);

    const int rows = src.rows + abs(dy); //输出图像的大小
    const int cols = src.cols + abs(dx);

    dst.create(rows, cols, src.type());
    cv::Vec3b *p;
    for (int i = 0; i < rows; i++)
    {
        p = dst.ptr<cv::Vec3b>(i);
        for (int j = 0; j < cols; j++)
        {
            int x = j - dx;
            int y = i - dy;

            if (x >= 0 && y >= 0 && x < src.cols && y < src.rows)
                p[j] = src.ptr<cv::Vec3b>(y)[x];
        }
    }
}

void CFaceRecognition::setCamerData(const cv::Mat &matImage)
{
    //需要注意不能进行耗时操作
    addRecognitionImage(matImage);
}

void CFaceRecognition::dealThreadLogic()
{
    if(m_matCurRecognition.empty())
        return;

    if(!m_bFaceEngineRun)
        return;

    //限制识别人脸速度
    double lfTemp = clock();
    //每隔100ms去检测是否有人脸
    if(lfTemp - m_lfTime < 1000)
    {
        return;
    }

    //
    s_llStartTime = QDateTime::currentMSecsSinceEpoch();

    dealImage(m_matCurRecognition);

    ImageData data;
    data.iWidth = m_matCurRecognition.cols;
    data.iHeight = m_matCurRecognition.rows;
    data.iChannels = m_matCurRecognition.channels();
    data.pData = m_matCurRecognition.data;

    vector<FaceRect> faces;
    //实时检测是否有人脸，去避免识别过程人脸移出造成对空气说识别成功的情况
    if(m_pFaceDetector != nullptr && m_pFaceDetector->detectFaces(&data,faces) > 0)
    {
        //限定人脸识别的区域，需要从界面上根据人脸识别框计算，限定人脸为水平上正中间，垂直上距顶130pix，距低300pix
        FaceRect roiRect;
        //取x为水平上1/5处为起点
        roiRect.x = data.iWidth / 5;
        //取y为水平上1/20处为起点
        roiRect.y = data.iHeight / 20;
        //取水平人脸区域宽度为3/5宽，
        roiRect.width = data.iWidth * 3 / 5;
        //取垂直人脸区域高度为1/10 ~ 4/5之间的高度
        roiRect.height = (data.iHeight * 4 / 5) - roiRect.y;

        //人脸区域，该方法在Debug模式下运行超过1s，Release模式下200ms左右
        FaceRect face;
        bool bSuccess = m_pFaceDetector->detectROIFace(&data, roiRect, face, m_fFeature);
        if(bSuccess)
        {
            //发送人脸特征值到对比查询线程中
            int byteSize = m_nMaxFeatureSize * static_cast<int>(sizeof(float));
            QByteArray byteArray(byteSize, 0);
            memcpy(byteArray.data(), m_fFeature, static_cast<size_t>(byteSize));
            QString strFeature(byteArray.toHex());

            emit signalFindFaceFeature(strFeature);
        }
    }

    m_lfTime = lfTemp;
}
