#include "networktools.h"

#include <utility>

using namespace across::network;

DNSTools::DNSTools(QObject *parent) : QObject(parent) {}

DNSTools::DNSTools(const QString &domain, QDnsLookup::Type type,
                   const QHostAddress &dns_server) {
    query(domain, type, dns_server);
}

void DNSTools::query(const QString &domain, QDnsLookup::Type type,
                     const QHostAddress &dns_server) {

    p_dns = std::make_shared<QDnsLookup>(new QDnsLookup(this));
    p_dns->setType(type);
    p_dns->setName(domain);
    p_dns->setNameserver(dns_server);

    connect(p_dns.get(), SIGNAL(finished()), this, SLOT(callback()));

    p_dns->lookup();
}

QList<QString> DNSTools::callback() {
    if (p_dns == nullptr) {
        return {};
    }

    if (p_dns->error() != QDnsLookup::NoError) {
        return {};
    }

    QList<QString> list;

    switch (p_dns->type()) {
    case QDnsLookup::A:
    case QDnsLookup::AAAA:
        for (auto &&record : p_dns->hostAddressRecords()) {
            list.append(record.value().toString());
        }
        break;
    case QDnsLookup::CNAME:
        for (auto &&record : p_dns->canonicalNameRecords()) {
            list.append(record.value());
        }
        break;
    case QDnsLookup::NS:
        for (auto &&record : p_dns->nameServerRecords()) {
            list.append(record.value());
        }
        break;
    case QDnsLookup::MX:
        for (auto &&record : p_dns->mailExchangeRecords()) {
            list.append(record.exchange());
        }
        break;
    case QDnsLookup::TXT:
        for (auto &&record : p_dns->textRecords()) {
            for (auto &&value : record.values()) {
                list.append(value);
            }
        }
        break;
    default:
        break;
    }

    return list;
}

CURLTools::CURLTools(QObject *parent) : QObject(parent) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

CURLTools::~CURLTools() {
    for (auto &task : m_tasks)
        task.cancel();

    m_tasks.clear();

    curl_global_cleanup();
}

CURLcode CURLTools::download(const DownloadTask &task) {
    m_tasks.enqueue(QtConcurrent::run([this, task] {
        auto temp_task = task;

        // create buffer
        std::stringstream buffer;

        // create handle
        CURL *handle = curl_easy_init();

        // download setting
        curl_easy_setopt(handle, CURLOPT_URL,
                         temp_task.url.toStdString().c_str());
        if (!temp_task.user_agent.isEmpty()) {
            curl_easy_setopt(handle, CURLOPT_USERAGENT,
                             temp_task.user_agent.toStdString().c_str());
        }
        if (!temp_task.proxy.isEmpty()) {
            curl_easy_setopt(handle, CURLOPT_PROXY,
                             temp_task.proxy.toStdString().c_str());
        }

        // data callback
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &dataCallback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);

        // execute
        if (auto err = curl_easy_perform(handle); err == CURLE_OK) {
            temp_task.content = QString::fromStdString(buffer.str());
        }

        // clean handle
        curl_easy_cleanup(handle);

        emit handleResult(QVariant::fromValue<DownloadTask>(temp_task));
    }));

    return CURLE_OK;
}

void CURLTools::handleResult(const QVariant &content) {
    emit downloadFinished(content);
}
size_t CURLTools::dataCallback(void *contents, size_t size, size_t nmemb,
                               void *p_data) {
    size_t real_size = size * nmemb;
    auto stream = reinterpret_cast<std::stringstream *>(p_data);
    stream->write(reinterpret_cast<const char *>(contents), real_size);
    return real_size;
}

QString UpdateTools::getVersion(const QString &content) {
    Json::string_t err_msg;
    Json root;

    try {
        root = Json::parse(content.toStdString());
    } catch (Json::exception &e) {
        qDebug() << e.what();
        return "";
    }

    if (!root.is_null() && !root.empty() && !root.dump().empty()) {
        if (root.is_array()) {
            root = root.at(0);
        }

        if (auto tag = root["tag_name"];
            !tag.is_null() && tag.is_string() && !tag.empty()) {
            auto result = QString::fromStdString(tag.get<std::string>());
            return result.remove("v");
        }
    }

    return "";
}

int UpdateTools::compareVersion(const QString &ver_a, const QString &ver_b) {
    // Output values
    //   : <0 : if ver1<ver2 0
    //   : if ver1 == ver2> 0 : if ver1>
    //   ver2

    constexpr auto qstr2ver = [](QString qstr_ver) -> semver::version {
        auto std_str = qstr_ver.remove("v").toStdString();
        std::string_view view_str(std_str.data(), std_str.size());

        if (semver::valid(view_str))
            return semver::version(view_str);

        return {};
    };

    if (auto v_a = qstr2ver(ver_a), v_b = qstr2ver(ver_b); v_a < v_b)
        return -1;
    else if (v_a == v_b)
        return 0;
    else
        return 1;
}

QStringList UpdateTools::getNews(const QString &content) {
    if (content.isEmpty())
        return {};

    Json::string_t err_msg;
    Json root;
    QStringList news;

    try {
        root = Json::parse(content.toStdString());
    } catch (Json::exception &e) {
        qDebug() << e.what();
        return {e.what()};
    }

    if (!root.is_null() && !root.empty() && !root.dump().empty()) {
        if (root.is_array()) {
            for (auto item : root) {
                if (auto body = item["body"];
                    !body.is_null() && body.is_string()) {
                    auto split_str =
                        QString("### %1 \n\n")
                            .arg(item["tag_name"].get<std::string>().c_str());
                    QString body_str = body.get<std::string>().c_str();
                    split_str.append(body_str);

                    news.emplace_back(split_str);
                }
            }
        } else if (auto body = root["body"];
                   !body.is_null() && body.is_string()) {
            news.emplace_back(QString::fromStdString(body.get<std::string>()));
        }
    }

    return news;
}
