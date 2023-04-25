#ifndef CCAMERINFOFORTEST_H
#define CCAMERINFOFORTEST_H
#include "DshowCameraInfo.h"
#include "CDevCamera.h"

namespace camerType {
    const long camer_OK = 0;
    const long camer_Recognition = 1;
    const long camer_Photograph = 2;
}

//根据当前测试项目和当前体测设备的连接进行相机配置
//调用流程为 先检测相机信息 然后获取错误 然后配置相机

class CCamerInfoForTest
{
public:
    CCamerInfoForTest();

    //根据当前设备类型检查摄像头问题(每一次进行设备切换之后都需要进行摄像头信息检测)
    bool checkCamerInfo(short shSteDevType, bool bDirectConnect);

    //当摄像头信息检测成功之后调用当前函数进行摄像头设置
    //参数分别为识别摄像头和作弊（拍照）摄像头
    void cfgCamer(CDevCamera *pRecognition, CDevCamera *pPhotograph);

    //返回定义的camer_Recognition之类的信息
    vector<long> getErrCamer();

    //两个函数配套使用
    vector<long> getErrCamer(const long &lErrCode);
    const long &getErrCode();
protected:
    //获取测试项摄像头所在位置，opencv所需的摄像头信息
    //传入：设备类型 用途：人脸识别、防作弊拍照
    int getTestItem(short shSteDevType, short shUseType);

    void loadCameraCfgFromDataBase();
private:
    CDshowCameraInfo m_camerainfo;

    //记录识别摄像头和仿作弊摄像头索引，在检测摄像头信息时进行 赋值
    int m_nRecognition{-1};
    int m_nPhotograph{-1};

    long m_lLastErr;

    map<short, string> m_mapTargetCamerName;

    vector<string> m_vCamera;
};

#endif // CCAMERINFOFORTEST_H
