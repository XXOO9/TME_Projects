#include "CHttpUpLoadBodyScore.h"
#include "../Config/CDatabaseManage.h"
CHttpUpLoadBodyScore::CHttpUpLoadBodyScore( QObject *parent ) : CHttpAbstractNetWork ( parent )
{
    m_strNetWorkName = "CHttpUpLoadBodyScore";

    if( nullptr == m_pMultiPart ){
        m_pMultiPart = new QHttpMultiPart( QHttpMultiPart::FormDataType, this );
    }

    initConfig();
    initConnection();
}

CHttpUpLoadBodyScore::~CHttpUpLoadBodyScore()
{

}

void CHttpUpLoadBodyScore::httpRequest()
{
    //组装人体成分信息
    if( !constructInfoString() ){
        return;
    }

    if(m_pNetWorkMgr == nullptr){
        LOGERROR("m_pNetWorkMgr == nullptr");
        return;
    }

    if(m_pMultiPart == nullptr){
        LOGERROR("m_pMultiPart == nullptr");
        return;
    }

    QByteArray tmpAuthorzationVal = rawHeaderKeys::HEADERVALUE_BEARER + getServerTokenString().toLocal8Bit();
    setRequestRawHeader(rawHeaderKeys::HEADERVALUE_AUTHORIZATION, tmpAuthorzationVal);

    qDebug() << "upload bodyElement url = " << request().url().toString();
    m_pReply = m_pNetWorkMgr->post( request(), m_pMultiPart );
    startTimeoutTimer( UPLOAD_TIMEOUT );
}

void CHttpUpLoadBodyScore::initConfig()
{
    setInterfaceType( requestType::UPLOADBODYELEMENT );
    setUrl( QString::fromStdString(CDatabaseManage::GetInstance().getServerFunConfig().getUpdateHumanInfoFuncUrl() ) );
}

bool CHttpUpLoadBodyScore::parseJson()
{
    if( !isJson() ){
        return false;
    }

    if( !isRequestSuccess() ){
        return false;
    }
    return true;
}

bool CHttpUpLoadBodyScore::isParameterEmpty()
{
    if( nullptr == m_pMultiPart ){
        return true;
    }
    return false;
}

void CHttpUpLoadBodyScore::setUserId(QString userId)
{
    this->appendTextPart( UPLOADBODYSCORE_KEY_USERID, userId );
}

void CHttpUpLoadBodyScore::setDeviceCode(QString deviceCode)
{
    this->appendTextPart( UPLOADBODYSCORE_KEY_DEVICECODE, deviceCode );
}

void CHttpUpLoadBodyScore::setDeviceType(short deviceType)
{
    QString devType = QString::number( deviceType );
    this->appendTextPart( UPLOADBODYSCORE_KEY_DEVICETYPE, devType );
}

void CHttpUpLoadBodyScore::setRecordTime(QString recordTime)
{
    this->appendTextPart( UPLOADBODYSCORE_KEY_RECORDTIME, recordTime );
}

void CHttpUpLoadBodyScore::setProcessFiles(QStringList paths)
{
    for( auto &eachPath : paths ){
        if( eachPath.isEmpty() ){
            continue;
        }
        this->appendFilePart( eachPath );
    }
}

void CHttpUpLoadBodyScore::setSignPic(QString signPath)
{
    if( signPath.isEmpty() ){
        return;
    }
    this->appendSignPart( signPath );
}

void CHttpUpLoadBodyScore::setheight(QString height)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_HEIGHT, height );
}

void CHttpUpLoadBodyScore::setweight(QString weight)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_WEIGHT, weight );
}

void CHttpUpLoadBodyScore::setFatRate(QString fatRate)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_FARATE, fatRate );
}

void CHttpUpLoadBodyScore::setBMI(QString BMI)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_BMI, BMI );
}

void CHttpUpLoadBodyScore::setHeadWeight(QString headWeight)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_HEADWEIGHT, headWeight );
}

void CHttpUpLoadBodyScore::setHeadFat(QString headFat)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_HEADFAT, headFat );
}

void CHttpUpLoadBodyScore::setHeadMuscle(QString headMuscle)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_HEADMUSCLE, headMuscle );
}

void CHttpUpLoadBodyScore::setTBMM(QString TBMM)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_TBMM, TBMM );
}

void CHttpUpLoadBodyScore::setWBIS(QString WBIS)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_WBIS, WBIS );
}

void CHttpUpLoadBodyScore::setTBW(QString TBW)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_TBW, TBW );
}

void CHttpUpLoadBodyScore::setWHR(QString WHR)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_WHR, WHR );
}

void CHttpUpLoadBodyScore::setVFA(QString VFA)
{
    appendMultiParaMap( UPLOADBODYSCORE_KEY_VFA, VFA );
}

void CHttpUpLoadBodyScore::setParaMap(QMap<QString, QVariant> infos)
{
    QString userId     =  infos.value( HTTP_KEYS::userId ).toString();
    QString devCode    =  infos.value( HTTP_KEYS::devCode ).toString();
    short   devType    =  short(infos.value( HTTP_KEYS::devType ).toInt());
    QString recordTime =  infos.value( HTTP_KEYS::recordTime ).toString();
    QStringList files  =  infos.value( HTTP_KEYS::files ).toStringList();
    QString signPic    =  infos.value( HTTP_KEYS::signPic ).toString();

    QString height     =  infos.value( UPLOADBODYSCORE_KEY_HEIGHT ).toString();
    QString weight     =  infos.value( UPLOADBODYSCORE_KEY_WEIGHT ).toString();
    QString fatRate    =  infos.value( UPLOADBODYSCORE_KEY_FARATE ).toString();
    QString bmi        =  infos.value( UPLOADBODYSCORE_KEY_BMI ).toString();
    QString headWeight =  infos.value( UPLOADBODYSCORE_KEY_HEADWEIGHT ).toString();
    QString headFat    =  infos.value( UPLOADBODYSCORE_KEY_HEADFAT ).toString();
    QString headMuscle =  infos.value( UPLOADBODYSCORE_KEY_HEADMUSCLE ).toString();
    QString tbmm       =  infos.value( UPLOADBODYSCORE_KEY_TBMM ).toString();
    QString wbis       =  infos.value( UPLOADBODYSCORE_KEY_WBIS ).toString();
    QString twb        =  infos.value( UPLOADBODYSCORE_KEY_TBW ).toString();
    QString whr        =  infos.value( UPLOADBODYSCORE_KEY_WHR ).toString();
    QString vfa        =  infos.value( UPLOADBODYSCORE_KEY_VFA ).toString();


    //4项用户信息 必填
    setUserId( userId );
    setDeviceCode( devCode );
    setDeviceType( devType );
    setRecordTime( recordTime );

    //12项测试结果信息，必填
    setheight( height );
    setweight( weight );
    setFatRate( fatRate );
    setBMI( bmi );
    setHeadWeight( headWeight );
    setHeadFat( headFat );
    setHeadMuscle( headMuscle );
    setTBMM( tbmm );
    setWBIS( wbis );
    setTBW( twb );
    setWHR( whr );
    setVFA( vfa );

//    setProcessFiles( files );
//    setSignPic( signPic );
}

bool CHttpUpLoadBodyScore::constructInfoString()
{
    m_bodyElement.clear();
    QString bodyHeight = getSinglePara( UPLOADBODYSCORE_KEY_HEIGHT );
    if( bodyHeight.isEmpty() ){
        return false;
    }

    QString bodyWeight = getSinglePara( UPLOADBODYSCORE_KEY_WEIGHT );
    if( bodyWeight.isEmpty() ){
        return false;
    }

    QString fatRate      =    getSinglePara( UPLOADBODYSCORE_KEY_FARATE );
    QString BMI          =    getSinglePara( UPLOADBODYSCORE_KEY_BMI );
    QString headWeight   =    getSinglePara( UPLOADBODYSCORE_KEY_HEADWEIGHT );
    QString headFat      =    getSinglePara( UPLOADBODYSCORE_KEY_HEADFAT );
    QString headMuscl    =    getSinglePara( UPLOADBODYSCORE_KEY_HEADMUSCLE );
    QString TBMM         =    getSinglePara( UPLOADBODYSCORE_KEY_TBMM );
    QString WBIS         =    getSinglePara( UPLOADBODYSCORE_KEY_WBIS );
    QString TBW          =    getSinglePara( UPLOADBODYSCORE_KEY_TBW );
    QString WHR          =    getSinglePara( UPLOADBODYSCORE_KEY_WHR );
    QString VFA          =    getSinglePara( UPLOADBODYSCORE_KEY_VFA );

    m_bodyElement        +=    HEIGHT_FLAG     +   bodyHeight;
    m_bodyElement        +=    WEIGHT_FLAG     +   bodyWeight;
    m_bodyElement        +=    FATRATE_FLAG    +   fatRate;
    m_bodyElement        +=    BMI_FLAG        +   BMI;
    m_bodyElement        +=    HEADWEIGHT_FLAG +   headWeight;
    m_bodyElement        +=    HEADFAT_FLAG    +   headFat;
    m_bodyElement        +=    HEADMUSCLE_FLAG +   bodyHeight;
    m_bodyElement        +=    TBMM_FLAG       +   headMuscl;
    m_bodyElement        +=    WBIS_FLAG       +   bodyHeight;
    m_bodyElement        +=    TBW_FLAG        +   bodyHeight;
    m_bodyElement        +=    WHR_FLAG        +   bodyHeight;
    m_bodyElement        +=    VFA_FLAG        +   bodyHeight;

    this->appendTextPart( UPLOADBODYSCORE_KEYRECORDLISTSTR, m_bodyElement );
    return true;
}

void CHttpUpLoadBodyScore::onNewReplyAvailable(QNetworkReply *pReply)
{
    stopTimer();
    if( QNetworkReply::NoError != pReply->error() ){
        QString errorString = pReply->errorString();
        setErrType( REQUEST_ERROR, errorString );
        emit sigRequestRetAvailable( false );
        qDebug() << "error string = " << errorString;

        //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
        if( nullptr != m_pReply){
            pReply->deleteLater();
            pReply = nullptr;
            m_pReply = nullptr;
        }

        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );

#if QT_NO_DEBUG
#else
    qDebug() << "upload body score, recv from server ret = : "  << recvString;
#endif

    this->setRecvJsonData( recvData );
    bool ok = parseJson();

    //接收完成后, 清除接收到的缓存, 使用deleteLater 让m_pReply自行决定清除缓存的时机
    if( nullptr != m_pReply){
        pReply->deleteLater();
        pReply = nullptr;
        m_pReply = nullptr;
    }

    emit sigRequestRetAvailable( ok );
}

void CHttpUpLoadBodyScore::onTimeoutHandler()
{
    if( nullptr == m_pReply ){
        return;
    }

    if( m_pReply->isFinished() ){
        return;
    }

    setErrType( errorType::TIMEOUT_ERROR );
    setRequestRetCode( TIMEOUT );
    m_pReply->abort();
}
