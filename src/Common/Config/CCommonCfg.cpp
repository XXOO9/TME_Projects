#include "CCommonCfg.h"
#include "Logger/CLogger.h"
#include "CFilePath.h"
#include "commondefin.h"
#include <QApplication>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>

//路径
const QString arcSoftConfigPath = "/cfg/commonCfg.txt";

//当前存在的项名称
const QString FaceConfidence = "FaceConfidence";  //人脸识别置信度
const QString SoftwareVersion = "SoftwareVersion";//软件版本
const QString MultiCameraFlag = "MultiCameraFlag";//一对多摄像头标志

const QString GroupName = "commonCfg";

QMutex CCommonCfg::s_csManager_InstanceSafe;

CCommonCfg *CCommonCfg::s_pManagerInstance = nullptr;

CCommonCfg::CCommonCfg()
    : m_strFilePath(QApplication::applicationDirPath() + arcSoftConfigPath)
    , m_settings(m_strFilePath, QSettings::IniFormat)
{
    init();
}

CCommonCfg &CCommonCfg::GetInstance()
{
    /// 1.--------------获取单实例唯一指针---------------
    if( s_pManagerInstance == nullptr)
    {
        /// 1.1--------------多线程同时调用时，线程安全锁---------------
        s_csManager_InstanceSafe.lock();

        /// 1.2--------------新建CConfig_Manager对象--------------------------
        if(s_pManagerInstance == nullptr)
        {
            s_pManagerInstance = new CCommonCfg();
        }

        /// 1.3--------------多程程同步锁，解锁-------------------------
        s_csManager_InstanceSafe.unlock();
    }

    /// 2.--------------返回对象指针---------------------
    return *s_pManagerInstance;
}

void CCommonCfg::init()
{
    QString strFilePath = QApplication::applicationDirPath() + arcSoftConfigPath;
    QFileInfo fileInfo(strFilePath);

    //文件不存在设置默认值
    if (!fileInfo.exists())
    {
        Default();
    }
    //文件存在读取内部默认值，若单项不存在那么写入
    else
    {
        m_settings.beginReadArray(GroupName);

        if(Check_Field(m_settings, FaceConfidence))
        {
            m_fFaceConfidence = m_settings.value(FaceConfidence).toFloat();
        }
        else
        {
            m_settings.setValue(FaceConfidence, QString::number(static_cast<double>(m_fFaceConfidence)));
        }


        if(Check_Field(m_settings, SoftwareVersion))
        {
            m_nSoftwareVersion = m_settings.value(SoftwareVersion).toInt();
        }
        else
        {
            m_settings.setValue(SoftwareVersion, m_nSoftwareVersion);
        }


        if(Check_Field(m_settings, MultiCameraFlag))
        {
            m_bMultiCameraFlag = m_settings.value(MultiCameraFlag).toBool();
        }
        else
        {
            m_settings.setValue(MultiCameraFlag, m_bMultiCameraFlag);
        }

        m_settings.sync();
    }
}

void CCommonCfg::Default()
{
    m_settings.beginGroup(GroupName);

    m_settings.setValue(FaceConfidence, QString::number(static_cast<double>(m_fFaceConfidence)));
    m_settings.setValue(SoftwareVersion, m_nSoftwareVersion);
    m_settings.setValue(MultiCameraFlag, m_bMultiCameraFlag);

    m_settings.sync();
}

bool CCommonCfg::Check_Field(const QSettings& settings, const QString &strField)
{
    return  settings.contains(strField);
}
