#ifndef APP_H
#define APP_H

#include <QFont>
#include <QGuiApplication>
#include <QIcon>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qguiapplication.h>
#include <qtmetamacros.h>

class App : public QGuiApplication {
  Q_OBJECT
public:
  explicit App(int &argc, char **argv);
  QQmlApplicationEngine &engine() { return m_engine; };

private:
  QQmlApplicationEngine m_engine;
signals:
};

#endif // APP_H
