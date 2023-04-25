#include "CInterAction.h"

CInterAction::CInterAction(QObject *parent) : QObject(parent)
{

}

void CInterAction::loadView()
{
    m_viewEngine.load();
}
