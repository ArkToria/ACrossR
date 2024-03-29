#include "acolorstools.h"

using namespace across::acolorsapi;
using namespace acolors;

AColoRSNotifications::AColoRSNotifications(
    const std::shared_ptr<Channel> &channel) {
    this->p_channel = channel;
    this->p_stub = Notifications::NewStub(p_channel);
}
AColoRSNotifications::~AColoRSNotifications() { this->stop(); }

void AColoRSNotifications::start() {
    if (this->state)
        return;
    this->context = std::make_unique<ClientContext>();

    this->future = QtConcurrent::run([&] {
        this->setState(true);
        GetNotificationsRequest request;
        std::unique_ptr<grpc::ClientReader<AColorSignal>> reader(
            p_stub->GetNotifications(this->context.get(), request));
        AColorSignal reply;
        bool shutdown = false;
        while (reader->Read(&reply)) {
            auto signal_case = reply.signal_case();
            switch (signal_case) {
            case AColorSignal::SignalCase::kEmpty:
                emit this->empty();
                break;
            case AColorSignal::SignalCase::kAppendGroup: {
                auto group_id = reply.append_group().group_id();
                emit this->appendGroup(group_id);
            } break;
            case AColorSignal::SignalCase::kUpdateCoreStatus:
                emit this->updateCoreStatus();
                break;
            case AColorSignal::SignalCase::kUpdateInbounds:
                emit this->updateInbounds();
                break;
            case AColorSignal::SignalCase::kCoreConfigChanged:
                emit this->coreConfigChanged();
                break;
            case AColorSignal::SignalCase::kCoreChanged:
                emit this->coreChanged();
                break;
            case AColorSignal::SignalCase::kRemoveGroupById: {
                auto group_id = reply.remove_group_by_id().group_id();
                emit this->removeGroupByID(group_id);
            } break;
            case AColorSignal::SignalCase::kRemoveNodeById: {
                auto node_id = reply.remove_node_by_id().node_id();
                emit this->removeNodeByID(node_id);
            } break;
            case AColorSignal::SignalCase::kSetGroupById: {
                auto group_id = reply.set_group_by_id().group_id();
                emit this->setGroupByID(group_id);
            } break;
            case AColorSignal::SignalCase::kSetNodeById: {
                auto node_id = reply.set_node_by_id().node_id();
                emit this->setNodeByID(node_id);
            } break;
            case AColorSignal::SignalCase::kAppendNode: {
                auto group_id = reply.append_node().group_id();
                auto node_id = reply.append_node().node_id();
                emit this->appendNode(group_id, node_id);
            } break;
            case AColorSignal::SignalCase::kUpdateGroup: {
                auto group_id = reply.update_group().group_id();
                emit this->updateGroup(group_id);
            } break;
            case AColorSignal::SignalCase::kRuntimeValueChanged: {
                std::string key = reply.runtime_value_changed().key();
                emit this->runtimeValueChanged(std::move(key));
            } break;
            case AColorSignal::SignalCase::kEmptyGroup: {
                auto group_id = reply.empty_group().group_id();
                emit this->emptyGroup(group_id);
            } break;
            case AColorSignal::SignalCase::kSetApiStatus:
                emit this->setApiStatus();
                break;
            case AColorSignal::SignalCase::kShutdown: {
                shutdown = true;
                emit this->shutdown();
            } break;
            default:
                qDebug() << "Not Implemented";
                break;
            }
            if (shutdown)
                break;
        }
        this->setState(false);
    });
}
void AColoRSNotifications::setState(bool state) {
    if (this->state == state)
        return;
    this->state = state;
    emit this->stateChanged();
}
bool AColoRSNotifications::getState() { return this->state; }

void AColoRSNotifications::stop() {
    if (this->state)
        this->context->TryCancel();
    this->future.waitForFinished();
}

void AColoRSNotifications::setChannel(const std::shared_ptr<Channel> &channel) {
    bool flag = false;
    if (this->state) {
        this->stop();
        flag = true;
    }
    this->p_channel = channel;
    this->p_stub = Notifications::NewStub(p_channel);

    if (flag) {
        this->start();
    }
    emit channelChanged();
}

AColoRSProfile::AColoRSProfile(const std::shared_ptr<Channel> &channel) {
    this->p_channel = channel;
    this->p_stub = ProfileManager::NewStub(p_channel);
}

AColoRSProfile::~AColoRSProfile() {}

pair<uint64_t, Status> AColoRSProfile::countGroups() {
    ClientContext context;
    CountGroupsRequest request;
    CountGroupsReply reply;
    auto status = this->p_stub->CountGroups(&context, request, &reply);
    return std::make_pair(reply.count(), status);
}
pair<uint64_t, Status> AColoRSProfile::countNodes(int32_t group_id) {
    ClientContext context;
    CountNodesRequest request;
    request.set_group_id(group_id);
    CountNodesReply reply;
    auto status = this->p_stub->CountNodes(&context, request, &reply);
    return std::make_pair(reply.count(), status);
}

pair<QList<across::GroupInfo>, Status> AColoRSProfile::listAllGroups() {
    ClientContext context;
    ListAllGroupsRequest request;
    GroupList reply;
    auto status = this->p_stub->ListAllGroups(&context, request, &reply);
    auto result = this->groupListFrom(reply);
    return std::make_pair(result, status);
}
pair<QList<across::NodeInfo>, Status>
AColoRSProfile::listAllNodes(int32_t group_id) {
    ClientContext context;
    ListAllNodesRequest request;
    request.set_group_id(group_id);
    NodeList reply;
    auto status = this->p_stub->ListAllNodes(&context, request, &reply);
    auto result = nodeListFrom(reply);
    return std::make_pair(result, status);
}

pair<across::GroupInfo, Status> AColoRSProfile::getGroupById(int32_t group_id) {
    ClientContext context;
    GetGroupByIDRequest request;
    request.set_group_id(group_id);
    GroupData reply;
    auto status = this->p_stub->GetGroupByID(&context, request, &reply);
    auto result = this->groupFrom(reply);
    return std::make_pair(result, status);
}

pair<across::NodeInfo, Status> AColoRSProfile::getNodeById(int32_t node_id) {
    ClientContext context;
    GetNodeByIDRequest request;
    request.set_node_id(node_id);
    NodeData reply;
    auto status = this->p_stub->GetNodeByID(&context, request, &reply);
    auto result = nodeFrom(reply);
    return std::make_pair(result, status);
}

Status AColoRSProfile::setGroupById(int32_t group_id, const GroupInfo &data) {
    ClientContext context;
    SetGroupByIDRequest request;
    request.set_group_id(group_id);
    *request.mutable_data() = groupTo(data);
    SetGroupByIDReply reply;
    return this->p_stub->SetGroupByID(&context, request, &reply);
}

Status AColoRSProfile::setGroups(const QList<GroupInfo> &data) {
    for (const auto &group_info : data) {
        auto status = this->setGroupById(group_info.id, group_info);
        if (!status.ok())
            return status;
    }
    return Status::OK;
}
Status AColoRSProfile::setNodeById(int32_t node_id, const NodeInfo &data) {
    ClientContext context;
    SetNodeByIDRequest request;
    request.set_node_id(node_id);
    *request.mutable_data() = nodeTo(data);
    SetNodeByIDReply reply;
    auto result = this->p_stub->SetNodeByID(&context, request, &reply);
    return result;
}

Status AColoRSProfile::setNodeByUrl(int32_t node_id, std::string url) {
    ClientContext context;
    SetNodeByUrlRequest request;
    request.set_node_id(node_id);
    *request.mutable_url() = url;
    SetNodeByUrlReply reply;
    return this->p_stub->SetNodeByUrl(&context, request, &reply);
}

Status AColoRSProfile::removeGroupById(int32_t group_id) {
    ClientContext context;
    RemoveGroupByIDRequest request;
    request.set_group_id(group_id);
    RemoveGroupByIDReply reply;
    return this->p_stub->RemoveGroupByID(&context, request, &reply);
}
Status AColoRSProfile::removeNodeById(int32_t node_id) {
    ClientContext context;
    RemoveNodeByIDRequest request;
    request.set_node_id(node_id);
    RemoveNodeByIDReply reply;
    return this->p_stub->RemoveNodeByID(&context, request, &reply);
}

pair<int64_t, Status> AColoRSProfile::appendGroup(const GroupInfo &data) {
    ClientContext context;
    AppendGroupRequest request;
    *request.mutable_data() = groupTo(data);
    AppendGroupReply reply;
    auto status = this->p_stub->AppendGroup(&context, request, &reply);
    return std::make_pair(reply.group_id(), status);
}
pair<int64_t, Status> AColoRSProfile::appendNode(int32_t group_id,
                                                 const NodeInfo &data) {
    ClientContext context;
    AppendNodeRequest request;
    request.set_group_id(group_id);
    *request.mutable_data() = nodeTo(data);
    AppendNodeReply reply;
    auto status = this->p_stub->AppendNode(&context, request, &reply);
    return std::make_pair(reply.node_id(), status);
}
Status AColoRSProfile::appendNodes(int32_t group_id,
                                   const QList<NodeInfo> &data) {
    for (const auto &node_info : data) {
        auto status = this->appendNode(group_id, node_info).second;
        if (!status.ok())
            return status;
    }
    return Status::OK;
}

pair<int64_t, Status> AColoRSProfile::appendNodeByUrl(int32_t group_id,
                                                      std::string url) {
    ClientContext context;
    AppendNodeByUrlRequest request;
    request.set_group_id(group_id);
    *request.mutable_url() = url;
    AppendNodeByUrlReply reply;
    auto status = this->p_stub->AppendNodeByUrl(&context, request, &reply);
    return std::make_pair(reply.node_id(), status);
}

Status AColoRSProfile::updateGroupById(int32_t group_id, bool use_proxy) {
    ClientContext context;
    UpdateGroupByIDRequest request;
    request.set_group_id(group_id);
    request.set_use_proxy(use_proxy);
    UpdateGroupByIDReply reply;
    return this->p_stub->UpdateGroupByID(&context, request, &reply);
}

Status AColoRSProfile::emptyGroupById(int32_t group_id) {
    ClientContext context;
    EmptyGroupByIDRequest request;
    request.set_group_id(group_id);
    EmptyGroupByIDReply reply;
    return this->p_stub->EmptyGroupByID(&context, request, &reply);
}

across::GroupInfo AColoRSProfile::groupFrom(const acolors::GroupData &data) {
    across::GroupInfo result;
    result.id = data.id();
    result.name = QString::fromStdString(data.name());
    result.is_subscription = data.is_subscription();
    switch (data.group_type()) {
    case acolors::SubscriptionType::BASE64:
        result.type = SubscriptionType::base64;
        break;
    case acolors::SubscriptionType::SIP008:
        result.type = SubscriptionType::sip008;
        break;
    case acolors::SubscriptionType::JSONRAW:
        result.type = SubscriptionType::jsonraw;
        break;
    default:
        break;
    }
    result.url = QString::fromStdString(data.url());
    result.cycle_time = data.cycle_time();
    result.created_time = QDateTime::fromSecsSinceEpoch(data.create_at());
    result.modified_time = QDateTime::fromSecsSinceEpoch(data.modified_at());
    auto count = this->countNodes(int32_t(result.id));
    result.items = int(count.first);
    return result;
}

across::NodeInfo AColoRSProfile::nodeFrom(const acolors::NodeData &data) {
    across::NodeInfo result;
    result.id = data.id();
    result.name = QString::fromStdString(data.name());
    result.group_id = data.group_id();
    result.group_name = QString::fromStdString(data.group_name());
    result.routing_id = data.routing_id();
    result.routing_name = QString::fromStdString(data.routing_name());
    switch (data.protocol()) {
    case acolors::EntryType::VMESS:
        result.protocol = across::EntryType::vmess;
        break;
    case acolors::EntryType::SHADOWSOCKS:
        result.protocol = across::EntryType::shadowsocks;
        break;
    case acolors::EntryType::TROJAN:
        result.protocol = across::EntryType::trojan;
        break;
    case acolors::EntryType::RAW:
        result.protocol = across::EntryType::raw;
        break;
    case acolors::EntryType::SCHEME:
        result.protocol = across::EntryType::scheme;
        break;
    case acolors::EntryType::UNKNOWN:
        result.protocol = across::EntryType::unknown;
        break;
    case acolors::EntryType::NAIVEPROXY:
        result.protocol = across::EntryType::naiveproxy;
        break;

    default:
        break;
    }
    result.address = QString::fromStdString(data.address());
    result.port = data.port();
    result.password = QString::fromStdString(data.password());
    result.raw = QString::fromStdString(data.raw());
    result.url = QString::fromStdString(data.url());
    result.latency = data.latency();
    result.upload = data.upload();
    result.download = data.download();
    result.created_time = QDateTime::fromSecsSinceEpoch(data.create_at());
    result.modified_time = QDateTime::fromSecsSinceEpoch(data.modified_at());
    return result;
}

AColoRSCore::AColoRSCore(const std::shared_ptr<Channel> &channel,
                         const std::shared_ptr<AColoRSProfile> &profile) {
    this->p_channel = channel;
    this->p_profile = profile;
    this->p_stub = CoreManager::NewStub(p_channel);
}

AColoRSCore::~AColoRSCore() {}

Status AColoRSCore::run() {
    ClientContext context;
    RunRequest request;
    RunReply reply;
    return this->p_stub->Run(&context, request, &reply);
}

Status AColoRSCore::stop() {
    ClientContext context;
    StopRequest request;
    StopReply reply;
    return this->p_stub->Stop(&context, request, &reply);
}

Status AColoRSCore::restart() {
    ClientContext context;
    RestartRequest request;
    RestartReply reply;
    return this->p_stub->Restart(&context, request, &reply);
}

pair<bool, Status> AColoRSCore::isRunning() {
    ClientContext context;
    GetIsRunningRequest request;
    GetIsRunningReply reply;
    auto status = this->p_stub->GetIsRunning(&context, request, &reply);
    return std::make_pair(reply.is_running(), status);
}

pair<across::NodeInfo, Status> AColoRSCore::currentNode() {
    ClientContext context;
    GetCurrentNodeRequest request;
    NodeData reply;
    auto status = this->p_stub->GetCurrentNode(&context, request, &reply);
    auto result = AColoRSProfile::nodeFrom(reply);
    return std::make_pair(result, status);
}

Status AColoRSCore::setConfigByNodeId(int32_t node_id) {
    ClientContext context;
    SetConfigByNodeIDRequest request;
    request.set_node_id(node_id);
    SetConfigByNodeIDReply reply;
    return this->p_stub->SetConfigByNodeID(&context, request, &reply);
}

QList<across::GroupInfo>
AColoRSProfile::groupListFrom(const acolors::GroupList &group_list) {
    QList<across::GroupInfo> result;
    auto length = group_list.length();
    for (const auto &group : group_list.entries()) {
        result.push_back(this->groupFrom(group));
    }
    return result;
}
QList<across::NodeInfo>
AColoRSProfile::nodeListFrom(const acolors::NodeList &node_list) {
    QList<across::NodeInfo> result;
    auto length = node_list.length();
    for (const auto &node : node_list.entries()) {
        result.push_back(nodeFrom(node));
    }
    return result;
}

acolors::GroupData AColoRSProfile::groupTo(const GroupInfo &data) {
    acolors::GroupData result;
    result.set_id(int32_t(data.id));
    result.set_name(data.name.toStdString());
    result.set_is_subscription(data.is_subscription);
    switch (data.type) {
    case SubscriptionType::base64:
        result.set_group_type(acolors::SubscriptionType::BASE64);
        break;
    case SubscriptionType::sip008:
        result.set_group_type(acolors::SubscriptionType::SIP008);
        break;
    case SubscriptionType::jsonraw:
        result.set_group_type(acolors::SubscriptionType::JSONRAW);
        break;
    default:
        break;
    }
    result.set_url(data.url.toStdString());
    result.set_cycle_time(data.cycle_time);
    result.set_create_at(data.created_time.toSecsSinceEpoch());
    result.set_modified_at(data.modified_time.toSecsSinceEpoch());
    return result;
}

acolors::NodeData AColoRSProfile::nodeTo(const NodeInfo &data) {
    acolors::NodeData result;
    result.set_id(int32_t(data.id));
    result.set_name(data.name.toStdString());
    result.set_group_id(int32_t(data.group_id));
    result.set_group_name(data.group_name.toStdString());
    result.set_routing_id(int32_t(data.routing_id));
    result.set_routing_name(data.routing_name.toStdString());
    switch (data.protocol) {
    case across::EntryType::vmess:
        result.set_protocol(acolors::EntryType::VMESS);
        break;
    case across::EntryType::shadowsocks:
        result.set_protocol(acolors::EntryType::SHADOWSOCKS);
        break;
    case across::EntryType::trojan:
        result.set_protocol(acolors::EntryType::TROJAN);
        break;
    case across::EntryType::raw:
        result.set_protocol(acolors::EntryType::RAW);
        break;
    case across::EntryType::scheme:
        result.set_protocol(acolors::EntryType::SCHEME);
        break;
    case across::EntryType::unknown:
        result.set_protocol(acolors::EntryType::UNKNOWN);
        break;
    case across::EntryType::naiveproxy:
        result.set_protocol(acolors::EntryType::NAIVEPROXY);
        break;

    default:
        break;
    }
    result.set_address(data.address.toStdString());
    result.set_port(int32_t(data.port));
    result.set_password(data.password.toStdString());
    result.set_raw(data.raw.toStdString());
    result.set_url(data.url.toStdString());
    result.set_latency(int32_t(data.latency));
    result.set_upload(data.upload);
    result.set_download(data.download);
    result.set_create_at(data.created_time.toSecsSinceEpoch());
    result.set_modified_at(data.modified_time.toSecsSinceEpoch());
    return result;
}

void AColoRSProfile::setChannel(const std::shared_ptr<Channel> &channel) {
    this->p_channel = channel;
    this->p_stub = ProfileManager::NewStub(p_channel);
    emit channelChanged();
}

Status AColoRSCore::setCoreByTag(std::string tag) {
    ClientContext context;
    SetCoreByTagRequest request;
    *request.mutable_tag() = tag;
    SetCoreByTagReply reply;
    return this->p_stub->SetCoreByTag(&context, request, &reply);
}
pair<QList<QString>, Status> AColoRSCore::listAllTags() {
    ClientContext context;
    ListAllTagsRequest request;
    ListAllTagsReply reply;
    QList<QString> tags;
    auto status = this->p_stub->ListAllTags(&context, request, &reply);

    for (const auto &tag : reply.tags()) {
        tags.push_back(QString::fromStdString(tag));
    }
    auto result = std::make_pair(tags, status);
    return result;
}

Status AColoRSCore::setDefaultConfigByNodeId(int32_t node_id) {
    ClientContext context;
    SetDefaultConfigByNodeIDRequest request;
    request.set_node_id(node_id);
    SetDefaultConfigByNodeIDReply reply;
    return this->p_stub->SetDefaultConfigByNodeID(&context, request, &reply);
}
Status AColoRSCore::setApiStatus(bool enable) {
    ClientContext context;
    SetApiStatusRequest request;
    request.set_enable(enable);
    SetApiStatusReply reply;
    return this->p_stub->SetApiStatus(&context, request, &reply);
}

pair<AColoRSCore::CoreInfo, Status> AColoRSCore::getCoreInfo() {
    ClientContext context;
    GetCoreInfoRequest request;
    GetCoreInfoReply reply;
    auto status = this->p_stub->GetCoreInfo(&context, request, &reply);
    auto coreInfo = AColoRSCore::CoreInfo{
        .name = QString::fromStdString(reply.name()),
        .version = QString::fromStdString(reply.version())};
    return std::make_pair(coreInfo, status);
}

void AColoRSCore::setChannel(const std::shared_ptr<Channel> &channel) {
    this->p_channel = channel;
    this->p_stub = CoreManager::NewStub(p_channel);
    emit channelChanged();
}

AColoRSConfig::AColoRSConfig(const std::shared_ptr<Channel> &channel) {
    this->p_channel = channel;
    this->p_stub = ConfigManager::NewStub(p_channel);
}

AColoRSConfig::~AColoRSConfig() {}

Status AColoRSConfig::setInbounds(const Inbounds &inbounds) {
    ClientContext context;
    SetInboundsReply reply;
    return this->p_stub->SetInbounds(&context, inbounds, &reply);
}

pair<Inbounds, Status> AColoRSConfig::getInbounds() {
    ClientContext context;
    GetInboundsRequest request;
    Inbounds reply;
    auto status = this->p_stub->GetInbounds(&context, request, &reply);
    return std::make_pair(reply, status);
}

void AColoRSConfig::setChannel(const std::shared_ptr<Channel> &channel) {
    this->p_channel = channel;
    this->p_stub = ConfigManager::NewStub(p_channel);
    emit channelChanged();
}
AColoRSTools::AColoRSTools(const std::shared_ptr<Channel> &channel) {
    this->p_channel = channel;
    this->p_stub = Tools::NewStub(p_channel);
}

AColoRSTools::~AColoRSTools() {}
void AColoRSTools::setChannel(const std::shared_ptr<Channel> &channel) {
    this->p_channel = channel;
    this->p_stub = Tools::NewStub(p_channel);
    emit channelChanged();
}
pair<int32_t, Status> AColoRSTools::tcping(const std::string &target) {
    ClientContext context;
    TcpingRequest request;
    *request.mutable_target() = target;
    TcpingReply reply;
    auto status = this->p_stub->Tcping(&context, request, &reply);
    return std::make_pair(reply.duration().nanos() / 1000000, status);
}

AColoRSAPITools::AColoRSAPITools(const std::string &target) {
    this->target = target;
    this->p_channel =
        grpc::CreateChannel(this->target, grpc::InsecureChannelCredentials());

    this->p_stub = Manager::NewStub(p_channel);
    this->p_notifications = std::make_unique<AColoRSNotifications>(p_channel);
    this->p_profile = std::make_shared<AColoRSProfile>(p_channel);
    this->p_config = std::make_unique<AColoRSConfig>(p_channel);
    this->p_core = std::make_unique<AColoRSCore>(p_channel, p_profile);
    this->p_tools = std::make_unique<AColoRSTools>(p_channel);

    this->p_notifications->start();

    connect(p_notifications.get(), &AColoRSNotifications::stateChanged, this,
            &AColoRSAPITools::updateConnected);
}
AColoRSAPITools::~AColoRSAPITools() { this->is_reconnect.waitForFinished(); }
void AColoRSAPITools::init(const QString &program, uint32_t port,
                           const QString &corePath, const QString &configPath,
                           const QString &dbPath) {
    this->program = program;
    this->port = port;
    this->corePath = corePath;
    this->configPath = configPath;
    this->dbPath = dbPath;
}

void AColoRSAPITools::updateConnected() {
    auto connected = this->p_notifications->getState();
    if (connected == this->connected)
        return;
    this->connected = connected;
    emit connectedChanged();
}

void AColoRSAPITools::setTarget(const std::string target) {
    this->target = target;
    reconnect();
    emit targetChanged(target);
}

bool AColoRSAPITools::startProcess() {
    QStringList arguments;
    arguments.append(QString("serve"));
    arguments.append(QString("--port"));
    arguments.append(QVariant(port).toString());
    arguments.append(QString("--config"));
    arguments.append(configPath);
    arguments.append(QString("--dbpath"));
    arguments.append(dbPath);
    arguments.append(QString("--corepath"));
    arguments.append(corePath);
    QProcess process;
    process.setProgram(program);
    process.setArguments(arguments);
    auto result = process.startDetached(&this->process_pid);
    return result;
}

Status AColoRSAPITools::shutdown() {
    ClientContext context;
    ShutdownRequest request;
    ShutdownReply reply;
    auto result = this->p_stub->Shutdown(&context, request, &reply);

    return result;
}

void AColoRSAPITools::reconnect() {
    this->p_notifications->stop();
    this->p_channel =
        grpc::CreateChannel(this->target, grpc::InsecureChannelCredentials());

    this->p_stub = Manager::NewStub(p_channel);
    this->p_notifications->setChannel(this->p_channel);
    this->p_profile->setChannel(this->p_channel);
    this->p_config->setChannel(this->p_channel);
    this->p_core->setChannel(this->p_channel);
    this->p_tools->setChannel(this->p_channel);

    this->p_notifications->start();
}

void AColoRSAPITools::restartAColoRS() {
    this->restarting = true;
    this->shutdown();
    if (this->is_reconnect.isFinished()) {
        checkAndReconnect();
    };
}

void AColoRSAPITools::wait(int msec) {
    QTimer timer;
    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(msec);
    loop.exec();
}

void AColoRSAPITools::checkAndReconnect() {
    this->is_reconnect = QtConcurrent::run([&] {
        wait(200);
        if (this->isConnected())
            return;
        this->startProcess();
        for (int i = 0; i < 10; i++) {
            wait(200);
            if (this->isConnected())
                break;
            this->reconnect();
        }
        if (enableAutoConnect) {
            this->core()->setApiStatus(apiEnable);
            auto status = this->core()->run();
            if (!status.ok())
                qDebug() << status.error_message().c_str();
        }
    });
}
void AColoRSAPITools::setConfig(bool enableAutoConnect, bool apiEnable) {
    this->enableAutoConnect = enableAutoConnect;
    this->apiEnable = apiEnable;
}