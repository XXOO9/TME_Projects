#include "CCustFVChartView.h"
#include "qcustomplot.h"
#include "commondefin.h"
#include <QDebug>

CCustFVChartView::CCustFVChartView( QQuickItem* parent ) : QQuickPaintedItem( parent )
    , m_CustomPlot( nullptr ),m_fvExCurve(nullptr), m_fvInCurve(nullptr), m_timerId( 0 )
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::AllButtons );

    connect( this, &QQuickPaintedItem::widthChanged, this, &CCustFVChartView::updateCustomPlotSize );
    connect( this, &QQuickPaintedItem::heightChanged, this, &CCustFVChartView::updateCustomPlotSize );
}

CCustFVChartView::~CCustFVChartView()
{
    //safeDeleteClassPoint(&m_fvExCurve);  //delete m_CustomPlot会同时释放m_fvExCurve
    safeDeleteClassPoint(&m_CustomPlot);

    if(m_timerId != 0) {
        killTimer(m_timerId);
    }
}

void CCustFVChartView::initCustomPlot()
{
    m_CustomPlot = new QCustomPlot();

    m_dataCount = 0;

    updateCustomPlotSize();

    //初始化
    m_CustomPlot->clearGraphs();

    //创建图形
    m_CustomPlot->addGraph();

    //初始化画笔颜色
    m_color = Qt::blue;

    //图形设置数据点 笔刷
    m_CustomPlot->graph( 0 )->setPen( QPen( m_color ) );

    //y轴相关设置：子刻度、范围、末端箭头
    m_CustomPlot->yAxis->ticker()->setTickCount(6);
    m_CustomPlot->yAxis->setRange( -4, 8 );
    m_CustomPlot->yAxis->setSubTicks(false);
    m_CustomPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //x轴相关设置
    m_CustomPlot->xAxis->ticker()->setTickCount(3);
    m_CustomPlot->xAxis->setRange( 0, 6 );
    m_CustomPlot->xAxis->setSubTicks(false);
    m_CustomPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);//x轴终点箭头图案
    m_CustomPlot->xAxis->grid()->setPen(QPen(QColor(180, 180, 180,0))); //网格白色虚线-隐藏
    //x轴偏移至y轴0点
    //m_CustomPlot->xAxis->setPadding(-int(this->height() * 0.295));//填充50的空间
    m_CustomPlot->xAxis->setPadding(-int(this->height() * 0.295));//填充50的空间
    m_CustomPlot->xAxis->setOffset(-int(this->height() * 0.295));//偏移50


    //图表操作：拖拽、放大缩小
    //m_CustomPlot ->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );

    //闭合曲线
    m_fvExCurve = new QCPCurve(m_CustomPlot->xAxis, m_CustomPlot->yAxis);
    m_fvInCurve = new QCPCurve(m_CustomPlot->xAxis, m_CustomPlot->yAxis);

    //曲线笔刷渐变
    QLinearGradient linearGradient(0, 100, 0, -50); //初始化，设置开始和结束点
    linearGradient.setColorAt(1.0, QColor(m_color.red(), m_color.green(), m_color.blue(), 80));
    linearGradient.setColorAt(0.6, QColor(m_color.red(), m_color.green(), m_color.blue(), 50));
    linearGradient.setColorAt(0.25, QColor(m_color.red(), m_color.green(), m_color.blue(), 30));
    linearGradient.setColorAt(0.05, QColor(Qt::white));
    linearGradient.setSpread(QGradient::ReflectSpread);
    m_fvExCurve->setBrush(QBrush(linearGradient));
    m_fvExCurve->setLineStyle(QCPCurve::lsLine);

    QColor inCureveColor("#CF0000");
    QLinearGradient inlinearGradient(0, 100, 0, -50); //初始化，设置开始和结束点
    linearGradient.setColorAt(1.0, QColor(inCureveColor.red(), inCureveColor.green(), inCureveColor.blue(), 80));
    linearGradient.setColorAt(0.6, QColor(inCureveColor.red(), inCureveColor.green(), inCureveColor.blue(), 50));
    linearGradient.setColorAt(0.25, QColor(inCureveColor.red(), inCureveColor.green(), inCureveColor.blue(), 30));
    linearGradient.setColorAt(0.05, QColor(Qt::white));
    linearGradient.setSpread(QGradient::ReflectSpread);
    m_fvInCurve->setBrush(QBrush(linearGradient));
    m_fvInCurve->setLineStyle(QCPCurve::lsLine);

//    int off_x = 10;
//    QPainterPath _path;
//    _path.moveTo(5-off_x, -5);
//    _path.lineTo(5,0);
//    _path.lineTo(5-off_x,5);

    //for (int i=0; i<3; ++i)
         //customScatterPath.cubicTo(qCos(2*M_PI*i/3.0)*9, qSin(2*M_PI*i/3.0)*9, qCos(2*M_PI*(i+0.9)/3.0)*9, qSin(2*M_PI*(i+0.9)/3.0)*9, 0, 0);
    //m_fvExCurve->setScatterStyle(QCPScatterStyle(_path, QPen(Qt::black, 0)));

    //数据点标记样式
    m_fvExCurve->setScatterStyle(QCPScatterStyle::ssTriangle);
    //m_fvExCurve->setScatterSkip(2);

    //startTimer(1000);

    connect( m_CustomPlot, &QCustomPlot::afterReplot, this, &CCustFVChartView::onCustomReplot );

    m_CustomPlot->replot();
}


void CCustFVChartView::paint( QPainter* painter )
{
    if (m_CustomPlot)
    {
        QPixmap    picture( boundingRect().size().toSize() );
        QCPPainter qcpPainter( &picture );

        m_CustomPlot->toPainter( &qcpPainter );
        setRenderTarget(QQuickPaintedItem::FramebufferObject);
        painter->drawPixmap( QPoint(), picture );
    }
}

void CCustFVChartView::mousePressEvent( QMouseEvent* event )
{
    //qDebug() << Q_FUNC_INFO;
    routeMouseEvents( event );
}

void CCustFVChartView::mouseReleaseEvent( QMouseEvent* event )
{
    //qDebug() << Q_FUNC_INFO;
    routeMouseEvents( event );
}

void CCustFVChartView::mouseMoveEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CCustFVChartView::mouseDoubleClickEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CCustFVChartView::wheelEvent( QWheelEvent *event )
{
    routeWheelEvents( event );
}

void CCustFVChartView::timerEvent(QTimerEvent *event)
{
    static double t, U;  //创建实例对象

    U = (double(rand()) / RAND_MAX) * 8;

    if(t<=4)
    {
        if(int(t)==4)
        {
            U = 0;
        }
        m_fvExCurve->data()->add(QCPCurveData(t, t, U));
        qDebug()  << QString("Adding dot t = %1, S = %2").arg(t).arg(U);
    }
    else if(t>4 && t<=8)
    {
        if(int(t)==5)
        {
            m_fvExCurve->data()->add(QCPCurveData(t, 3, -U));
        }
        else if(int(t)==6)
        {
            m_fvExCurve->data()->add(QCPCurveData(t, 2, -U));
        }
        else if(int(t)==7)
        {
            m_fvExCurve->data()->add(QCPCurveData(t, 1, -U));
        }
        else
        {
            m_fvExCurve->data()->add(QCPCurveData(t, 0, -U));
        }
        qDebug()  << QString("Adding dot t = %1, S = %2").arg(t).arg(-U);
    }
    else
    {
        return;
    }

    qDebug()<<"m_CustomPlot->graph()->dataCount()"<<m_fvExCurve->dataCount();

    t++;
    m_CustomPlot->replot();
}

void CCustFVChartView::graphClicked( QCPAbstractPlottable* plottable )
{
    qDebug() << Q_FUNC_INFO << QString( "Clicked on graph '%1 " ).arg( plottable->name() );
}

void CCustFVChartView::routeMouseEvents( QMouseEvent* event )
{
    if (m_CustomPlot)
    {
        QMouseEvent* newEvent = new QMouseEvent( event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers() );
        QCoreApplication::postEvent( m_CustomPlot, newEvent );
    }
}

void CCustFVChartView::routeWheelEvents( QWheelEvent* event )
{
    if (m_CustomPlot)
    {
        QWheelEvent* newEvent = new QWheelEvent( event->pos(), event->delta(), event->buttons(), event->modifiers(), event->orientation() );
        QCoreApplication::postEvent( m_CustomPlot, newEvent );
    }
}

void CCustFVChartView::updateCustomPlotSize()
{
    if (m_CustomPlot)
    {
        m_CustomPlot->setGeometry(0, 0, int(width()), int(height()));
        m_CustomPlot->setViewport(QRect(0, 0, int(width()), int(height())));
    }
}

void CCustFVChartView::onCustomReplot()
{
    //qDebug() << Q_FUNC_INFO;
    update();
}

void CCustFVChartView::recvData(const int &stageType, const float &volume, const float &flow)
{
    //记录采样点
    m_dataCount++;

    if(m_dataCount==1)
    {
        emit sigStartExpiration();
    }
    else
    {
        if(flow<=0)
        {
            emit sigStartInspiration();
        }
    }

    if(stageType == CProjectEnums::Expiration)
    {
        //图表添加数据点
        m_fvExCurve->data()->add(QCPCurveData(m_dataCount, double(volume), double(flow)));
    }
    else
    {
        //图表添加数据点
        m_fvInCurve->data()->add(QCPCurveData(m_dataCount, double(volume), double(flow)));
    }

    //图表刷新
    m_CustomPlot->replot();
}
