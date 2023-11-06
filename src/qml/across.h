#ifndef ACROSS_H
#define ACROSS_H

#include "groupmodel.h"
#include "nodemodel.h"
#include "rusty_bridge/across_rpc.h"
#include <QCommandLineParser>
#include <QObject>
#include <QQuickItem>
#include <memory>
#include <qtmetamacros.h>

class Across : public QObject {
  Q_OBJECT
  Q_PROPERTY(GroupModel *groupModel READ groupModel NOTIFY groupModelChanged)
  Q_PROPERTY(NodeModel *nodeModel READ nodeModel NOTIFY nodeModelChanged)
public:
  explicit Across(const char *uri_str, QObject *parent = nullptr);
  void setChannel(const char *str);
  GroupModel *groupModel() { return this->group_model; }
  NodeModel *nodeModel() { return this->node_model; }

private:
  GroupModel *group_model;
  NodeModel *node_model;
signals:
  void groupModelChanged();
  void nodeModelChanged();
};

#endif // ACROSS_H
