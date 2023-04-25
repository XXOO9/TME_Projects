#include "MyPainterWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyPainterWidget w(0);
//    w.show();

    return a.exec();
}
