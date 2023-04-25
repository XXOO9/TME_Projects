#ifndef CABSTRACTRESULT_H
#define CABSTRACTRESULT_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QVector>

class CAbstractResult : public QObject
{
    Q_OBJECT
public:
    CAbstractResult(QObject *parent = nullptr);
    virtual ~CAbstractResult();

    void clear();

    void appendRet( QMap<QString, QString> retMap );

    //获取某一结果的集合
    QMap<QString, QString> getRetMapByIndex( int index );

    //获取某一结果集合的某一具体值
    QString getRet( int index, QString key );

    int size() const;


private:
    QVector< QMap<QString, QString> > m_vecRets;
};

#endif // CABSTRACTRESULT_H
