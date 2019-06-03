#ifndef KIBANADATABASE_H
#define KIBANADATABASE_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtWidgets/QWidget>
#include <QTextCodec>
#include <QString>
#include <QUrl>
#include <QDate>
#include <QDateTime>

#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

class KibanaDatabase: public QWidget
{
    Q_OBJECT

public:
    KibanaDatabase(QWidget *parent = 0);
    ~KibanaDatabase();

    void QueryByDate(QDate fromDate, QDate toDate);
    bool QueryDateIsExist(QDate date);

    bool CreateDatabase();
    bool InsertDatabase(QJsonObject rootObj);
    void SplitJsonFromRecvData(QByteArray recvData);

    QString getDatabaseName() const;
    QString getTableName() const;
    QSqlDatabase getSqlDatabase() const;

    void ClearOverWeekRecords();

private Q_SLOTS:
    void slot_replyFinished(QNetworkReply *reply);

private:
    // 表名（not used）
    QString tableName;
    // 数据库名字
    QString databaseName;
    // 日志入口链接
    QString kibana_url;
    // 查询链接
    QString query_url;
    // X-DATA-ID头部信息
    QString xDataId;
    // 设备名称：
    QString deviceName;

    QNetworkAccessManager *m_manager;
    QSqlDatabase m_sqlDatabase;
};
#endif // KIBANADATABASE_H
