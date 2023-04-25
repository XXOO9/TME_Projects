#ifndef CHTTPUPLOADMULTIBODYSCORES_H
#define CHTTPUPLOADMULTIBODYSCORES_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "CHttpUpLoadCommonScore.h"
#include "CMultiUploadRetInfo.h"
#include "../Logger/CLogger.h"
#include "../common_global.h"

namespace uploadMultiBodyScores {
    const QString devCode       =    "deviceCode";
    const QString devType       =    "deviceType";
    const QString userId        =    QString( "recordList[%1].userId" );
    const QString recordListStr =    QString( "recordList[%1].recordListStr" );
    const QString recordTime    =    QString( "recordList[%1].recordTime" );
    const QString files         =    QString( "recordList[%1].files[%2]" );
    const QString scoreType     =    QString( "recordList[%1].type" );
    const QString signPic       =    QString( "recordList[%1].signPic" );
}

class COMMONSHARED_EXPORT CHttpUpLoadMultiBodyScores : public CHttpAbstractNetWork
{
    Q_OBJECT
public:
    CHttpUpLoadMultiBodyScores( QObject *parent = nullptr );
    virtual ~CHttpUpLoadMultiBodyScores() override;

    virtual void httpRequest() override;

    void setUpLoadMultiScores( QVariantList &scoresList );
    QString lastErrorString() const;
    QVariantList getFailedScoreInfoList() const;

    static QString translateBodyComposition( const QVariantMap &bodyCompositionMap );

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
    QVector<CMultiUploadRetInfo>  m_vecUploadMulRetInfos;
    QVariantList                  m_listMapUpLoadScores;
    QVariantList                  m_listMapRetInfos;
    QString                       m_lastErrorString = "";
    DATA_SRC                      m_dataSrc = UserExam;
};
#endif // CHTTPUPLOADMULTIBODYSCORES_H
