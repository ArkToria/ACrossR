#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QEventLoop>
#include <QFont>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringLiteral>
#include <QTranslator>
#include <QUrl>
#include <QtConcurrent>
#include <QtQuickControls2>

#include "singleapplication.h"

#ifdef QT_DEBUG
#include "models/confighelper.h"
#endif

#include "models/dbtools.h"
#include "models/envtools.h"
#include "view_models/configtools.h"
#include "view_models/grouplist.h"
#include "view_models/groupmodel.h"
#include "view_models/imageprovider.h"
#include "view_models/logview.h"
#include "view_models/nodeformmodel.h"
#include "view_models/nodelist.h"
#include "view_models/nodemodel.h"
#include "view_models/notificationmodel.h"
#include "view_models/systemtray.h"

namespace across {
#define ORG_NAME "Arktoria"
#define APP_NAME "ACross"
#define QML_MAJOR_VERSION 1
#define QML_MINOR_VERSION 1

enum ACrossExitReason {
    EXIT_NORMAL = 0,
    EXIT_SECONDARY_INSTANCE = EXIT_NORMAL + 2,
};

class Application : public SingleApplication {
    Q_OBJECT
  public:
    explicit Application(int &argc, char **argv);

    ~Application();

    bool initialize();

    ACrossExitReason getExitReason();

    int run();

    void setRootContext();

    void setTranslator(const QString &lang = "current");

    void registerModels();

    static void removeImageProvider(ImageProvider *img_provider);

  private slots:
    void onMessageReceived(quint32 clientId, const QByteArray &msg);
    void handleShutdown();

  private:
    LogView m_log;
    QSharedPointer<across::setting::ConfigTools> p_config;
    QSharedPointer<across::acolorsapi::AColoRSAPITools> p_acolors;
    QSharedPointer<across::core::CoreTools> p_core;
    QSharedPointer<across::network::CURLTools> p_curl;
    QSharedPointer<across::NodeList> p_nodes;
    QSharedPointer<across::GroupList> p_groups;
    QSharedPointer<across::SystemTray> p_tray;
    QSharedPointer<across::NotificationModel> p_notifications;
    across::ImageProvider *p_image_provider;

    ACrossExitReason exitReason = EXIT_NORMAL;

    const QString m_app_name = APP_NAME;
    QTranslator m_translator;
    QQmlApplicationEngine m_engine;
};
} // namespace across

#endif // APPLICATION_H
