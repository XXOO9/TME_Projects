#ifndef MYPAINTERWIDGET_H
#define MYPAINTERWIDGET_H
#include <QWidget>
#include <QPoint>
#include <vector>
#include <map>
#include <QDebug>
#include <QLabel>
#include <QProcess>
#include <QLocalServer>
#include <QLocalSocket>
#include <QFile>
#include <QStringList>
#include "../Common/CSignPainterCommandDefin.h"


using namespace std;
//线段
typedef struct myLine{
    QPoint startPnt;
    QPoint endPnt;
}myLine;


class MyPainterWidget: public QWidget
{
    Q_OBJECT
public:
    MyPainterWidget(QWidget* parent);
    ~MyPainterWidget();
    //继承
    void paintEvent(QPaintEvent* p);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    //功能：保存图片
    void savePic(QString qsPath);

    //功能：清除绘制
    void clearDraw();

    //功能：移动位置
    void moveDialogPos(int iXpos, int iYpos, int iWidth, int iHeight );

    //初始化服务
    void initLcoalSocket();

    QVector<QString> parseData(QString &qsData);

public slots:
    void onSocketReadyRead();
    void onSocketConnected();
    void onSocketDisConnected();

private:
    void writeLog( QString content );
    void initCmdList();

private:
    QPoint startPnt;   //起点
    QPoint endPnt;     //终点
    bool isPressed;    //鼠标是否按下
    vector<myLine*> lines; //存放所有的线段
    QLocalSocket  *m_pLocalSocket{nullptr};
    QFile          *m_pFileWriter{nullptr};
    QStringList    m_cmdList;
};
#endif // MYPAINTERWIDGET_H
