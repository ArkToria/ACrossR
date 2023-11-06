#include "across.h"

Across::Across(const char *uri_str, QObject *parent) : QObject{parent} {
  across::acolors::set_channel(uri_str);
  group_model = new GroupModel(this);
  node_model = new NodeModel(this);
  auto group_list = std::make_unique<across::profile::GroupList>(
      across::profile::list_all_groups());
  group_model->setList(std::move(group_list));
}
void Across::setChannel(const char *str) { across::acolors::set_channel(str); }
