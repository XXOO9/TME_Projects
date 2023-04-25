#ifndef CINTERACTONSTUDENT_H
#define CINTERACTONSTUDENT_H

#include <QObject>
#include "CAbstractInteraction.h"
#include "ProjectMetaEnums.h"
#include "CStudentInfoMgr.h"
#include "CAbstractTableModel.h"
#include "CIdentificDevNFC.h"

class CInteractionStudent : public CAbstractInteraction
{
    Q_OBJECT
    Q_PROPERTY(QString stuId READ getStuId WRITE setStuId NOTIFY sigDisplayIdChanged)
    Q_PROPERTY(QString m_stuCardNum READ getStuCardNum WRITE setStuCardNum NOTIFY sigCurStuCardNumChanged)
public:
    CInteractionStudent( QObject *parent = nullptr );


    QString getStuId() const;
    Q_INVOKABLE void setStuId(QString value);

    QString getStuCardNum() const;
    Q_INVOKABLE void setStuCardNum(const QString &stuCardNum);

    Q_INVOKABLE void detectedCard( ProjectMetaEnums::cardDetectState cardState );

    //表格操作
public:
    Q_INVOKABLE void appendNewRow( QString val );


public:
    //测试   卡绑定状态
    Q_INVOKABLE void testStuView_cardBind();

    //测试   卡未绑定状态
    Q_INVOKABLE void testStuView_cardNotBind();
public:
    // 切换界面
    Q_INVOKABLE virtual void setCurrentPage();

public slots:
    void onNFCDecetedStateChanged(bool ok);

private:
    void initConnection();

signals:
    void sigDisplayIdChanged( QString id );
    void sigCurStuCardNumChanged( QString cardNum );

    void sigDecetedCardStatedChanged( int cardState );

    //
    void sigNewRowInfoAvailable( QString valList );



private:
    QString  stuId;
    QString  m_stuCardNum;
    CIdentificDevNFC        m_NFC;
};

#endif // CINTERACTONSTUDENT_H
