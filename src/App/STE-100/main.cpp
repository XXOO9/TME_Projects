#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QApplication>

#include "CMainFrame.h"

#include <Windows.h>
#include <DbgHelp.h>
#include <QMessageBox>
#include <QSharedMemory>

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"

#pragma comment( lib, "Dbghelp.lib" )

// 初始化日志
void initLogFirst();

//设置软件是否自启动
void setAutoStart(bool is_auto_start)
{
    //NativeFormat在windows下就是系统注册表
    QSettings nsettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);

    QString napppath = QApplication::applicationFilePath();
    QString nappname = QApplication::applicationName();
    napppath = napppath.replace("/","\\");
    if(is_auto_start) {
        nsettings.setValue(nappname,napppath);
    }
    else {
        nsettings.remove(nappname);
    }
}

// 创建 Dump 文件
static LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException) {
    WCHAR buf[255] = { 0 };
    QString path = QDateTime::currentDateTime().toString("ste-100-yyyy年MM月dd日 HH时mm分ss秒zzz") + QString(".dmp"); path.toWCharArray(buf);
    HANDLE hDumpFile = CreateFile(buf, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hDumpFile != INVALID_HANDLE_VALUE) {
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, nullptr, nullptr);
    }

    return 0;
}

int main(int argc, char *argv[])
{
//    //避免程序多次启动
//    QSharedMemory shared("APP");//这个名字无所谓
//    if(shared.attach())
//    {
//        QMessageBox box;
//        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
//        box.setText(QStringLiteral("程序已启动!"));
//        if(box.exec() == QMessageBox::Yes){

//        }else{
//            exit(0);
//        }
//    }
//    shared.create(1);




    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts );
    //QApplication::setAttribute( Qt::AA_DisableShaderDiskCache );
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER(ApplicationCrashHandler));
    //qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    //qputenv("QT_VIRTUALKEYBOARD_STYLE", "keyBoardStyle");	//对应主题名，主题名也是随便写

    QApplication app(argc, argv);

    setAutoStart(true);

    initLogFirst();

    const QUrl url(QStringLiteral("qrc:/qmlFiles/main.qml"));
    CMainFrame mainFrame;
    QObject::connect(&mainFrame.m_objQmlEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    mainFrame.m_objQmlEngine.load(url);

    //QObject::connect(&mainFrame, &CMainFrame::sigQuite, &app, &QApplication::quit, Qt::QueuedConnection);

    return app.exec();
}

void initLogFirst()
{
    CLogger &Log = instLog;

    {
        //设置log文件夹位置
        string strDir = QApplication::applicationDirPath().toStdString() + "/" + "log/";
        if (!strDir.empty())
            Log.set_Log_Path(strDir);

        //设置log文件名
        string strFileName = QCoreApplication::applicationName().toStdString();
        if (!strFileName.empty())
            Log.set_Log_File("STE100");

        //设置是否分级存储
        Log.set_Grade_Separation_Flag(false);

        //设置存储的天数
        unsigned int nSaveNumOfDay = 5;
        if (nSaveNumOfDay > 0)
            Log.set_Max_Log_Count(nSaveNumOfDay);

        //设置最低日志信息存储等级
        Log.set_Min_Output_Level(CLogger::LOG_INFO);
    }

    Log.initialize();
}
