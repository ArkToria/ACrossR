#include "groupmodel.h"

GroupModel::GroupModel(QObject *parent) : QAbstractListModel(parent)
{
}

int GroupModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || p_list == nullptr)
        return 0;
    else
        return p_list->length;
}

QVariant GroupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || p_list == nullptr || index.row() >= rowCount())
        return {};

    const across::profile::GroupData item = p_list->entries.at(index.row());

    switch (role)
    {
    case IdRole:
        return QVariant(item.id);
    case NameRole:
        return QVariant(QString::fromStdString(std::string(item.name)));
    case IsSubscriptionRole:
        return QVariant(item.is_subscription);
    case GroupTypeRole:
        return QVariant(item.group_type);
    case UrlRole:
        return QVariant(QString::fromStdString(std::string(item.url)));
    case CycleTimeRole:
        return QVariant(item.cycle_time);
    case ModifiedAtRole:
        return QVariant(item.modified_at);
    case CountRole:
        return QVariant(across::profile::count_nodes(item.id));
    default:
        return {};
    }
}

void GroupModel::setList(std::unique_ptr<across::profile::GroupList> p_list)
{
    this->p_list = std::move(p_list);
}

bool GroupModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags GroupModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> GroupModel::roleNames() const
{
    QHash<int, QByteArray> names{{IdRole, "id"},
                                 {NameRole, "name"},
                                 {IsSubscriptionRole, "isSubscription"},
                                 {GroupTypeRole, "type"},
                                 {UrlRole, "url"},
                                 {CycleTimeRole, "cycle_Ttime"},
                                 {CreatedAtRole, "createdAt"},
                                 {ModifiedAtRole, "modifiedAt"},
                                 {CountRole, "count"}};
    return names;
}
