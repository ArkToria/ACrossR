#ifndef ACROSS_H
#define ACROSS_H

#include "rusty_bridge/ffi/acolors.h"
#include "rusty_bridge/ffi/init.h"
#include "rusty_bridge/ffi/profile.h"
#include <QObject>
#include <cstdint>
#include <qtmetamacros.h>

class Across : QObject {
  Q_OBJECT
public:
  Across(QObject *parent = nullptr);
  ~Across();
  void serve();
  void close();
  std::uint16_t serve_port();
};

#endif // ACROSS_H