#ifndef CHTTPUPLOADINFOS_H
#define CHTTPUPLOADINFOS_H

#include <QObject>

class CHttpUploadInfos
{
public:
    CHttpUploadInfos();
    QString m_userId;
    QString m_recordTime;

    //当前这次提交是否成功
    bool    m_isSuccess; //


    //超时定为-10086
    int     requestCode = -10086;


    QString m_testItem;

    //是否是当前提交的vec中的最后一个
    bool    m_isFinished;

    //当前这条数据的数据源是数据库还是用户测试      0 -> 数据库    1 -> 用户测试
    short   m_dataSrc;

    //当前服务器状态
    bool   m_isServerOnline{ false };

    //剩余未提交的数量
    int    m_remainCount = 0;

    //已提交的数量
    QString    m_commitProgress = "";

    QString    m_testRet = "";
};

#endif // CHTTPUPLOADINFOS_H
