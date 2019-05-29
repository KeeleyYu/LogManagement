#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "kibanachart.h"
#include "kibanaslice.h"
#include "kibanachartview.h"
#include "kibanadatabase.h"

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFontDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QtAlgorithms>

QT_BEGIN_NAMESPACE
class QRadioButton;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QPieSeries;
class QPieSlice;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWidget:public QWidget {
    Q_OBJECT

public Q_SLOTS:
    // logLevel变化槽
    void UpdateSwitchLogLevelSettings();
    // logMsg Top显示槽
    void UpdateSwitchLogMsgTopSettings();

    void UpdateSwitchPlatformVerSearchSettings();
    void UpdateSwitchLogMsgSearchSettings();

    // logLevel变化导致platformVer搜索变化槽
    void UpdatePlatformVerSearchContents();
    // logLevel变化导致logMsg搜索变化槽
    void UpdateLogMsgSearchContents();

    void UpdatePieBarSettingsSlice(QPieSlice *slice);
    void UpdatePieBarSettingsString(QString sliceLabel);

    const QStringList AllLogTargetList(QString logTarget);
public:
    explicit MainWidget(QWidget *parent = 0);

private:
    QString m_logLevel;
    int m_grade;

    KibanaDatabase m_kibanaDatabase;

    KibanaChart *m_pieChart;
    KibanaChart *m_barChart;

    QChartView *m_pieChartView;
    KibanaChartView *m_barChartView;

    QPieSeries *m_errorSeries;
    QPieSeries *m_warningSeries;

    QButtonGroup *m_logLevelGroup;
    QRadioButton *m_errorRadioButton;
    QRadioButton *m_warningRadioButton;
    QPushButton *m_refreshPushButton;

    QCheckBox *m_logMsgLimitInvisible;
    QSpinBox *m_logMsgLimit;
    QComboBox *m_logMsgSearch;
    QComboBox *m_logLevelSearch;
};

#endif // MAINWIDGET_H
