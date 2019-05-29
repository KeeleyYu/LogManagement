#include "KibanaDatabase.h"

KibanaDatabase::KibanaDatabase(QWidget *parent)
    : QWidget(parent)
{
}

KibanaDatabase::~KibanaDatabase()
{

}

void KibanaDatabase::Init() {
    m_manager = new QNetworkAccessManager();
    databaseName= "KibanaLog.db";

    qDebug() << "creating database...";
    CreateDatabase();

    QString kibana_url = "http://myou.cvte.com/grail/api/query/sql";
    QString query_url = "?sql=SELECT * FROM log_grail_pro_cros_28762406_";

    // 默认获取过去7天日志，并保存到本地数据库
    for (int i = 0; i < 7; i++) {
        QString date = QDate::currentDate().addDays(-i).toString("yyyy_MM_dd");
        qDebug() << "Getting date on " + date;

        QString final_url = kibana_url + query_url + date;
        QNetworkRequest request;
        request.setUrl(final_url);
        request.setRawHeader("X-DATA-ID", "cros_28762406");

        // 发送请求
        m_manager->get(request);
    }

    // 信号
    connect(m_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(slot_replyFinished(QNetworkReply*)));
}

void KibanaDatabase::slot_replyFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForName("utf8");
        //使用utf8编码, 这样可以显示中文
    QByteArray recvData = reply->readAll();
    QString recvStr = codec->toUnicode(recvData);
    qDebug() << recvStr;
    reply->deleteLater();//最后要释放replay对象
    qDebug() << "received data size: " << recvData.size();
    qDebug() << "transferred to str size: " << recvStr.size();

    qDebug() << "Parsing data...";
    SplitJsonFromRecvData(recvData);
    qDebug() << "Parsing data finished.";
}

void KibanaDatabase::SplitJsonFromRecvData(QByteArray recvData) {
    // 预处理
    recvData = recvData.mid(recvData.indexOf("[") + 1, recvData.lastIndexOf("]") - recvData.indexOf("[") - 1);
    int startIndex = 0, endIndex = 0;
    // 截取当前JSON
    for (; startIndex != -1; startIndex = recvData.indexOf("{", endIndex)) {
        endIndex = recvData.indexOf("}}", startIndex);
        //QString curData = recvData.mid(startIndex, endIndex - startIndex + 2);
        QByteArray curData;
        for (int i = startIndex; i < endIndex + 2; i++) {
            if (recvData[i] == '\\')
                    continue;
            curData.append(recvData[i]);
        }
        //qDebug() << curData;
        // 转化为JSON对象
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(curData, &jsonError));
        if(jsonError.error != QJsonParseError::NoError) {
            qDebug() << curData;
            qDebug() << jsonError.errorString();
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
    QString insertSql = "INSERT INTO LogInfo"
                        "(_id, platformVer, logLevel, logMsg) "
                        "VALUES(?, ?, ?, ?)";
    jsonQuery.prepare(insertSql);
    jsonQuery.addBindValue(rootObj.value("_id").toString());
    jsonQuery.addBindValue(subObj.value("platformVer").toString());
    jsonQuery.addBindValue(subObj.value("logLevel").toString());
    jsonQuery.addBindValue(subObj.value("logMsg").toString());
    if (!jsonQuery.exec()) {
        //qDebug() << "Insert db error!" << jsonQuery.lastError();
        return false;
    }
    //qDebug() << "Insert db succeed.";
    return true;
}

bool KibanaDatabase::CreateDatabase() {
    // create database
    m_sqlDatabase = QSqlDatabase::addDatabase("QSQLITE");
    m_sqlDatabase.setDatabaseName(databaseName);
    if (!m_sqlDatabase.open()) {
        qDebug() << "open m_sqlDatabase error!";
        return false;
    }

    // create table
    QSqlQuery jsonQuery;
    if (!jsonQuery.exec("create table LogInfo ("
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

QSqlDatabase KibanaDatabase::getSqlDatabase() const {
    return m_sqlDatabase;
}
