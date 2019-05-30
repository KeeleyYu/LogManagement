#include "mainwidget.h"

QT_CHARTS_USE_NAMESPACE

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), m_logLevel("ERROR"), m_grade(1) {
    // 打开数据库
    m_kibanaDatabase.CreateDatabase();

    // 创建pie chart
    m_pieChart = new KibanaChart();
    m_pieChart->setTheme(QChart::ChartThemeLight);
    m_pieChart->setAnimationOptions(QChart::AllAnimations);
    m_pieChart->legend()->setVisible(true);
    m_pieChart->legend()->setAlignment(Qt::AlignRight);
    m_pieChart->legend()->setShowToolTips(true);

    // 创建pie chartview
    m_pieChartView = new QChartView(m_pieChart);
    m_pieChartView->setRenderHint(QPainter::Antialiasing);

    // 创建bar chart
    m_barChart = new KibanaChart();
    m_barChart->setTheme(QChart::ChartThemeLight);
    m_barChart->setAnimationOptions(QChart::AllAnimations);
    m_barChart->legend()->setVisible(true);
    m_barChart->legend()->setAlignment(Qt::AlignRight);
    m_barChart->legend()->setShowToolTips(true);

    // 创建bar chartview
    m_barChartView = new KibanaChartView(m_barChart);
    m_barChartView->setRenderHint(QPainter::Antialiasing);

    // 创建刷新键
    m_refreshPushButton = new QPushButton();
    m_refreshPushButton->setText("Refresh");

    // 创建日期检索键
    QDate minimumDate(2000, 1, 1), maximumDate(2100, 1, 1);
    m_minimumDateEdit = new QDateEdit;
    m_minimumDateEdit->setDisplayFormat("yyyy/MM/dd/");
    m_minimumDateEdit->setDateRange(minimumDate, maximumDate);
    m_minimumDateEdit->setDate(QDate::currentDate());

    m_maximumDateEdit = new QDateEdit;
    m_maximumDateEdit->setDisplayFormat("yyyy/MM/dd/");
    m_maximumDateEdit->setDateRange(minimumDate, maximumDate);
    m_maximumDateEdit->setDate(QDate::currentDate());

    m_dateSearchPushButton = new QPushButton();
    m_dateSearchPushButton->setText("Search");

    // 创建logLevel选择键
    m_errorRadioButton = new QRadioButton();
    m_warningRadioButton = new QRadioButton();
    m_errorRadioButton->setChecked(true);

    m_logLevelGroup = new QButtonGroup();
    m_logLevelGroup->addButton(m_errorRadioButton, 0);
    m_logLevelGroup->addButton(m_warningRadioButton, 1);

    // 创建logLevel检索键
    m_logLevelSearch = new QComboBox();
    m_logLevelSearch->setEditable(true);
    m_logLevelSearch->addItems(AllLogTargetList("platformVer"));
    m_logLevelSearch->setMaximumWidth(200);

    // 创建logMsg对比键
    m_logMsgLimitInvisible = new QCheckBox();
    m_logMsgLimitInvisible->setCheckState(Qt::Checked);

    m_logMsgLimit = new QSpinBox();
    m_logMsgLimit->setMinimum(0);
    m_logMsgLimit->setMaximum(10);
    m_logMsgLimit->setSingleStep(1);
    m_logMsgLimit->setValue(0);
    m_logMsgLimit->setReadOnly(true);

    // 创建logMsg检索键
    m_logMsgSearch = new QComboBox();
    m_logMsgSearch->setEditable(true);
    m_logMsgSearch->addItems(AllLogTargetList("logMsg"));
    m_logMsgSearch->setMaximumWidth(200);

    // 创建formLayout
    QFormLayout *chartSettingsLayout = new QFormLayout();
    chartSettingsLayout->addRow(m_refreshPushButton);
    QGroupBox *chartSettings = new QGroupBox("Settings");
    chartSettings->setLayout(chartSettingsLayout);

    QFormLayout *dateSettingsLayout = new QFormLayout();
    dateSettingsLayout->addRow("From", m_minimumDateEdit);
    dateSettingsLayout->addRow("To", m_maximumDateEdit);
    dateSettingsLayout->addRow(m_dateSearchPushButton);
    QGroupBox *dateSettings = new QGroupBox("Date Query");
    dateSettings->setLayout(dateSettingsLayout);

    QFormLayout *logLevelSettingsLayout = new QFormLayout();
    logLevelSettingsLayout->addRow("ERROR", m_errorRadioButton);
    logLevelSettingsLayout->addRow("WARNING", m_warningRadioButton);
    QGroupBox *logLevelSettings = new QGroupBox("LogLevel");
    logLevelSettings->setLayout(logLevelSettingsLayout);

    QFormLayout *platformVerSearchSettingsLayout = new QFormLayout();
    platformVerSearchSettingsLayout->addRow(m_logLevelSearch);
    QGroupBox *platformVerSearchSettings = new QGroupBox("PlatformVer Search");
    platformVerSearchSettings->setLayout(platformVerSearchSettingsLayout);

    QFormLayout *logMsgSettingsLayout = new QFormLayout();
    logMsgSettingsLayout->addRow("Compare Disabled", m_logMsgLimitInvisible);
    logMsgSettingsLayout->addRow("Compare Top", m_logMsgLimit);
    QGroupBox *logMsgSettings = new QGroupBox("LogMsg");
    logMsgSettings->setLayout(logMsgSettingsLayout);

    QFormLayout *logMsgSearchSettingsLayout = new QFormLayout();
    logMsgSearchSettingsLayout->addRow(m_logMsgSearch);
    QGroupBox *logMsgSearchSettings = new QGroupBox("LogMsg Search");
    logMsgSearchSettings->setLayout(logMsgSearchSettingsLayout);

    // 创建boxLayout
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(chartSettings);
    settingsLayout->addWidget(dateSettings);
    settingsLayout->addWidget(logLevelSettings);
    settingsLayout->addWidget(platformVerSearchSettings);
    settingsLayout->addWidget(logMsgSettings);
    settingsLayout->addWidget(logMsgSearchSettings);
    settingsLayout->addStretch();

    // 创建gridLayout
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addLayout(settingsLayout, 0, 0);
    baseLayout->addWidget(m_barChartView, 0, 1);
    baseLayout->addWidget(m_pieChartView, 0, 2);
    setLayout(baseLayout);

    // 信号
    connect(m_refreshPushButton, &QPushButton::clicked, this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_dateSearchPushButton, &QPushButton::clicked, this, &MainWidget::QueryByDate);
    connect(m_errorRadioButton, &QRadioButton::clicked, this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_warningRadioButton, &QRadioButton::clicked, this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_logLevelSearch, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWidget::UpdateSwitchPlatformVerSearchSettings);
    connect(m_logMsgLimitInvisible, &QCheckBox::toggled, this,  &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_logMsgLimit, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_logMsgSearch, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWidget::UpdateSwitchLogMsgSearchSettings);

    // 创建pie series 和 bar series
    UpdatePieBarSettingsString(m_logLevel);
}

void MainWidget::UpdateSwitchLogLevelSettings() {
    m_logLevel = (m_logLevelGroup->checkedId() == 0) ? "ERROR" : "WARNING";

    UpdateLogMsgSearchContents();
    UpdatePlatformVerSearchContents();
    if (!m_logMsgLimitInvisible->isChecked()) {
        m_logMsgLimit->setReadOnly(false);
        UpdateSwitchLogMsgTopSettings();
    } else {
        m_logMsgLimit->setReadOnly(true);
        m_grade = 1;
        UpdatePieBarSettingsString(m_logLevel);
    }
}

void MainWidget::UpdateSwitchLogMsgTopSettings() {
    // bar
    QBarSeries *barSeries = new QBarSeries();
    barSeries->setName("Top " + QString::number(m_logMsgLimit->value()) + " - bar chart");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    QSqlQuery sqlQuery(m_kibanaDatabase.getDatabaseName());
    // 得到各platformVer（用于插入x轴）
    QString platformVerSql = "select distinct platformVer "
                             "from " + m_kibanaDatabase.getTableName() +
                             " where logLevel=:loglevel "
                             "and datestamp between :fromdate and :todate "
                             "order by platformVer";
    sqlQuery.prepare(platformVerSql);
    sqlQuery.bindValue(":loglevel", m_logLevel);
    sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
    sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    if (!sqlQuery.exec()) {
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
        return;
    }
    while (sqlQuery.next()) {
        axisX->append(sqlQuery.value(0).toString());
    }

    // 对总数排序得到各logMsg
    QString logMsgSql = "select logMsg "
                        "from LogInfo "
                        "where logLevel=:loglevel "
                        "and datestamp between :fromdate and :todate "
                        "group by logMsg "
                        "order by count(*) desc";
    sqlQuery.prepare(logMsgSql);
    sqlQuery.bindValue(":loglevel", m_logLevel);
    sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
    sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    if (!sqlQuery.exec()) {
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
        return;
    }
    for (int i(0); i < m_logMsgLimit->value(); i++) {
        if (!sqlQuery.next())
            break;
        QString thisLogMsg = sqlQuery.value(0).toString();
        QBarSet *barSet = new QBarSet(thisLogMsg);

        QSqlQuery tmp_sqlQuery;
        // 得到该logMsg的各platformVer个数
        QString selectSql = "select count(*) "
                            "from LogInfo "
                            "where logLevel=:loglevel and "
                            "logMsg=:logmsg "
                            "and datestamp between :fromdate and :todate "
                            "group by platformVer ";
        tmp_sqlQuery.prepare(selectSql);
        tmp_sqlQuery.bindValue(":loglevel", m_logLevel);
        tmp_sqlQuery.bindValue(":logmsg", thisLogMsg);
        tmp_sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
        tmp_sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
        if (!tmp_sqlQuery.exec()) {
            qDebug() << __FUNCTION__ << tmp_sqlQuery.lastError();
            return;
        }
        while (tmp_sqlQuery.next()) {
            int thisCount = tmp_sqlQuery.value(0).toInt();
            *barSet << thisCount;
        }
        barSeries->append(barSet);
    }

    // 构造bar
    m_barChart->changeSeries(barSeries);

    // bar坐标轴设置
    axisX->setLabelsAngle(-90);
    m_barChart->removeAxis(m_barChart->axisX());
    m_barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    m_barChart->removeAxis(m_barChart->axisY());
    m_barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    // 信号
    connect(barSeries, &QBarSeries::hovered, barSeries, &QBarSeries::setLabelsVisible);
}

void MainWidget::UpdateSwitchPlatformVerSearchSettings() {
    m_grade = 2;
    UpdatePieBarSettingsString(m_logLevelSearch->currentText());
}

void MainWidget::UpdateSwitchLogMsgSearchSettings() {
    m_grade = 3;
    UpdatePieBarSettingsString(m_logMsgSearch->currentText());
}

void MainWidget::UpdateLogMsgSearchContents() {
    while (m_logMsgSearch->count())
        m_logMsgSearch->removeItem(0);
    m_logMsgSearch->addItems(AllLogTargetList("logMsg"));
}

void MainWidget::UpdatePlatformVerSearchContents() {
    while (m_logLevelSearch->count())
        m_logLevelSearch->removeItem(0);
    m_logLevelSearch->addItems(AllLogTargetList("platformVer"));
}

void MainWidget::UpdatePieBarSettingsSlice(QPieSlice *slice) {
    UpdatePieBarSettingsString(slice->label());
}

void MainWidget::UpdatePieBarSettingsString(QString sliceLabel) {
    QSqlQuery sqlQuery(m_kibanaDatabase.getDatabaseName());
    if (m_grade == 1) {
        // 得到各platformVer的m_logLevel总数
        QString selectSql = "select platformVer, count(*) "
                            "from LogInfo "
                            "where logLevel=:loglevel "
                            "and datestamp between :fromdate and :todate "
                            "group by platformVer";
        sqlQuery.prepare(selectSql);
        sqlQuery.bindValue(":loglevel", m_logLevel);
        sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
        sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    } else if (m_grade == 2) {
        // 得到该platformVer的各logMsg个数
        QString selectSql = "select logMsg, count(*) "
                            "from LogInfo "
                            "where logLevel=:loglevel and "
                            "platformVer=:platformver "
                            "and datestamp between :fromdate and :todate "
                            "group by logMsg";
        sqlQuery.prepare(selectSql);
        sqlQuery.bindValue(":loglevel", m_logLevel);
        sqlQuery.bindValue(":platformver", sliceLabel);
        sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
        sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    } else if (m_grade == 3) {
        // 得到该logMsg的各platformVer个数
        QString selectSql = "select platformVer, count(*) "
                            "from LogInfo "
                            "where logLevel=:loglevel and "
                            "logMsg=:logmsg "
                            "and datestamp between :fromdate and :todate "
                            "group by platformVer";
        sqlQuery.prepare(selectSql);
        sqlQuery.bindValue(":loglevel", m_logLevel);
        sqlQuery.bindValue(":logmsg", sliceLabel);
        sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
        sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    } else {
        qDebug() << "grade wrong!";
        return;
    }
    if (!sqlQuery.exec()) {
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
        return;
    }

    // 第二级pie
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName(sliceLabel + " - pie chart");

    // 第二级bar
    QBarSeries *barSeries = new QBarSeries();
    barSeries->setName(sliceLabel + " - bar chart");
    QBarSet *barSet = new QBarSet("counts");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    // 构造pie 和 bar
    while (sqlQuery.next()) {
        QString thisName = sqlQuery.value(0).toString();
        int thisCount = sqlQuery.value(1).toInt();
        *pieSeries << new KibanaSlice(thisCount, thisName, pieSeries);
        *barSet << thisCount;
        axisX->append(thisName);
    }
    barSeries->append(barSet);
    m_pieChart->changeSeries(pieSeries);
    m_barChart->changeSeries(barSeries);

    // 第二级bar坐标轴设置
    axisX->setLabelsAngle(-90);
    m_barChart->removeAxis(m_barChart->axisX());
    m_barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    m_barChart->removeAxis(m_barChart->axisY());
    m_barChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    // 发送信号
    connect(barSeries, &QBarSeries::hovered, barSeries, &QBarSeries::setLabelsVisible);
    if (m_grade == 1) {
        m_grade = 2;
        connect(pieSeries, &QPieSeries::clicked, this, &MainWidget::UpdatePieBarSettingsSlice);
    } else if (m_grade == 2) {
        m_grade = 3;
        connect(pieSeries, &QPieSeries::clicked, this, &MainWidget::UpdatePieBarSettingsSlice);
    } else if (m_grade == 3) {
        m_grade = 2;
        connect(pieSeries, &QPieSeries::clicked, this, &MainWidget::UpdatePieBarSettingsSlice);
    } else {

    }
}

const QStringList MainWidget::AllLogTargetList(QString logTarget) {
    QSqlQuery sqlQuery(m_kibanaDatabase.getDatabaseName());
    QStringList logMsgList;
    QString platformVerSql = "select distinct " + logTarget +
                             " from LogInfo "
                             "where logLevel=:loglevel "
                             "and datestamp between :fromdate and :todate "
                             "order by " + logTarget;
    sqlQuery.prepare(platformVerSql);
    sqlQuery.bindValue(":loglevel", m_logLevel);
    sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
    sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    if (!sqlQuery.exec()) {
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
        return logMsgList;
    }
    while (sqlQuery.next()) {
        logMsgList << sqlQuery.value(0).toString();
    }
    return logMsgList;
}

void MainWidget::QueryByDate() {
    m_kibanaDatabase.QueryByDate(m_minimumDateEdit->date(), m_maximumDateEdit->date());
    UpdateSwitchLogLevelSettings();
}
