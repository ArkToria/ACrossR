#ifndef NETWORKTOOLS_H
#define NETWORKTOOLS_H

#include "curl/curl.h"
#include "nlohmann/json.hpp"
#include "semver.hpp"

#include <QDnsLookup>
#include <QFuture>
#include <QHostAddress>
#include <QList>
#include <QObject>
#include <QQueue>
#include <QString>
#include <QTcpSocket>
#include <QThread>
#include <QTime>
#include <QtConcurrent>

#include <iostream>
#include <sstream>
#include <string>

#ifdef Q_OS_WIN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

namespace across {
namespace network {
using Json = nlohmann::json;

class DNSTools : QObject {
    Q_OBJECT;

  public:
    DNSTools(QObject *parent = nullptr);

    DNSTools(const QString &domain, QDnsLookup::Type type,
             const QHostAddress &dns_server = QHostAddress::LocalHost);

    void query(const QString &domain, QDnsLookup::Type type,
               const QHostAddress &dns_server = QHostAddress::LocalHost);

  public slots:
    QList<QString> callback();

  private:
    std::shared_ptr<QDnsLookup> p_dns = nullptr;
};

struct DownloadTask {
    qint64 id;
    QString name;
    QString url;
    QString user_agent;
    QString proxy;
    QString content;
    bool is_updated = false;
};

class CURLTools : public QObject {
    Q_OBJECT
  public:
    CURLTools(QObject *parent = nullptr);

    ~CURLTools();

    CURLcode download(const DownloadTask &task);

  public slots:
    void handleResult(const QVariant &content);

  signals:
    void operate(const QVariant &data);

    void downloadFinished(const QVariant &content);

  private:
    QQueue<QFuture<void>> m_tasks;

    static size_t dataCallback(void *contents, size_t size, size_t nmemb,
                               void *p_data);
};

class UpdateTools {
  public:
    static QString getVersion(const QString &content);
    static int compareVersion(const QString &ver_a, const QString &ver_b);
    static QStringList getNews(const QString &content);
};
} // namespace network
} // namespace across

#endif // NETWORKTOOLS_H
