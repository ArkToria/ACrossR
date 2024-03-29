#ifndef ACOLORSAPITOOLS_H
#define ACOLORSAPITOOLS_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QThread>
#include <QVariant>
#include <QtConcurrent>

#include <atomic>
#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <vector>

#include "acolors.grpc.pb.h"
#include "dbtools.h"

using acolors::NodeData;
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using std::pair;

namespace across::acolorsapi {

class AColoRSNotifications : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isRunning READ getState NOTIFY stateChanged)
  public:
    explicit AColoRSNotifications(const std::shared_ptr<Channel> &channel);

    ~AColoRSNotifications() override;

    Q_INVOKABLE void start();

    Q_INVOKABLE void stop();

    void setChannel(const std::shared_ptr<Channel> &channel);

    bool getState();

  signals:
    void empty();
    void appendGroup(int64_t group_id);
    void updateCoreStatus();
    void updateInbounds();
    void coreConfigChanged();
    void coreChanged();
    void removeGroupByID(int64_t group_id);
    void removeNodeByID(int64_t node_id);
    void setGroupByID(int64_t group_id);
    void setNodeByID(int64_t node_id);
    void appendNode(int64_t group_id, int64_t node_id);
    void updateGroup(int64_t group_id);
    void runtimeValueChanged(std::string key);
    void emptyGroup(int64_t group_id);
    void shutdown();
    void setApiStatus();

    void channelChanged();
    void stateChanged();

  private:
    void setState(bool state);

  private:
    bool state = false;
    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<acolors::Notifications::Stub> p_stub;
    std::unique_ptr<ClientContext> context;

    QFuture<void> future;
};
class AColoRSProfile : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSProfile(const std::shared_ptr<Channel> &channel);
    ~AColoRSProfile() override;

    pair<uint64_t, Status> countGroups();
    pair<QList<GroupInfo>, Status> listAllGroups();
    pair<uint64_t, Status> countNodes(int32_t group_id);
    pair<QList<NodeInfo>, Status> listAllNodes(int32_t group_id);
    pair<GroupInfo, Status> getGroupById(int32_t group_id);
    pair<NodeInfo, Status> getNodeById(int32_t node_id);
    Status setGroupById(int32_t group_id, const GroupInfo &data);
    Status setGroups(const QList<GroupInfo> &data);
    Status setNodeById(int32_t node_id, const NodeInfo &data);
    Status setNodeByUrl(int32_t node_id, std::string url);
    Status removeGroupById(int32_t group_id);
    Status removeNodeById(int32_t node_id);
    pair<int64_t, Status> appendGroup(const GroupInfo &data);
    pair<int64_t, Status> appendNode(int32_t group_id, const NodeInfo &data);
    Status appendNodes(int32_t group_id, const QList<NodeInfo> &data);
    pair<int64_t, Status> appendNodeByUrl(int32_t group_id, std::string url);
    Status updateGroupById(int32_t group_id, bool use_proxy);
    Status emptyGroupById(int32_t group_id);

    GroupInfo groupFrom(const acolors::GroupData &data);
    static NodeInfo nodeFrom(const acolors::NodeData &data);
    QList<GroupInfo> groupListFrom(const acolors::GroupList &group_list);
    static QList<NodeInfo> nodeListFrom(const acolors::NodeList &node_list);
    acolors::GroupData groupTo(const GroupInfo &data);
    static acolors::NodeData nodeTo(const NodeInfo &data);

    void setChannel(const std::shared_ptr<Channel> &channel);

  signals:
    void channelChanged();

  private:
    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<acolors::ProfileManager::Stub> p_stub;
};
class AColoRSCore : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSCore(const std::shared_ptr<Channel> &channel,
                         const std::shared_ptr<AColoRSProfile> &profile);
    ~AColoRSCore() override;

    Status run();
    Status stop();
    Status restart();
    pair<bool, Status> isRunning();
    pair<NodeInfo, Status> currentNode();
    Status setConfigByNodeId(int32_t node_id);
    Status setCoreByTag(std::string tag);
    pair<QList<QString>, Status> listAllTags();
    Status setDefaultConfigByNodeId(int32_t node_id);
    Status setApiStatus(bool enable);

    struct CoreInfo {
        QString name;
        QString version;
    };
    pair<CoreInfo, Status> getCoreInfo();

    void setChannel(const std::shared_ptr<Channel> &channel);

  signals:
    void channelChanged();

  private:
    std::shared_ptr<Channel> p_channel;
    std::shared_ptr<AColoRSProfile> p_profile;
    std::unique_ptr<acolors::CoreManager::Stub> p_stub;
};
class AColoRSConfig : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSConfig(const std::shared_ptr<Channel> &channel);
    ~AColoRSConfig() override;

    Status setInbounds(const acolors::Inbounds &inbounds);
    pair<acolors::Inbounds, Status> getInbounds();

    void setChannel(const std::shared_ptr<Channel> &channel);

  signals:
    void channelChanged();

  private:
    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<acolors::ConfigManager::Stub> p_stub;
};
class AColoRSTools : public QObject {
    Q_OBJECT
  public:
    explicit AColoRSTools(const std::shared_ptr<Channel> &channel);
    ~AColoRSTools() override;

    Status setDefaultConfigByNodeId(int32_t node_id);

    void setChannel(const std::shared_ptr<Channel> &channel);
    pair<int32_t, Status> tcping(const std::string &target);

  signals:
    void channelChanged();

  private:
    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<acolors::Tools::Stub> p_stub;
};
class AColoRSAPITools : public QObject {
    Q_OBJECT
    Q_PROPERTY(AColoRSNotifications notifications READ notifications NOTIFY
                   notificationsChanged)
  public:
    explicit AColoRSAPITools(const std::string &target);

    ~AColoRSAPITools() override;

    [[nodiscard]] inline AColoRSNotifications *notifications() const {
        return this->p_notifications.get();
    };
    [[nodiscard]] inline AColoRSProfile *profile() const {
        return this->p_profile.get();
    };
    [[nodiscard]] inline AColoRSConfig *config() const {
        return this->p_config.get();
    };
    [[nodiscard]] inline AColoRSCore *core() const {
        return this->p_core.get();
    };
    [[nodiscard]] inline AColoRSTools *tools() const {
        return this->p_tools.get();
    };
    [[nodiscard]] inline const std::shared_ptr<Channel> &channel() const {
        return this->p_channel;
    };

    void init(const QString &program, uint32_t port, const QString &corePath,
              const QString &configPath, const QString &dbPath);

    bool isConnected() const { return this->connected; };

    bool startProcess();

    void setTarget(const std::string target);

    Q_INVOKABLE Status shutdown();

    void wait(int msec);

    void checkAndReconnect();
    void setConfig(bool enableAutoConnect, bool apiEnable);

    Q_INVOKABLE void reconnect();

  signals:
    void targetChanged(const std::string &target);
    void notificationsChanged();
    void connectedChanged();

  public slots:
    Q_INVOKABLE void restartAColoRS();

  private slots:
    void updateConnected();

  public:
    bool restarting = false;

  private:
    qint64 process_pid = -1;
    std::string target;
    bool connected = false;

    bool enableAutoConnect;
    bool apiEnable;

    QFuture<void> is_reconnect;

    QString program;
    uint32_t port;
    QString corePath;
    QString configPath;
    QString dbPath;

    std::shared_ptr<Channel> p_channel;
    std::unique_ptr<acolors::Manager::Stub> p_stub;
    std::unique_ptr<AColoRSNotifications> p_notifications;
    std::shared_ptr<AColoRSProfile> p_profile;
    std::unique_ptr<AColoRSConfig> p_config;
    std::unique_ptr<AColoRSCore> p_core;
    std::unique_ptr<AColoRSTools> p_tools;
};
} // namespace across::acolorsapi

#endif // ACOLORSAPITOOLS_H
