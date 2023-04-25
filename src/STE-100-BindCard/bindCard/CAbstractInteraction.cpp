#include "CAbstractInteraction.h"

CAbstractInteraction::CAbstractInteraction(QObject *parent) : QObject(parent)
  ,m_NfcCardNum( "" )
{
    init();
}

void CAbstractInteraction::setCurrentStuInfo(CStudentInfo stuInfo)
{
    m_stuInfoJsonString.clear();
    QJsonObject jsObj;
    jsObj.insert( SyncUserInfoKeys::keyName, stuInfo.name() );
    jsObj.insert( SyncUserInfoKeys::keyClassName, stuInfo.name() );
    jsObj.insert( SyncUserInfoKeys::keyName, stuInfo.name() );
}

QString CAbstractInteraction::cardNum() const
{
    return m_NfcCardNum;
}

void CAbstractInteraction::updateUserCardNumber(QString userId, QString cardId)
{
    m_updateUserCardNum.setCardNo( cardId );
    m_updateUserCardNum.setUserId( userId );
    m_updateUserCardNum.setCardType( ProjectMetaEnums::School_Card );

    //通知服务器更新该学生的卡号, 等待服务器更新成功后，在更新本地临时存储的卡号
    m_updateUserCardNum.httpRequest();
}

void CAbstractInteraction::disBindUserCard(QString userId)
{
    m_disBindUserCard.setUserId( userId );
    m_disBindUserCard.httpRequest();
}

void CAbstractInteraction::onDecetedCardExisteChanged(bool exist)
{
    qDebug() << "is card existed " << exist;
    if( !exist ){
        m_NfcCardNum.clear();
    }
    m_cardExist = exist;
    emit sigDecetedCardExist( exist );

}

void CAbstractInteraction::onReadCardNumAvailable(QString cardId)
{
    cardId = cardId.left( 10 );
    if( m_NfcCardNum != cardId ){
        qDebug() << "new card number = " << cardId;
        m_NfcCardNum = cardId;
    }
}

void CAbstractInteraction::onUpdateCardRetAvailable(bool ok)
{
    if( ok ){
        qDebug() << "update card number success";
        emit sigUpdateLocalTemporaryStuCardNum( m_baseStuId, m_NfcCardNum );
    }else{
        qDebug() << "update card number failed";
    }

    m_updateUserCardNum.cleanMultiParaMap();
}

void CAbstractInteraction::onDisBindUserCard(bool ok)
{
    if( ok ){
        qDebug() << "disBind card number success";
        emit sigDisBindLocalTemporaryStuCardNum( m_baseStuId );
    }else{
        qDebug() << "disBind card number failed";
    }

    m_disBindUserCard.cleanMultiParaMap();
}

void CAbstractInteraction::openNFC()
{
    m_NFC.open();
}

void CAbstractInteraction::closeNFC()
{
    m_NFC.close();
}

void CAbstractInteraction::init()
{
    connect( &m_NFC, &CIdentificDevNFC::sigDecetedCard, this, &CAbstractInteraction::onDecetedCardExisteChanged );
    connect( &m_NFC, &CIdentificDevNFC::signalFindCard, this, &CAbstractInteraction::onReadCardNumAvailable );
    connect( &m_updateUserCardNum, &CHttpAbstractNetWork::sigRequestRetAvailable, this, &CAbstractInteraction::onUpdateCardRetAvailable );
    connect( &m_disBindUserCard, &CHttpAbstractNetWork::sigRequestRetAvailable, this, &CAbstractInteraction::onDisBindUserCard );
}

void CAbstractInteraction::setNfcCardNum(const QString &NfcCardNum)
{
    m_NfcCardNum = NfcCardNum;
}

QString CAbstractInteraction::baseStuId() const
{
    return m_baseStuId;
}

void CAbstractInteraction::setBaseStuId(const QString &baseStuId)
{
    m_baseStuId = baseStuId;
}

bool CAbstractInteraction::cardExist() const
{
    return m_cardExist;
}
