#include "common_error.h"

CErrorCode::CErrorCode()
{

}

const map<int, QString> CErrorCode::map_Code_To_Describe(initCodeToDescribe());

QString CErrorCode::getErrorDescribe(int uiErrorCode)
{
    auto iter = map_Code_To_Describe.find(uiErrorCode);

    if(iter != map_Code_To_Describe.end())
    {
        return iter->second;
    }

    return "";
}

void CErrorCode::setErrorCode(int uiErrorCode)
{
    s_Error_Code = uiErrorCode;
}

int CErrorCode::getErrorCode()
{
    return s_Error_Code;
}

const map<int, QString> CErrorCode::initCodeToDescribe()
{
    map<int, QString> mapMapping;
    mapMapping[MOK]                        = QStringLiteral("成功");
    mapMapping[MERR_UNKNOWN]               = QStringLiteral("错误原因不明");

    mapMapping[MERR_SYNC_USER]             = QStringLiteral("正在同步用户信息");
    mapMapping[MERR_SYNC_CLASS]            = QStringLiteral("正在同步班级信息");
    mapMapping[MERR_SYNC_SCORE]            = QStringLiteral("正在同步成绩标准");
    mapMapping[MINFO_LOAD_SCORE]           = QStringLiteral("正在加载成绩标准");
    mapMapping[MINFO_LOAD_FACE_FEATURE]    = QStringLiteral("正在加载本地人脸特征");

    //设备断连
    mapMapping[MERR_CAMERA_RECOGNITION]    = QStringLiteral("人脸识别摄像头断连");
    mapMapping[MERR_CAMERA_ANTICHEATING]   = QStringLiteral("防作弊摄像头断连");
    mapMapping[MERR_USER_RECONGNITION_NFC] = QStringLiteral("身份识别NFC断连");
    mapMapping[MERR_RUN_MID_NFC]           = QStringLiteral("中长跑识别断连");
    mapMapping[MERR_POWER]                 = QStringLiteral("电源设备断连");
    mapMapping[MINFO_STARTING_GUN]         = QStringLiteral("发令枪设备断连");
    mapMapping[MINFO_FOUL_CONTROL]         = QStringLiteral("犯规控制设备断连");

    //
    mapMapping[MINFO_RUN_CARD_BIND_FULL]   = QStringLiteral("底层设备卡号已满");

    return mapMapping;
}

int CErrorCode::s_Error_Code = 0x00;
