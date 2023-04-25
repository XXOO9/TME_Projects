#ifndef CHTTPSYNCLOCALSCORES_H
#define CHTTPSYNCLOCALSCORES_H

#include "CHttpAbstractNetWork.h"
#include "../Logger/CLogger.h"
#include <QObject>
#include "../common_global.h"
#include "CHttpUpLoadCommonScore.h"
#include "CHttpUpLoadBodyScore.h"
#include "CHttpUpLoadEyeSightScore.h"
#include "CHttpUploadInfos.h"
#include "CHttpRequestMgr.h"
#include <QThread>

class CHttpRequestMgr;

class COMMONSHARED_EXPORT CHttpSyncLocalScores : public QObject
{
    Q_OBJECT
public:
    CHttpSyncLocalScores();
    virtual ~CHttpSyncLocalScores() override;

    void setVecCommonScore(const QVector<QMap<QString, QVariant> > &vecCommonScore);

    void setVecBodyScore(const QVector<QMap<QString, QVariant> > &vecBodyScore);

    void setVecEyeSightScore(const QVector<QMap<QString, QVariant> > &vecEyeSightScore);

    void startSync();

signals:
    void sigSingleSyncResultAvailable( QVariant data, int UploadType );

public slots:
    void onSyncCommonFinished( bool ok );
    void onSyncBodyFinished( bool ok );
    void onSyncEyeSightFinished( bool ok );

private:
    void init();
    void initConnection();

    void startSyncCommonScore();

    void startSyncBodyScore();

    void startSyncEyeSightScore();

    CHttpUploadInfos getUploadInfos(DATA_SRC dataSrc, UPLOAD_TYPE type, bool ok, int requestRetCode);


private:
    QVector<QMap<QString, QVariant>>    m_vecCommonScore;
    QVector<QMap<QString, QVariant>>    m_vecBodyScore;
    QVector<QMap<QString, QVariant>>    m_vecEyeSightScore;

    QMap<QString, QVariant>             m_mapCurCommonScore;
    QMap<QString, QVariant>             m_mapCurBodyScore;
    QMap<QString, QVariant>             m_mapCurEyeSightScore;

    CHttpUpLoadCommonScore             *m_pUploadCommonScore{ nullptr };
    CHttpUpLoadBodyScore               *m_pUploadBodyScore{ nullptr };
    CHttpUpLoadEyeSightScore           *m_pUploadEyeSightScore{ nullptr };

    int                                 m_uploadCommonTotalCount = 0;
    int                                 m_uploadBodyTotalCount = 0;
    int                                 m_uploadEyeSightTotalCount = 0;
};

#endif // CHTTPSYNCLOCALSCORES_H
