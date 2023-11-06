#include "across.h"
#include "qmlprocess.h"
#include "tray.h"
#include <QApplication>
#include <cstdio>

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);

  app.setApplicationName("ACross");

  Across across;
  AcrossTray tray = AcrossTray();
  QmlProcess qmlProcess;

  int result = 0;
  try {
    across.serve();
    qmlProcess.setPort(across.serve_port());
    qmlProcess.start();
    tray.show();
    result = app.exec();
  } catch (const std::exception &e) {
    printf("%s", e.what());
  }

  return result;
}
