#ifndef CUSERFILTERMODEL_H
#define CUSERFILTERMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include "CUserInfos.h"

class CUserFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    CUserFilterModel();
    virtual ~CUserFilterModel() override;

    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    QString filterPattern() const;
    void setFilterPattern(const QString &filterPattern);

private:
    QString m_filterPattern = "";
};

#endif // CUSERFILTERMODEL_H
