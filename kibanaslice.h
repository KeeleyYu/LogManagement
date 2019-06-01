#ifndef KIBANASLICE_H
#define KIBANASLICE_H

#include <QtCharts/QPieSlice>

QT_CHARTS_BEGIN_NAMESPACE
class QAbstractSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class KibanaSlice : public QPieSlice
{
    Q_OBJECT

public:
    KibanaSlice(qreal value, QString prefix, QAbstractSeries *drilldownSeries);
    virtual ~KibanaSlice();
    QAbstractSeries *drilldownSeries() const;
    QString sliceLabel() const;

public Q_SLOTS:
    void updateLabel();
    void showHighlight(bool show);

private:
    QAbstractSeries *m_drilldownSeries;
    QString m_label;
    QString m_showLabel;
};

#endif // KIBANASLICE_H
