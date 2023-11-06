#include "tray.h"

const static QString FORMAT_URL =
    QString(":/qt/qml/misc/icons/%1/tray_logo_%2.svg");
void AcrossTray::retranslate() {
  trayActionToggleVisibility->setText(tr("Show"));
  trayActionStart->setText(tr("Connect"));
  trayActionStop->setText(tr("Disconnect"));
  trayActionRestart->setText(tr("Reconnect"));
  trayActionQuit->setText(tr("Quit"));
}
void AcrossTray::initMenu() {
  trayActionToggleVisibility = new QAction(&mainMenu);
  trayActionStart = new QAction(&mainMenu);
  trayActionStop = new QAction(&mainMenu);
  trayActionRestart = new QAction(&mainMenu);
  trayActionQuit = new QAction(&mainMenu);

  mainMenu.addAction(trayActionToggleVisibility);
  mainMenu.addSeparator();
  mainMenu.addAction(trayActionStart);
  mainMenu.addAction(trayActionStop);
  mainMenu.addAction(trayActionRestart);
  mainMenu.addSeparator();
  mainMenu.addAction(trayActionQuit);
  this->setContextMenu(&mainMenu);

  this->retranslate();
}
AcrossTray::AcrossTray(QObject *parent) : QSystemTrayIcon(parent) {
  this->status = "stop";
  this->theme = "light";

  initMenu();
  connect(this, SIGNAL(statusChanged(const QString &)), this,
          SLOT(updateIcon()));
  connect(this, SIGNAL(themeChanged(const QString &)), this,
          SLOT(updateIcon()));
  emit statusChanged(status);
  emit themeChanged(theme);
}
AcrossTray::~AcrossTray() {}
void AcrossTray::setTheme(const QString &theme) {
  if (theme == this->theme)
    return;
  this->theme = theme;
  emit themeChanged(theme);
}
void AcrossTray::setStatus(const QString &status) {
  if (status == this->status)
    return;
  this->status = status;
  emit statusChanged(status);
}
QString AcrossTray::getTheme() { return theme; }
QString AcrossTray::getStatus() { return status; }
void AcrossTray::updateIcon() {
  setIcon(QIcon(FORMAT_URL.arg(theme).arg(status)));
}