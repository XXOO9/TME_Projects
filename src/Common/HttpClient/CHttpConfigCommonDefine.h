#ifndef CHTTPCOMMONDEFINE_H
#define CHTTPCOMMONDEFINE_H

#include <QString>
#include <QObject>

const QString CONFIGFILEPATH = "./cfg/httpConfig.txt";

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
const QString REQUEST_INDEX                              = "index";
const QString NO_ERROR_STRING                            = "no error";
const int     TIMEOUT_SHORT                              = 5000;
const int     TIMEOUT_LONG                               = 8 * 60 *1000;
const int     UPLOAD_TIMEOUT                             = 5 * 1000;
const int     TIMEOUT_RECOG                              = 5 * 1000;
const int     HEARTBEART_TIMEOUT                         = 3000;
const int     HEARTBEART_INTERVAL                        = 5000;

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
const QString SYNCSCORE_AGESTART                         = "ageStart";
const QString SYNCSCORE_AGEEND                           = "ageEnd";
const QString SYNCSCORE_VERSION                          = "version";

//用户信息同步
const QString INTERFACE_SYNCUSERINFO_KEY                 = "interface_syncUserinfo";
const QString INTERFACE_SYNCUSERINFO_VALUE               = "/api/base/user/sync";
const QString SYNCUSERINFO_NAME_UPDATETIME               = "?updateTime=";
const QString SYNCUSERINFO_ARRAY_NAME                    = "userInfoList";
const QString SYNCUSERINFO_del_SPLITIME                  = "deleteSplitTime";
const QString SYNCUSERINFO_del_USER_ARRAY                = "deletedUser";
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
const QString SYNCUSERINFO_KEY_FACEENGINE                = "faceEngineType";


//用户识别
const QString INTERFACE_USERIDENTIFY_KEY                 = "interface_userIdentify";
const QString INTERFACE_USERIDENTIFY_VALUE               = "/api/base/user/identify";
const QString USERIDENTIFY_FACEFEATURE_KEY               = "faceFeature";
const QString USERIDENTIFY_FACEENGINE_KEY                = "faceEngineType";
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
const QString USERIDENTIFY_KEY_STAGE                     = "stage";



//发送心跳
const QString INTERFACE_HEARTBEAT_KEY                    = "interface_heartBeat";
const QString INTERFACE_HEARTBEAT_VALUE                  = "/api/standard/device/heart";
const QString HEARTBEAT_KEY_STATUS                       = "status";
const QString HEARTBEAT_KEY_CODE                         = "code";
const QString HEARTBEAT_KEY_BATTERY                      = "battery";
const QString HEARTBEAT_KEY_TYPE                         = "type";
const QString HEARTBEAT_KEY_INDEX                        = "index";
const QString HEARTBEAT_KEY_LASTSYNCTIME                 = "lastUpdateTime";
const QString HEARTBEAT_KEY_CURRENTTIME                  = "currentTime";

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
const QString SYNCCLASSINFO_DELSPLITIME                  = "deleteSplitTime";
const QString SYNCCLASSINFO_DEL_CLASSID                  = "deletedClazz";
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


//请求结果返回码
const int SUCCESS = 0;
const int OFFLINE = -102;
const int UNKNOWNERROR = -101;
const int TIMEOUT = -10086;
const int REPEATEUPLOAD = 30004;
const int REQUESTERROR = -103;
const int INVALID_INTERFACE = 6004;


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
    UPLOADEYESIGHT,
    UPLOADMULTICOMMONSCORES,
    UPLOADMULTIBODYSCORES,
    UPLOADMULTIEYESIGHTSCORES,
    MULTIHEARTBEAT
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
    examType     = 2,
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
    STATUS_DISCONNECTED = 0,
    STATUS_CONNECTED = 1,
};

//TEST_ITEM定义
enum TEST_ITEM{
    HTTP_TEST_ITEM_UNKNOW            = 0x00,
    HTTP_TEST_ITEM_HEIGHT            = 0x01,
    HTTP_TEST_ITEM_WEIGHT            = 0x02,
    HTTP_TEST_ITEM_HBC               = 0x03,
    HTTP_TEST_ITEM_VC                = 0x04,
    HTTP_TEST_ITEM_SBJ               = 0x05,
    HTTP_TEST_ITEM_SFB               = 0x06,
    HTTP_TEST_ITEM_PULL_UP           = 0x07,
    HTTP_TEST_ITEM_SIT_UP            = 0x08,
    HTTP_TEST_ITEM_BMI               = 0x09,
    HTTP_TEST_ITEM_RUN_1500          = 0xAB,
    HTTP_TEST_ITEM_RUN_1000          = 0xA0,
    HTTP_TEST_ITEM_RUN_800           = 0xA1,
    HTTP_TEST_ITEM_RUN_50            = 0xA2,
    HTTP_TEST_ITEM_ROPE_SKIP         = 0xA3,
    HTTP_TEST_ITEM_RUN_BACK_8_50     = 0xA4,
    HTTP_TEST_ITEM_RUN_60            = 0xA5,
    HTTP_TEST_ITEM_RUN_100           = 0xA6,
    HTTP_TEST_ITEM_RUN_200           = 0xA8,
    HTTP_TEST_ITEM_RUN_400           = 0xA9,
    HTTP_TEST_ITEM_RUN_4_100         = 0xAA,
    HTTP_TEST_ITEM_RUN_HURDLES_100   = 0xAC,
    HTTP_TEST_ITEM_RUN_HURDLES_110   = 0xA7,
    HTTP_TEST_ITEM_GRIP              = 0x12,
    HTTP_TEST_ITEM_PUSH_UP           = 0x13,
    HTTP_TEST_ITEM_VISION_LEFT       = 0x14,
    HTTP_TEST_ITEM_VISION_RIGHT      = 0x15,
    HTTP_TEST_ITEM_FOOTBALL          = 0x16,
    HTTP_TEST_ITEM_BASKETBALL        = 0x17,
    HTTP_TEST_ITEM_VOLLEYBALL        = 0x18,
    HTTP_TEST_ITEM_PUO               = 0x19,
    HTTP_TEST_ITEM_SOLDBALL          = 0x1A,
    HTTP_TEST_ITEM_VERTICALBALL      = 0x1B,
    HTTP_TEST_ITEM_RUN_BACK_4_10     = 0xAD,
    HTTP_TEST_ITEM_RUN_2000          = 0xAE,


    //这里为体成分对应的值
    TEST_TEST_ITEM_FAT_PERCENT       = 0x03, // 全身体脂百分比(单位:%) PBF
    TEST_TEST_ITEM_BMI               = 0x09, // 额外测试项目 BMI
    TEST_TEST_ITEM_HEAD_WEIGHT       = 0x0A, // 头部重量(可能推定值)(单位:Kg)
    TEST_TEST_ITEM_HEAD_FAT          = 0x0B, // 头部脂肪(可能推定值)(单位:Kg)
    TEST_TEST_ITEM_HEAD_MUSCLE       = 0x0C, // 头部肌肉(可能推定值)(单位:Kg)
    TEST_TEST_ITEM_TBMM              = 0x0D, // 全身肌肉量(单位:Kg) Total body muscle mass
    TEST_TEST_ITEM_WBIS              = 0x0E, // 全身无机盐(单位kg） Whole body inorganic salt
    TEST_TEST_ITEM_TBW               = 0x0F, // 全身身体总水分TBW(单位:Kg)
    TEST_TEST_ITEM_WHR               = 0x10, // 全身腰臀比 WHR
    TEST_TEST_ITEM_VFA               = 0x11, // 内脏脂肪指数VFA(单位:面积/10cm)
};

//DEV_TYPE定义
enum DEV_TYPE{
    HTTP_DEV_TYPE_UNKNOW             = 0x00,
    HTTP_DEV_TYPE_BODY               = 0x01,
    HTTP_DEV_TYPE_VC                 = 0x02,
    HTTP_DEV_TYPE_SBJ                = 0x03,
    HTTP_DEV_TYPE_SFB                = 0x04,
    HTTP_DEV_TYPE_PULL_UP            = 0x05,
    HTTP_DEV_TYPE_SIT_UP             = 0x06,
    HTTP_DEV_TYPE_RUN_SHORT          = 0x07,
    HTTP_DEV_TYPE_RUN_MID            = 0x08,
    HTTP_DEV_TYPE_ROPE_SKIP          = 0x09,
    HTTP_DEV_TYPE_ROPE_GRIP          = 0x0A,
    HTTP_DEV_TYPE_PUSH_UP            = 0x0B,
    HTTP_DEV_TYPE_VISION             = 0x0C,

    HTTE_DEV_TYPE_FOOTBALL           = 0x0D,
    HTTE_DEV_TYPE_BASKETBALL         = 0x0E,
    HTTE_DEV_TYPE_VOLLEYBALL         = 0x0F,
    HTTE_DEV_TYPE_PUO                = 0x10,
    HTTE_DEV_TYPE_SOLIDBALL          = 0x11,
    HTTE_DEV_TYPE_VERTICALJUMP       = 0x12,
};

//人脸识别引擎
enum FACE_ENGINE{
    Arc_Soft = 1,
    TMEc_Soft = 2,
};

//上传成绩的类型
enum UPLOAD_TYPE{
    Upload_Common = 0,
    Upload_Body,
    Upload_EyeSight
};

//上传成绩的数据源
enum DATA_SRC{
    DataBase = 0,
    UserExam
};

//上传成绩结果中, 要修改的值
enum CHANGE_SCORE_TYPE{
    ErrorCode = 0,
    ErrorMsg,
    UpLoadSuccessFlag,
};

namespace rawHeaderKeys {
    const QByteArray HEADERNAME_CONTENTTYPE = "Content-type";
    const QByteArray HEADERVALUE_JSON = "application/json";
    const QByteArray HEADERVALUE_URLENCODED = "application/x-www-form-urlencoded";
    const QByteArray HEADERVALUE_AUTHORIZATION = "Authorization";
    const QByteArray HEADERVALUE_BEARER = "Bearer ";
}


#endif // CHTTPCOMMONDEFINE_H
