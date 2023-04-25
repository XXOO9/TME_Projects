#ifndef CCUSTRTCHARTVIEW_H
#define CCUSTRTCHARTVIEW_H


#include <QtQuick>
class QCustomPlot;
class QCPCurve;
class QCPAbstractPlottable;
class QCPScatterStyle;
class QCPItemText;

class CCustRTChartView : public QQuickPaintedItem
{
    Q_OBJECT

public:
    CCustRTChartView(QQuickItem* parent = nullptr);
    virtual ~CCustRTChartView();

    void paint(QPainter* painter);

    Q_INVOKABLE void initCustomPlot();

    Q_INVOKABLE void recvData(const float &volume);

    int sampleRate() const;
    Q_INVOKABLE void setSampleRate(int sampleRate);

protected:
    void routeMouseEvents( QMouseEvent* event );
    void routeWheelEvents( QWheelEvent* event );

    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseReleaseEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void mouseDoubleClickEvent( QMouseEvent* event );
    virtual void wheelEvent( QWheelEvent *event );

    virtual void timerEvent(QTimerEvent *event);

private:
    QCustomPlot* m_CustomPlot;
    int          m_timerId;
    long long    m_dataCount{0};
    QColor       m_color;
    int          m_sampleRate{100};
    int          m_totalXSec{5};       //x轴范围：10s
    int          m_totalXPoints{500};   //x轴总点数：10s / (1/50hz) = 500
    double m_yMax{30.0};
    double m_yMin{-30.0};

private slots:
    void graphClicked( QCPAbstractPlottable* plottable );
    void onCustomReplot();
    void updateCustomPlotSize();
};


#endif // CCUSTRTCHARTVIEW_H
