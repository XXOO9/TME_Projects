#include "CMultiUploadRetInfo.h"

CMultiUploadRetInfo::CMultiUploadRetInfo()
{

}

QString CMultiUploadRetInfo::userId() const
{
    return m_userId;
}

short CMultiUploadRetInfo::testItem() const
{
    return m_testItem;
}

QString CMultiUploadRetInfo::timeStamp() const
{
    return m_timeStamp;
}

bool CMultiUploadRetInfo::upLoadSuccess() const
{
    return m_upLoadSuccess;
}

void CMultiUploadRetInfo::setUpLoadSuccess(bool upLoadSuccess)
{
    m_upLoadSuccess = upLoadSuccess;
}

void CMultiUploadRetInfo::setUserId(const QString &userId)
{
    m_userId = userId;
}

void CMultiUploadRetInfo::setTestItem(short testItem)
{
    m_testItem = testItem;
}

void CMultiUploadRetInfo::setTimeStamp(const QString &timeStamp)
{
    m_timeStamp = timeStamp;
}

QString CMultiUploadRetInfo::errorCode() const
{
    return m_errorCode;
}

void CMultiUploadRetInfo::setErrorCode(const QString &errorCode)
{
    m_errorCode = errorCode;
}

QString CMultiUploadRetInfo::errorMsg() const
{
    return m_errorMsg;
}

void CMultiUploadRetInfo::setErrorMsg(const QString &errorMsg)
{
    m_errorMsg = errorMsg;
}

UPLOAD_TYPE CMultiUploadRetInfo::UploadType() const
{
    return m_UploadType;
}

void CMultiUploadRetInfo::setUploadType(const UPLOAD_TYPE &UploadType)
{
    m_UploadType = UploadType;
}

DATA_SRC CMultiUploadRetInfo::scoreDataSrc() const
{
    return m_scoreDataSrc;
}

void CMultiUploadRetInfo::setScoreDataSrc(const DATA_SRC &scoreDataSrc)
{
    m_scoreDataSrc = scoreDataSrc;
}
