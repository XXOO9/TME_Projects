#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QApplication>
#include <QDebug>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QSettings>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>

#include <set>
#include "common_global.h"
#include "../Common/Config/CUserInfo.h"
#include "../Common/Config/CClassInfo.h"
#include "../Common/Config/CScoreRule.h"

using namespace std;

typedef struct HttpInterface
{
    QString baseIp;
    QString videoSync;
    QString scoreStandardSync;
    QString userInfoSync;
    QString upDateTestRet;
    QString upDataHeartBeat;
    QString getHistoryScore;
    QString classInfoSync;
    QString upDateHumanFormMeasure;
    QString userIdentify;
    QString touristUsageRecord;
    QString queryClassInfo;

    void clear()
    {
        baseIp.clear();
        videoSync.clear();
        scoreStandardSync.clear();
        userInfoSync.clear();
        upDateTestRet.clear();
        upDataHeartBeat.clear();
        getHistoryScore.clear();
        classInfoSync.clear();
        upDateHumanFormMeasure.clear();
        userIdentify.clear();
        touristUsageRecord.clear();
        queryClassInfo.clear();
    }

}structHttpInterface;

//上传成绩(非身高体重)使用的结构体
typedef struct upDateTestResultDate{
    QString userId;
    QString type;
    QString testItem;
    QString result;
    QString recordTime;
    QString devicedCode;
    QString deviceType;
    QStringList filePaths;
    QString signPath;

    void clean()
    {
        userId.clear();
        type.clear();
        testItem.clear();
        result.clear();
        recordTime.clear();
        deviceType.clear();
        devicedCode.clear();
        filePaths.clear();
        signPath.clear();
    }
} structUpDateTestResultDate;

//上传成绩(身高体重)使用的结构体
typedef struct humManFormMeasure
{
    QString userId;
    QString deviceCode;
    QString deviceType;
    QString humanHeight;    //身高
    QString humanWeight;    //体重
    QString BMI;            //BMI值
    QString fatRate;        //体脂率
    QString headWeight;     //头部重量
    QString headFat;        //头部脂肪
    QString headMuscle;     //头部肌肉
    QString TBMM;           //全身肌肉量
    QString WBIS;           //全身无机盐
    QString TBW;            //全身身体总水分
    QString WHR;            //全身身体总水分TBW
    QString VFA;            //内脏脂肪指数VFA
    QString timeStamp;
    QString testingPicturesPath;
    void clean()
    {
        userId.clear();
        deviceCode.clear();
        deviceType.clear();
        humanHeight.clear();
        humanWeight.clear();
        fatRate.clear();
        timeStamp.clear();
        testingPicturesPath.clear();
        headWeight.clear();
        headFat.clear();
        headMuscle.clear();
        TBMM.clear();
        WBIS.clear();
        TBW.clear();
        WHR.clear();
        VFA.clear();
        timeStamp.clear();
        testingPicturesPath.clear();
    }
}structUpdateHumanFormMeasure;

//发送心跳使用的结构体
typedef struct HeartBeat
{
    QString code;
    QString type;
    QString status;
    QString battery;

    void clean()
    {
        code.clear();
        type.clear();
        status.clear();
        battery.clear();
    }
}structDevHeartBeat;

//获取同步视频连接的结构体
typedef struct sampleVideoSync
{
    QString url;
    QString upDateTime;
    QString type;

    void clean()
    {
        url.clear();
        upDateTime.clear();
        type.clear();
    }
}structSampleVideoSync;

//班级信息结构体
typedef struct classInfo
{
    QString id;
    QString name;
    QString year;
    QString schoolId;
    QString type;
    QString enable;
    QString graduate;
    QString description;
    QString updateTime;
    QString grade;
    void clean()
    {
        id.clear();
        name.clear();
        year.clear();
        schoolId.clear();
        type.clear();
        enable.clear();
        graduate.clear();
        description.clear();
        updateTime.clear();
        grade.clear();
    }
}structClassInfo;

//历史成绩结构体
typedef struct historyScore
{
    //测试项目
    QString testItem;

    //测试时间戳
    QString recordTime;

    //测试结果
    QString record;

    //测试分数
    QString score;

    void clean()
    {
        testItem.clear();
        recordTime.clear();
        record.clear();
        score.clear();
    }
}structHistory;

//请求历史成绩需要的参数
typedef struct requestHistory
{
    QString userId;
    QString testItem;
    QString timeInterval;
    QString type;

    void clean()
    {
        userId.clear();
        testItem.clear();
        timeInterval.clear();
        type.clear();
    }
}structRequestHistory;

class COMMONSHARED_EXPORT HttpConnection : public QObject
{
    Q_OBJECT

public:
    enum STEHttpConnectionType
    {
        POST_UPLOADFILE = 1,
        POST_PARAMETER,
        POST_PARAMETERWAIT,
        GET_WITHPARAMETER,
        GET_WITHOUTPARAMETER,
        GET_DOWNLOADFILE
    };

    Q_ENUM( STEHttpConnectionType )

//    explicit HttpConnection(QObject *parent = nullptr);

//    ~HttpConnection();

//    void initThread();

//    void initNetwork();

//    //post  上传文件附带参数
//    void postFormDataWithMultipart( QString filePath, QByteArray id );

//    //post  (异步)在请求体中带参数，发送json字符串
//    void postJsonInBody( QByteArray jsonData );

//    //post  (同步)在请求体中带参数，发送json字符串
//    void postJsonInBodyWait( QByteArray jsonData );

//    //get   带参数( id )的Get请求
//    void getWithParameter( QString id );

//    //get   不带参数的get请求
//    void getWithoutParameter();

//    //get 带参数( 时间戳 )的get请求
//    void getWithParameterTimeStampWait( QString timeStamp );


//    //get 下载文件
//    void getDownloadFile( QString fileName );

//    //销毁文件指针
//    void destoryFilePointer();

//    //装载要通过form-data发送的文件
//    QHttpPart loadFilePart( QString filePath );

//    //装载要发送的form-data发送的参数
//    QHttpPart loadTextPart( QString content , QString key );

//    //创建人脸特征值和卡号json字符串
//    QByteArray createFaceFeatureAndCardNoJson( QLatin1String faceFeature, QString cardNo = "" );

//    void setCurRequestUrl( QString url );

//    QString getCurRequestUrl();

//    //获取http返回的原始json字符串
//    QByteArray getHttpReplyJsonData();

//    bool checkStuInfoReplyCorrect( QByteArray jsonData );

//    //json字符串转json Obj
//    QJsonObject jsonDataToJsonObject( QByteArray jsonData );

//    QMap< QString, QString > parseRecvCurStudentInfo( QJsonObject jsObj );

//    /****************************************************************************************/
//    //从 jsonObject 中提取对应Key的值
//    QVariant takeValueOfKeyFromJsonObject( QJsonObject jsObj, QString key );

//    //解析 get请求时间戳后的http返回值( 用户信息同步 )
//    bool parseRecvJsonUpDateUserInfo( vector< CUserInfo > &vec, vector<int> &deleteinfo, qint64 &timeStamp );

//    //提取最大的时间戳
//    qint64  getMaxTimeStampFromJsonArray( QJsonArray jsArray );

//    //提取用户信息
//    QVector<CUserInfo> getEachUserInfo( QJsonArray jsArray );



//    /*******************与服务器对接的接口****************************/

//    //发起获取需要同步的示范视频连接请求
//    bool sampleVideoSync( QString timeStamp, bool bBlock = false );

//    //提取需要同步的示范视频连接
//    QVector< structSampleVideoSync > parseSampleVideoSync();

//    //发起成绩标准信息同步请求
//    bool scoreStanderdSync( bool bBlock = false );

//    //提取成绩标准信息同步
//    QVector< CScoreRule > parseScoreStanderdSync();

//    //发起班级信息同步请求
//    bool classInfoSync( bool bBlock = false );

//    //提取班级同步信息
//    QVector< CClassInfo > parseClassInfo();

//    //发起查询历史成绩的请求
//    bool queryHistoryResult( structRequestHistory requestHistory, bool bBlock = false );

//    //提取查询历史成绩的信息
//    QVector< structHistory > parseHistory();

//    //向服务器上传体测成绩( 非身高体重 )
//    void onSendCurTestRetToServerWait( structUpDateTestResultDate upDateInfo );

//    //向服务器上传体测成绩( 身高体重 )
//    void onSendCurTestToServerForHumanFormMeasure( structUpdateHumanFormMeasure humanMeasure );

//    //将人体成分信息拼接成一个字符串依靠@符号作为分隔符
//    QString getHumanComposition( structUpdateHumanFormMeasure humanMeasure );

//    //发送心跳, 默认非阻塞
//    bool onSendHeartBeat( structDevHeartBeat heartBeatInfo , bool bBlock = false );

//    bool singalCheck( QByteArray data );

//    //通过http返回判断是否上传成功
//    bool checkRequestSuccessful();
//    int getLastErrorRequest(QString &qsLastError);

//    //根据classID查询班级信息
//    structClassInfo queryClassInfoByClassID( int classID );

//    QString getClassName();

//    /************2021.10.13 添加**********************/

//    //创建心跳包
//    QByteArray createHeartBeatJson( structDevHeartBeat heartBeatInfo );

//    //从配置文件读取 http接口, 如果配置文件不存在，则创建默认配置文件
//    void readConfigFile();

//    //创建默认配置文件
//    void createDefaultConfigFile();

//    /**********************2021.10.28 添加****************************/
//    void downloadFromUrl( QString strUrl );

//    /**************2021.11.04 添加, 游客模式使用记录上传 ************************/
//    void recordTouristUsageToServer( QString devCode, QString devType );

//signals:
//    //启动http下载信号
//    void signalStartHttpDownload( QString downloadAddr );

//    void upDateLastTime();

//    void signalFinish();

//public slots:
//    //响应http的返回
//    void onNewNetworkReplyFinished( QNetworkReply *pReply );

//    //响应http下载完成
//    void onHttpDownloadFinished();

//private:
//    QNetworkAccessManager   *m_pNetworkMgr{nullptr};
//    QNetworkReply           *m_pReply{nullptr};
//    QFile                   *m_pFile{nullptr};
//    QString                  m_sBaseUrl;
//    QString                  m_sExFormData;
//    QString                  m_sExRaw;
//    QString                  m_sExParameter;
//    QString                  m_sExDownload;
//    QString                  m_sCurRequestUrl;
////    HttpDownloadThread      *m_pHttpDownloader{nullptr};
//    QThread                 *m_pDownloaderWorkThread{nullptr};
//    QByteArray               m_baRecvJsonData;
//    QJsonObject              m_objRecvJsonObj;

//    QSettings                *m_pSettingConfiger{nullptr};
//    structHttpInterface      m_structHttpInterface;
//    structSampleVideoSync    m_structSampleVideoInfos;

//    int                   m_iHttpTimeout;

//    //通过两个变量进行获取错误
//    QString                 m_qsLastErr;
//    int                     m_nLastErr;
};

#endif // HTTPCONNECTION_H
