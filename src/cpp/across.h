#ifndef ACROSS_H
#define ACROSS_H

#include <QObject>
#include <QQuickItem>
#include <memory>
#include <qtmetamacros.h>
#include "groupmodel.h"
#include "nodemodel.h"

class Across : public QObject
{
    Q_OBJECT
public:
    explicit Across(const char *uri_str,QObject *parent = nullptr);
    void set_channel(const char *str);
    std::shared_ptr<GroupModel> group_model;
    std::shared_ptr<NodeModel> node_model;

signals:

};

#endif // ACROSS_H
