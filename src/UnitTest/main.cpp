#include <QtTest>
#include <QCoreApplication>
#include <QApplication>
#include "CHttpTest.h"

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    CHttpTest test;
    int ret = QTest::qExec( &test, argc, argv );
    return ret;
}
