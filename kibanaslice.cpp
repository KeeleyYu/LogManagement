#include "kibanaslice.h"

QT_CHARTS_USE_NAMESPACE

KibanaSlice::KibanaSlice(qreal value, QString prefix, QAbstractSeries *drilldownSeries)
    : m_drilldownSeries(drilldownSeries),
      m_label(prefix)
{
    setValue(value);
    updateLabel();
    setLabelFont(QFont("Arial", 8));
    connect(this, &KibanaSlice::percentageChanged, this, &KibanaSlice::updateLabel);
    connect(this, &KibanaSlice::hovered, this, &KibanaSlice::showHighlight);
}

KibanaSlice::~KibanaSlice()
{

}

QAbstractSeries *KibanaSlice::drilldownSeries() const
{
    return m_drilldownSeries;
}

void KibanaSlice::updateLabel()
{
    QString label = m_label;
    m_showLabel = QString::number(this->percentage() * 100, 'f', 1);
    m_showLabel += "%, ";
    m_showLabel += label;
    setLabel(m_showLabel);
}

void KibanaSlice::showHighlight(bool show)
{
    setLabelVisible(show);
    setExploded(show);
}

QString KibanaSlice::sliceLabel() const{
    return m_label;
}
