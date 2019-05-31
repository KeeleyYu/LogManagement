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
#include <QDateEdit>
#include <QCompleter>

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

    // 更新搜索栏
    void UpdateSwitchSearchSettings(QComboBox *searchBox, QString searchTarget);

    // 更新pie和bar图
    void UpdatePieBarSettingsSlice(QPieSlice *slice);
    void UpdatePieBarSettingsString(QString sliceLabel);

    // 更新logMsg或platformVer搜索列表
    void UpdateLogTargetList(QString logTarget);

    // 按日期查询
    void QueryByDate();

public:
    explicit MainWidget(QWidget *parent = 0);

private:
    QString m_logLevel;
    QStringList m_logMsgList;
    QStringList m_platformVerList;
    int m_grade;

    KibanaDatabase m_kibanaDatabase;

    KibanaChart *m_barChart;
    KibanaChart *m_pieChart;
    KibanaChartView *m_barChartView;
    QChartView *m_pieChartView;

    QPieSeries *m_errorSeries;
    QPieSeries *m_warningSeries;

    QPushButton *m_refreshPushButton;

    QDateEdit *m_minimumDateEdit;
    QDateEdit *m_maximumDateEdit;
    QPushButton *m_dateSearchPushButton;

    QButtonGroup *m_logLevelGroup;
    QRadioButton *m_errorRadioButton;
    QRadioButton *m_warningRadioButton;

    QCheckBox *m_logMsgLimitInvisible;
    QSpinBox *m_logMsgLimit;

    QComboBox *m_logMsgSearch;
    QComboBox *m_platformVerSearch;
};

#endif // MAINWIDGET_H
