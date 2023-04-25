#ifndef CCLASSLISTMODEL_H
#define CCLASSLISTMODEL_H

#include <QAbstractListModel>
#include <QDebug>

class CClassInfoItem;
class CClassListModel;

class CClassListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CClassListModel(QObject *parent = nullptr);

    // Basic functionality:
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QVariant data( int rowIndex );

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool appendRows( CClassInfoItem newClassInfo );


    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QVector< CClassInfoItem >   m_vecClassList;
};

class CClassInfoItem
{
public:
    CClassInfoItem();
    ~CClassInfoItem();

    QString classId() const;
    void setClassId(const QString &classId);

    QString className() const;
    void setClassName(const QString &className);

private:
    QString m_classId;
    QString m_className;
};

#endif // CCLASSLISTMODEL_H
