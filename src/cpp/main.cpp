#include "app.h"

int main(int argc, char *argv[]) {

  App app(argc, argv);

  if (app.engine().rootObjects().isEmpty()) {
    return -1;
  }
  return app.exec();
}
