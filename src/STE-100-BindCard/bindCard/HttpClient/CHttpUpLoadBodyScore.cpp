#include "CHttpUpLoadBodyScore.h"

CHttpUpLoadBodyScore::CHttpUpLoadBodyScore( QObject *parent ) : CHttpAbstractNetWork ( parent )
{
    if( nullptr == m_pNetWorkMgr ){
        m_pNetWorkMgr = new QNetworkAccessManager( this );
    }

    if( nullptr == m_pMultiPart ){
        m_pMultiPart = new QHttpMultiPart( QHttpMultiPart::FormDataType, this );
    }

    initConfig();
    initConnection();
}

CHttpUpLoadBodyScore::~CHttpUpLoadBodyScore()
{

}

void CHttpUpLoadBodyScore::initConfig()
{
    setInterfaceType( requestType::UPLOADBODYELEMENT );
    CHttpConfigReaderSingeton  *configReader = CHttpConfigReaderSingeton::getInstance();
    QString baseIp = configReader->getValue( BASEIP_KEY );
    QString interface = configReader->getValue( INTERFACE_BODYSCORE_KEY );
    QString urlString = baseIp + interface;
    setUrl( baseIp + interface );
}

void CHttpUpLoadBodyScore::initConnection()
{
    connect( m_pNetWorkMgr, &QNetworkAccessManager::finished, this, &CHttpUpLoadBodyScore::onNewReplyAvailable );
    connect( this, &CHttpUpLoadBodyScore::sigTimeout, this, &CHttpUpLoadBodyScore::onTimeoutHandler );
}

void CHttpUpLoadBodyScore::httpRequest()
{
    //组装人体成分信息
    if( !constructInfoString() ){
        return;
    }
    qDebug() << "upload bodyElement url = " << request().url().toString();
    m_pReply = m_pNetWorkMgr->post( request(), m_pMultiPart );
    startTimer( TIMEOUT_LONG );
}

bool CHttpUpLoadBodyScore::parseJson()
{
    return true;
}

bool CHttpUpLoadBodyScore::isParameterEmpty()
{
    return true;
}

void CHttpUpLoadBodyScore::setUserId(QString userId)
{
    this->appendTextPart( UPLOADBODYSCORE_KEY_USERID, userId );
}

void CHttpUpLoadBodyScore::setDeviceCode(QString deviceCode)
{
    this->appendTextPart( UPLOADBODYSCORE_KEY_DEVICECODE, deviceCode );
}

void CHttpUpLoadBodyScore::setDeviceType(DEV_TYPE deviceType)
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
        setErrType( REQUEST_ERROR );
        emit sigRequestRetAvailable( false );
        qDebug() << "error string = " << pReply->errorString();
        return;
    }
    QByteArray recvData = pReply->readAll();
    QString recvString =  QString::fromUtf8( recvData );
    this->setRecvJsonData( recvData );
    bool ok = parseJson();
    emit sigRequestRetAvailable( ok );
}

void CHttpUpLoadBodyScore::onTimeoutHandler()
{
    setErrType( errorType::TIMEOUT_ERROR );
    m_pReply->abort();
}
