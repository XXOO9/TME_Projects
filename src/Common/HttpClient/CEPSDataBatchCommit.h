#ifndef CEPSDATABATCHCOMMIT_H
#define CEPSDATABATCHCOMMIT_H

#include "CHttpAbstractNetWork.h"
#include "../common_global.h"
#include <QObject>

const int MaxCommitCount = 10;  //单次请求最大提交量
class CEPSElement;
class CEPSDataBatchCommit;

namespace parameterTemplateKeys {

    const QString userId                    = QString( "recordList[%1].userId" );
    const QString recordTime                = QString( "recordList[%1].recordTime" );
    const QString sandboxName               = QString( "recordList[%1].sandboxName" );
    const QString sandboxScenes             = QString( "recordList[%1].sandboxScenes" );
    const QString sandboxTimeCost           = QString( "recordList[%1].sandboxTimeCost" );
    const QString picFile                   = QString( "recordList[%1].picFile" );

    const QString itemUseTrackListUseOrder  = QString( "recordList[%1].itemUseTrackList[%2].useOrder" );
    const QString itemUseTrackListItemName  = QString( "recordList[%1].itemUseTrackList[%2].itemName" );
    const QString itemUseTrackListCategory  = QString( "recordList[%1].itemUseTrackList[%2].category" );
    const QString itemUseTrackListIsDeleted = QString( "recordList[%1].itemUseTrackList[%2].isDeleted" );

    const QString itemUseStaticListCategory = QString( "recordList[%1].itemUseStatisList[%2].category" );
    const QString itemUseStaticListColdNum  = QString( "recordList[%1].itemUseStatisList[%2].coldNum" );
    const QString itemUseStaticListWarmNum  = QString( "recordList[%1].itemUseStatisList[%2].warmNum" );

    const QString itemMeaningListIndex      = QString( "recordList[%1].itemMeaningList[%2].index" );
    const QString itemMeaningListItemName   = QString( "recordList[%1].itemMeaningList[%2].itemName" );
    const QString itemMeaningListMeaning    = QString( "recordList[%1].itemMeaningList[%2].meaning" );
}

namespace EPSKeys {
    const QString userId = "userId";
    const QString recordTime = "recordTime";
    const QString sandboxName = "sandboxName";
    const QString sandboxScenes = "sandboxScenes";
    const QString sandboxTimeCost = "sandboxTimeCost";
    const QString useOrder = "useOrder";
    const QString itemName = "itemName";
    const QString category = "category";
    const QString isDeleted = "isDeleted";
    const QString coldNum = "coldNum";
    const QString warmNum = "warmNum";
    const QString index = "index";
    const QString meaning = "meaning";
    const QString picFile = "picFile";
}



class CEPSElement{
public:
    QString m_userId;
    QString m_recordTime;
    QString m_sandboxName;
    QString m_sandboxScenes;
    QString m_sandboxTimeCost;

    QList< QVariantMap > m_itemUseTrackList;
    QList< QVariantMap > m_itemUseStatisList;
    QList< QVariantMap > m_itemMeaningList;

    QString m_filePath = "";
};



class COMMONSHARED_EXPORT CEPSDataBatchCommit : public CHttpAbstractNetWork
{
public:
    CEPSDataBatchCommit( CHttpAbstractNetWork *parent = nullptr );
    virtual ~CEPSDataBatchCommit() override;

public:
    virtual void httpRequest() override;
    void setCommitParameters(const QList<CEPSElement> &paraMapList );
    QList<QVariantMap> errorElementList() const;

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
    QList< QVariantMap >    m_errorElementList;  //请求失败的错误单元
    QList< CEPSElement >    m_commitParaMapList;
    QByteArray              m_sendJson;
};

#endif // CEPSDATABATCHCOMMIT_H
