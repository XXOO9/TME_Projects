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
};

#endif // CUSERFILTERMODEL_H
