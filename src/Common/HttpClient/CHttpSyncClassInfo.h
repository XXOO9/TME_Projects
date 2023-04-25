#ifndef CHTTPSYNCCLASSINFO_H
#define CHTTPSYNCCLASSINFO_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"
#include "CSyncClassInfoRet.h"

class COMMONSHARED_EXPORT CHttpSyncClassInfo : public CHttpAbstractNetWork
{
public:
    CHttpSyncClassInfo( QString timeSatmp = "", QObject *parent = nullptr );
    virtual ~CHttpSyncClassInfo() override;

    virtual void httpRequest() override;

    int resultCount();
    QMap<QString, QString> getGroupInfo( int index );
    QString getSingleRet( int index, QString key );


    QString getDelSpliTime();

    QVector<QString> getVecDelClass();

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    CSyncClassInfoRet m_ret;
    QString           m_delSpliTime;
    QVector<QString>  m_vecDelClass;
};

#endif // CHTTPSYNCCLASSINFO_H
