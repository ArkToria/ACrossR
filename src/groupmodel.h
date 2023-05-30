#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include <QAbstractListModel>

#include "rusty_bridge/lib.h"


class GroupModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GroupModel(QObject *parent = nullptr);

    enum {
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

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;
private:
    across::core::GroupList *p_list;
};

#endif // GROUPMODEL_H
