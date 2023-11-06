#include "app.h"
#include "across.h"
#include <qcommandlineoption.h>
#include <qcoreapplication.h>
#include <qqmlapplicationengine.h>

inline const QStringList FONT_LIST();
const QStringList FONTS = FONT_LIST();
const char *APP_NAME = "ACross";
const char *APP_ICON_FILE = ":qt/qml/misc/design/logo.svg";
const QString QML_URL = u"qrc:/qt/qml/Main/main.qml"_qs;
const QString DEFAULT_ACOLORS_URL = u"http://127.0.0.1:%1"_qs;
const std::uint16_t DEFAULT_PORT = 11451;

App::App(int &argc, char **argv) : QGuiApplication(argc, argv) {
  setApplicationName(APP_NAME);
  setWindowIcon(QIcon(APP_ICON_FILE));
  setFont(QFont(FONTS, 11));

  QCommandLineParser parser;
  QCommandLineOption portOption(
      QStringList() << "p"
                    << "port",
      QCoreApplication::translate("App", "Acolors server port"),
      QCoreApplication::translate("App", "port"));
  parser.addOption(portOption);
  parser.parse(this->arguments());

  bool ok = false;
  int port = parser.value(portOption).toInt(&ok);
  if (!ok) {
    port = DEFAULT_PORT;
  }

  const QUrl url(QML_URL);
  QObject::connect(
      &m_engine, &QQmlApplicationEngine::objectCreated, this,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  m_engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
  m_engine.addImportPath(":/");

  auto uri = DEFAULT_ACOLORS_URL.arg(port).toStdString();
  Across *across = new Across(uri.c_str(), this);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossCxx"),
                                             across);

  m_engine.load(url);
}

inline const QStringList FONT_LIST() {
  const static QStringList default_fonts = {
      "Noto Sans",     "Segoe UI Variable", "Segoe UI",
      "Noto Sans CJK", "PingFang SC",       "Microsoft YaHei UI",
  };

  auto fonts_fallback =
      QStringLiteral("Noto Sans, Segoe UI Variable, Segoe UI, Noto Sans CJK, "
                     "PingFang SC, Microsoft YaHei UI");
  do {
    if (fonts_fallback.isEmpty())
      break;

    QStringList temp_fonts;
    for (auto &font : fonts_fallback.split(",")) {
      temp_fonts.append(font.trimmed());
    }

    if (temp_fonts.isEmpty())
      break;

    return temp_fonts;
  } while (false);

  return default_fonts;
}
