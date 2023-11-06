#ifndef QMLPROCSS_H
#define QMLPROCSS_H

#include "rusty_bridge/qml_process.h"
#include <QObject>
#include <cstdint>
#include <qtmetamacros.h>

class QmlProcess : QObject {
  Q_OBJECT
public:
  QmlProcess(QObject *parent = nullptr);
  ~QmlProcess();
  void setPort(std::uint16_t);
  void start();
  void close();
};

#endif // QMLPROCSS_H