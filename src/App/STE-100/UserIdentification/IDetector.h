#ifndef IDECTOR_H
#define IDECTOR_H


#ifdef _MSC_VER

#define DETECTOR_EXPORTS

#ifdef DETECTOR_EXPORTS
#define DETECTOR_API  __declspec(dllexport)
#else
#define DETECTOR_API __declspec(dllimport)
#endif
#else
#endif

#include<vector>

using namespace std;

//方便其他语音调用该库，定义人脸区域
struct FaceRect
{
	//x轴位置，一般是相对于原始图片
    int x;
	//y轴位置，一般是相对于原始图片
    int y;
	//人脸图像宽度
    int width;
	//人脸图像高度
    int height;
};

//方便其他语音调用该库，定义图片数据，主要用于原始图像数据
struct ImageData
{
	//原始图片宽度
	int iWidth;
	//原始图片高度
	int iHeight;
	//原始图片通道数，限制为3通道
	int iChannels;
	//图像数据
	unsigned char *pData;
};

//方便其他语音调用该库，定义人脸正面判定的角度范围
struct AngleRange
{
	//Yaw 范围(围绕Y轴)
	float fLowYaw;
	float fHighYaw;

	//Pitch 范围（围绕X轴）
	float fLowPitch;
	float fHighPitch;

	//Roll 范围（围绕Z轴）
	float fLowRoll;
	float fHighRoll;
};

/*************************************************
  <类    名>    IDetector
  <功能描述>     对人脸进行年龄、性别、检测和识别等相关特征探测
				是对开源项目SeetaFace6类库的封装，仅采用了该
				开源项目的

				***需要注意的是该类库要求人脸区域图片大小最小为80个像素***

  <说    明>    针对该人脸识别库封装的几点说明：

				1，需要传入是否探测年龄，性别，质量评估标识，建议在
				  仅进行人脸识别的应用场景中不进行年龄和性别探测，
				  人脸图形质量的探测可酌情考虑；

				2，通过对人脸的探测后保存截图来看，人脸的区域会出现
				  额头上部分被截断，在多次测试后如果需要改进，联系
				  作者把人脸区域适当扩大；

				3，建议对人脸图片进行特征提取（常见于服务器端应用）时
				   调用detectTrustFace方法；

				4，建议在需要对人脸区域进行限定时（常见于门禁等刷脸）
				  时调用detectROIFace方法；

				5，被封装的开源库还能进行眼睛，活体，姿态，追踪，口罩
				   的探测，但均不在本次封装中开放

				6，方法调用耗时（I5+16G+集显，单人模式）：
				   a,detectFaces	 ----> 50ms
				   b,detectTrustFace ----> 200ms 
				                     ----> 800ms(附加性别，年龄，质量)
				   c,detectROIFace   ----> 同上

  <作    者>    zhy
  <时    间>    2022-03-03
**************************************************/
class  IDetector
{

public:
    virtual ~IDetector()
    {
    }

    //初始化模型对象
    virtual void initilize() = 0;

    //释放模型对象
    virtual void release() = 0;

    //探测出全部人脸，传入3通道图像数据，传出脸部区域，返回人脸数量
    virtual int detectFaces(ImageData *pImage, vector<FaceRect> &faces) = 0;

    //探测出某一区域的人脸，传入3通道图像数据，传出脸部区域，识别特征，返回当前人脸的分数
    virtual bool detectROIFace(ImageData *pImage, FaceRect roiRect, FaceRect &faceRect, float *pFeature) = 0;

    //探测最可信的人脸，传入3通道图像数据，传出脸部区域，识别特征，返回当前人脸的可信分数
    virtual float detectTrustFace(ImageData *pImage, FaceRect &faceRect, float *pFeature) = 0;

    //获得年龄（-1-不探测，0-失败）
    virtual int getFaceAge() = 0;

    //获得性别(-1-不探测，0-失败，1-男性，2-女性)
    virtual int getFaceGender() = 0;

    //获得质量评估结果描述
    virtual const char *getQualtiy() = 0;

	//设置人脸正面姿态的角度范围,如果不设置将以默认值判定，欧拉角±5
	virtual void setFacePoseRange(AngleRange range) = 0;

    //计算人脸相似度，传入512长度的特征值，非轻量级识别模型需要传入1024长度
    virtual float getRecognizeScore(const float *pFeature1, const float *pFeature2) = 0;

	//设置探测器为高精度模式，此时人脸特征为1024个浮点数；默认为轻量级模式，512个浮点数
	//如果需要采用高精度模式，需要在探测器初始化前完成设置
	virtual void  setDetectorHighPrecision() = 0;

	//设置检测器能检测人脸大小的下限，默认值为20，该值越大计算的速度会提升，但是小于该范围的人脸将不能探测
	//一般在服务器导出图片生成人脸特征是采用默认值，在使用客户端可以 根据实际情况调整该值，来达到一定距离才开始探测的功能
	virtual void  setMinFaceSize(int iFaceSize) = 0;

    //设置人脸检测模型路径，默认为当前路径的models文件夹下对应的face_detector.csta文件
    virtual void setDetectModelPath(const char *strModelPath) = 0;

    //设置人脸5点特征检测模型路径，默认为当前路径的models文件夹下对应的face_landmarker_pts5.csta文件
    virtual void setLandMarkerModelPath(const char *strModelPath) = 0;

    //设置年龄探测模型路径，默认为当前路径的models文件夹下对应的age_predictor.csta件
    virtual void setAgePredictorModelPath(const char *strModelPath) = 0;

    //设置性别探测模型路径，默认为当前路径的models文件夹下对应的gender_predictor.csta文件
    virtual void setGenderPredictorModelPath(const char *strModelPath) = 0;

    //设置人脸识别模型路径，默认为当前路径的models文件夹下对应的face_recognizer_light.csta文件
    virtual void setRecognizerModelPath(const char *strModelPath) = 0;

    //设置人脸姿态模型路径，默认为当前路径的models文件夹下对应的pose_estimation.csta文件
    virtual void setPoseExModelPath(const char *strModelPath) = 0;

    //设置人脸质量检测LBN模型路径，默认为当前路径的models文件夹下对应的quality_lbn.csta文件
    virtual void setQualtiyLBNModelPath(const char *strModelPath) = 0;

};

/*********************************************************************************************/
/*                     导出以下接口，方便其他编程语言调用                                    */
/*															                                 */
/*															                                 */
/*********************************************************************************************/

//导出获取创建的探测器对象地址方法
extern "C" DETECTOR_API IDetector *getDetector(bool bAge, bool bGender, bool bQuality);

//导出销毁探测器对象方法
extern "C" DETECTOR_API void releaseDetector(IDetector *pDetector);

//设置探测器为高精度模式，此时人脸特征为1024个浮点数；默认为轻量级模式，512个浮点数
//如果需要采用高精度模式，需要在探测器初始化前完成设置
extern "C" DETECTOR_API void  setDetectorHighPrecision();

//设置检测器能检测人脸大小的下限，默认值为20，该值越大计算的速度会提升，但是小于该范围的人脸将不能探测
//一般在服务器导出图片生成人脸特征是采用默认值，在使用客户端可以 根据实际情况调整该值，来达到一定距离才开始探测的功能
extern "C" DETECTOR_API void setMinFaceSize(int iFaceSize);

//设置人脸正面姿态的角度范围,如果不设置将以默认值判定，欧拉角±5
extern "C" DETECTOR_API void setFacePoseRange(AngleRange range);

//初始化模型对象
extern "C" DETECTOR_API  void initilize();

//释放模型对象
extern "C" DETECTOR_API  void release();

//探测出全部人脸，传入cv::Mat图像数据，传出脸部区域，返回人脸数量
extern "C" DETECTOR_API  int detectFaces(ImageData *pImage, vector<FaceRect> &faces);

//探测出某一区域的人脸，传入cv::Mat图像数据，传出脸部区域，识别特征，返回当前人脸的分数
extern "C" DETECTOR_API  bool detectROIFace(ImageData *pImage, FaceRect roiRect, FaceRect &faceRect, float *pFeature);

//探测最可信的人脸，传入cv::Mat图像数据，传出脸部区域，识别特征，返回当前人脸的可信分数
extern "C" DETECTOR_API  float *detectTrustFace(ImageData *pImage, FaceRect &faceRect);

//获得年龄（-1-不探测，0-失败）
extern "C" DETECTOR_API  int getFaceAge();

//获得性别(-1-不探测，0-失败，1-男性，2-女性)
extern "C" DETECTOR_API  int getFaceGender();

//获得质量评估结果描述
extern "C" DETECTOR_API  const char *getQualtiy();

//计算人脸相似度，传入1024长度的特征值
extern "C" DETECTOR_API  float getRecognizeScore(const float *pFeature1, const float *pFeature2);

//设置人脸检测模型路径，默认为当前路径的models文件夹下对应的face_detector.csta文件
extern "C" DETECTOR_API  void setDetectModelPath(const char *strModelPath);

//设置人脸5点特征检测模型路径，默认为当前路径的models文件夹下对应的face_landmarker_pts5.csta文件
extern "C" DETECTOR_API  void setLandMarkerModelPath(const char *strModelPath);

//设置年龄探测模型路径，默认为当前路径的models文件夹下对应的age_predictor.csta件
extern "C" DETECTOR_API  void setAgePredictorModelPath(const char *strModelPath);

//设置性别探测模型路径，默认为当前路径的models文件夹下对应的gender_predictor.csta文件
extern "C" DETECTOR_API  void setGenderPredictorModelPath(const char *strModelPath);

//设置人脸识别模型路径，默认为当前路径的models文件夹下对应的face_recognizer_light.csta文件
extern "C" DETECTOR_API  void setRecognizerModelPath(const char *strModelPath);

//设置人脸姿态模型路径，默认为当前路径的models文件夹下对应的pose_estimation.csta文件
extern "C" DETECTOR_API  void setPoseExModelPath(const char *strModelPath);

//设置人脸质量检测LBN模型路径，默认为当前路径的models文件夹下对应的quality_lbn.csta文件
extern "C" DETECTOR_API  void setQualtiyLBNModelPath(const char *strModelPath);

/*********************************************************************************************/
#endif // IDECTOR_H
