#include "CStuListModel.h"

CStuListModel::CStuListModel(QObject *parent) : QObject(parent)
{

}

int CStuListModel::size() const
{
    return m_stuList.size();
}

QString CStuListModel::data(int index)
{
    return m_stuList.at(index);
}

void CStuListModel::append(QString data)
{
    m_stuList.append(data);
}
