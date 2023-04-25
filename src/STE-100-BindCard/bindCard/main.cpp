#include <QGuiApplication>
#include "CMainFrame.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    CMainFrame frame;
    return app.exec();
}
