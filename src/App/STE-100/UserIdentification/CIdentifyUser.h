///////////////////////////////////////////////////////////
//  CIdentifyUser.h
//  Implementation of the Class CIdentifyUser
//  Created on:      05-1月-2022 17:28:41
//  Original author: 陈磊
///////////////////////////////////////////////////////////

#if !defined(EA_3DFA1664_0D7F_470b_A347_E230390AE384_INCLUDED_)
#define EA_3DFA1664_0D7F_470b_A347_E230390AE384_INCLUDED_
#include "../../Common/Config/CUserInfo.h"
#include "CFindUserByFaceFeature.h"
#include "CFindUserByMagneticCard.h"
#include "CFindUserByStudentID.h"
#include "CFaceRecognition.h"
#include "../Common/HttpClient/CHttpRequestMgr.h"

#include <string>
#include <QObject>
#include <atomic>
using namespace std;

//用于用户身份识别
class CIdentifyUser : public QObject
{
    Q_OBJECT

public:
	CIdentifyUser();
	virtual ~CIdentifyUser();

    //识别用户状态设置
    void start();
    void stop();
    void pause();
    void resum();

    //设置人脸信息
    void setFindSrcFaceFeature(const QString &strFeature);

    //设置校园卡
    void setFindSrcMagneticCard(const QString &strSrc);

    //设置学生信息
    void setFindSrcStudentID(const QString &strSrc);

    //设置支持的识别类型,外部在进行手动输入的时候会屏蔽其他识别方式
    //函数使用方式需要注意
    void setIdentifyType(int nIdentifyType);
    void resumIdentifyType();

    CFaceRecognition *getFeaceRecongition();

    //获取人脸识别状态，在进行人脸识别查找时不进行图像识别，避免资源占用
    static bool getFaceRecognitionState(){return s_bFindingFace;}

    //重新加载人脸特征
    void reloadFaceFeature();
signals:
    //阻塞方式进行发送
    void signalIdentifySucess(CUserInfo &userInfo);
    void signalIdentifyFalse();
protected slots:
    //刷卡设备触发函数，刷卡成功通过信号触发
    void onFindCardID(string strCardID);

    //人脸识别初次过滤触发函数
    void onFindFaceFeature(QString strFeature);

    //录入ID触发函数
    void onFindStudentID(QString strUserID);

    //服务器状态变更
    void onServerConnectState(bool bConnect);

    //识别成功需要进行一定的处理
    void onIdentifySuccess(CUserInfo userInfo, void *pLookUpBasic);
    void onIdentifyFalse(void *pLookUpBasic);

    //查找人脸状态变更
    inline void onFindFaceStateChange(const bool &bFind){s_bFindingFace = bFind;}
protected:

    //处理身份识别响应 PIV（personal identity verification）
    void procPIV(CUserInfo &userInfo);
private:
    std::atomic<bool> m_bServerConnect{false};

    int m_nLastIdentifyType;
    int m_nIdentifyType{ CFindUserBasic::IDENTIFY_BY_FACE_FEATURE | CFindUserBasic::IDENTIFY_BY_MAG_CARD | CFindUserBasic::IDENTIFY_BY_USER_ID };

    bool m_bIdentityState{false};

    CFaceRecognition m_objFaceRecognition;
    CFindUserByFaceFeature m_objFindFaceFeature;
    CFindUserByMagneticCard m_objFindMagCard;
    CFindUserByStudentID m_objFindStudentID;

    set<CFindUserBasic *> m_setFindUser;

    bool m_bFinding{false};

    QMutex m_mutex;
    //当前人脸识别状态
    static std::atomic<bool> s_bFindingFace;

    CHttpRequestMgr     *m_pRequestMgr;
};
#endif // !defined(EA_3DFA1664_0D7F_470b_A347_E230390AE384_INCLUDED_)
