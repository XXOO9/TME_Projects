#ifndef CSTULISTMODEL_H
#define CSTULISTMODEL_H

#include <QObject>
#include <QDebug>

class CStuListModel : public QObject
{
    Q_OBJECT
public:
    explicit CStuListModel(QObject *parent = nullptr);

    Q_INVOKABLE int size() const;

    Q_INVOKABLE QString data( int index );

    void append( QString data );

signals:

public slots:

private:
    QStringList     m_stuList;
};

#endif // CSTULISTMODEL_H
