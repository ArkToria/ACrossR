#include "nodemodel.h"

NodeModel::NodeModel(QObject *parent) : QAbstractListModel(parent)
{
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || p_list == nullptr)
        return 0;
    else
        return p_list->length;
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || p_list == nullptr || index.row() >= rowCount())
        return {};

    const across::core::NodeData item = p_list->entries.at(index.row());

    switch (role)
    {
    case IdRole:
        return QVariant(item.id);
    case NameRole:
        return QVariant(QString::fromStdString(std::string(item.name)));
    case GroupIdRole:
        return QVariant(item.group_id);
    case GroupNameRole:
        return QVariant(QString::fromStdString(std::string(item.group_name)));
    case RoutingIdRole:
        return QVariant(item.routing_id);
    case RoutingNameRole:
        return QVariant(QString::fromStdString(std::string(item.routing_name)));
    case ProtocolRole:
        return QVariant(QString::fromStdString(std::string(item.protocol)));
    case AddressRole:
        return QVariant(QString::fromStdString(std::string(item.address)));
    case PortRole:
        return QVariant(item.port);
    case PasswordRole:
        return QVariant(QString::fromStdString(std::string(item.password)));
    case RawRole:
        return QVariant(QString::fromStdString(std::string(item.raw)));
    case UrlRole:
        return QVariant(QString::fromStdString(std::string(item.url)));
    case LatencyRole:
        return QVariant(item.latency);
    case UploadRole:
        return QVariant(item.upload);
    case DownloadRole:
        return QVariant(item.download);
    case CreatedAtRole:
        return QVariant(item.create_at);
    case ModifiedAtRole:
        return QVariant(item.modified_at);
    default:
        return {};
    }
}

void NodeModel::select(int64_t group_id)
{
    auto node_list = std::make_unique<across::core::NodeList>(across::core::list_all_nodes(group_id));
    this->setList(std::move(node_list));
}
void NodeModel::setList(std::unique_ptr<across::core::NodeList> p_list)
{
    if (!p_list)
        return;

    beginResetModel();

    this->p_list = std::move(p_list);

    endResetModel();
}

bool NodeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags NodeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> NodeModel::roleNames() const
{
    QHash<int, QByteArray> names{
        {IdRole, "id"},
        {NameRole, "name"},
        {GroupIdRole, "groupId"},
        {GroupNameRole, "groupname"},
        {RoutingIdRole, "routingId"},
        {RoutingNameRole, "routingName"},
        {ProtocolRole, "protocol"},
        {AddressRole, "address"},
        {PortRole, "port"},
        {PasswordRole, "password"},
        {RawRole, "raw"},
        {UrlRole, "url"},
        {LatencyRole, "latency"},
        {UploadRole, "upload"},
        {DownloadRole, "download"},
        {CreatedAtRole, "createdAt"},
        {ModifiedAtRole, "modifiedAt"},
    };
    return names;
}
