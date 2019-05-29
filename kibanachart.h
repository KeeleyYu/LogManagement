#ifndef KIBANACHART_H
#define KIBANACHART_H

#include <QtCharts/QChart>

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QAbstractSeries;
class QPieSlice;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class KibanaChart : public QChart
{
    Q_OBJECT
public:
    explicit KibanaChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    ~KibanaChart();
    void changeSeries(QAbstractSeries *series);

protected:
    bool sceneEvent(QEvent *event);

public Q_SLOTS:
    void handleSliceClicked(QPieSlice *slice);
    void handleMsgSliceClicked(QPieSlice *slice);

private:
    QAbstractSeries *m_currentSeries;
    bool gestureEvent(QGestureEvent *event);
};

#endif // KIBANACHART_H
