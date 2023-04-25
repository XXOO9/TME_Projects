#ifndef CABSTRACTINTERACTION_H
#define CABSTRACTINTERACTION_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include "CHttpUpDateUserCardNum.h"

#include "CStudentInfoDataBase.h"
#include "ProjectMetaEnums.h"
#include "CIdentificDevNFC.h"
#include "CHttpDisBindUserCardNumber.h"

class CAbstractInteraction : public QObject
{
    Q_OBJECT
public:
    CAbstractInteraction( QObject *parent = nullptr );
    void setCurrentStuInfo( CStudentInfo stuInfo );

    //获取最新卡号
    Q_INVOKABLE QString cardNum() const;

    Q_INVOKABLE void setNfcCardNum(const QString &NfcCardNum);

    //绑定卡号
    Q_INVOKABLE void updateUserCardNumber( QString userId, QString cardId );

    //解除绑定卡号
    Q_INVOKABLE void disBindUserCard( QString userId );

    Q_INVOKABLE QString baseStuId() const;
    Q_INVOKABLE void setBaseStuId(const QString &baseStuId);

    //发射信号设置告诉ViewManager当前所在的页面
    Q_INVOKABLE virtual void setCurrentPage() = 0;


public slots:
    void onDecetedCardExisteChanged(bool exist);

    void onReadCardNumAvailable( QString cardId );
    void onUpdateCardRetAvailable(bool ok);
    void onDisBindUserCard(bool ok);
public:
    Q_INVOKABLE void openNFC();

    Q_INVOKABLE void closeNFC();

    bool cardExist() const;



private:
    void init();


signals:
    void sigNewDisplayStuInfoAvailable( QString jsonString );

    //是否存在卡
    void sigDecetedCardExist(bool exist);

    //更新临时存储的卡号
    void sigUpdateLocalTemporaryStuCardNum(QString stuId, QString cardNum);

    //清除临时存储的卡号
    void sigDisBindLocalTemporaryStuCardNum( QString stuId );

    //向ViewManager设置当前页面
    void sigCurrentPageChanged( ProjectMetaEnums::PageIndex pageIndex );



private:
    QString                     m_stuInfoJsonString;
    QString                     m_baseStuId;
    CIdentificDevNFC            m_NFC;
    QString                     m_NfcCardNum;
    bool                        m_cardExist;
    CHttpUpDateUserCardNum      m_updateUserCardNum;
    CHttpDisBindUserCardNumber  m_disBindUserCard;
};

#endif // CABSTRACTINTERACTION_H
