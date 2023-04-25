#ifndef CUSERINFOS_H
#define CUSERINFOS_H

#include <QAbstractListModel>
#include <QDebug>

class CUserInfoElement;
class CUserInfos;

class CUserInfoElement{
public:
    enum InfoElement{
        UserName,
        UserUniqueNumber,
        UserClassID,
        UserBindCardNumber
    };

    QString m_userName = "";
    QString m_userUniqueNumber = "";
    QString m_userClassID = "";
    QString m_userBindCardNumber = "-1";
};

class CUserInfos : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CUserInfos(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray>  roleNames() const override;

    void appendNewUsers( const QVector<CUserInfoElement> &srcUserInfos );

private:

    QVector<CUserInfoElement>   m_vecTotalUsers;
};

#endif // CUSERINFOS_H
