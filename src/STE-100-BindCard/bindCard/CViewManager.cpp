#include "CViewManager.h"

CViewManager::CViewManager( QUrl viewFileUrl, QObject *parent) : QObject(parent), m_pUserBindDataModel( nullptr )
  ,m_viewFile( viewFileUrl )
  ,m_pClassDataModel( nullptr )
  ,m_pStuDataModel( nullptr )
  ,m_pClassBindDataModel( nullptr )
  ,m_pUiClass( nullptr )
{
    m_pUiStudent = new CInteractionStudent( this );
    m_pUiClass = new CInteractionClass( this );
    initEngine();
    m_NFC.open(true);
}

void CViewManager::loadView(const QUrl url)
{
    m_viewEngine.load( url );
}

CInteractionStudent *CViewManager::uiStudent() const
{
    return m_pUiStudent;
}


void CViewManager::initConnection()
{
    connect( &m_viewEngine, &QQmlApplicationEngine::objectCreated,
             this, &CViewManager::onViewObjectCreated, Qt::QueuedConnection );

    connect( m_pUiStudent, &CInteractionStudent::sigDisplayIdChanged,
             this, &CViewManager::onStuViewDisplayId );

    connect( m_pUiClass, &CInteractionClass::sigDisplayIdChanged,
             this, &CViewManager::onClassViewDisplayId );

    connect( m_pUiStudent, &CInteractionStudent::sigNewRowInfoAvailable,
             this, &CViewManager::onInserStuRow );

    connect( m_pUiClass, &CInteractionClass::sigNewRowInfoAvailable,
             this, &CViewManager::onInsertClassRow );

    connect( m_pUiStudent, &CAbstractInteraction::sigUpdateLocalTemporaryStuCardNum,
             this, &CViewManager::onUpdateLocalTemporaryCardNum );

    connect( m_pUiStudent, &CAbstractInteraction::sigDisBindLocalTemporaryStuCardNum,
             this, &CViewManager::onDisBindLocalTemporaryCardNum );

    connect( &m_NFC, &CIdentificDevNFC::sigDecetedCard,
             this, &CViewManager::onDecetedCardExisteChanged );

    connect( m_pUiStudent, &CAbstractInteraction::sigCurrentPageChanged,
             this, &CViewManager::onCurrentPageChanged );

    connect( m_pUiClass, &CAbstractInteraction::sigCurrentPageChanged,
             this, &CViewManager::onCurrentPageChanged );

    connect( m_pUiClass, &CAbstractInteraction::sigDisBindLocalTemporaryStuCardNum,
             this, &CViewManager::onDisBindLocalTemporaryCardNum );

    connect( m_pUiClass, &CAbstractInteraction::sigUpdateLocalTemporaryStuCardNum,
             this, &CViewManager::onUpdateLocalTemporaryCardNum );

    connect( this, &CViewManager::sigStuDecetedCardStateChanged,
             m_pUiStudent, &CAbstractInteraction::onDecetedCardExisteChanged );

    connect( this, &CViewManager::sigClassDecetedCardStateChanged,
             m_pUiClass, &CAbstractInteraction::onDecetedCardExisteChanged );

    connect( &m_NFC, &CIdentificDevNFC::signalFindCard,
             this, &CViewManager::onNewNfcCardNumAvailable );

    connect( this, &CViewManager::sigStuPageCardNumAvailable,
             m_pUiStudent, &CAbstractInteraction::onReadCardNumAvailable );

    connect( this, &CViewManager::sigClassPageCardNumAvailable,
             m_pUiClass, &CAbstractInteraction::onReadCardNumAvailable );
}

void CViewManager::initEngine()
{
    initConnection();
    initDataModel();
    registInteraction();
}

void CViewManager::initDataModel()
{
    m_pUserBindDataModel = new CAbstractTableModel( m_columnCount, this );
    m_pClassBindDataModel = new CAbstractTableModel( m_columnCount, this );
    m_pClassDataModel = new CClassListModel(this);
    m_pStuDataModel = new CStuListModel( this );
    m_viewEngine.rootContext()->setContextProperty( "UserDataModel", m_pUserBindDataModel );
    m_viewEngine.rootContext()->setContextProperty( "ClassBindDataModel", m_pClassBindDataModel );
    m_viewEngine.rootContext()->setContextProperty( "ClassDataModel", m_pClassDataModel );
    m_viewEngine.rootContext()->setContextProperty( "StuDataModel", m_pStuDataModel );
}

void CViewManager::registInteraction()
{
    qmlRegisterType<ProjectMetaEnums>( "com.Company.TME", 1, 0, "Enums" );
    qmlRegisterType<CDataModel>( "com.Company.TME", 1, 0, "TableModel" );
    m_viewEngine.rootContext()->setContextProperty( "StudentView", m_pUiStudent );
    m_viewEngine.rootContext()->setContextProperty( "ClassView", m_pUiClass );
}

void CViewManager::onViewObjectCreated(QObject *object, const QUrl url)
{
    if( !object && (url == m_viewFile )){
        emit sigLoadFinished( false );
        QCoreApplication::exit( -1 );
    }

    emit sigLoadFinished( true );
}

void CViewManager::onStuViewDisplayId(QString id)
{
    emit sigStuViewDisplayIdChaged( id );
    qDebug() << "id = " << id;
}

void CViewManager::onClassViewDisplayId(QString id)
{
    emit sigClassViewDisplayIdChaged(id);
}

void CViewManager::onInserStuRow(QString val)
{
    QStringList valList = val.split( "+" );
    m_pUserBindDataModel->appendRowDatas( valList, valList.first() );
}

void CViewManager::onInsertClassRow(QString val)
{
    QStringList valList = val.split( "+" );
    m_pClassBindDataModel->appendRowDatas( valList, valList.first() );
}

void CViewManager::onUpdateLocalTemporaryCardNum(QString stuId, QString cardNum)
{
    emit sigUpdateLocalStuCardNum( stuId, cardNum );
}

void CViewManager::onDisBindLocalTemporaryCardNum(QString stuId)
{
    emit sigUpdateLocalStuCardNum( stuId, "" );
}

void CViewManager::onDecetedCardExisteChanged(bool exist)
{
//    qDebug() << "card existed state = " << exist;
    if( m_curPage == ProjectMetaEnums::Page_User_Bind ){
        qDebug() << "state changed stu";
        emit sigStuDecetedCardStateChanged(exist);
    }

    if( m_curPage == ProjectMetaEnums::Page_Class_Bind ){
        qDebug() << "state changed class";
        emit sigClassDecetedCardStateChanged(exist);
    }
}

void CViewManager::onNewNfcCardNumAvailable(QString cardNum)
{
//    qDebug() << "ViewManager cardNum = " << cardNum;
    if( m_curPage == ProjectMetaEnums::Page_User_Bind ){
        qDebug() << "send to stu";
        emit sigStuPageCardNumAvailable( cardNum );
    }

    if( m_curPage == ProjectMetaEnums::Page_Class_Bind ){
        qDebug() << "send to class";
        emit sigClassPageCardNumAvailable( cardNum );
    }
}

void CViewManager::onCurrentPageChanged(ProjectMetaEnums::PageIndex pageIndex)
{
//    qDebug() << "current page = " << pageIndex;
    m_curPage = pageIndex;
}

ProjectMetaEnums::PageIndex CViewManager::curPage() const
{
    return m_curPage;
}

void CViewManager::setCurPage(const ProjectMetaEnums::PageIndex &curPage)
{
    m_curPage = curPage;
}

CInteractionClass *CViewManager::uiClass() const
{
    return m_pUiClass;
}

CStuListModel *CViewManager::pStuDataModel() const
{
    return m_pStuDataModel;
}


CClassListModel *CViewManager::classDataModel() const
{
    return m_pClassDataModel;
}
