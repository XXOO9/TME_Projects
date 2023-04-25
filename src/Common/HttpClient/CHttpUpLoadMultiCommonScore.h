#ifndef CHTTPUPLOADMULTICOMMONSCORE_H
#define CHTTPUPLOADMULTICOMMONSCORE_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "CHttpUpLoadCommonScore.h"
#include "CMultiUploadRetInfo.h"
#include "../Logger/CLogger.h"
#include "../common_global.h"

namespace uploadMultiScoreKeys {
    const QString userId     = QString( "recordList[%1].userId" );
    const QString scoreType  = QString( "recordList[%1].type" );
    const QString result     = QString( "recordList[%1].result" );
    const QString recordTime = QString( "recordList[%1].recordTime" );
    const QString files      = QString( "recordList[%1].files[%2]" );
    const QString signPic    = QString( "recordList[%1].signPic" );
}

class COMMONSHARED_EXPORT CHttpUpLoadMultiCommonScore : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpUpLoadMultiCommonScore( QObject *parent = nullptr );
    virtual ~CHttpUpLoadMultiCommonScore() override;

    virtual void httpRequest() override;
    void setUpLoadMultiScores( QVariantList &scoresList );
    void appendUpLoadScore( QVariantMap &score );
    QString lastErrorString() const;

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
    virtual void parseParam( DATA_SRC dataSrc = UserExam ) override;

    void setSpecificScoreVecVal( QMap<CHANGE_SCORE_TYPE, QVariant> &valMap, QList<int> tarIndexs = QList<int>() );

private:
    QVector<CMultiUploadRetInfo>    m_vecUploadMulRetInfos;
    QVariantList                    m_listMapUpLoadScores;
    QVariantList                    m_listMapFailedScore;
    QString                         m_lastErrorString = "";
    DATA_SRC                        m_dataSrc = UserExam;
};

#endif // CHTTPUPLOADMULTICOMMONSCORE_H
