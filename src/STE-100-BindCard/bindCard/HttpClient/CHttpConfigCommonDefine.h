#ifndef CHTTPCOMMONDEFINE_H
#define CHTTPCOMMONDEFINE_H

#include <QString>
#include <QObject>

const QString CONFIGFILEPATH = "./cfg/httpConfig.ini";

const QString BASEIP_KEY   = "baseIp";
const QString BASEIP_VALUE = "http://172.16.10.182:7701";

/**************测试服务器接口*********************/
const QString INTERFACE_GET_PARAMETER_KEY          = "interface_getParameter";
const QString INTERFACE_GET_PARAMETER_VALUE        = "/http-test/param";
const QString INTERFACE_GET_PARAMETER_VALUE_ID     = "?id=";


/*********************STE-100服务器******************************************/
const QString REQUEST_RESULT_KEY                         = "code";
const QString REQUEST_RESULT_SUCCESS                     = "0";
const QString REQUEST_VALUE                              = "data";
const QString REQUEST_MESSAGE                            = "msg";
const QString NO_ERROR_STRING                            = "no error";
const int     TIMEOUT_SHORT                              = 5000;
const int     TIMEOUT_LONG                               = 2000000;

//同步指导视频
const QString INTERFACE_SYNCVIDEO_KEY                    = "interface_syncVideo";
const QString INTERFACE_SYNCVIDEO_VALUE                  = "/api/standard/sample-video/sync";
const QString INTERFACE_SYNVVIDEO_UPDATETIME             = "?updateTime=";
const QString SYNCVIDEO_ARRAY_NAME                       = "data";
const QString SYNCVIDEO_NAME_TYPE                        = "type";
const QString SYNCVIDEO_NAME_URL                         = "url";
const QString SYNCVIDEO_NAME_UPDATETIME                  = "updateTime";
const QString SYNCVIDEO_NAME_PICURL                      = "picUrl";

//成绩标准同步
const QString INTERFACE_SYNCSCORE_KEY                    = "interface_syncScore";
const QString INTERFACE_SYNCSCORE_VALUE                  = "/api/standard/sport-rule-detail/sync";
const QString SYNCSCORE_ARRAY_NAME                       = "data";
const QString SYNCSCORE_GRADE                            = "grade";
const QString SYNCSCORE_TESTITEM                         = "testItem";
const QString SYNCSCORE_SCORE                            = "score";
const QString SYNCSCORE_GENDER                           = "gender";
const QString SYNCSCORE_RESULTSTART                      = "resultStart";
const QString SYNCSCORE_RESULTEND                        = "resultEnd";

//用户信息同步
const QString INTERFACE_SYNCUSERINFO_KEY                 = "interface_syncUserinfo";
const QString INTERFACE_SYNCUSERINFO_VALUE               = "/api/base/user/sync";
const QString SYNCUSERINFO_NAME_UPDATETIME               = "?updateTime=";
const QString SYNCUSERINFO_ARRAY_NAME                    = "userInfoList";
const QString SYNCUSERINFO_KEY_ID                        = "id";
const QString SYNCUSERINFO_KEY_YEAR                      = "year";
const QString SYNCUSERINFO_KEY_CLASSID                   = "clazzId";
const QString SYNCUSERINFO_KEY_ROLE                      = "role";
const QString SYNCUSERINFO_KEY_CODE                      = "code";
const QString SYNCUSERINFO_KEY_CARDNO                    = "cardNo";
const QString SYNCUSERINFO_KEY_NAME                      = "name";
const QString SYNCUSERINFO_KEY_GENDER                    = "gender";
const QString SYNCUSERINFO_KEY_BIRTHDAY                  = "birthday";
const QString SYNCUSERINFO_KEY_NATION                    = "nation";
const QString SYNCUSERINFO_KEY_FACEFEATURE               = "faceFeature";
const QString SYNCUSERINFO_KEY_DESCRIPTION               = "description";
const QString SYNCUSERINFO_KEY_AVATARURL                 = "avatarUrl";
const QString SYNCUSERINFO_KEY_UPDATETIME                = "updateTime";


//用户识别
const QString INTERFACE_USERIDENTIFY_KEY                 = "interface_userIdentify";
const QString INTERFACE_USERIDENTIFY_VALUE               = "/api/base/user/identify";
const QString USERIDENTIFY_FACEFEATURE_KEY               = "faceFeature";
const QString USERIDENTIFY_CARDNO_KEY                    = "cardNo";
const QString USERIDENTIFY_KEY_ID                        = "id";
const QString USERIDENTIFY_KEY_YEAR                      = "year";
const QString USERIDENTIFY_KEY_CLASSID                   = "clazzId";
const QString USERIDENTIFY_KEY_ROLE                      = "role";
const QString USERIDENTIFY_KEY_CODE                      = "code";
const QString USERIDENTIFY_KEY_CARDNO                    = "cardNo";
const QString USERIDENTIFY_KEY_NAME                      = "name";
const QString USERIDENTIFY_KEY_GENDER                    = "gender";
const QString USERIDENTIFY_KEY_BIRTHDAY                  = "birthday";
const QString USERIDENTIFY_KEY_NATION                    = "nation";
const QString USERIDENTIFY_KEY_FACEFEATURE               = "faceFeature";
const QString USERIDENTIFY_KEY_DESCRIPTION               = "description";
const QString USERIDENTIFY_KEY_AVATARURL                 = "avatarUrl";
const QString USERIDENTIFY_KEY_UPDATETIME                = "updateTime";



//发送心跳
const QString INTERFACE_HEARTBEAT_KEY                    = "interface_heartBeat";
const QString INTERFACE_HEARTBEAT_VALUE                  = "/api/standard/device/heart";
const QString HEARTBEAT_KEY_STATUS                       = "status";
const QString HEARTBEAT_KEY_CODE                         = "code";
const QString HEARTBEAT_KEY_BATTERY                      = "battery";
const QString HEARTBEAT_KEY_TYPE                         = "type";

//成绩上传( 非身高体重 )
const QString INTERFACE_UPLOADSOCRE_KEY                  = "interface_uploadScore";
const QString INTERFACE_UPLOADSOCRE_VALUE                = "/api/standard/device-record";
const QString UPLOADSOCRE_USERID_KEY                     = "userId";
const QString UPLOADSOCRE_SCORETYPE_KEY                  = "type";
const QString UPLOADSOCRE_TESTITEM_KEY                   = "testItem";
const QString UPLOADSOCRE_RESULT_KEY                     = "result";
const QString UPLOADSOCRE_RECORDTIME_KEY                 = "recordTime";
const QString UPLOADSOCRE_FILES_KEY                      = "files";
const QString UPLOADSOCRE_DEVICECODE_KEY                 = "deviceCode";
const QString UPLOADSOCRE_DEVICETYPE_KEY                 = "deviceType";
const QString UPLOADSOCRE_SIGN_KEY                       = "signPic";


//班级信息同步
const QString INTERFACE_SYNCCLASSINFO_KEY                = "interface_syncClassInfo";
const QString INTERFACE_SYNCCLASSINFO_VALUE              = "/api/base/clazz/sync";
const QString INTERFACE_SYNCCLASSINFO_UPDATETIME         = "?updateTime=";
const QString SYNCCLASSINFO_ARRAYNAME                    = "clazzInfoList";
const QString SYNCCLASSINFO_KEY_ID                       = "id";
const QString SYNCCLASSINFO_KEY_NAME                     = "name";
const QString SYNCCLASSINFO_KEY_YEAR                     = "year";
const QString SYNCCLASSINFO_KEY_SCHOOL                   = "schoolId";
const QString SYNCCLASSINFO_KEY_TYPE                     = "type";
const QString SYNCCLASSINFO_KEY_ENABLE                   = "enable";
const QString SYNCCLASSINFO_KEY_GRADUATE                 = "graduate";
const QString SYNCCLASSINFO_KEY_DESCRIPTION              = "description";
const QString SYNCCLASSINFO_KEY_UPDATETIME               = "updateTime";
const QString SYNCCLASSINFO_KEY_GRADE                    = "grade";

//列历史成绩
const QString INTERFACE_QUERYHISTORY_KEY                 = "interface_queryHistory";
const QString INTERFACE_QUERYHISTORY_VALUE               = "/api/standard/device-record/student-history-record";
const QString QUERYHISTORY_KEY_ARRAY_NAME                = "data";
const QString QUERYHISTORY_KEY_USERID                    = "userId";
const QString QUERYHISTORY_USERID                        = "?userId=";
const QString QUERYHISTORY_KEY_TESTITEM                  = "testItem";
const QString QUERYHISTORY_TESTITEM                      = "&testItem=";
const QString QUERYHISTORY_KEY_TIMEINTERVAL              = "timeInterval";
const QString QUERYHISTORY_TIMEINTERVAL                  = "&timeInterval=";
const QString QUERYHISTORY_KEY_TYPE                      = "type";
const QString QUERYHISTORY_TYPE                          = "&type=";
const QString QUERYHISTORY_TEST_ITEM                     = "testItem";
const QString QUERYHISTORY_SCORE                         = "score";
const QString QUERYHISTORY_RECORD                        = "record";
const QString QUERYHISTORY_RECORDTIME                    = "recordTime";

//上传身高体重成绩
const QString INTERFACE_BODYSCORE_KEY                    = "interface_bodyScore";
const QString INTERFACE_BODYSCORE_VALUE                  = "/api/standard/device-record/bc-data";
const QString UPLOADBODYSCORE_KEYRECORDLISTSTR           = "recordListStr";
const QString UPLOADBODYSCORE_KEY_USERID                 = "userId";
const QString UPLOADBODYSCORE_KEY_HEIGHT                 = "height";
const QString UPLOADBODYSCORE_KEY_WEIGHT                 = "weight";
const QString UPLOADBODYSCORE_KEY_FARATE                 = "faRate";
const QString UPLOADBODYSCORE_KEY_BMI                    = "BMI";
const QString UPLOADBODYSCORE_KEY_HEADWEIGHT             = "headWeight";
const QString UPLOADBODYSCORE_KEY_HEADFAT                = "headFat";
const QString UPLOADBODYSCORE_KEY_HEADMUSCLE             = "headMuscle";
const QString UPLOADBODYSCORE_KEY_TBMM                   = "TBMM";
const QString UPLOADBODYSCORE_KEY_WBIS                   = "WBIS";
const QString UPLOADBODYSCORE_KEY_TBW                    = "TBW";
const QString UPLOADBODYSCORE_KEY_WHR                    = "WHR";
const QString UPLOADBODYSCORE_KEY_VFA                    = "VFA";
const QString UPLOADBODYSCORE_KEY_RECORDTIME             = "recordTime";
const QString UPLOADBODYSCORE_KEY_DEVICETYPE             = "deviceType";
const QString UPLOADBODYSCORE_KEY_DEVICECODE             = "deviceCode";
const QString UPLOADBODYSCORE_KEY_RETSTRING              = "recordListStr";
const QString HEIGHT_FLAG                                = "1=";
const QString WEIGHT_FLAG                                = "@2=";
const QString FATRATE_FLAG                               = "@3=";
const QString BMI_FLAG                                   = "@9=";
const QString HEADWEIGHT_FLAG                            = "@10=";
const QString HEADFAT_FLAG                               = "@11=";
const QString HEADMUSCLE_FLAG                            = "@12=";
const QString TBMM_FLAG                                  = "@13=";
const QString WBIS_FLAG                                  = "@14=";
const QString TBW_FLAG                                   = "@15=";
const QString WHR_FLAG                                   = "@16=";
const QString VFA_FLAG                                   = "@17=";

//查询人体成分
const QString INTERFACE_QUERYBODYELEMENT_KEY             = "interface_queryBodyElement";
const QString INTERFACE_QUERYBODYELEMENT_VALUE           = "/api/standard/device-record/student-history-record/bc";
const QString QUERYBODYELEMENT_KEY_USERID                = "userId";
const QString QUERYBODYELEMENT_KEY_TIMEINTERVAL          = "timeInterval";
const QString QUERYBODYELEMENT_KEY_TYPE                  = "type";

//游客使用记录
const QString INTERFACE_TOURISTRECORD_KEY                = "interface_touristRecord";
const QString INTERFACE_TOURISTRECORD_VALUE              = "/api/standard/device-usage/visitor";
const QString TOURISTRECORD_KEY_DEVICECODE               = "code";
const QString TOURISTRECORD_KEY_DEVICETYPE               = "type";
const QString TOURISTRECORD_KEY_CREATETIME               = "createTime";
const QString TOURISTRECORD_DEVICECODE                   = "?code=";
const QString TOURISTRECORD_DEVICETYPE                   = "&type=";
const QString TOURISTRECORD_CREATETIME                   = "&createTime=";

//请求接口类型
enum requestType{
    UNKNOWNREQUEST = 1,
    GETVIDEOURLS,
    SYNCUSERINFO,
    USERIDENTIFY,
    SYNCSCORESTD,
    HEARTBEAT,
    UPLOADSCORECOMMON,
    SYNCCLASSINFO,
    TOURISTRECORD,
    QUERYBODYELEMENTHISTORY,
    QUERYCOMMONHISTORYSCORE,
    TOURISTUSAGERECORD,
    UPLOADBODYELEMENT,
};

//查询时间尺度
enum queryTimeInterval{
    recent_one_week = 1,
    recent_one_month,
    recent_three_month
};

//成绩类型
enum testType{
    exerciseType = 1,
    examType
};

//本地错误类型
enum errorType{
    UNKNOWN_ERR          = 1,
    NO_ERR,
    TIMEOUT_ERROR,
    PARSE_JSON_FORMAT_ERR,
    REQUEST_ERROR,
};


//发送心跳包时，设备状态类型
enum devStatus{
    HARDWARE_DISCONNECTED = 0,
    HARDWARE_CONNECTED = 1,
    NETWORK_CONNECTED,
    NETWORK_DISCONNECTED,
};

//TEST_ITEM定义
enum TEST_ITEM{
    TEST_ITEM_UNKNOW            = 0x00,
    TEST_ITEM_HEIGHT            = 0x01,
    TEST_ITEM_WEIGHT            = 0x02,
    TEST_ITEM_BCI               = 0x03,
    TEST_ITEM_VC                = 0x04,
    TEST_ITEM_SBJ               = 0x05,
    TEST_ITEM_SFB               = 0x06,
    TEST_ITEM_PULL_UP           = 0x07,
    TEST_ITEM_SIT_UP            = 0x08,
    TEST_ITEM_BMI               = 0x09,
    TEST_ITEM_RUN_1000          = 0xA0,
    TEST_ITEM_RUN_800           = 0xA1,
    TEST_ITEM_RUN_50            = 0xA2,
    TEST_ITEM_ROPE_SKIP         = 0xA3,
    TEST_ITEM_RUN_BACK_8_50     = 0xA4,
};

//DEV_TYPE定义
enum DEV_TYPE{
    DEV_TYPE_UNKNOW             = 0x00,
    DEV_TYPE_BODY               = 0x01,
    DEV_TYPE_VC                 = 0x02,
    DEV_TYPE_SBJ                = 0x03,
    DEV_TYPE_SFB                = 0x04,
    DEV_TYPE_PULL_UP            = 0x05,
    DEV_TYPE_SIT_UP             = 0x06,
    DEV_TYPE_RUN_SHORT          = 0x07,
    DEV_TYPE_RUN_MID            = 0x08,
    DEV_TYPE_ROPE_SKIP          = 0x09,
};

namespace rawHeaderKeys {
const QByteArray HEADERNAME_CONTENTTYPE = "Content-type";
const QByteArray HEADERVALUE_JSON       = "application/json";
}


#endif // CHTTPCOMMONDEFINE_H
