#ifndef CFindUserBasic_H
#define CFindUserBasic_H
#include "CUserInfo.h"
#include "CThreadResourceCtrlBase.h"
#include "CHttpConnection.h"
#include "CHttpDownloadThread.h"
#include "CHttpRequestMgr.h"
#include "CHttpUserIdentify.h"

//从服务器查找可能会存在阻塞，这里将查找放在线程中进行
//某一个查找方式被阻塞时不会影响另一个识别的正常执行

class CFindUserBasic : public CThreadResourceCtrlBase
{
    Q_OBJECT

public:
    //人脸特征值 磁卡
    enum emIdentifyType{
        IDENTIFY_ALL = 0,      //全部启用和全部不启用目前暂时没用
        IDENTIFY_ALL_NOT = 1,
        IDENTIFY_BY_FACE_FEATURE = 2,
        IDENTIFY_BY_MAG_CARD = 4,
        IDENTIFY_BY_USER_ID = 8
    };
public:
    CFindUserBasic();
    virtual ~CFindUserBasic() override;

    //设置查找资源,在每个查找当中需要核对查找信息的准确性，每个类需要进行继承重写
    virtual void setFindUserInfo(const CUserInfo &userInfo);

    //启动查找逻辑
    virtual void startFind();

    //停止查找逻辑
    virtual void stopFind();

    //当前查找是否可用
    bool isAvailable();

    //在从服务器查找时可能存在延时，此时其他的方式识别成功，或者开启手动输入时将某些方式设置为本次识别无效
    void setCurFindValid(bool bValid);

    CHttpUserIdentify *pUserIndentify() const;

    void userIndentify( QString faceFeature = QLatin1String("0"), QString cardNo = QLatin1String("0"), FACE_ENGINE facegine = TMEc_Soft, QString code = QLatin1String("0") );

    inline int getIdentifyType() const{return  m_nIdentifyType;}

public slots:
    //处理从服务器发回来的识别结果
    void onUserIdentifyFinished(bool ok);

    void onHttp(QString face, QString cardNo, int faceengine );

protected:
    //处理详细识别逻辑
    virtual void dealThreadLogic() override;

    ///（若不需要在识别中进行特殊处理，在子类中直接继承这两个函数即可即可）
    //本地识别
    virtual void findOnLocal(CUserInfo &findCondition, vector<CUserInfo> &vecFindRes) = 0;
    //服务器识别
    virtual void findOnServer(CUserInfo &findCondition) = 0;

    //初始化识别类
    void initServer();

signals:
    void signalFindSucess( CUserInfo userInfo, void *pLookUpBasic );
    void signalFindFail(void *pLookUpBasic);

    //在本地查不到在服务器找到时通知从服务器获取用户信息
    void signalUpdateUserInfo();
public slots:
    void onServerConnectState(bool bConnect);
protected:
    //服务器连接标志
    std::atomic<bool> m_bServerConnect{false};

    //缓冲变量外部设置查询信息
    CUserInfo m_objUserInfo;
    vector<CUserInfo> m_vecFindRes;

    //当外部识别成功之后，将当前正在识别的所有识别全都设置为无效
    atomic<bool> m_bCurFindValid{true};

    //默认为服务器已经返回，可以进行下一次同步
    atomic_bool     m_isHttpReply{true};

    //表示服务器识别返回
    std::atomic<bool> m_bServerReturn;

    CHttpUserIdentify   *m_pUserIndentify{ nullptr };

    int m_nIdentifyType;
};

#endif // CFindUserBasic_H
