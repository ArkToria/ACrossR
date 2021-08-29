#ifndef DBUSTOOLS_H
#define DBUSTOOLS_H

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusReply>

namespace across {
namespace utils {
struct NotifyDBus {
    const QString server = "org.freedesktop.Notifications";
    const QString path = "/org/freedesktop/Notifications";
    const QString interface = "org.freedesktop.Notifications";
    const QString method = "Notify";

    QString app_name = "ACross";
    uint replace_id;
    QString app_icon;
    QString summary;
    QString body;
    QStringList actions;
    QVariantMap hints;
    int timeout = 3500;
};

class DBusTools {
public:
    DBusTools();

    void sendNotify(const QString& message, const QString& summary = "");
    void sendNotify(NotifyDBus& notifyDBus);

private:
};
}
}

#endif // DBUSTOOLS_H
