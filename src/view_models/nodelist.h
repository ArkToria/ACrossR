#ifndef NODELIST_H
#define NODELIST_H

#include "../models/acolorstools.h"
#include "../models/apitools.h"
#include "../models/clipboardtools.h"
#include "../models/coretools.h"
#include "../models/dbtools.h"
#include "../models/notifytools.h"
#include "../models/qrcodetools.h"
#include "../models/serializetools.h"

#include "configtools.h"
#include "jsonhighlighter.h"
#include "logtools.h"
#include "notificationmodel.h"

#include "magic_enum.hpp"
#include <QObject>
#include <QPointer>
#include <QQuickTextDocument>
#include <QSharedPointer>
#include <QSystemTrayIcon>
#include <QUrl>
#include <QVariant>
#include <QtConcurrent>
#include <functional>

namespace across {
using Json = nlohmann::json;

class NodeList : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        qint64 currentNodeID READ currentNodeID NOTIFY currentNodeIDChanged)
    Q_PROPERTY(
        qint64 currentGroupID READ currentGroupID NOTIFY currentGroupIDChanged)
    Q_PROPERTY(qint64 displayGroupID READ displayGroupID WRITE setDisplayGroupID
                   NOTIFY displayGroupIDChanged)
    Q_PROPERTY(
        QString uploadTraffic READ uploadTraffic NOTIFY uploadTrafficChanged)
    Q_PROPERTY(QString downloadTraffic READ downloadTraffic NOTIFY
                   downloadTrafficChanged)
    Q_PROPERTY(QVariantMap currentNodeInfo READ currentNodeInfo NOTIFY
                   currentNodeInfoChanged)
  public:
    explicit NodeList(QObject *parent = nullptr);

    ~NodeList() override;

    void init(QSharedPointer<across::setting::ConfigTools> config,
              QSharedPointer<across::core::CoreTools> core,
              QSharedPointer<across::acolorsapi::AColoRSAPITools> acolors,
              QSharedPointer<across::NotificationModel> notifications,
              const QSharedPointer<QSystemTrayIcon> &tray = nullptr);

    bool run();

    QString generateConfig();

    void setFilter(const QMap<qint64, QList<qint64>> &search_results);
    void clearFilter();

    void clearItems();

    void appendNode(NodeInfo node);
    void updateNode(NodeInfo node);

    void setUploadTraffic(double newUploadTraffic);
    void setDownloadTraffic(double newDownloadTraffic);

    void testLatency(
        const NodeInfo &node, int index, std::function<void()> after = [] {});

    void setDownloadProxy(across::network::DownloadTask &task);

    bool isRunning();

    Q_INVOKABLE void setAsDefault(int id);
    Q_INVOKABLE void setCurrentNodeByID(int id);
    Q_INVOKABLE void removeNodeByID(int id);
    Q_INVOKABLE QVariantMap getNodeInfoByIndex(int index);
    Q_INVOKABLE qint64 getIndexByNode(qint64 node_id, qint64 group_id);

    Q_INVOKABLE void testLatency(int id);
    Q_INVOKABLE QString getQRCode(int node_id, int group_id);
    Q_INVOKABLE void saveQRCodeToFile(int id, const QUrl &url);
    Q_INVOKABLE void copyURLToClipboard(const QString &node_name,
                                        const QString &node_url);
    Q_INVOKABLE void copyCurrentNodeURLToClipboard();

    Q_INVOKABLE void setDocument(QVariant &v);
    Q_INVOKABLE static QString jsonFormat(const QString &json_str);

  public:
    void reloadItems();

    QList<NodeInfo> items();
    [[nodiscard]] qint64 currentNodeID() const;
    [[nodiscard]] qint64 currentGroupID() const;
    [[nodiscard]] qint64 displayGroupID() const;
    [[nodiscard]] QString uploadTraffic() const;
    [[nodiscard]] QString downloadTraffic() const;
    QVariantMap currentNodeInfo();

  public slots:
    void setDisplayGroupID(int group_id);
    void handleLatencyChanged(qint64 group_id, int index,
                              const across::NodeInfo &node);
    void resetCurrentNode();

  signals:
    void itemReset(int index);
    void itemLatencyChanged(qint64 group_id, int index, across::NodeInfo node);

    void preItemsReset();
    void postItemsReset();
    void groupSizeChanged(qint64 group_id, int size);

    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void currentNodeIDChanged();
    void currentGroupIDChanged();
    void displayGroupIDChanged();

    void nodeLatencyChanged(int id, const QString &group, int latency);

    void updateQRCode(const QString &id, const QString &content);
    void uploadTrafficChanged(const QString &uploadTraffic);
    void downloadTrafficChanged(const QString &downloadTraffic);
    void currentNodeInfoChanged(const QVariantMap &nodeModel);
    void currentNodeChanged(const NodeInfo &nodeInfo);

  private:
    std::shared_ptr<spdlog::logger> p_logger;
    QSharedPointer<across::acolorsapi::AColoRSAPITools> p_acolors;
    QSharedPointer<across::core::APITools> p_api;
    QSharedPointer<across::setting::ConfigTools> p_config;
    QSharedPointer<across::core::CoreTools> p_core;
    QSharedPointer<across::NotificationModel> p_notifications;
    QSharedPointer<QSystemTrayIcon> p_tray;

    QQueue<QFuture<void>> m_tasks;

    across::JSONHighlighter jsonHighlighter;

    NodeInfo m_node;
    QList<NodeInfo> m_nodes;
    QList<NodeInfo> m_origin_nodes;
    qint64 m_display_group_id = 1;

    QMap<qint64, QList<qint64>> m_search_results;

    across::core::TrafficInfo m_traffic = {0, 0};
    across::core::TrafficInfo m_traffic_last = {0, 0};
    across::core::TrafficInfo m_traffic_last_rate = {0, 0};
    across::core::TrafficInfo m_traffic_sum = {0, 0};
};
} // namespace across

#endif // NODELIST_H
