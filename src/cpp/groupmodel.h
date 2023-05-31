#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include <QAbstractListModel>
#include <memory>

#include "rusty_bridge/profile.h"

class GroupModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit GroupModel(QObject *parent = nullptr);

    enum
    {
        IdRole = Qt::UserRole,
        NameRole,
        IsSubscriptionRole,
        GroupTypeRole,
        UrlRole,
        CycleTimeRole,
        CreatedAtRole,
        ModifiedAtRole,
        CountRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setList(std::unique_ptr<across::profile::GroupList> p_list);

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

  private:
    std::unique_ptr<across::profile::GroupList> p_list;
};

#endif // GROUPMODEL_H
