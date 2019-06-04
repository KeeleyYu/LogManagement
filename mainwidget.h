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
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QDateEdit>
#include <QCompleter>
#include <QPlainTextEdit>
#include <QMap>

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
class QBarSet;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWidget:public QWidget {
    Q_OBJECT

public Q_SLOTS:
    // 更新显示图， event:
    // 1=得到当前logLevel下每个platformVer的总数
    // 2=得到当前logLevel下指定platformVer的每个logMsg总数，更新变化栏
    // 3=得到当前logLevel下指定logMsg的每个platformVer总数
    void UpdatePieBarSettingsSlice(QPieSlice *slice);
    void UpdatePieBarSettingsString(QString sliceLabel, int event);

    // 更新logLevel变化槽
    void UpdateSwitchLogLevelSettings();

    // 更新LogMsgChange按钮状态
    void UpdateSwitchLogMsgChangeButton();
    // 更新LogMsgChange显示图
    void UpdateLogMsgChangeSettings();
    // 更新LogMsgTop按钮状态
    void UpdateSwitchLogMsgTopButton();
    // 更新logMsgTop显示图
    void UpdateLogMsgTopSettings();

    // 更新搜索
    void UpdateSwitchPlatformVerSearchSettings();
    void UpdateSwitchLogMsgSearchSettings();
    // 更新logMsg或platformVer搜索下拉栏
    void UpdateSearchComboBox(QComboBox *searchBox, QString searchTarget);
    // 更新logMsg或platformVer搜索列表
    void UpdateLogTargetList(QString logTarget);

    // 更新platformVer变化栏
    void UpdatePlatformVerChangesSettings(QString platformVer);

    // 按日期查询，如果本地数据库不存在则从kibana获取
    void QueryByDate();

public:
    explicit MainWidget(QWidget *parent = 0);

private:
    QString m_logLevel;
    QStringList m_logMsgList;
    QStringList m_platformVerList;

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
    QRadioButton *m_infoRadioButton;
    QRadioButton *m_allRadioButton;

    QCheckBox *m_logMsgChangeEnable;
    QCheckBox *m_logMsgTopEnable;
    QSpinBox *m_logMsgTop;

    QComboBox *m_logMsgSearch;
    QComboBox *m_platformVerSearch;

    QPlainTextEdit *m_platformVerChanges;
};

#endif // MAINWIDGET_H
