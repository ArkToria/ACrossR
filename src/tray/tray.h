#ifndef TRAY_H
#define TRAY_H

#include <QMenu>
#include <QSystemTrayIcon>
#include <qtmetamacros.h>

class AcrossTray : QSystemTrayIcon {
  Q_OBJECT
  Q_PROPERTY(QString theme READ getTheme WRITE setTheme NOTIFY themeChanged)
  Q_PROPERTY(QString status READ getStatus WRITE setStatus NOTIFY statusChanged)
public:
  AcrossTray(QObject *parent = nullptr);
  ~AcrossTray();

  QString getTheme();
  QString getStatus();
public slots:
  inline void show() { setVisible(true); }
  inline void hide() { setVisible(false); }
  void setTheme(const QString &theme);
  void setStatus(const QString &status);
  void updateIcon();
  void retranslate();

signals:
  void themeChanged(const QString &theme);
  void statusChanged(const QString &status);

private:
  void initMenu();
  QString status;
  QString theme;
  QMenu mainMenu;
  QAction *trayActionToggleVisibility, *trayActionStart, *trayActionStop,
      *trayActionRestart, *trayActionQuit;
};

#endif // TRAY_H