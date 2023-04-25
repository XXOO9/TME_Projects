#ifndef CQUERYSANDREPORT_H
#define CQUERYSANDREPORT_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

namespace QueryReportKeys {
    const QString baseDoc = "baseDoc";
    const QString sandboxCount = "sandboxCount";
    const QString sandboxUseGap = "sandboxUseGap";
    const QString msgAfterTest = "msgAfterTest";
    const QString sandboxAnalysis = "sandboxAnalysis";
    const QString data = "data";
}

class COMMONSHARED_EXPORT CQuerySandReport : public CHttpAbstractNetWork
{
public:
    CQuerySandReport( QObject *parent = nullptr );
    virtual ~CQuerySandReport() override;

public:
    virtual void httpRequest() override;
    void setQueryCondition( const QString &userId, const QString &recordTime );
    QVariantMap reportInfoMap() const;

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;
    virtual bool isRequestSuccess() override;
    virtual void parseParam( DATA_SRC dataSrc = UserExam ) override;

    void handleRequestError( QNetworkReply *pReply );

private:
    QString m_userId = "";
    QString m_queryRecordTime = "";
    QVariantMap m_reportInfoMap;
};

#endif // CQUERYSANDREPORT_H
