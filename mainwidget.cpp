#include "mainwidget.h"

QT_CHARTS_USE_NAMESPACE

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), m_logLevel("ERROR") {
    // 打开数据库
    m_kibanaDatabase.CreateDatabase();
    // 清除一周前记录
    m_kibanaDatabase.ClearOverWeekRecords(QDate(QDate::currentDate()));

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
    m_infoRadioButton = new QRadioButton();
    m_allRadioButton = new QRadioButton();
    m_errorRadioButton->setChecked(true);

    m_logLevelGroup = new QButtonGroup();
    m_logLevelGroup->addButton(m_errorRadioButton, 0);
    m_logLevelGroup->addButton(m_warningRadioButton, 1);
    m_logLevelGroup->addButton(m_infoRadioButton, 2);
    m_logLevelGroup->addButton(m_allRadioButton, 3);

    // 创建logMsg change和 logMsg top对比键
    m_logMsgChangeEnable = new QCheckBox();
    m_logMsgTopEnable = new QCheckBox();

    m_logMsgTop = new QSpinBox();
    m_logMsgTop->setMinimum(0);
    m_logMsgTop->setMaximum(10);
    m_logMsgTop->setSingleStep(1);
    m_logMsgTop->setValue(0);
    m_logMsgTop->setReadOnly(true);

    // 创建logMsg检索键
    m_logMsgSearch = new QComboBox();
    m_logMsgSearch->setEditable(true);
    m_logMsgSearch->setMaximumWidth(200);
    UpdateLogTargetList("logMsg");
    m_logMsgSearch->addItems(m_logMsgList);

    // 创建platformVer检索键
    m_platformVerSearch = new QComboBox();
    m_platformVerSearch->setEditable(true);
    m_platformVerSearch->setMaximumWidth(200);
    UpdateLogTargetList("platformVer");
    m_platformVerSearch->addItems(m_platformVerList);

    // 创建platformVer变化状态栏
    m_platformVerChanges = new QPlainTextEdit();
    m_platformVerChanges->setPlainText("cvte kibana");
    m_platformVerChanges->setReadOnly(true);
    m_platformVerChanges->setMaximumWidth(200);
    m_platformVerChanges->setMinimumHeight(350);

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
    logLevelSettingsLayout->addRow("INFO", m_infoRadioButton);
    //logLevelSettingsLayout->addRow("ALL(not used yet)", m_allRadioButton);
    QGroupBox *logLevelSettings = new QGroupBox("LogLevel");
    logLevelSettings->setLayout(logLevelSettingsLayout);

    QFormLayout *logMsgSettingsLayout = new QFormLayout();
    //logMsgSettingsLayout->addRow("Compare Change on/off", m_logMsgChangeEnable);
    logMsgSettingsLayout->addRow("Compare Top on/off", m_logMsgTopEnable);
    logMsgSettingsLayout->addRow("Compare Top", m_logMsgTop);
    QGroupBox *logMsgSettings = new QGroupBox("LogMsg");
    logMsgSettings->setLayout(logMsgSettingsLayout);

    QFormLayout *logMsgSearchSettingsLayout = new QFormLayout();
    logMsgSearchSettingsLayout->addRow(m_logMsgSearch);
    QGroupBox *logMsgSearchSettings = new QGroupBox("LogMsg Search");
    logMsgSearchSettings->setLayout(logMsgSearchSettingsLayout);

    QFormLayout *platformVerSearchSettingsLayout = new QFormLayout();
    platformVerSearchSettingsLayout->addRow(m_platformVerSearch);
    QGroupBox *platformVerSearchSettings = new QGroupBox("PlatformVer Search");
    platformVerSearchSettings->setLayout(platformVerSearchSettingsLayout);

    QFormLayout *platformVerChangesSettingsLayout = new QFormLayout();
    platformVerChangesSettingsLayout->addRow(m_platformVerChanges);
    QGroupBox *platformVerChangesSettings = new QGroupBox("PlatformVer Changes");
    platformVerChangesSettings->setLayout(platformVerChangesSettingsLayout);

    // 创建boxLayout
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(chartSettings);
    settingsLayout->addWidget(dateSettings);
    settingsLayout->addWidget(logLevelSettings);
    settingsLayout->addWidget(logMsgSettings);
    settingsLayout->addWidget(logMsgSearchSettings);
    settingsLayout->addWidget(platformVerSearchSettings);
    settingsLayout->addWidget(platformVerChangesSettings);
    settingsLayout->addStretch();

    // 创建gridLayout
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addLayout(settingsLayout, 0, 0);
    baseLayout->addWidget(m_barChartView, 0, 1);
    baseLayout->addWidget(m_pieChartView, 0, 2);
    baseLayout->setColumnStretch(0, 0);
    baseLayout->setColumnStretch(1, 1);
    baseLayout->setColumnStretch(2, 1);
    setLayout(baseLayout);

    // 信号
    connect(m_refreshPushButton, &QPushButton::clicked, this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_dateSearchPushButton, &QPushButton::clicked, this, &MainWidget::QueryByDate);
    connect(m_errorRadioButton, &QRadioButton::clicked, this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_warningRadioButton, &QRadioButton::clicked, this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_infoRadioButton, &QRadioButton::clicked, this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_allRadioButton, &QRadioButton::clicked, this, &MainWidget::UpdateSwitchLogLevelSettings);
    connect(m_platformVerSearch, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this, &MainWidget::UpdateSwitchPlatformVerSearchSettings);
    connect(m_logMsgTopEnable, &QCheckBox::toggled, this,  &MainWidget::UpdateSwitchLogMsgTopButton);
    connect(m_logMsgChangeEnable, &QCheckBox::toggled, this,  &MainWidget::UpdateSwitchLogMsgChangeButton);
    connect(m_logMsgTop, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWidget::UpdateLogMsgTopSettings);
    connect(m_logMsgSearch, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
            this, &MainWidget::UpdateSwitchLogMsgSearchSettings);

    // 初始化
    UpdateSwitchLogLevelSettings();
}

void MainWidget::UpdatePieBarSettingsSlice(QPieSlice *slice) {
    KibanaSlice *Kslice = dynamic_cast<KibanaSlice*>(slice);
    if (m_logMsgList.contains(Kslice->sliceLabel()))
        UpdatePieBarSettingsString(Kslice->sliceLabel(), 3);
    else if (m_platformVerList.contains(Kslice->sliceLabel()))
        UpdatePieBarSettingsString(Kslice->sliceLabel(), 2);
    else
        qDebug() << __FUNCTION__ << " wrong!";
}

void MainWidget::UpdatePieBarSettingsString(QString sliceLabel, int event) {
    QSqlQuery sqlQuery(m_kibanaDatabase.getDatabaseName());
    QString selectSql;
    if (event == 1) {
        selectSql = "select platformVer, count(*) "
                            "from " + m_kibanaDatabase.getTableName() +
                            " where logLevel=:loglevel "
                            "and datestamp between :fromdate and :todate "
                            "group by platformVer";
        sqlQuery.prepare(selectSql);
    } else if (event == 2) {
        UpdatePlatformVerChangesSettings(sliceLabel);
        selectSql = "select logMsg, count(*) "
                            "from " + m_kibanaDatabase.getTableName() +
                            " where logLevel=:loglevel and "
                            "platformVer=:platformver "
                            "and datestamp between :fromdate and :todate "
                            "group by logMsg";
        sqlQuery.prepare(selectSql);
        sqlQuery.bindValue(":platformver", sliceLabel);
    } else if (event == 3) {
        selectSql = "select platformVer, count(*) "
                            "from " + m_kibanaDatabase.getTableName() +
                            " where logLevel=:loglevel and "
                            "logMsg=:logmsg "
                            "and datestamp between :fromdate and :todate "
                            "group by platformVer";
        sqlQuery.prepare(selectSql);
        sqlQuery.bindValue(":logmsg", sliceLabel);
    } else {
        qDebug() << "event wrong!";
        return;
    }
    sqlQuery.bindValue(":loglevel", m_logLevel);
    sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
    sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    if (!sqlQuery.exec()) {
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
        return;
    }

    // pie
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName(sliceLabel + " - pie chart");

    // bar
    QBarSeries *barSeries = new QBarSeries();
    barSeries->setName(sliceLabel + " - bar chart");

    // 构造pie 和 bar
    while (sqlQuery.next()) {
        QString thisName = sqlQuery.value(0).toString();
        int thisCount = sqlQuery.value(1).toInt();

        *pieSeries << new KibanaSlice(thisCount, thisName, pieSeries);

        QBarSet *barSet = new QBarSet(thisName);
        *barSet << thisCount;
        barSeries->append(barSet);
    }
    m_pieChart->changeSeries(pieSeries);
    m_barChart->changeSeries(barSeries);
    if (m_barChart->axisX() != NULL)
        m_barChart->removeAxis(m_barChart->axisX());

    barSeries->setBarWidth(1);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    barSeries->setLabelsVisible(true);

    // 信号
    m_barChart->connectMarkers("Bar");
    m_pieChart->connectMarkers("Pie");
    connect(pieSeries, &QPieSeries::clicked, this, &MainWidget::UpdatePieBarSettingsSlice);
    connect(barSeries, &QBarSeries::hovered, m_barChart, &KibanaChart::handleBarSetHovered);
}

void MainWidget::UpdateSwitchLogLevelSettings() {
    if (m_logLevelGroup->checkedId() == 0)
        m_logLevel = "ERROR";
    else if (m_logLevelGroup->checkedId() == 1)
        m_logLevel = "WARNING";
    else if (m_logLevelGroup->checkedId() == 2)
        m_logLevel = "INFO";
    else if (m_logLevelGroup->checkedId() == 3)
        ;

    UpdateSearchComboBox(m_logMsgSearch, "logMsg");
    UpdateSearchComboBox(m_platformVerSearch, "platformVer");

    if (m_logMsgChangeEnable->checkState() == Qt::Checked) {
        UpdateSwitchLogMsgChangeButton();
    } else if (m_logMsgTopEnable->checkState() == Qt::Checked) {
        UpdateSwitchLogMsgTopButton();
    } else {
        UpdatePieBarSettingsString(m_logLevel, 1);
    }
}

void MainWidget::UpdateSwitchLogMsgChangeButton() {
    if (m_logMsgChangeEnable->checkState() == Qt::Unchecked) {
        UpdateSwitchLogLevelSettings();
    } else if (m_logMsgChangeEnable->checkState() == Qt::Checked) {
            m_logMsgTopEnable->setCheckState(Qt::Unchecked);
            m_logMsgTop->setReadOnly(true);
            UpdateLogMsgChangeSettings();
    }
}

void MainWidget::UpdateLogMsgChangeSettings() {

}

void MainWidget::UpdateSwitchLogMsgTopButton() {
    if (m_logMsgTopEnable->checkState() == Qt::Unchecked) {
        UpdateSwitchLogLevelSettings();
    } else if (m_logMsgTopEnable->checkState() == Qt::Checked){
        m_logMsgChangeEnable->setCheckState(Qt::Unchecked);
        m_logMsgTop->setReadOnly(false);
        UpdateLogMsgTopSettings();
    }
}

void MainWidget::UpdateLogMsgTopSettings() {
    // bar
    QBarSeries *barSeries = new QBarSeries();
    barSeries->setName("Top " + QString::number(m_logMsgTop->value()) + " - bar chart");
    QBarCategoryAxis *axisX = new QBarCategoryAxis();

    // pie
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setName("Top " + QString::number(m_logMsgTop->value()) + " - pie chart");

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

    // 对个数排序得到各logMsg
    QString logMsgSql = "select logMsg, count(*) "
                        "from " + m_kibanaDatabase.getTableName() +
                        " where logLevel=:loglevel "
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
    for (int i(0); i < m_logMsgTop->value(); i++) {
        if (!sqlQuery.next())
            break;
        QString thisLogMsg = sqlQuery.value(0).toString();
        int thisCount = sqlQuery.value(1).toInt();

        // 构建bar和pie
        QBarSet *barSet = new QBarSet(thisLogMsg);
        *pieSeries << new KibanaSlice(thisCount, thisLogMsg, pieSeries);

        QSqlQuery tmp_sqlQuery;
        // 得到该logMsg的各platformVer个数
        QString selectSql = "select count(*) "
                            "from " + m_kibanaDatabase.getTableName() +
                            " where logLevel=:loglevel and "
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

    // barchart和piechart
    m_barChart->changeSeries(barSeries);
    m_pieChart->changeSeries(pieSeries);

    // bar坐标轴设置
    axisX->setLabelsAngle(-90);
    if (m_barChart->axisX() != NULL)
        m_barChart->removeAxis(m_barChart->axisX());
    m_barChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);
    barSeries->setBarWidth(1);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
    barSeries->setLabelsVisible(true);

    // 信号
    m_barChart->connectMarkers("Bar");
    m_pieChart->connectMarkers("Pie");
    connect(barSeries, &QBarSeries::hovered, m_barChart, &KibanaChart::handleBarSetHovered);

}

void MainWidget::UpdateSwitchPlatformVerSearchSettings() {
    UpdatePieBarSettingsString(m_platformVerSearch->currentText(), 2);
}

void MainWidget::UpdateSwitchLogMsgSearchSettings() {
    UpdatePieBarSettingsString(m_logMsgSearch->currentText(), 3);
}

void MainWidget::UpdateSearchComboBox(QComboBox *searchBox, QString searchTarget) {
    while (searchBox->count())
        searchBox->removeItem(0);

    UpdateLogTargetList(searchTarget);
    QStringList searchList;
    if (searchTarget == "logMsg")
        searchList = m_logMsgList;
    else if (searchTarget == "platformVer")
        searchList = m_platformVerList;

    searchBox->addItems(searchList);

    QCompleter *completer = new QCompleter(searchList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    searchBox->setCompleter(completer);
}

void MainWidget::UpdateLogTargetList(QString logTarget) {
    QSqlQuery sqlQuery(m_kibanaDatabase.getDatabaseName());
    QStringList logMsgList;
    QString platformVerSql = "select distinct " + logTarget +
                             " from " + m_kibanaDatabase.getTableName() +
                             " where logLevel=:loglevel "
                             "and datestamp between :fromdate and :todate "
                             "order by " + logTarget;
    sqlQuery.prepare(platformVerSql);
    sqlQuery.bindValue(":loglevel", m_logLevel);
    sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
    sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    if (!sqlQuery.exec()) {
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
        return;
    }
    while (sqlQuery.next()) {
        logMsgList << sqlQuery.value(0).toString();
    }
    if (logTarget == "logMsg")
        m_logMsgList = logMsgList;
    else if (logTarget == "platformVer")
        m_platformVerList = logMsgList;
}

void MainWidget::UpdatePlatformVerChangesSettings(QString platformVer) {
    m_platformVerChanges->clear();
    m_platformVerChanges->setPlainText("###Current version###");
    m_platformVerChanges->appendPlainText(platformVer);

    QMap<QString, int> platformVerChangesMap;
    QSqlQuery sqlQuery(m_kibanaDatabase.getDatabaseName());

    // 当前版本logMsg统计
    QString selectSql = "select logMsg, count(*) "
                        "from " + m_kibanaDatabase.getTableName() +
                        " where logLevel=:loglevel and "
                        "platformVer=:platformVer "
                        "and datestamp between :fromdate and :todate "
                        "group by logMsg";
    sqlQuery.prepare(selectSql);
    sqlQuery.bindValue(":loglevel", m_logLevel);
    sqlQuery.bindValue(":platformVer", platformVer);
    sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
    sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    if (!sqlQuery.exec()) {
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
        return;
    }
    while (sqlQuery.next()) {
        QString thisName = sqlQuery.value(0).toString();
        int thisCount = sqlQuery.value(1).toInt();
        platformVerChangesMap[thisName] = thisCount;
    }
    if (m_platformVerList.indexOf(platformVer) == 0) {
        m_platformVerChanges->appendPlainText("###NEW###");
        for (QMap<QString, int>::iterator it = platformVerChangesMap.begin(); it != platformVerChangesMap.end(); it++) {
            m_platformVerChanges->appendPlainText("LogMsg:" + it.key());
            m_platformVerChanges->appendPlainText("Number:" + QString::number(it.value()));
        }
        return;
    }

    // 上一版本logMsg统计
    QStringList solvedlist, oldlist;
    platformVer = m_platformVerList[m_platformVerList.indexOf(platformVer) - 1];
    selectSql = "select logMsg, count(*) "
                        "from " + m_kibanaDatabase.getTableName() +
                        " where logLevel=:loglevel and "
                        "platformVer=:platformVer "
                        "and datestamp between :fromdate and :todate "
                        "group by logMsg";
    sqlQuery.prepare(selectSql);
    sqlQuery.bindValue(":loglevel", m_logLevel);
    sqlQuery.bindValue(":platformVer", platformVer);
    sqlQuery.bindValue(":fromdate", m_minimumDateEdit->date().toString("yyyy_MM_dd"));
    sqlQuery.bindValue(":todate", m_maximumDateEdit->date().toString("yyyy_MM_dd"));
    if (!sqlQuery.exec()) {
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
        return;
    }
    while (sqlQuery.next()) {
        QString thisName = sqlQuery.value(0).toString();
        int thisCount = sqlQuery.value(1).toInt();
        if (!platformVerChangesMap.contains(thisName))
            solvedlist << "LogMsg:" + thisName << "Number:" + QString::number(thisCount);
        else
            oldlist << "LogMsg:" + thisName << "Number:" + QString::number(thisCount);
    }
    if (platformVerChangesMap.size() != 0) {
        m_platformVerChanges->appendPlainText("###NEW###");
        for (QMap<QString, int>::iterator it = platformVerChangesMap.begin(); it != platformVerChangesMap.end(); it++){
            m_platformVerChanges->appendPlainText("LogMsg:" + it.key());
            m_platformVerChanges->appendPlainText("Number:" + QString::number(it.value()));
        }
    }
    m_platformVerChanges->appendPlainText("###SOLVED###");
    for (QString str: solvedlist)
        m_platformVerChanges->appendPlainText(str);
    m_platformVerChanges->appendPlainText("###OLD###");
    for (QString str: oldlist)
        m_platformVerChanges->appendPlainText(str);
}

void MainWidget::QueryByDate() {
    m_kibanaDatabase.QueryByDate(m_minimumDateEdit->date(), m_maximumDateEdit->date());
    UpdateSwitchLogLevelSettings();
}
