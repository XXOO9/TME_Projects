#include "CCustRTChartView.h"
#include "qcustomplot.h"
#include "commondefin.h"
#include <QDebug>

CCustRTChartView::CCustRTChartView( QQuickItem* parent ) : QQuickPaintedItem( parent )
    , m_CustomPlot( nullptr ), m_timerId( 0 )
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::AllButtons );

    connect( this, &QQuickPaintedItem::widthChanged, this, &CCustRTChartView::updateCustomPlotSize );
    connect( this, &QQuickPaintedItem::heightChanged, this, &CCustRTChartView::updateCustomPlotSize );
    //connect( m_CustomPlot->yAxis, &QCPAxis::rangeChanged, m_CustomPlot->yAxis2, &QCPAxis::setRange);
}

CCustRTChartView::~CCustRTChartView()
{
    safeDeleteClassPoint(&m_CustomPlot);

    if(m_timerId != 0) {
        killTimer(m_timerId);
    }
}

void CCustRTChartView::initCustomPlot()
{
    m_CustomPlot = new QCustomPlot();

    m_dataCount = 0;

    updateCustomPlotSize();

    //初始化
    m_CustomPlot->clearGraphs();

    //创建图形
    m_CustomPlot->addGraph();

    //初始化画笔颜色
    m_color = Qt::darkGreen;

    //图形设置数据点 笔刷
    m_CustomPlot->graph( 0 )->setPen( QPen( m_color ) );

    //设置渐变笔刷
    QLinearGradient linearGradient(0, 300, 0, 0);//初始化，设置开始和结束点
    linearGradient.setColorAt(1.0, QColor(m_color.red(), m_color.green(), m_color.blue(), 50));//设置60%-100%的区域为蓝色
    linearGradient.setColorAt(0.5, QColor(m_color.red(), m_color.green(), m_color.blue(), 30));//设置30%-60%的区域为绿色
    linearGradient.setColorAt(0.1, QColor(Qt::white));//设置从0-30%的区域为红色
    m_CustomPlot->graph(0)->setBrush(QBrush(linearGradient));

    //坐标轴相关设置
    m_CustomPlot->xAxis->setRange( 0, 5);
    m_CustomPlot->xAxis2->setVisible(true);
    m_CustomPlot->xAxis2->setTicks(false);

    m_CustomPlot->yAxis->setRange( m_yMin, m_yMax );
    m_CustomPlot->yAxis2->setRange( m_yMin, m_yMax );
    m_CustomPlot->yAxis2->setVisible(true);

    //坐标轴同步缩放
    QList<QCPAxis*> axes;
    axes << m_CustomPlot->yAxis2 << m_CustomPlot->xAxis2 << m_CustomPlot->yAxis << m_CustomPlot->xAxis;
    m_CustomPlot->axisRect()->setRangeZoomAxes(axes);

    //图表操作：拖拽、放大缩小
    m_CustomPlot ->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );

    //startTimer(1000);

    connect( m_CustomPlot, &QCustomPlot::afterReplot, this, &CCustRTChartView::onCustomReplot );

    m_CustomPlot->replot();
}

void CCustRTChartView::paint( QPainter* painter )
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

void CCustRTChartView::mousePressEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CCustRTChartView::mouseReleaseEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CCustRTChartView::mouseMoveEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CCustRTChartView::mouseDoubleClickEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CCustRTChartView::wheelEvent( QWheelEvent *event )
{
    routeWheelEvents( event );
}

void CCustRTChartView::timerEvent(QTimerEvent *event)
{
    static double t, U;
    U = ((double)rand() / RAND_MAX) * 30;
    m_CustomPlot->graph(0)->addData(t, U);
    if(t>=4)
    {
        m_CustomPlot->xAxis->setRange(t, 4, Qt::AlignRight);
    }
    t++;
    m_CustomPlot->replot();
}

int CCustRTChartView::sampleRate() const
{
    return m_sampleRate;
}

void CCustRTChartView::setSampleRate(int sampleRate)
{
    m_sampleRate = sampleRate;
    m_totalXPoints = m_totalXSec / (1/m_sampleRate);
    m_CustomPlot->replot();
}

void CCustRTChartView::graphClicked( QCPAbstractPlottable* plottable )
{
    //qDebug() << Q_FUNC_INFO << QString( "Clicked on graph '%1 " ).arg( plottable->name() );
}

void CCustRTChartView::routeMouseEvents( QMouseEvent* event )
{
    if (m_CustomPlot)
    {
        QMouseEvent* newEvent = new QMouseEvent( event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers() );
        QCoreApplication::postEvent( m_CustomPlot, newEvent );
    }
}

void CCustRTChartView::routeWheelEvents( QWheelEvent* event )
{
    if (m_CustomPlot)
    {
        QWheelEvent* newEvent = new QWheelEvent( event->pos(), event->delta(), event->buttons(), event->modifiers(), event->orientation() );
        QCoreApplication::postEvent( m_CustomPlot, newEvent );
    }
}

void CCustRTChartView::updateCustomPlotSize()
{
    if (m_CustomPlot)
    {
        m_CustomPlot->setGeometry(0, 0, int(width()), int(height()));
        m_CustomPlot->setViewport(QRect(0, 0, int(width()), int(height())));
    }
}

void CCustRTChartView::onCustomReplot()
{
    update();
}

void CCustRTChartView::recvData(const float &volume)
{
    //记录采样点
    m_dataCount++;

    //放大y轴坐标范围
    if(volume > float(m_yMax))
    {
        m_yMax = double(volume) + 1.0;
        m_CustomPlot->yAxis->setRange(m_yMin, m_yMax);
    }

    if(volume < float(m_yMin))
    {
        m_yMin = double(volume) - 1.0;
        m_CustomPlot->yAxis->setRange(m_yMin, m_yMax);
    }

    double xVal = double(m_dataCount * (1.0/m_sampleRate));

    //图表添加数据点
    m_CustomPlot->graph(0)->addData(xVal, double(volume));

    //数据点超过500，坐标轴10s时间长度后开始偏移
    if(m_dataCount>m_totalXPoints)
    {
        m_CustomPlot->xAxis->setRange(xVal, 5, Qt::AlignRight);
    }

    //图表刷新
    m_CustomPlot->replot();
}
