#include <QFont>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "rusty_bridge/profile.h"
#include "across.h"

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

    Across across("http://127.0.0.1:11451");
    engine.rootContext()->setContextProperty(QStringLiteral("cxxGroupModel"), across.group_model.get());
    engine.rootContext()->setContextProperty(QStringLiteral("cxxNodeModel"), across.node_model.get());

    engine.load(url);

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    return app.exec();
}
