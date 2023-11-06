#include "across.h"
#include <cstdio>
Across::Across(QObject *parent) : QObject(parent) {}
Across::~Across() { across::acolors::force_close(); }
void Across::serve() { across::acolors::serve(); }
void Across::close() { across::acolors::close(); }
std::uint16_t Across::serve_port() { return across::acolors::serve_port(); }