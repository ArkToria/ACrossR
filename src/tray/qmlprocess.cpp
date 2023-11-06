#include "qmlprocess.h"
QmlProcess::QmlProcess(QObject *parent) : QObject(parent) {}
QmlProcess::~QmlProcess() { across::qmlprocess::force_close(); }
void QmlProcess::setPort(std::uint16_t port) {
  across::qmlprocess::set_port(port);
}
void QmlProcess::start() { across::qmlprocess::start(); }
void QmlProcess::close() { across::qmlprocess::close(); }