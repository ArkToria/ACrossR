#include <QFont>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "groupmodel.h"
#include "nodemodel.h"
#include "rusty_bridge/profile.h"

inline const QStringList FONT_LIST()
{
    const static QStringList default_fonts = {
        "Noto Sans", "Segoe UI Variable", "Segoe UI", "Noto Sans CJK", "PingFang SC", "Microsoft YaHei UI",
    };

    auto fonts_fallback = QStringLiteral("Noto Sans, Segoe UI Variable, Segoe UI, Noto Sans CJK, "
                                         "PingFang SC, Microsoft YaHei UI");
    do
    {
        if (fonts_fallback.isEmpty())
            break;

        QStringList temp_fonts;
        for (auto &font : fonts_fallback.split(","))
        {
            temp_fonts.append(font.trimmed());
        }

        if (temp_fonts.isEmpty())
            break;

        return temp_fonts;
    } while (false);

    return default_fonts;
}
int main(int argc, char *argv[])
{
    auto fonts = FONT_LIST();
    QGuiApplication app(argc, argv);

    app.setApplicationName("ACross");
    app.setWindowIcon(QIcon(":qt/qml/misc/design/logo.svg"));
    app.setFont(QFont(fonts, 11));

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qt/qml/Main/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");

    across::profile::set_channel("http://127.0.0.1:11451");
    GroupModel group_model;
    auto group_list = std::make_unique<across::profile::GroupList>(across::profile::list_all_groups());
    group_model.setList(std::move(group_list));
    NodeModel node_model;
    engine.rootContext()->setContextProperty(QStringLiteral("cxxGroupModel"), &group_model);
    engine.rootContext()->setContextProperty(QStringLiteral("cxxNodeModel"), &node_model);

    engine.load(url);

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
