#ifndef CINTERACTION_H
#define CINTERACTION_H

#include <QObject>
#include <QDebug>
#include "CUserInfos.h"
#include "CUserFilterModel.h"
#include "CNetWorkManager.h"
#include <QQmlApplicationEngine>

class CInterAction : public QObject
{
    Q_OBJECT
public:
    explicit CInterAction(QObject *parent = nullptr);

private:
    void loadView();

private:
    CNetWorkManager         m_networkMgr;
    CUserInfos              m_totalUserInfosModel;
    CUserFilterModel        m_userFilterModel;
    QQmlApplicationEngine   m_viewEngine;

    const QUrl              m_viewFile = "main.qml";
};

#endif // CINTERACTION_H
