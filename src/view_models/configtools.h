#ifndef CONFIGTOOLS_H
#define CONFIGTOOLS_H

#include "../models/acolorstools.h"
#include "../models/apitools.h"
#include "../models/confighelper.h"
#include "../models/envtools.h"
#include "../models/networktools.h"
#include "../models/notifytools.h"
#include "../models/serializetools.h"
#include "buildinfo.h"
#include "logtools.h"

#include "nlohmann/json.hpp"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QObject>
#include <QRegularExpression>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>
#include <QUrl>

#ifdef Q_OS_WIN
#else
#include <wordexp.h>
#endif

namespace across::setting {
using Json = nlohmann::json;

class ConfigTools : public QObject {
    Q_OBJECT
    // database setting
    Q_PROPERTY(
        QString dataDir READ dataDir WRITE setDataDir NOTIFY dataDirChanged)
    Q_PROPERTY(QString dbPath READ dbPath NOTIFY dbPathChanged)
    Q_PROPERTY(bool shutdownOnExit READ shutdownOnExit WRITE setShutdownOnExit
                   NOTIFY shutdownOnExitChanged)

    // acolors settings
    Q_PROPERTY(QString acolorsPath READ acolorsPath WRITE setAcolorsPath NOTIFY
                   acolorsPathChanged)
    Q_PROPERTY(QString acolorsConfigPath READ acolorsConfigPath WRITE
                   setAcolorsConfigPath NOTIFY acolorsConfigPathChanged)
    Q_PROPERTY(QString acolorsDbPath READ acolorsDbPath WRITE setAcolorsDbPath
                   NOTIFY acolorsDbPathChanged)
    Q_PROPERTY(QString acolorsAPIPort READ acolorsAPIPort WRITE
                   setAcolorsAPIPort NOTIFY acolorsAPIPortChanged)

    // core setting
    Q_PROPERTY(QString coreName READ coreName NOTIFY coreNameChanged)
    Q_PROPERTY(QString coreVersion READ coreVersion NOTIFY coreVersionChanged)
    Q_PROPERTY(QString guiVersion READ guiVersion CONSTANT)
    Q_PROPERTY(
        QString corePath READ corePath WRITE setCorePath NOTIFY corePathChanged)
    Q_PROPERTY(QString assetsPath READ assetsPath WRITE setAssetsPath NOTIFY
                   assetsPathChanged)
    Q_PROPERTY(
        QString logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)
    Q_PROPERTY(bool apiEnable READ apiEnable WRITE setApiEnable NOTIFY
                   apiEnableChanged)
    Q_PROPERTY(
        QString apiResultText READ apiResultText NOTIFY apiResultTextChanged)

    // inbounds setting
    Q_PROPERTY(QString inboundAddress READ inboundAddress WRITE
                   setInboundAddress NOTIFY inboundAddressChanged)
    Q_PROPERTY(bool socksEnable READ socksEnable WRITE setSocksEnable NOTIFY
                   socksEnableChanged)
    Q_PROPERTY(QString socksPort READ socksPort WRITE setSocksPort NOTIFY
                   socksPortChanged)
    Q_PROPERTY(QString socksUsername READ socksUsername WRITE setSocksUsername
                   NOTIFY socksUsernameChanged)
    Q_PROPERTY(QString socksPassword READ socksPassword WRITE setSocksPassword
                   NOTIFY socksPasswordChanged)
    Q_PROPERTY(bool socksUDPEnable READ socksUDPEnable WRITE setSocksUDPEnable
                   NOTIFY socksUDPEnableChanged)
    Q_PROPERTY(bool httpEnable READ httpEnable WRITE setHttpEnable NOTIFY
                   httpEnableChanged)
    Q_PROPERTY(
        QString httpPort READ httpPort WRITE setHttpPort NOTIFY httpPortChanged)
    Q_PROPERTY(QString httpUsername READ httpUsername WRITE setHttpUsername
                   NOTIFY httpUsernameChanged)
    Q_PROPERTY(QString httpPassword READ httpPassword WRITE setHttpPassword
                   NOTIFY httpPasswordChanged)

    // user interface setting
    Q_PROPERTY(QString textColor READ textColor WRITE setTextColor NOTIFY
                   textColorChanged)
    Q_PROPERTY(QString backgroundColor READ backgroundColor WRITE
                   setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QString highlightColor READ highlightColor WRITE
                   setHighlightColor NOTIFY highlightColorChanged)
    Q_PROPERTY(QString highlightTextColor READ highlightTextColor WRITE
                   setHighlightTextColor NOTIFY highlightTextColorChanged)
    Q_PROPERTY(QString warnColor READ warnColor WRITE setWarnColor NOTIFY
                   warnColorChanged)
    Q_PROPERTY(QString warnTextColor READ warnTextColor WRITE setWarnTextColor
                   NOTIFY warnTextColorChanged)
    Q_PROPERTY(QString shadowColor READ shadowColor WRITE setShadowColor NOTIFY
                   shadowColorChanged)
    Q_PROPERTY(QString borderColor READ borderColor WRITE setBorderColor NOTIFY
                   borderColorChanged)
    Q_PROPERTY(QString deepColor READ deepColor WRITE setDeepColor NOTIFY
                   deepColorChanged)
    Q_PROPERTY(QString deepTextColor READ deepTextColor WRITE setDeepTextColor
                   NOTIFY deepTextColorChanged)
    Q_PROPERTY(QString styleColor READ styleColor WRITE setStyleColor NOTIFY
                   styleColorChanged)
    Q_PROPERTY(QString styleTextColor READ styleTextColor WRITE
                   setStyleTextColor NOTIFY styleTextColorChanged)
    Q_PROPERTY(QString bannerTextColor READ bannerTextColor WRITE
                   setBannerTextColor NOTIFY bannerTextColorChanged)
    Q_PROPERTY(QString bannerMaskColor READ bannerMaskColor NOTIFY
                   bannerMaskColorChanged)
    Q_PROPERTY(QString trayStylish READ trayStylish WRITE setTrayStylish NOTIFY
                   trayStylishChanged)
    Q_PROPERTY(QString trayColor READ trayColor WRITE setTrayColor NOTIFY
                   trayColorChanged)
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius NOTIFY
                   borderRadiusChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY
                   borderWidthChanged)
    Q_PROPERTY(int itemSpacing READ itemSpacing WRITE setItemSpacing NOTIFY
                   itemSpacingChanged)
    Q_PROPERTY(QString iconStyle READ iconStyle WRITE setIconStyle NOTIFY
                   iconStyleChanged)
    Q_PROPERTY(QStringList themeNames READ themeNames NOTIFY themeNamesChanged)
    Q_PROPERTY(QString currentThemeName READ currentThemeName WRITE
                   setCurrentTheme NOTIFY currentThemeChanged)
    Q_PROPERTY(QString currentLanguage READ currentLanguage WRITE
                   setCurrentLanguage NOTIFY currentLanguageChanged)
    Q_PROPERTY(bool enableTray READ enableTray WRITE setEnableTray NOTIFY
                   enableTrayChanged)
    Q_PROPERTY(
        bool enableStartFromMinimized READ enableStartFromMinimized WRITE
            setEnableStartFromMinimized NOTIFY enableStartFromMinimizedChanged)
    Q_PROPERTY(bool enableBanner READ enableBanner WRITE setEnableBanner NOTIFY
                   enableBannerChanged)
    Q_PROPERTY(bool enableAutoConnect READ enableAutoConnect WRITE
                   setEnableAutoConnect NOTIFY enableAutoConnectChanged)
    Q_PROPERTY(bool enableAutoStart READ enableAutoStart WRITE
                   setEnableAutoStart NOTIFY enableAutoStartChanged)
    Q_PROPERTY(bool enableAutoExport READ enableAutoExport WRITE
                   setEnableAutoExport NOTIFY enableAutoExportChanged)
    Q_PROPERTY(QString backgroundImage READ backgroundImage WRITE
                   setBackgroundImage NOTIFY backgroundImageChanged)
    Q_PROPERTY(double backgroundOpacity READ backgroundOpacity WRITE
                   setBackgroundOpacity NOTIFY backgroundOpacityChanged)
    Q_PROPERTY(
        QString logMode READ logMode WRITE setLogMode NOTIFY logModeChanged)
    Q_PROPERTY(
        int logLines READ logLines WRITE setLogLines NOTIFY logLinesChanged)

    // network setting
    Q_PROPERTY(QString networkTestMethod READ networkTestMethod WRITE
                   setNetworkTestMethod NOTIFY networkTestMethodChanged)
    Q_PROPERTY(QString networkTestURL READ networkTestURL WRITE
                   setNetworkTestURL NOTIFY networkTestURLChanged)
    Q_PROPERTY(QString networkUserAgent READ networkUserAgent WRITE
                   setNetworkUserAgent NOTIFY networkUserAgentChanged)

    // help page
    Q_PROPERTY(QString buildInfo READ buildInfo CONSTANT)
    Q_PROPERTY(QString extraInfo READ extraInfo CONSTANT)
    Q_PROPERTY(QString buildTime READ buildTime CONSTANT)
    Q_PROPERTY(QString sourceCodeURL READ sourceCodeURL CONSTANT)
    Q_PROPERTY(QString reportURL READ reportURL CONSTANT)
    Q_PROPERTY(QString licenseURL READ licenseURL CONSTANT)
    Q_PROPERTY(QString apiURL READ apiURL CONSTANT)
    Q_PROPERTY(QString releaseURL READ releaseURL CONSTANT)
    Q_PROPERTY(
        QStringList versionNews READ versionNews NOTIFY versionNewsChanged)

  public:
    explicit ConfigTools(QObject *parent = nullptr);
    ~ConfigTools() override;

    bool
    init(QSharedPointer<across::network::CURLTools> curl,
         const QSharedPointer<across::acolorsapi::AColoRSAPITools> &acolors,
         const QString &file_path = "");

    void loadThemeConfig();
    bool loadConfigPath(const QString &file_path = "");
    void mergeConfigFromJSON(const std::string &json_str);

    void setAPIObject(v2ray::config::V2RayConfig &config);
    void setLogObject(v2ray::config::V2RayConfig &config);
    void setInboundObject(v2ray::config::V2RayConfig &config);

    Q_INVOKABLE QString getConfigVersion();
    Q_INVOKABLE QString getLanguage();
    Q_INVOKABLE QUrl getInboundProxy();

    Q_INVOKABLE void freshColors();
    Q_INVOKABLE void testAPI();
    Q_INVOKABLE bool testAndSetAddr(const QString &addr);
    Q_INVOKABLE void freshInbound();
    Q_INVOKABLE void saveConfig(const QString &config_path = "");
    Q_INVOKABLE void checkUpdate();
    Q_INVOKABLE void setNews();

    across::config::Config *config();
    across::config::Theme *currentTheme();

    static bool isFileExist(const QString &file_path);

    const QString ACROSS_CONFIG = "across.json";
    const QString ACROSS_NEWS = "across_news.json";
    const QString CURRENT_CONFIG = "across_current.json";

  public:
    // database setting
    QString dataDir();
    QString dbPath();

    // acolors setting
    QString acolorsPath();
    QString acolorsConfigPath();
    QString acolorsDbPath();
    QString acolorsAPIPort();
    bool shutdownOnExit();

    // core setting
    QString coreName();
    QString coreVersion();
    static QString guiVersion();
    QString corePath();
    QString assetsPath();
    QString logLevel();
    int logLines();
    bool apiEnable();
    QString apiResultText();

    // inbounds setting
    QString inboundAddress();
    bool socksEnable();
    bool socksUDPEnable();
    QString socksPort();
    QString socksUsername();
    QString socksPassword();
    bool httpEnable();
    QString httpPort();
    QString httpUsername();
    QString httpPassword();

    // user interface setting
    QString textColor();
    QString backgroundColor();
    QString highlightColor();
    QString highlightTextColor();
    QString warnColor();
    QString warnTextColor();
    QString shadowColor();
    QString borderColor();
    QString deepColor();
    QString deepTextColor();
    QString styleColor();
    QString styleTextColor();
    QString bannerTextColor();
    QString bannerMaskColor();
    QString trayStylish();
    QString trayColor();
    int borderRadius();
    int borderWidth();
    int itemSpacing();
    QString iconStyle();
    QStringList themeNames();
    QString currentThemeName();
    QString currentLanguage();
    bool enableTray();
    bool enableStartFromMinimized();
    bool enableBanner();
    bool enableAutoConnect();
    bool enableAutoStart();
    bool enableAutoExport() const;
    QString backgroundImage();
    double backgroundOpacity();
    QString logMode();

    // network setting
    QString networkTestMethod();
    QString networkTestURL();
    QString networkUserAgent();

    // help page
    static QString buildInfo();
    static QString extraInfo();
    static QString buildTime();
    static QString sourceCodeURL();
    static QString reportURL();
    static QString licenseURL();
    static QString apiURL(uint per_page = 0);
    static QString releaseURL();
    static QString DateTimeFormat();
    QStringList versionNews();

  public slots:
    void setDataDir(const QString &dir = "");
    void setAcolorsPath(const QUrl &val);
    void setAcolorsConfigPath(const QUrl &val);
    void setAcolorsDbPath(const QUrl &val);
    void setAcolorsAPIPort(const QString &val);
    void setShutdownOnExit(bool val);
    void setCorePath(const QUrl &val);
    void setAssetsPath(const QUrl &val);
    void setLogLevel(const QString &val);
    void setApiEnable(bool val);
    void resetInbounds();
    void setInboundAddress(const QString &val);
    void setSocksEnable(bool val);
    void setSocksUDPEnable(bool val);
    void setSocksPort(const QString &val);
    void setSocksUsername(const QString &val);
    void setSocksPassword(const QString &val);
    void setHttpEnable(bool val);
    void setHttpPort(QString &val);
    void setHttpUsername(const QString &val);
    void setHttpPassword(const QString &val);
    void setTextColor(const QString &val);
    void setBackgroundColor(const QString &val);
    void setHighlightColor(const QString &val);
    void setHighlightTextColor(const QString &val);
    void setWarnColor(const QString &val);
    void setWarnTextColor(const QString &val);
    void setShadowColor(const QString &val);
    void setBorderColor(const QString &val);
    void setDeepColor(const QString &val);
    void setDeepTextColor(const QString &val);
    void setStyleColor(const QString &val);
    void setStyleTextColor(const QString &val);
    void setBannerTextColor(const QString &val);
    void setTrayStylish(const QString &val);
    void setTrayColor(const QString &val);
    void setBorderRadius(int val);
    void setBorderWidth(int val);
    void setItemSpacing(int val);
    void setIconStyle(const QString &val);
    void setCurrentTheme(const QString &val);
    void setCurrentLanguage(const QString &val);
    void setEnableTray(bool val);
    void setEnableStartFromMinimized(bool val);
    void setEnableBanner(bool val);
    void setEnableAutoConnect(bool val);
    void setEnableAutoStart(bool val);
    void setEnableAutoExport(bool val);
    void setBackgroundImage(const QString &val);
    void setBackgroundOpacity(double val);
    void setLogMode(QString log_mode = "");
    void setLogLines(int val);

    // network setting
    void setNetworkTestMethod(const QString &val);
    void setNetworkTestURL(const QString &val);
    void setNetworkUserAgent(const QString &val);

    // help page
    void handleUpdated(const QVariant &content);

  signals:
    void dataDirChanged();
    void dbPathChanged();
    void acolorsPathChanged();
    void acolorsConfigPathChanged();
    void acolorsDbPathChanged();
    void acolorsAPIPortChanged();
    void shutdownOnExitChanged();
    void coreNameChanged();
    void coreVersionChanged();
    void corePathChanged();
    void assetsPathChanged();
    void logLevelChanged();
    void apiEnableChanged();
    void apiResultTextChanged();
    void apiStatsChanged(bool stats);
    void inboundAddressChanged();
    void socksEnableChanged();
    void socksUDPEnableChanged();
    void socksPortChanged();
    void socksUsernameChanged();
    void socksPasswordChanged();
    void httpEnableChanged();
    void httpPortChanged();
    void httpUsernameChanged();
    void httpPasswordChanged();
    void textColorChanged();
    void backgroundColorChanged();
    void highlightColorChanged();
    void highlightTextColorChanged();
    void warnColorChanged();
    void warnTextColorChanged();
    void shadowColorChanged();
    void borderColorChanged();
    void deepColorChanged();
    void deepTextColorChanged();
    void styleColorChanged();
    void styleTextColorChanged();
    void bannerTextColorChanged();
    void bannerMaskColorChanged();
    void trayStylishChanged();
    void trayColorChanged();
    void borderRadiusChanged();
    void borderWidthChanged();
    void itemSpacingChanged();
    void iconStyleChanged();
    void themeNamesChanged();
    void currentThemeChanged(const across::config::Theme &theme);
    void currentLanguageChanged(const QString &lang);
    void enableTrayChanged();
    void enableStartFromMinimizedChanged();
    void enableAutoConnectChanged();
    void enableAutoStartChanged();
    void enableAutoExportChanged();
    void buildInfoChanged();
    void configChanged();
    void enableBannerChanged();
    void backgroundImageChanged();
    void backgroundOpacityChanged();
    void logLinesChanged(int max_lines);
    void logModeChanged(const QString &logMode);
    void fontSizeChanged(int fontSize);

    // network setting
    void networkTestMethodChanged();
    void networkTestURLChanged();
    void networkUserAgentChanged();

    // help page
    void updatedChanged(const QString &version);
    void versionNewsChanged();

  private:
    QQueue<QFuture<void>> m_tasks;
    QStringList m_version_news;
    QString m_config_path = "./" + ACROSS_CONFIG;
    QString m_api_result_text = "";

    QSharedPointer<across::acolorsapi::AColoRSAPITools> p_acolors_api;

    QSharedPointer<across::network::CURLTools> p_curl;
    across::config::Config m_config = ConfigHelper::defaultConfig();
    acolors::Inbounds m_inbounds;
    across::config::AColoRS *p_acolors{};
    across::config::Core *p_core{};
    across::config::Database *p_db{};
    across::config::Interface *p_interface{};
    across::config::Theme *p_theme{};
    across::config::Network *p_network{};
};
} // namespace across::setting

#endif // CONFIGTOOLS_H
