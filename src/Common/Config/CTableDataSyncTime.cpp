#include "CTableDataSyncTime.h"
#include "Logger/CLogger.h"
#include <QDebug>

//定义日期格式
static const QString s_datetimeFormat( "yyyy-MM-dd hh:mm:ss" );
//定义最早的日期
static const QString s_datetimeFirst("1997-01-01 00:00:00");

CTableDataSyncTime::CTableDataSyncTime()
{

}

bool CTableDataSyncTime::Check_Config_Table()
{
    return true;
}

bool CTableDataSyncTime::updateSyncTime(short shTableType, const QDateTime &datetime)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try
    {
        //float值不能作为判断条件只能通过查询之后的值进行判断
        vector<DataSyncTime> vecSearch;

        //生成条件出错标识rule不符合插入或者更新条件
        vecSearch = select<DataSyncTime>(*m_pDatabase, DataSyncTime::TableType == shTableType).all();
        bool bExist = false;

        QString qsDatime = datetime.toString(s_datetimeFormat);
        //更新
        for(auto &element : vecSearch)
        {
            bExist = true;
            element.lastSyncTime = qsDatime.toStdString();
            element.update();
        }

        //新增
        if(!bExist)
        {
            DataSyncTime scoreRule(*m_pDatabase);
            scoreRule.tableType = int(shTableType);
            scoreRule.lastSyncTime = qsDatime.toStdString();
            scoreRule.update();
        }

    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

bool CTableDataSyncTime::getSyncTime(short shType, QDateTime &datetime)
{
    if(m_pDatabase == nullptr)
    {
        LOGERROR("m_pDatabase == nullptr");
        return false;
    }

    lock();
    bool bRes = true;

    try
    {
        if(shType < TABLE_TYPE_USER_INFO || shType > TYPE_VEDIO_RUN )
        {
            bRes = false;
        }

        if(bRes)
        {
            vector<DataSyncTime> vecSearch;

            vecSearch = select<DataSyncTime>(*m_pDatabase, DataSyncTime::TableType == shType).all();

            if(vecSearch.size() > 0)
            {
                auto element = vecSearch[ 0 ];
                datetime = QDateTime::fromString(QString::fromStdString(element.lastSyncTime), s_datetimeFormat);
            }
            else
            {
                if(shType >= TABLE_TYPE_USER_INFO && shType <= TABLE_TYPE_SCORE_RULE)
                {
                    datetime = QDateTime::fromString(s_datetimeFirst, s_datetimeFormat);
                }
            }
        }
    }
    catch(Except e)
    {
        bRes = false;
    }

    unlock();
    return bRes;
}

QString CTableDataSyncTime::getDateTimeFormat()
{
    return s_datetimeFormat;
}
