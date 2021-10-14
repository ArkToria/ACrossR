#ifndef DBTOOLS_H
#define DBTOOLS_H

#include "../view_models/configtools.h"
#include "../view_models/logtools.h"

#include <QDateTime>
#include <QMap>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

#include "magic_enum.hpp"

namespace across {
enum SubscriptionType : int
{
  sip008 = 0,
  base64,
  jsonraw,
  none
};

enum EntryType : int
{
  vmess,
  shadowsocks,
  trojan,
  raw,
  scheme,
  unknown,
};

struct NodeInfo
{
  qint64 id = 0;
  QString name = "";
  qint64 group_id = 0;
  QString group_name = "";
  EntryType protocol;
  QString address = "";
  uint port = 0;
  QString password = "";
  QString raw = "";
  QString url = "";
  qint64 latency = -1;
  qint64 upload = 0;
  qint64 download = 0;
  QDateTime created_time;
  QDateTime modified_time;
};

struct GroupInfo
{
  qint64 id = 0;
  QString name = "";
  bool is_subscription = false;
  SubscriptionType type = none;
  QString url = "";
  int cycle_time = 0;
  QDateTime created_time;
  QDateTime modified_time;
  int items = 0;
};

enum RunTimeValues : int
{
  CURRENT_NODE_ID = 0,
  CURRENT_GROUP_ID,
  DEFAULT_NODE_ID,
  DEFAULT_GROUP_ID,

  OTHER_INFO = 100,
};

struct RuntimeValue
{
  const QString key = "";
  const int type = QMetaType::QString;
  QVariant value;

  RuntimeValue(const QString& key, const QVariant& value);
  RuntimeValue(const RunTimeValues key, const QVariant& value);
  RuntimeValue(const QString& key, int type, const QVariant& value);
};

class DBTools : public QObject
{
  Q_OBJECT

public:
  explicit DBTools(QObject* parent = nullptr);

  ~DBTools();

  void init(QSharedPointer<LogView> log_view,
            QSharedPointer<across::setting::ConfigTools> config);
  void reload();
  bool isTableExists(const QString& table_name);
  bool isGroupExists(const QString& group_name);

  QSqlError createDefaultTables();
  QSqlError createDefaultValues();
  QSqlError createDefaultGroup();

  QSqlError insert(NodeInfo& node);
  QSqlError insert(QList<NodeInfo>& nodes);
  QSqlError update(NodeInfo& node);
  QSqlError update(QList<NodeInfo>& nodes);
  QSqlError insert(GroupInfo& group);
  QSqlError update(GroupInfo& group);
  QSqlError update(QList<GroupInfo>& groups);
  QSqlError removeNodeFromID(qint64 id);
  QSqlError removeGroupFromID(qint64 id, bool keep_group = false);
  QSqlError reloadAllGroupsInfo();

  QSqlError createRuntimeValue(const RuntimeValue& value);
  std::optional<RuntimeValue> readRuntimeValue(const QString& key);
  std::optional<RuntimeValue> readRuntimeValue(const RunTimeValues& key);
  QSqlError updateRuntimeValue(const RuntimeValue& value);
  QSqlError deleteRuntimeValue(const QString& key);
  QSqlError deleteRuntimeValue(const RunTimeValues& key);

  qint64 getCurrentNodeID();
  qint64 getCurrentGroupID();
  qint64 getDefaultNodeID();
  qint64 getDefaultGroupID();
  qsizetype getSizeFromGroupID(qint64 group_id);
  std::optional<GroupInfo> getGroupFromID(qint64 group_id);
  QList<GroupInfo> getAllGroupsInfo();
  QList<NodeInfo> listAllNodesFromGroupID(qint64 group_id);
  QMap<qint64, QList<qint64>> search(const QString& value);

public slots:
  void close();
  void beginTransaction();
  void endTransaction();

signals:
  void destroy();

private:
  QSqlError directExec(const QString& sql_str);
  QPair<QSqlError, qint64> stepExec(
    const QString& sql_str,
    QVariantList* inputCollection = nullptr,
    int outputColumns = 0,
    QList<QVariantList>* outputCollections = nullptr);

private:
  QSqlDatabase m_db;
  QList<GroupInfo> m_groups;

  std::shared_ptr<across::utils::LogTools> p_logger;
  QSharedPointer<across::setting::ConfigTools> p_config = nullptr;
};
}

#endif // DBTOOLS_H
