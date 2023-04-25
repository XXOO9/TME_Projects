#include "CInteractionStudent.h"

CInteractionStudent::CInteractionStudent(QObject *parent) : CAbstractInteraction ( parent )
{
    initConnection();
}

QString CInteractionStudent::getStuId() const
{
    return stuId;
}

void CInteractionStudent::setStuId(QString value)
{
    stuId = value;

    emit sigDisplayIdChanged( value );
}

void CInteractionStudent::detectedCard(ProjectMetaEnums::cardDetectState cardState)
{
    emit sigDecetedCardStatedChanged( (int)cardState );
}

void CInteractionStudent::testStuView_cardBind()
{
    detectedCard( ProjectMetaEnums::Exist_Card_Bind );
}

void CInteractionStudent::testStuView_cardNotBind()
{
    detectedCard( ProjectMetaEnums::Exist_Card_UnBind );
}

void CInteractionStudent::setCurrentPage()
{
    emit sigCurrentPageChanged( ProjectMetaEnums::Page_User_Bind );
}

void CInteractionStudent::onNFCDecetedStateChanged(bool ok)
{
//    QString cardNumberTmp = QString::number( cardNum().toInt() + 982 );
    QString cardNumberTmp = cardNum();
    QString stuCardnum    = m_stuCardNum;

    qDebug() << "tmp card no = " << cardNumberTmp;
    qDebug() << "cur stu card no = " << stuCardnum;

    //判断当前学生的卡号是否和检测到的卡号相等
    //相等则为已绑定状态r
    if( ok && ( m_stuCardNum == cardNumberTmp )){
        detectedCard( ProjectMetaEnums::Exist_Card_Bind );
    }
    //不相等则为未绑定状态
    else if(ok && ( m_stuCardNum != cardNumberTmp )){
        detectedCard( ProjectMetaEnums::Exist_Card_UnBind );
    }
    //其他状态统一未无卡状态
    else{
        detectedCard( ProjectMetaEnums::NO_Card );
    }
}

void CInteractionStudent::appendNewRow(QString val)
{
    emit sigNewRowInfoAvailable( val );
}

void CInteractionStudent::initConnection()
{
    connect( this, &CAbstractInteraction::sigDecetedCardExist, this, &CInteractionStudent::onNFCDecetedStateChanged );
}

QString CInteractionStudent::getStuCardNum() const
{
    return m_stuCardNum;
}

void CInteractionStudent::setStuCardNum(const QString &stuCardNum)
{
    m_stuCardNum = stuCardNum;
    emit sigCurStuCardNumChanged( stuCardNum );
}
