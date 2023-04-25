#ifndef CFindUserByFaceFeature_H
#define CFindUserByFaceFeature_H

#include "CFindUserBasic.h"
#include <map>
#include <string>
#include "CHttpRequestMgr.h"
using namespace std;

/*************************************************
  <类    名>    CFindUserByFaceFeature
  <功能描述>     通过人脸特征值进行身份识别
                1，本次改动逻辑为先在本地进行识别，如果没有没有识别成功，则去服务器进行识别，
                   原因为每次开始体测是会先同步用户数据到本地，先在本地识别可以提高速度；

                2，本次改动后引用中科视拓商业级开源识别库SeetaFace6，该库有2种模式；轻量级
                   和高精度模式。分别在软件中进行了测试（开发过程中样本有限），轻量级识别相似
                   度得分下限在0.6，高精度为0.8，表明高精度的确对人脸识别最有效。但是这个模式
                   的特征值为1024个浮点数，通过服务器识别时，会占用较高带宽；

                3，采用高精度模式会让本地加载人脸特征库时占用较高的内存，10000用户仅特征值将占用40M

  <作    者>    zhy
  <时    间>    2022-03-20
**************************************************/

/*************************************************
  <类    名>    UserFaceFeature
  <功能描述>     主要用于加载本地数据库中的人脸特征，直接转换为float数据
               提高计算效率

  <作    者>    zhy
  <时    间>    2022-04-08
**************************************************/
namespace faceFeatureProperty {
    enum {featureLen = 4096}; //人脸特征值长度 byte
}

struct UserFaceFeature
{
    long long lUserID;
    float fFeatures[faceFeatureProperty::featureLen / 4];
};

class CFindUserByFaceFeature : public CFindUserBasic
{
    Q_OBJECT
public:
    CFindUserByFaceFeature();
    virtual ~CFindUserByFaceFeature() override;

    //设置查找资源,在每个查找当中需要核对查找信息的准确性，每个类需要进行继承重写
    virtual void setFindUserInfo(const CUserInfo &userInfo) override;

    //更新人脸信息（外部更新完成人脸信息之后将人脸信息重新进行加载）
    void setUpdateFaceFeature(bool bUpdate);

    //启动查找逻辑(和父类的差别在于，第一次开启之后触发一次线程进行加载人脸信息)
    virtual void startFind() override;

    //重新加载人脸，在用户信息加载完成之后重新加载人脸
    void reloadFaceFeature();
signals:
    void signalFindFaceState(const bool &bFind);

protected:
    virtual void dealThreadLogic() override;

    //本地识别
    virtual void findOnLocal(CUserInfo &findCondition, vector<CUserInfo> &vecFindRes) override;
    //服务器识别
    virtual void findOnServer(CUserInfo &findCondition) override;

    //从本地加载同步好的用户信息
    bool loadFeature();

    //从配置文件加载置信度
    void loadFaceConfidence(float &fConfidence);

    void initConnection();

    void getFaceConfidence(float &fConfidence);
private:
    //计算两个特征值之间的可信度分数
    float calculateConfidenceScore(QString &strHotFeature, QString &strFrozenFeature);

    //计算两个特征值之间的可信度分数
    float calculateConfidenceScore(float *fHotFeature, float *fFrozenFeature, int featureSize);

private:
    //验证一个人脸特征值多大，一个人脸特征值约为4096字节(4k) 10000个约为 40M，加载在程序中基本不存在影响
    //采用哪一个容器作为存储在之后进行详细的优化,在线程中进行加载
    //前面对应的用户ID，由服务器决定，后面为人脸特征
    vector<UserFaceFeature> m_vFrozenFeatures;

    //加载人脸标志
    std::atomic<bool> m_bLoadFaceFeature{false};

    //从文件进行加载，以控制人脸识别的识别度要求 范围 从 0 ~ 1，值越大越相似
    float m_fFaceConfidence{0.62f};

    //定义特征数据长度，目前可以是512和1024两个长度，根据人脸识别时加载的模型库确定（轻量级和高精度级）
    const static int FEATURE_SIZE{1024};
};

#endif // CFindUserByFaceFeature_H
