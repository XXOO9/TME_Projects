#ifndef CDATABASEMANAGER_H
#define CDATABASEMANAGER_H

#include <QObject>
#include "CStudentInfoDataBase.h"

class CDataBaseManager : public QObject
{
    Q_OBJECT
public:
    explicit CDataBaseManager(QObject *parent = nullptr);

    void openStuDataBase();

    CStudentInfoDataBase *pStuDataBase() const;

signals:

public slots:

private:
    CStudentInfoDataBase    *m_pStuDataBase;
};

#endif // CDATABASEMANAGER_H
