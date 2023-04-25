#include <QString>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include "MyPainterWidget.h"

const QString changeDialog = "changeDialog";
const QString showWindow = "showWindow";
const QString saveToPng = "saveToPng";


MyPainterWidget::MyPainterWidget(QWidget* parent)
    :QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    this->setWindowFlags(Qt::FramelessWindowHint); //设置无边框窗口
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setMouseTracking(true);
    this->isPressed = false;

    m_pFileWriter = new QFile( "./painter.txt", this );
    m_pFileWriter->open( QIODevice::ReadWrite | QIODevice::Append );
    writeLog("\n即将写入日志\n");
    initLcoalSocket();
}

MyPainterWidget::~MyPainterWidget()
{
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        if(nullptr != *it)
        {
            delete *it;
            *it = nullptr;
        }
    }

    if( m_pFileWriter != nullptr && m_pFileWriter->isOpen() )
    {
        writeLog("\n文件即将关闭\n");
        m_pFileWriter->close();
    }
}


void MyPainterWidget::paintEvent( QPaintEvent*p )
{
    //0.建立画布
    QPainter painter(this);

    //1.设置白色底与圆角
    painter.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::transparent);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 15, 15);
    QPen pen;

    //2.在没有绘画时，显示 签字版
    if(lines.size() <= 0)
    {
        pen.setColor(qRgb(229,229,229));
        QFont font;
        font.setPixelSize( 50 );
        painter.setPen(pen);
        painter.setFont(font);

        QString text = QString::fromLocal8Bit( "签字区" );
        int width  = painter.fontMetrics().width(text);
        int height = painter.fontMetrics().height();

        painter.drawText( (this->width() - width) / 2, (this->height() - height) / 2, text);
    }
    //3.绘制签名
    else
    {
        pen.setColor(Qt::black);
        pen.setWidth(5);
        painter.setPen(pen);

        for(auto &line : lines)
        {
            if(line != nullptr)
            {
                painter.drawLine(line->startPnt,line->endPnt);
            }
        }
    }

    if(p == nullptr)
        return;
}
void MyPainterWidget::mousePressEvent(QMouseEvent *e)
{
    setCursor(Qt::PointingHandCursor);

    if(e != nullptr)
    {
        startPnt = e->pos();
        endPnt = e->pos();
    }

    isPressed = true;
}
void MyPainterWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(isPressed)
    {
        endPnt = e->pos();
        myLine* line = new myLine;
        line->startPnt = startPnt;
        line->endPnt = endPnt;
        lines.push_back(line);
        update();
        startPnt = endPnt;
    }
}
void MyPainterWidget::mouseReleaseEvent(QMouseEvent *e)
{
    setCursor( Qt::ArrowCursor );
    this->isPressed = false;

    if(e == nullptr)
        return;
}

void MyPainterWidget::savePic(QString qsPath)
{
    if(lines.size() <= 0 || m_pLocalSocket == nullptr)
    {
        return ;
    }

    QPixmap pixmap = QWidget::grab();
    bool ok = pixmap.save(qsPath,"PNG");
    QString logInfo;
    logInfo = nullptr == m_pLocalSocket ? "socket is not available..." : "socket is  available...";
    writeLog(logInfo);

    if( !ok ){
        logInfo = QString( "\nPainter sava failed!!!\n" );
        m_pLocalSocket->write( "saveRet+failed" );
    }else{
        logInfo = QString( "\nPainter sava success...\n" );
        m_pLocalSocket->write( "saveRet+success" );
    }

    writeLog(logInfo.toLocal8Bit());
}

void MyPainterWidget::clearDraw()
{
    //释放内存
    for(auto it = lines.begin(); it != lines.end(); it++)
    {
        if(nullptr != *it)
        {
            delete *it;
            *it = nullptr;
        }
    }

    //清除lines
    lines.clear();

    update();
}

void MyPainterWidget::moveDialogPos(int iXpos, int iYpos, int iWidth, int iHeight)
{
    QString infos = QString( "x:%1, y:%2, wid:%3, hei:%4" ).arg( QString::number( iXpos ) )
            .arg( QString::number( iYpos ) )
            .arg( QString::number( iWidth ) )
            .arg( QString::number( iHeight ) );


    writeLog( infos );
    this->resize( iWidth, iHeight );

    this->move( iXpos, iYpos );

    update();
}

void MyPainterWidget::initLcoalSocket()
{
    m_pLocalSocket = new QLocalSocket(this);
    connect( m_pLocalSocket, &QLocalSocket::connected, this, &MyPainterWidget::onSocketConnected, Qt::QueuedConnection );
    connect( m_pLocalSocket, &QLocalSocket::readyRead, this, &MyPainterWidget::onSocketReadyRead, Qt::UniqueConnection );
    connect( m_pLocalSocket, &QLocalSocket::disconnected, this, &MyPainterWidget::onSocketDisConnected, Qt::QueuedConnection );

    m_pLocalSocket->connectToServer( "SignaturePainter" );
}

QVector<QString> MyPainterWidget::parseData(QString &qsData)
{
    QVector<QString> vecList;
    //字符粗略是以空格进行解析
    QString cmd1 = "changeDialog";
    QString cmd2 = "showWindow";
    QString cmd3 = "saveToPng";
    QString cmd4 = "clearPainter";

    map<int, QString> mapMax;
    int index1, index2, index3, indexTemp;

    while (qsData.length() > 0) {
        mapMax.clear();

        index1 = qsData.lastIndexOf(cmd1);
        mapMax[index1] = cmd1;
        index2 = qsData.lastIndexOf(cmd2);
        mapMax[index2] = cmd2;
        index3 = qsData.lastIndexOf(cmd3);
        mapMax[index3] = cmd3;
        index3 = qsData.lastIndexOf( cmd4 );
        mapMax[index3] = cmd4;

        if(mapMax.size() > 0)
        {
            indexTemp = mapMax.rbegin()->first;
            if(indexTemp < 0)
            {
                qsData.clear();
            }

            //将找到的命令放入容器
            vecList.prepend(qsData.mid(indexTemp));

            qsData = qsData.left(indexTemp);

            if(indexTemp == 0)
                qsData.clear();
        }
        else
        {
            qsData.clear();
        }
    }

    return vecList;
}


void MyPainterWidget::onSocketReadyRead()
{
    if(m_pLocalSocket == nullptr)
        return;

    QString data = QString::fromLocal8Bit( m_pLocalSocket->readAll() );
    QString a = QString( "\ncmd is = -> " ) + data + QString( "\n" );
    QStringList list;

    writeLog(a.toLocal8Bit());

    QVector<QString> vec = parseData( data );
    for( auto &eachEle : vec ){
        QString newCmd = "new cmd = " + eachEle + "\n";
        writeLog(newCmd.toLocal8Bit());

        if( eachEle.startsWith( "showWindow" ) )
        {
            list = eachEle.split( ' ', QString::SkipEmptyParts );
            if( list.size() != 2 )
            {
                return ;
            }
            if( list.at( 1 ).toInt() == 1 )
            {
                this->clearDraw();

                this->show();
            }
            else
            {
                this->clearDraw();

                this->hide();
            }
        }
        else if( eachEle == "clearPainter" )
        {
            this->clearDraw();
        }
        else if( eachEle.startsWith("saveToPng") )
        {
            list = eachEle.split( ' ', QString::SkipEmptyParts );
            if( list.size() != 2 )
            {
                return ;
            }
            this->savePic(list.at(1));
        }
        else if( eachEle.startsWith("changeDialog"))
        {
            list = eachEle.split( ' ', QString::SkipEmptyParts );
            if( list.size() != 5 )
            {
                return ;
            }

            this->moveDialogPos(list.at(3).toInt(), list.at(4).toInt(), list.at(1).toInt(), list.at(2).toInt());
        }
    }
}

void MyPainterWidget::onSocketConnected()
{
    writeLog( "socket is connected..." );
}

void MyPainterWidget::onSocketDisConnected()
{
    writeLog( "socket dis connected, sign painter going to close..." );
    exit( 0 );
}

void MyPainterWidget::writeLog(QString content)
{
    QString logContent = "\n" + content + "\n";
    if(m_pFileWriter != nullptr)
    {
        m_pFileWriter->write( logContent.toLocal8Bit() );
        m_pFileWriter->flush();
    }
}

void MyPainterWidget::initCmdList()
{
    m_cmdList = CSignPainterCmd::controlCmdList();
}
