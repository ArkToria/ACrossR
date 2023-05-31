#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QAbstractListModel>
#include <memory>

#include "rusty_bridge/profile.h"

class NodeModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit NodeModel(QObject *parent = nullptr);

    enum
    {
        IdRole = Qt::UserRole,
        NameRole,
        GroupIdRole,
        GroupNameRole,
        RoutingIdRole,
        RoutingNameRole,
        ProtocolRole,
        AddressRole,
        PortRole,
        PasswordRole,
        RawRole,
        UrlRole,
        LatencyRole,
        UploadRole,
        DownloadRole,
        CreatedAtRole,
        ModifiedAtRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void select(::std::int64_t group_id);

    void setList(std::unique_ptr<across::profile::NodeList> p_list);

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

  private:
    std::unique_ptr<across::profile::NodeList> p_list;
};

#endif // NODEMODEL_H
