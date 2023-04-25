#ifndef CUSINGSETTINGCONFIG_H
#define CUSINGSETTINGCONFIG_H

#include <QObject>
#include <QDebug>
#include "CAbstractSettingElement.h"
#include "../Common/Config/CDatabaseManage.h"

const QString scoreSelectConfig   = "scoreSelectConfig";
const QString timeoutSelectConfig = "timeoutSelectConfig";
const QString testCntSelectConfig = "testCntSelectConfig";
const QString startFlagSelectConfig = "startFlagSelectConfig";
const QString sbjStartPosSelectConfig = "sbjStartPosSelectConfig";

const QString scoreConfig         = "scoreConfig";
const QString timeoutConfig       = "timeoutConfig";
const QString testCntConfig       = "testCntConfig";
const QString devType             = "devCorrespondingTestItem";
const QString curTestItemName     = "curTestItemName";
const QString curTestItem         = "curTestItem";
const QString satrtFlagConfig     = "satrtFlagConfig";
const QString satrtPosConfig      = "satrtPosConfig";
const QString ropeDelayConfig     = "ropeDelayConfig";
const QString circleLengthConfig  = "circleLengthConfig";

const QString queryTestItemConfig = "queryTestItemConfig";

class CUsingSettingConfig : public CAbstractSettingElement
{
    Q_OBJECT
public:
    enum CUsingElementData{
        Timeout = 0,
        TestCount,
        DevTYPECorresponding,
        TestStartPos,
        TestCircleLen,
        QueryTestItem
    };

    CUsingSettingConfig();
    virtual ~CUsingSettingConfig() override {}

    virtual QVariantMap getSpecificDataElement( const int element = -1 ) override;
    virtual void setSpecificElementData( const int &elementIndex, const QVariantMap &newConfigInfo ) override;

    QStringList testCountList() const;
    void setTestCountList(const QStringList &testCountList);

    QStringList timeoutSelectList() const;
    void setTimeoutSelectList(const QStringList &timeoutSelectList);

private:
    virtual void saveIntoDatabase( const QVariantMap &srcMap ) override;
    void setCurDevCorrespondingTestItems( const int &devType );
    void initTestItemList();
    void initQueryTestItemList();

    void setAllConfig( const QVariantMap &srcMap );

private:
    //是否显示成绩配置
    QVariantMap m_scoreSelectMap;

    //界面无操作自动退出时间列表
    QStringList m_timeoutSelectList;

    //测试次数配置列表
    QStringList m_testCountList;

    //短跑起点配置
    QStringList m_startFlagList;

    //立定跳远起跳长度
    QStringList m_sbjStatrPosList;

    //所有设备类型支持的测试项( TEST_ITEM_XXX )
    QMap<int, QVariantMap> m_totalTestItemMap;

    //查询成绩的所有测试项
    QVariantMap m_queryItemMap;

    //当前设备测试类型
    QStringList m_testTypeList;

    //当前设置的设备类型支持的测试项
    QVariantMap m_curTestItemMap;

    //中长跑一圈长度
    int m_runMidLength{800};

    //跳绳等待时长 ms
    int m_ropeRecvDealyTime{2000};

    //配置短跑起点模式
    QString m_runShortStartFlag{QString::fromLocal8Bit("存在起点")};

    //配置立定跳远起跳点长度
    int m_standingJumpStartPos{0};

    QString     m_curTestItemValue = "1";

    int         m_waitTime = 0;
    int         m_timeout = 60;
    int         m_testCnt = 2;
    bool        m_scoreDisPlay{true};
};

#endif // CUSINGSETTINGCONFIG_H
