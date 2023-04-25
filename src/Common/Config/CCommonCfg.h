#ifndef CCommonCfg_H
#define CCommonCfg_H
#include <QSettings>
#include <string>
#include <QMutex>
#include "../common_global.h"
using namespace std;

namespace soft {
    enum {
        VERSION_COMM = 0,   //公共版本
        VERSION_CD_PSB,   //成都公安局
        VERSION_QLNJXX,   //邛崃南街小学
        VERSION_SDJN      //山东济南版本
    };
}

//初始加载全部的信息
class COMMONSHARED_EXPORT CCommonCfg
{
private:
    CCommonCfg();
    ~CCommonCfg();
    CCommonCfg(const CCommonCfg &manager);
    CCommonCfg &operator = (const CCommonCfg &manager);

    /**
         * 配置管理者静态实例对象
         */
    static CCommonCfg		*s_pManagerInstance;
    static QMutex		    s_csManager_InstanceSafe;

public:
    static CCommonCfg &GetInstance();

    void  init();

    //获取当前人脸识别置信度
    float getFaceConfidence() const {return m_fFaceConfidence;}

    //获取当前体测版本
    int getCurSoftwareVersion() const {return m_nSoftwareVersion;}

    //获取是否需要开启一对多摄像头
    bool getNeedMultiCameraFlag() const {return m_bMultiCameraFlag;}
private:
    void    Default();
    bool    Check_Field(const QSettings& settings, const QString& strField);

    QString   m_strFilePath;
    QSettings m_settings;

    int m_nSoftwareVersion{soft::VERSION_COMM};
    bool m_bMultiCameraFlag{false};
    float m_fFaceConfidence{0.685f};
};

#endif // CARCSOFTCONFIG_H
