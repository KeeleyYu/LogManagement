#ifndef KIBANADATABASE_H
#define KIBANADATABASE_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtWidgets/QWidget>
#include <QTextCodec>
#include <QString>
#include <QUrl>

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
    void Init();
    bool CreateDatabase();
    bool InsertDatabase(QJsonObject rootObj);
    void SplitJsonFromRecvData(QByteArray recvData);
    QString getDatabaseName() const;
    QSqlDatabase getSqlDatabase() const;

private Q_SLOTS:
    void slot_replyFinished(QNetworkReply *reply);

private:
    // 表名："LogInfo"
    // 数据库名字："KibanaLog.db"
    QString databaseName;
    QNetworkAccessManager *m_manager;
    QSqlDatabase m_sqlDatabase;
};
#endif // KIBANADATABASE_H
