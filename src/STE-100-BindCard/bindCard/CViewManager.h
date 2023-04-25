#ifndef CVIEWMANAGER_H
#define CVIEWMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QQmlContext>
#include <QCoreApplication>
#include <QQmlApplicationEngine>

#include "ProjectMetaEnums.h"
#include "CDataModel.h"
#include "CAbstractTableModel.h"
#include "CInteractionStudent.h"
#include "CInteractionClass.h"
#include "CClassListModel.h"
#include "CStuListModel.h"
#include "CIdentificDevNFC.h"


class CViewManager : public QObject
{
    Q_OBJECT
public:
    explicit CViewManager(QUrl viewFileUrl, QObject *parent = nullptr);

    void loadView( const QUrl url  );
    CInteractionStudent *uiStudent() const;
    CClassListModel *classDataModel() const;
    CStuListModel *pStuDataModel() const;
    CInteractionClass *uiClass() const;

    ProjectMetaEnums::PageIndex curPage() const;
    void setCurPage(const ProjectMetaEnums::PageIndex &curPage);

private:

    void initConnection();

    void initEngine();

    void initDataModel();

    void registInteraction();
signals:
    void sigLoadFinished( bool ok );

    void sigStuViewDisplayIdChaged( QString id );
    void sigClassViewDisplayIdChaged( QString id );
    void sigUpdateLocalStuCardNum( QString stuId, QString cardNum );

    /*************************NFC**************************************/
    void sigStuDecetedCardStateChanged( bool exist );
    void sigClassDecetedCardStateChanged( bool exist );
    void sigStuPageCardNumAvailable( QString cardNum );
    void sigClassPageCardNumAvailable( QString cardNum );

public slots:
    void onViewObjectCreated( QObject *object, const QUrl url );
    void onStuViewDisplayId( QString id );
    void onClassViewDisplayId(QString id);

    //添加数据到表格
    void onInserStuRow( QString val );

    //添加数据到表格
    void onInsertClassRow( QString val );

    //更新本地临时用户卡号
    void onUpdateLocalTemporaryCardNum( QString stuId, QString cardNum = "" );
    void onDisBindLocalTemporaryCardNum( QString stuId );

    //检测到卡状态改变
    void onDecetedCardExisteChanged(bool exist);

    //获取卡号
    void onNewNfcCardNumAvailable( QString cardNum );

    //响应页面切换
    void onCurrentPageChanged( ProjectMetaEnums::PageIndex pageIndex );
private:
    QQmlApplicationEngine       m_viewEngine;
    CAbstractTableModel        *m_pUserBindDataModel;
    CAbstractTableModel        *m_pClassBindDataModel;
    CClassListModel            *m_pClassDataModel;
    CStuListModel              *m_pStuDataModel;
    bool                        m_isLoadSuccess{ false };
    QUrl                        m_viewFile;
    CIdentificDevNFC            m_NFC;
    ProjectMetaEnums::PageIndex m_curPage;


private:

    const qint8            m_columnCount{ 4 };
    CInteractionStudent   *m_pUiStudent;
    CInteractionClass     *m_pUiClass;
};

#endif // CVIEWMANAGER_H
