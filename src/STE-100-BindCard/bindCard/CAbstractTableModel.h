#ifndef CABSTRACTTABLEMODEL_H
#define CABSTRACTTABLEMODEL_H

#include <QObject>
#include "CDataModel.h"

/*
 * 这个类继承于CDataModel,是为了隐藏一些不必外对外的接口，只对后端数据操作提供接口
 * */

class CAbstractTableModel : public CDataModel
{
    Q_OBJECT
public:
    CAbstractTableModel( qint8 columnCount, QObject *parent = nullptr );
    virtual ~CAbstractTableModel() override;


    //操作水平表头
    Q_INVOKABLE QVariant horizontalHeader( int columnIndex );
    Q_INVOKABLE void setHorizontalHeader( QStringList headers );
    Q_INVOKABLE void setHorizontalHeader( QString val );
    Q_INVOKABLE int horizontalHeaderCount() const;



    //增：
    //严格按照水平表头的顺序来插入list, 并且设置每一行的uniqueString
    bool appendRowDatas( QStringList rowDisplayDatas, QString uniqueString );
    //直接插入一列,每一项CUnitData的uniqueString不能为空
    bool appendRowDatas( QHash<ColumnIndex, CUnitData> rowMap );

    //删：
    //删除uniqueString指定的行
    bool removeRow( QString uniqueingString );

    //改
    //修改指定uniqueString显示的值
    Q_INVOKABLE bool setSpecificData( QString uniqueString, ColumnIndex targetColumn, QString val );
    //给指定行打钩
    Q_INVOKABLE bool check( QString uniqueString );



    Q_INVOKABLE void testfunc1();
    Q_INVOKABLE void testfunc2();
    Q_INVOKABLE void testfunc3();
    Q_INVOKABLE void testfunc4();
    Q_INVOKABLE void testfunc5();
    Q_INVOKABLE void testfunc6();


private:
    qint8                         m_columnCount;
    QStringList                   m_horizontalHeaderList;
    QStringList                   m_rowListContents;
    QHash<ColumnIndex, CUnitData> m_rowMapContents;

private:
    const QString                 m_splitFlag{ "#" };
};

#endif // CABSTRACTTABLEMODEL_H
