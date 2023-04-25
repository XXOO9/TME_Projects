#ifndef CABSTRACTSETTINGELEMENT_H
#define CABSTRACTSETTINGELEMENT_H

#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <QObject>
#include "../Common/commondefin.h"
//这里表示变更项名称,这里变更项可能会超出unsigned long，添加变更项之前需要认为去人
namespace changeItem {
    enum common{
        CHANGE_NO = 0,
    };

    //
    namespace host {
        enum changeItem{
            CHANGE_DEV_Channel = 1 << 0,                //设备类型变更
            CHANGE_DEV_CONNECTED_MODE = 1 << 1,         //设备连接模式
            CHANGE_DEV_AUTO_COMMIT = 1 << 2,            //设备提交模式
            CHANGE_DEV_SYNC_MODE = 1 << 3,              //设备测试模式
            CHANGE_DEV_USERINFOPAGE_DISPLAY = 1 << 4    //用户信息是否显示
        };
    }

    namespace dev {
        enum changeItem{
            CHANGE_DEV_EXTENSION_NO = 1 << 0,  //分机编号变更
            CHANGE_PERIPHERAL = 1 << 1,        //外设变更包含 发令枪 等
        };
    }

    namespace user {
        enum changeItem{
            CHANGE_TEST_ITEM = 1 << 0,
            CHANGE_START_POS = 1 << 1,
            CHANGE_CIRCLE_LENGTH = 1 << 2,
            CHANGE_ROPE_DELAY = 1 << 3,
            CHANGE_START_FLAG = 1 << 4,
            CHANGE_SCORE_DISPLAY = 1 << 5,
            CHANGE_PAGE_COUNTTIME = 1 << 6
        };
    }
}

class CAbstractSettingElement  : public QObject
{
    Q_OBJECT
public:
    CAbstractSettingElement();
    virtual ~CAbstractSettingElement(){}

    virtual QVariantMap getSpecificDataElement( const int element ) = 0;
    virtual void setSpecificElementData( const int &elementIndex, const QVariantMap &newConfigInfo ) = 0;


    static QJsonArray translateVariantMap2JsonArray( const QVariantMap &srcMap, const QString &preKey, const QString &suffixKey );

    inline unsigned long getChangeItem(){return m_lChangeItem;}
    void resetChangeItem(){m_lChangeItem = changeItem::CHANGE_NO;}

protected:
    virtual void saveIntoDatabase( const QVariantMap &srcMap );

public:
    //每次获取之后再重置
    unsigned long m_lChangeItem{changeItem::CHANGE_NO};
};

#endif // CABSTRACTSETTINGELEMENT_H
