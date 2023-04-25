#ifndef CDATAMODEL_H
#define CDATAMODEL_H

#include <QAbstractTableModel>
#include <QDebug>

class CDataModel;
class CUnitData;

class CDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum RoleNames{
        DisplayRole,
        UniqueRole,
        DataRole,
    };

    enum ColumnIndex{
        FirstColumn = 0,
        SecondColumn,
        ThirdColumn,
        ForthColumn,
        FifthColumn,
        SixthColumn,
        SevengthColumn,
        EightColumn
    };

    Q_ENUM( RoleNames )
    Q_ENUM( ColumnIndex )

public:
    explicit CDataModel(QObject *parent = nullptr);


protected:

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool appendRow(QHash<CDataModel::ColumnIndex, CUnitData> data );

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    QHash<int, QByteArray> roleNames() const override;

    //change specific data in abstract table, even change it's role value
    Q_INVOKABLE bool setSingleData( const QString uniqeuString,
                                    const ColumnIndex column,
                                    const QString val,
                                    const RoleNames roleName = RoleNames::DisplayRole );

    Q_INVOKABLE void cleanAll();

    //check singleton
    bool isSingleton( QString uniqueString );

private:
    QVector< QHash<ColumnIndex, CUnitData> >  m_vecRets;
};

class CUnitData
{
public:
    CUnitData();
    ~CUnitData();

    void setDisplayContent( const QString val );
    QString displayContent() const;

    void setUnique( const QString val );
    QString unique() const;

    void setDataContent( const QString val );
    QString data() const;
    QString& operator[](CDataModel::RoleNames roleName);

private:
    QHash< CDataModel::RoleNames, QString > m_data;
};

#endif // CDATAMODEL_H
