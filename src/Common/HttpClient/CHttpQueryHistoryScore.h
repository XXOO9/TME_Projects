#ifndef CHTTPQUERYHISTORYSCORE_H
#define CHTTPQUERYHISTORYSCORE_H

#include <QObject>
#include <QObject>
#include "CHttpAbstractNetWork.h"
#include "../common_global.h"
#include "CQueryCommonHistoryRet.h"

namespace retKeys {
    const QString testItem         = "testItem";
    const QString score            = "score";
    const QString record           = "record";
    const QString recordTime       = "recordTime";
    const QString heightRecordKey  = "height";
    const QString weightRecordKey  = "weight";
    const QString inVailedFlag     = INVALID_RESULT_SERVER;
    const int     retStringMinSize = 3;
    const int     index_testRet    = 1;
    const int     index_testScore  = 2;
}

class COMMONSHARED_EXPORT CHttpQueryHistoryScore : public CHttpAbstractNetWork
{
public:
    CHttpQueryHistoryScore( QObject *parent = nullptr );
    virtual ~CHttpQueryHistoryScore() override;

    virtual void httpRequest() override;

    void cleanHumanScoreList();

    void setUserId( QString userId );
    void setTestItem( TEST_ITEM testItem );
    void setTimeInterval( queryTimeInterval  timeInterval );
    void setTestScoreType( testType type );

    int resultCount();
    QMap<QString,QString> getGroupInfo( int index );
    QString getSingleRet( int index, QString key );


    QVariantMap getMapLocalHistoryScores() const;
    void setMapLocalHistoryScores(const QVariantMap &mapLocalHistoryScores);

    static long long getQueryStartDate( queryTimeInterval timeInterval );

    CQueryCommonHistoryRet getRet() const;

    QVector<QStringList> getVecDisplayRets() const;

    //目前仅对Common类型的项目使用
    void setVecDisplayRets( QVector< QStringList> vecRets );
    void clearDisplayRets();

    //对Common类型的成绩进行去重
    void removeCommonDatas();

    //对身高体重类型的成绩进行组装并去重
    void mergeAndRemoveCommonBodyTestResults(QVector<QStringList> &localBodyResults );

    TEST_ITEM getCurTestItem() const;

    QVariantList getHuamenCompositionList() const;

private:
    virtual void initConfig() override;
    virtual bool parseJson() override;
    virtual bool isParameterEmpty() override;

    //组装身高体重成绩数据， 根据时间戳确定同一组数据
    void configHeightAndWeightData(const QJsonArray &srcArr );

    //判断从服务器查下来的成绩中是否包含 recordTime 下的成绩条目
    bool isServerRecordsContainsTargetRecordTime( const QString &recordTime );

    //根据时间对身高体重成绩进行排序
    void sortHumanScoreLists( QVariantList &humanScoreList );

public slots:
    virtual void onNewReplyAvailable( QNetworkReply *pReply ) override;
    virtual void onTimeoutHandler() override;

private:
    CQueryCommonHistoryRet   m_ret;
    QVector<QStringList>     m_vecDisplayRets;
    QVariantMap              m_mapLocalHistoryScores;
    QString                  m_baseInterfaceUrl;
    TEST_ITEM                m_curTestItem = HTTP_TEST_ITEM_UNKNOW;

    QVariantList             m_huamenCompositionList;
};

#endif // CHTTPQUERYHISTORYSCORE_H
