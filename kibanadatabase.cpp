#include "KibanaDatabase.h"

KibanaDatabase::KibanaDatabase(QWidget *parent)
    : QWidget(parent) {
    tableName = "LogInfo";
    databaseName = "KibanaLog.db";
    kibana_url = "http://myou.cvte.com/grail/api/query/sql";
    deviceName = "log_grail_pro_cros_28762406_";
    query_url = "?sql=SELECT * FROM " + deviceName;
    xDataId = "cros_28762406";
    timeInterval = 60 * 20;
}

KibanaDatabase::~KibanaDatabase() {}

void KibanaDatabase::QueryByDate(QDate fromDate, QDate toDate) {
    m_manager = new QNetworkAccessManager();
    // 信号
    connect(m_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(slot_replyFinished(QNetworkReply*)));

    for (int i = 0; i <= fromDate.daysTo(toDate); i++) {
        QDate curDate = fromDate.addDays(i);
        // 当天数据必须重新拉取（因为可能随时更新，或改成前timeInterval有当天数据就不用拉取）
        if (QueryDateIsExist(curDate) && curDate != QDate::currentDate()) {
            continue;
        }
        QString dateStr = curDate.toString("yyyy_MM_dd");
        qDebug() << "Getting data on " + dateStr + "...";

        QString middle_url = kibana_url + query_url + dateStr;
        // 默认一次最多取200条，因此需要手动设置时间戳区间
        QDateTime startTime(curDate), endTime(curDate.addDays(1));
        for (QDateTime curTime(startTime); curTime < endTime || curTime < QDateTime::currentDateTime(); curTime = curTime.addSecs(timeInterval)) {
            //qDebug() << curTime.toString("yyyy-MM-ddTHH:mm:ss.000Z");
            QString final_url = middle_url +
                    " where @timestamp <= '" +
                    curTime.toString("yyyy-MM-ddTHH:mm:ss.000Z") + "'";
            // 发送请求
            QNetworkRequest request;
            request.setUrl(final_url);
            request.setRawHeader("X-DATA-ID", xDataId.toLocal8Bit().data());
            m_manager->get(request);
        }
    }

}

void KibanaDatabase::slot_replyFinished(QNetworkReply *reply) {
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    //使用utf8编码, 这样可以显示中文
    QByteArray recvData = reply->readAll();
    QString recvStr = codec->toUnicode(recvData);

    //最后要释放replay对象
    reply->deleteLater();
    reply = nullptr;

    SplitJsonFromRecvData(recvData);
    //qDebug() << "Getting data finished.";
}

void KibanaDatabase::SplitJsonFromRecvData(QByteArray recvData) {
    // 预处理
    recvData = recvData.mid(recvData.indexOf("[") + 1, recvData.lastIndexOf("]") - recvData.indexOf("[") - 1);
    int startIndex = 0, endIndex = 0;
    // 截取当前JSON
    for (; startIndex != -1; startIndex = recvData.indexOf("{", endIndex)) {
        endIndex = recvData.indexOf("}}", startIndex);
        QByteArray curData;
        for (int i = startIndex; i < endIndex + 2; i++) {
            if (recvData[i] == '\\')
                    continue;
            curData.append(recvData[i]);
        }
        // 转化为JSON对象
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(curData, &jsonError));
        if(jsonError.error != QJsonParseError::NoError) {
            // qDebug() << curData;
            // qDebug() << jsonError.errorString();
            continue;
        }
        QJsonObject rootObj = jsonDoc.object();
        // 插入数据库
        InsertDatabase(rootObj);

    }
}

bool KibanaDatabase::InsertDatabase(QJsonObject rootObj) {
    QJsonObject subObj = rootObj.value("_source").toObject();

    QSqlQuery jsonQuery;
    QString insertSql = "INSERT INTO " + tableName +
                        " (_id, platformVer, logLevel, logMsg, timestamp, datestamp) "
                        "VALUES(?, ?, ?, ?, ?, ?)";
    jsonQuery.prepare(insertSql);
    jsonQuery.addBindValue(rootObj.value("_id").toString());
    jsonQuery.addBindValue(subObj.value("platformVer").toString());
    jsonQuery.addBindValue(subObj.value("logLevel").toString());
    jsonQuery.addBindValue(subObj.value("logMsg").toString());
    // @timestamp:"yyyy-mm-ddThh:mm:ss.mmmZ"
    jsonQuery.addBindValue(subObj.value("\@timestamp").toString());
    // _index:"log_grail_pro_cros_28762406_yyyy_mm_dd"
    jsonQuery.addBindValue(rootObj.value("_index").toString().mid(deviceName.length()));
    if (!jsonQuery.exec()) {
        //qDebug() << "Insert db error!" << jsonQuery.lastError();
        return false;
    }
    qDebug() << (subObj.value("\@timestamp").toString());
    //qDebug() << "Insert db succeed.";
    return true;
}

bool KibanaDatabase::CreateDatabase() {
    // 创建数据库
    m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDatabase.setDatabaseName(databaseName);
    if (!m_sqlDatabase.open()) {
        qDebug() << "open m_sqlDatabase error!";
        return false;
    }

    // 创建表
    QSqlQuery jsonQuery;
    if (!jsonQuery.exec("create table " + tableName + " ("
                   "_index varchar(50),"
                   "_type varchar(10),"
                   "_id varchar(50) primary key,"
                   "_score double,"
                   "fileName varchar(50),"
                   "line int,"
                   "channel varchar(10),"
                   "logMsg varchar(300),"
                   "source varchar(50),"
                   "platform varchar(10),"
                   "_realCity varchar(50),"
                   "logLevel varchar(10),"
                   "dataId varchar(50),"
                   "deviceMac varchar(50),"
                   "browserVer varchar(50),"
                   "timestamp varchar(50),"
                   "datestamp varchar(50)," // @timestamp
                   "serialNumber varchar(50),"
                   "_realCountry varchar(10),"
                   "method varchar(50),"
                   "dataType varchar(10),"
                   "sessionId varchar(50),"
                   "userId varchar(10),"
                   "platformVer varchar(50),"
                   "_realIp varchar(50),"
                   "_realIsp varhcar(10),"
                   "_realState varchar(10)"
                        ")")) {
        qDebug() << "Create db error: " << jsonQuery.lastError();
        return false;
    }
    qDebug() << "Create db succeed.";
    return true;
}

QString KibanaDatabase::getDatabaseName() const{
    return databaseName;
}

QString KibanaDatabase::getTableName() const {
    return tableName;
}

QSqlDatabase KibanaDatabase::getSqlDatabase() const {
    return m_sqlDatabase;
}

bool KibanaDatabase::QueryDateIsExist(QDate date) {
    QSqlQuery sqlQuery(databaseName);
    QStringList logMsgList;
    QString platformVerSql = "select datestamp from " + tableName +
                             " where datestamp=:date";
    sqlQuery.prepare(platformVerSql);
    sqlQuery.bindValue(":date", date.toString("yyyy_MM_dd"));
    if (!sqlQuery.exec())
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
    if (sqlQuery.next())
        return true;
    return false;
}

void KibanaDatabase::ClearOverWeekRecords(QDate date) {
    QDate minDate(date.addDays(-8));
    QString deleteSql = "delete from " + tableName +
                             " where datestamp>=':date'";
    QSqlQuery sqlQuery(databaseName);
    sqlQuery.prepare(deleteSql);
    sqlQuery.bindValue(":date", minDate.toString("yyyy_MM_dd"));
    if (!sqlQuery.exec())
        qDebug() << __FUNCTION__ << sqlQuery.lastError();
}
