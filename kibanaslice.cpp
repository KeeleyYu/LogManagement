#include "kibanaslice.h"

QT_CHARTS_USE_NAMESPACE

KibanaSlice::KibanaSlice(qreal value, QString prefix, QAbstractSeries *drilldownSeries)
    : m_drilldownSeries(drilldownSeries),
      m_prefix(prefix)
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
    QString label = m_prefix;
    QString show_label = label;
    show_label += QString::number(this->value());
    show_label += ", ";
    show_label += QString::number(this->percentage() * 100, 'f', 1);
    show_label += "%";
    setLabel(label);
}

void KibanaSlice::showHighlight(bool show)
{
    setLabelVisible(show);
    setExploded(show);
}
