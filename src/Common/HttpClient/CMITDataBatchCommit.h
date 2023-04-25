#ifndef CMITDATABATCHCOMMIT_H
#define CMITDATABATCHCOMMIT_H

#include "CHttpAbstractNetWork.h"
#include <QObject>

namespace MITKeys {
    const QString recordList = "recordList";
    const QString userId = "userId";
    const QString recordTime = "recordTime";
    const QString trainTimeCost = "trainTimeCost";
    const QString trainPart = "trainPart";
    const QString trainRecordCollection = "trainRecordCollection";
    const QString hlddsList = "hlddsList";
    const QString playIndex = "playIndex";
    const QString trainStartTime = "trainStartTime";
    const QString hitRate1 = "hitRate1";
    const QString hitRate2 = "hitRate2";
    const QString hitRate3 = "hitRate3";
    const QString hitRateTotal = "hitRateTotal";
    const QString hxyxList = "hxyxList";
    const QString trainDifficulty = "trainDifficulty";
    const QString directTimeCost = "directTimeCost";
    const QString divertTimeCost = "divertTimeCost";
    const QString selectTimeCost = "selectTimeCost";
    const QString computeTimeCost = "computeTimeCost";
    const QString fysxlList = "fysxlList";
    const QString mistakeTimes = "mistakeTimes";
    const QString AvgReactionTime = "AvgReactionTime";
    const QString totalReactionTime = "totalReactionTime";
    const QString setfkList = "setfkList";
    const QString trainLevel = "trainLevel";
    const QString result = "result";
    const QString sxjyxlList = "sxjyxlList";
    const QString memoryNum = "memoryNum";
    const QString score = "score";
    const QString gzjyxlList = "gzjyxlList";
    const QString txjyxlList = "txjyxlList";
    const QString mode = "mode";
    const QString vrmgList = "vrmgList";
    const QString ssxtxlList = "ssxtxlList";
    const QString avgCompleteTime = "avgCompleteTime";
    const QString avgMistakeTimes = "avgMistakeTimes";
    const QString lxhhList = "lxhhList";
    const QString tnfbList = "tnfbList";
    const QString vrslList = "vrslList";
    const QString vrxdList = "vrxdList";

}

class COMMONSHARED_EXPORT CMITDataBatchCommit : public CHttpAbstractNetWork
{
public:

    CMITDataBatchCommit( CHttpAbstractNetWork *parent = nullptr );
    virtual ~CMITDataBatchCommit() override;

public:
    virtual void httpRequest() override;
    void setCommitParameters(const QVariantList &paraMapList );
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

    void handleRequestError(QNetworkReply *pReply );

private:
    QList< QVariantMap >    m_errorElementList;  //请求失败的错误单元
    QVariantList            m_commitParaMapList;
    QByteArray              m_sendJson;
};

#endif // CMITDATABATCHCOMMIT_H
