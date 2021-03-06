#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include "../models/coretools.h"
#include "../models/dbtools.h"
#include "buildinfo.h"
#include "configtools.h"
#include "logtools.h"
#include "nodelist.h"

#include <QAction>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>
#include <utility>

namespace across {
class SystemTray : public QObject {
    Q_OBJECT
  public:
    enum Visibility { Minimized = false, Visible = true };

    explicit SystemTray(QObject *parent = nullptr);
    ~SystemTray() override;

    void init(QSharedPointer<across::setting::ConfigTools> config,
              QSharedPointer<across::core::CoreTools> core,
              QSharedPointer<across::NodeList> nodes);

    void setTrayMenu();
    void loadTrayIcons(const QString &stylish = "", const QString &color = "");
    void retranslate();

    QString titleString();
    QString inboundString();

    QSharedPointer<QSystemTrayIcon> getTrayIcon() { return p_tray_icon; };

    Q_INVOKABLE void toggleVisibilitySetText(bool vis);
    Q_INVOKABLE static inline bool isSystemTrayAvailable() {
        return QSystemTrayIcon::isSystemTrayAvailable();
    };
  public slots:
    void onRunningChanged();
    void onEnableTrayChanged();
    void onTrafficChanged();

  private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

  signals:
    void signalIconActivated();
    void signalShow();
    void signalQuit();

  private:
    QSharedPointer<QSystemTrayIcon> p_tray_icon;
    QSharedPointer<across::setting::ConfigTools> p_config;
    QSharedPointer<across::core::CoreTools> p_core;
    QSharedPointer<across::NodeList> p_nodes;
    QSharedPointer<across::core::APITools> p_api;

    std::shared_ptr<spdlog::logger> p_logger;

    QString upload_traffic = "";
    QString download_traffic = "";
    NodeInfo m_current_node_info;

    QIcon connected_icon;
    QIcon disconnected_icon;

    QSharedPointer<QMenu> tray_root_menu;
    QSharedPointer<QAction> tray_action_toggle_visibility, tray_action_start,
        tray_action_stop, tray_action_restart, tray_action_quit;
};
} // namespace across

#endif
