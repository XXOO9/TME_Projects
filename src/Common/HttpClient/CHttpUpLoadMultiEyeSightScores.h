#ifndef CHTTPUPLOADMULTIEYESIGHTSCORES_H
#define CHTTPUPLOADMULTIEYESIGHTSCORES_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "CMultiUploadRetInfo.h"
#include "../Logger/CLogger.h"
#include "../common_global.h"

namespace uploadMultiEyeSightScores {
    const QString userId = QString( "recordList[%1].userId" );
    const QString recordListStr = QString( "recordList[%1].recordListStr" );
    const QString recordTime = QString( "recordList[%1].recordTime" );
    const QString files = QString( "recordList[%1].files[%2]" );
    const QString signPic = QString( "recordList[%1].signPic" );
}
class COMMONSHARED_EXPORT CHttpUpLoadMultiEyeSightScores : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpUpLoadMultiEyeSightScores( QObject *parent = nullptr );
    virtual ~CHttpUpLoadMultiEyeSightScores() override;

    virtual void httpRequest() override;

    void setUpLoadMultiScores( QVariantList &scoresList );
    void appendUpLoadScore( QVariantMap &score );
    QString lastErrorString() const;

    QVariantList getFailedScoreInfoList() const;

    QVector<CMultiUploadRetInfo> getVecUploadMulRetInfos() const;

    void setDataSrc(const DATA_SRC &dataSrc);

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    virtual void initConfig() override;
    virtual bool isRequestSuccess() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

    virtual void parseParam();

private:
    QVector<CMultiUploadRetInfo>    m_vecUploadMulRetInfos;
    QVariantList                    m_listMapUpLoadScores;
    QVariantList                    m_listMapFailedScore;
    QString                         m_lastErrorString = "";
    DATA_SRC                        m_dataSrc = UserExam;


};

#endif // CHTTPUPLOADMULTIEYESIGHTSCORES_H
