#ifndef CUSTVTCHARTVIEW_H
#define CUSTVTCHARTVIEW_H

#include <QtQuick>
#include <QDebug>
#include "qcustomplot.h"


class QCustomPlot;
class QCPAbstractPlottable;
class QCPItemText;

class CCustVTChartView : public QQuickPaintedItem
{
    Q_OBJECT
public:
    CCustVTChartView( QQuickItem* parent = 0 );
    virtual ~CCustVTChartView();

    void paint( QPainter* painter );

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
    QColor m_color{Qt::red};
    int          m_sampleRate{50};
    int          m_totalXSec{10};       //x轴范围：10s
    int          m_totalXPoints{500};   //x轴总点数：10s / (1/50hz) = 500
    double m_yMax{0.0};
    double m_yMin{5.0};

private slots:
    void graphClicked( QCPAbstractPlottable* plottable );
    void onCustomReplot();
    void updateCustomPlotSize();
};

#endif // CUSTVTCHARTVIEW_H
