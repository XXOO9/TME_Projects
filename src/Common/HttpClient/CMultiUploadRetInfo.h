#ifndef CMULTIUPLOADRETINFOS_H
#define CMULTIUPLOADRETINFOS_H

#include <QString>
#include <QDebug>
#include "CHttpConfigCommonDefine.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CMultiUploadRetInfo
{
public:
    CMultiUploadRetInfo();
    ~CMultiUploadRetInfo(){}

    QString userId() const;
    short testItem() const;
    QString timeStamp() const;
    bool upLoadSuccess() const;

    void setUpLoadSuccess(bool upLoadSuccess);
    void setUserId(const QString &userId);
    void setTestItem(short testItem);
    void setTimeStamp(const QString &timeStamp);

    QString errorCode() const;
    void setErrorCode(const QString &errorCode);

    QString errorMsg() const;
    void setErrorMsg(const QString &errorMsg);

    UPLOAD_TYPE UploadType() const;
    void setUploadType(const UPLOAD_TYPE &UploadType);

    DATA_SRC scoreDataSrc() const;
    void setScoreDataSrc(const DATA_SRC &scoreDataSrc);

private:
    //是否上传成功标志, 初始化为 true
    bool            m_upLoadSuccess = true;

    QString         m_userId = "";
    short           m_testItem = -1;
    QString         m_timeStamp = "";
    QString         m_errorCode = QString::number( SUCCESS );
    QString         m_errorMsg = "";
    UPLOAD_TYPE     m_UploadType = UPLOAD_TYPE::Upload_Common;
    DATA_SRC        m_scoreDataSrc = DATA_SRC::DataBase;
};

#endif // CMULTIUPLOADRETINFOS_H
