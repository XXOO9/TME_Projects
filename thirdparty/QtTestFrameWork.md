# Qt的测试框架, QtTest

一些参考链接:

> 1.[参考了大部分的这篇文章](https://blog.csdn.net/weixin_34302561/article/details/92651579?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522164238765016780255249655%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=164238765016780255249655&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-1-92651579.first_rank_v2_pc_rank_v29&utm_term=Qt+%E5%8D%95%E5%85%83%E6%B5%8B%E8%AF%95&spm=1018.2226.3001.4187)

## 1.基本条件：
>### a.包含头文件  <QtTest>
>
>### b.在pro文件中， 加入测试模块: QT += testlib
## 2.测试方法
>1.将要测试的类实例化出来，然后作为参数，传入QTest::qExec( &Obj, argc, argv );
>
>```C++
>#include<QtTest>
>#include<QCoreApplocation>
>#include "DemoTestClass.h"
>
>int main( int argc, char** argv )
>{
>        QCoreApplication app( argc, argv );
>        DemoTestClass  testClass;
>        QTest::qExec( &testClass, argc, argv );
>        return app.exec();
>}
>```
>
>2.在DemoTestClass中，所有申明为private slots的函数都会被认定为需要测试的函数，
>
>```C++
>signals:
>	void sigTestSignal();// 定义一个信号
>private slots:
>	void testCase1(); //像这种没有参数的函数，在执行测试的时候，会直接被调用。如果绑定了信号槽，并且发出了信号,则会被执行两次
>	void testCase2( bool ok );// 这种有参数的函数，如果没有绑定信号槽，则不会被调用
>```
>
>|      | 有参数 | 无参数 |
>| :----: | :------: | :------: |
>|   绑定了信号   |   执行一次     |   执行两次     |
>|   没有绑定信号   |   不执行     |   执行一次     |
>
>

## 3.添加测试数据
>如果数据量比较大，不方便在测试函数中添加，则可以在另一个函数中统一添加测试数据
>
>例如:
>
>```c++
>private slots:
>	void testQString_data();
>
>	void testQstring();
>```
>
>在CPP中的形式为:
>
>```c++
>void XXX::testQString_data()
>{
>        QTest::addColumn<QString>( "string" );
>        QTest::addColumn<QString>( "result" );
>
>        QTest::newRow( "firstRow" ) << "hello" << "HELLO";
>        QTest::newRow( "secondRow" ) << "Hello" << "HELLO";
>        QTest::newRow( "thirdRow" ) << "HELLO" << "HELLO";
>}
>
>void XXX::testQstring()
>{
>        QFETCH( QString, string );
>        QFETCH( QString, result ); // 每执行一次QFETCH就会从testQString_data中存入的数据中取一次
>}
>```
>
>## 4.一些常用到的宏
>```c++
>QVERIFY( bool ok ); //测试布尔值
>QCOMPARE( type arg1, type arg2 ); //测试 arg1 和 arg2 是否相等
>QBENCHMARK{ ... }; //测试函数的执行性能
>```
