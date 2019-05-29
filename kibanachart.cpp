#include "kibanachart.h"
#include "kibanaslice.h"
#include <QtWidgets/QGesture>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>

QT_BEGIN_NAMESPACE
class KibanaSlice;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

KibanaChart::KibanaChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(QChart::ChartTypeCartesian, parent, wFlags),
      m_currentSeries(0)
{

}

KibanaChart::~KibanaChart()
{

}

void KibanaChart::changeSeries(QAbstractSeries *series)
{
    // NOTE: if the series is owned by the chart it will be deleted
    // here the "window" owns the series...
    if (m_currentSeries)
        removeSeries(m_currentSeries);
    m_currentSeries = series;
    addSeries(series);
    setTitle(series->name());
}

void KibanaChart::handleSliceClicked(QPieSlice *slice)
{
    KibanaSlice *kibanaSlice = static_cast<KibanaSlice *>(slice);
    changeSeries(kibanaSlice->drilldownSeries());
}

void KibanaChart::handleMsgSliceClicked(QPieSlice *slice) {

}

bool KibanaChart::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent *>(event));
    return QChart::event(event);
}

bool KibanaChart::gestureEvent(QGestureEvent *event)
{
    if (QGesture *gesture = event->gesture(Qt::PanGesture)) {
        QPanGesture *pan = static_cast<QPanGesture *>(gesture);
        QChart::scroll(-(pan->delta().x()), pan->delta().y());
    }

    if (QGesture *gesture = event->gesture(Qt::PinchGesture)) {
        QPinchGesture *pinch = static_cast<QPinchGesture *>(gesture);
        if (pinch->changeFlags() & QPinchGesture::ScaleFactorChanged)
            QChart::zoom(pinch->scaleFactor());
    }

    return true;
}
