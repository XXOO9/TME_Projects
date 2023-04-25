#ifndef CHTTPQUERYBODYELEMENT_H
#define CHTTPQUERYBODYELEMENT_H

#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"

class COMMONSHARED_EXPORT CHttpQueryBodyElement : public CHttpAbstractNetWork
{
public:
    CHttpQueryBodyElement( QObject *parent = nullptr );
    virtual ~CHttpQueryBodyElement() override;

    virtual void httpRequest() override;

    void setUserId( QString userId );
    void setTimeInterval( queryTimeInterval timeinterval );
    void setQueryDevType( QString devType );

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;


public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;
};

#endif // CHTTPQUERYBODYELEMENT_H
