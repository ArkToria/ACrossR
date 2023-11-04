#include "across.h"
#include "rusty_bridge/profile.h"
#include "src/cpp/groupmodel.h"
#include "src/cpp/nodemodel.h"
#include <memory>

Across::Across(const char *uri_str,QObject *parent)
    : QObject{parent}
{
    across::profile::set_channel(uri_str);
    group_model = std::make_shared<GroupModel>();
    node_model= std::make_shared<NodeModel>();
    auto group_list = std::make_unique<across::profile::GroupList>(across::profile::list_all_groups());
    group_model->setList(std::move(group_list));
}
void Across::set_channel(const char *str){
    across::profile::set_channel(str);
}
