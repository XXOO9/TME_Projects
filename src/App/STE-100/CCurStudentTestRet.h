#ifndef CCURSTUDENTTESTRET_H
#define CCURSTUDENTTESTRET_H

#include <QAbstractTableModel>
#include <QDebug>
#include <QVector>
#include <QHash>
#include <QString>

typedef struct structDataModel{
    QString index;
    QString name;
    QString ret;
    QString score;
    QString trackNumber; //中长跑:圈数 短跑：赛道号
    QString isCheck;

    void clean(){
        index.clear();
        name.clear();
        ret.clear();
        score.clear();
        trackNumber.clear();
        isCheck.clear();
    }
} structDataModel;

typedef struct  structRopeSkip{
    QString index;
    QString name;
    QString battery;
    QString ret;
    QString score;
    QString isCheck;

    void clean()
    {
        index.clear();
        name.clear();
        battery.clear();
        ret.clear();
        score.clear();
        isCheck.clear();
    }
}structRopeSkip;

class CCurStudentTestRet : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum DATATYPE{
        Type_Name = 0,
        Type_TrackNumber,
        Type_Ret,
        Type_Score,
        Type_Check,
        Type_Index
    };

    enum DATATYPE_RopeSkip{
        RopeSkip_Name = 0,
        RopeSkip_Battery,
        RopeSkip_Ret,
        RopeSkip_Score,
        RopeSkip_Check,
        RopeSkip_Index
    };
    Q_ENUM( DATATYPE )
    explicit CCurStudentTestRet(QObject *parent = nullptr);
    Q_INVOKABLE int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    Q_INVOKABLE int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
    Q_INVOKABLE QHash< int, QByteArray > roleNames()const override;

    Q_INVOKABLE QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    Q_INVOKABLE void addItem(QString name, QString trackNumber, QString ret, QString score );

    //新添加行 -> 短跑用
    Q_INVOKABLE void addItem( structDataModel dataModel );

    Q_INVOKABLE void addRopeSkip( structRopeSkip dataModel );

    Q_INVOKABLE void deleteItem(QString index);


    //中长跑显示成绩
    Q_INVOKABLE void updateItem( QString id, QString name, QString aws,QString score,QString number);
    Q_INVOKABLE void cleanAll();

    Q_INVOKABLE void check( QString index );

    //根据索引设置每一项的值， 如果index所在的行不存在，则无效果
    Q_INVOKABLE void setSingleData( QString index, CCurStudentTestRet::DATATYPE dataType, QString data );

    Q_INVOKABLE void setRopeSkinSingleData( QString index, CCurStudentTestRet::DATATYPE_RopeSkip dataType, QString data );


    Q_INVOKABLE void setSocre();

private:
    QVector< QVector< QString > > m_arrayCurTestData;
};

#endif // CCURSTUDENTTESTRET_H
