#ifndef CHTTPCONFIGREADERSINGETON_H
#define CHTTPCONFIGREADERSINGETON_H

#include "../common_global.h"
#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QFileInfo>
#include <QMutex>
#include "CHttpConfigCommonDefine.h"

class COMMONSHARED_EXPORT CHttpConfigReaderSingeton : public QObject
{
    Q_OBJECT

public:
    QString getValue(QString key);

private:
    explicit CHttpConfigReaderSingeton(QObject *parent = nullptr);
    void createDefaultConfig();


public:
    static CHttpConfigReaderSingeton* getInstance();

private:
    QSettings                          *m_pSetting;
    static CHttpConfigReaderSingeton   *m_pSingleton;
    static QMutex                       m_locker;
};

#endif // CHTTPCONFIGREADERSINGETON_H
