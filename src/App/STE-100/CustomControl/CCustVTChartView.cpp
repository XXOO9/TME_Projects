#include "CCustVTChartView.h"
#include "commondefin.h"

CCustVTChartView::CCustVTChartView( QQuickItem* parent ) : QQuickPaintedItem( parent )
    , m_CustomPlot( nullptr ), m_timerId( 0 )
{
    setFlag( QQuickItem::ItemHasContents, true );
    setAcceptedMouseButtons( Qt::AllButtons );

    connect( this, &QQuickPaintedItem::widthChanged, this, &CCustVTChartView::updateCustomPlotSize );
    connect( this, &QQuickPaintedItem::heightChanged, this, &CCustVTChartView::updateCustomPlotSize );
}

CCustVTChartView::~CCustVTChartView()
{
    safeDeleteClassPoint(&m_CustomPlot);

    if(m_timerId != 0) {
        killTimer(m_timerId);
    }
}

void CCustVTChartView::initCustomPlot()
{
    //qDebug()<<"CCustVTChartView init m_dataCount"<<m_dataCount;
    m_CustomPlot = new QCustomPlot();

    m_dataCount = 0;

    updateCustomPlotSize();

    //初始化
    m_CustomPlot->clearGraphs();

    //创建图形
    m_CustomPlot->addGraph();

    //初始化画笔颜色
    m_color = QColor("#CF0000");

    //图形设置数据点 笔刷
    m_CustomPlot->graph( 0 )->setPen( QPen( m_color ) );

    //设置渐变笔刷
    QLinearGradient linearGradient(0, m_CustomPlot->height(), 0, m_CustomPlot->height()-300);//初始化，设置开始和结束点
    linearGradient.setColorAt(1.0, QColor(m_color.red(), m_color.green(), m_color.blue(), 80));
    linearGradient.setColorAt(0.6, QColor(m_color.red(), m_color.green(), m_color.blue(), 50));
    linearGradient.setColorAt(0.25, QColor(m_color.red(), m_color.green(), m_color.blue(), 30));
    linearGradient.setColorAt(0.1, QColor(Qt::white));//设置从0-30%的区域为红色
    m_CustomPlot->graph(0)->setBrush(QBrush(linearGradient));

    //x轴相关设置：边距、刻度显示、范围、坐标终点箭头样式、白色网格线
    m_CustomPlot->xAxis->setPadding(30);
    m_CustomPlot->xAxis->setTicks(false);
    m_CustomPlot->xAxis->setRange( 0, m_totalXPoints );    //500 * 20 = 10s 采样率50hz，每20ms一个采样点
    m_CustomPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    m_CustomPlot->xAxis->grid()->setPen(QPen(QColor(180, 180, 180,0)));

    //y轴相关设置
    m_CustomPlot->yAxis->setTicks(false);
    m_CustomPlot->yAxis->setRange( m_yMin, m_yMax );
    m_CustomPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //图表操作：拖拽、放大缩小
    m_CustomPlot ->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom );

    //startTimer(100);

    connect( m_CustomPlot, &QCustomPlot::afterReplot, this, &CCustVTChartView::onCustomReplot );

    m_CustomPlot->replot();
}

void CCustVTChartView::paint( QPainter* painter )
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

void CCustVTChartView::mousePressEvent( QMouseEvent* event )
{
    //qDebug() << Q_FUNC_INFO;
    routeMouseEvents( event );
}

void CCustVTChartView::mouseReleaseEvent( QMouseEvent* event )
{
    //qDebug() << Q_FUNC_INFO;
    routeMouseEvents( event );
}

void CCustVTChartView::mouseMoveEvent( QMouseEvent* event )
{
    routeMouseEvents( event );
}

void CCustVTChartView::mouseDoubleClickEvent( QMouseEvent* event )
{
    //qDebug() << Q_FUNC_INFO;
    routeMouseEvents( event );
}

void CCustVTChartView::wheelEvent( QWheelEvent *event )
{
    routeWheelEvents( event );
}

void CCustVTChartView::timerEvent(QTimerEvent *event)
{
    static double U;
    U = ((double)rand() / RAND_MAX) * 5;
    m_CustomPlot->graph(0)->addData(m_dataCount, U);
    if(m_dataCount>=100)
    {
        m_CustomPlot->xAxis->setRange(m_dataCount, 100, Qt::AlignRight);
    }
    //qDebug() << Q_FUNC_INFO << QString("Adding dot t = %1, S = %2").arg(m_dataCount).arg(U);
    m_dataCount++;
    m_CustomPlot->replot();
}

int CCustVTChartView::sampleRate() const
{
    return m_sampleRate;
}

void CCustVTChartView::setSampleRate(int sampleRate)
{
    m_sampleRate = sampleRate;
    m_totalXPoints = m_totalXSec / (1.0/m_sampleRate);
    m_CustomPlot->replot();
}

void CCustVTChartView::graphClicked( QCPAbstractPlottable* plottable )
{
    qDebug() << Q_FUNC_INFO << QString( "Clicked on graph '%1 " ).arg( plottable->name() );
}

void CCustVTChartView::routeMouseEvents( QMouseEvent* event )
{
    if (m_CustomPlot)
    {
        QMouseEvent* newEvent = new QMouseEvent( event->type(), event->localPos(), event->button(), event->buttons(), event->modifiers() );
        QCoreApplication::postEvent( m_CustomPlot, newEvent );
    }
}

void CCustVTChartView::routeWheelEvents( QWheelEvent* event )
{
    if (m_CustomPlot)
    {
        QWheelEvent* newEvent = new QWheelEvent( event->pos(), event->delta(), event->buttons(), event->modifiers(), event->orientation() );
        QCoreApplication::postEvent( m_CustomPlot, newEvent );
    }
}

void CCustVTChartView::updateCustomPlotSize()
{
    if (m_CustomPlot)
    {
        m_CustomPlot->setGeometry(0, 0, int(width()), int(height()));
        m_CustomPlot->setViewport(QRect(0, 0, int(width()), int(height())));
    }
}

void CCustVTChartView::onCustomReplot()
{
    //qDebug() << Q_FUNC_INFO;
    update();
}

void CCustVTChartView::recvData(const float &volume)
{
    //记录采样点
    m_dataCount++;

    //放大y轴坐标范围
    if(volume > float(m_yMax))
    {
        m_yMax = double(volume) + 1.0;
        m_CustomPlot->yAxis->setRange(m_yMin, m_yMax);
    }

    //图表添加数据点
    m_CustomPlot->graph(0)->addData(m_dataCount, double(volume));

    //数据点超过100，坐标轴10s时间长度后开始偏移
    if(m_dataCount>m_totalXPoints)
    {
        m_CustomPlot->xAxis->setRange(m_dataCount, m_totalXPoints, Qt::AlignRight);
    }

    //图表刷新
    m_CustomPlot->replot();
}
