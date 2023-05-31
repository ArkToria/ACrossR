use core_protobuf::acolors_proto;

impl From<crate::profile::ffi::GroupData> for acolors_proto::GroupData {
    fn from(group_data: crate::profile::ffi::GroupData) -> Self {
        acolors_proto::GroupData {
            id: group_data.id,
            name: group_data.name,
            is_subscription: group_data.is_subscription,
            group_type: group_data.group_type,
            url: group_data.url,
            cycle_time: group_data.cycle_time,
            created_at: group_data.created_at,
            modified_at: group_data.modified_at,
        }
    }
}
impl From<acolors_proto::GroupData> for crate::profile::ffi::GroupData {
    fn from(group_data: acolors_proto::GroupData) -> Self {
        crate::profile::ffi::GroupData {
            id: group_data.id,
            name: group_data.name,
            is_subscription: group_data.is_subscription,
            group_type: group_data.group_type,
            url: group_data.url,
            cycle_time: group_data.cycle_time,
            created_at: group_data.created_at,
            modified_at: group_data.modified_at,
        }
    }
}
impl From<crate::profile::ffi::NodeData> for acolors_proto::NodeData {
    fn from(node_data: crate::profile::ffi::NodeData) -> Self {
        acolors_proto::NodeData {
            id: node_data.id,
            name: node_data.name,
            group_id: node_data.group_id,
            group_name: node_data.group_name,
            routing_id: node_data.routing_id,
            routing_name: node_data.routing_name,
            protocol: node_data.protocol,
            address: node_data.address,
            port: node_data.port,
            password: node_data.password,
            raw: node_data.raw,
            url: node_data.url,
            latency: node_data.latency,
            upload: node_data.upload,
            download: node_data.download,
            created_at: node_data.created_at,
            modified_at: node_data.modified_at,
        }
    }
}
impl From<acolors_proto::NodeData> for crate::profile::ffi::NodeData {
    fn from(node_data: acolors_proto::NodeData) -> Self {
        crate::profile::ffi::NodeData {
            id: node_data.id,
            name: node_data.name,
            group_id: node_data.group_id,
            group_name: node_data.group_name,
            routing_id: node_data.routing_id,
            routing_name: node_data.routing_name,
            protocol: node_data.protocol,
            address: node_data.address,
            port: node_data.port,
            password: node_data.password,
            raw: node_data.raw,
            url: node_data.url,
            latency: node_data.latency,
            upload: node_data.upload,
            download: node_data.download,
            created_at: node_data.created_at,
            modified_at: node_data.modified_at,
        }
    }
}

impl From<crate::profile::ffi::GroupList> for acolors_proto::GroupList {
    fn from(group_data: crate::profile::ffi::GroupList) -> Self {
        acolors_proto::GroupList {
            length: group_data.length,
            entries: group_data.entries.into_iter().map(|d| d.into()).collect(),
        }
    }
}
impl From<acolors_proto::GroupList> for crate::profile::ffi::GroupList {
    fn from(group_data: acolors_proto::GroupList) -> Self {
        crate::profile::ffi::GroupList {
            length: group_data.length,
            entries: group_data.entries.into_iter().map(|d| d.into()).collect(),
        }
    }
}

impl From<crate::profile::ffi::NodeList> for acolors_proto::NodeList {
    fn from(group_data: crate::profile::ffi::NodeList) -> Self {
        acolors_proto::NodeList {
            length: group_data.length,
            entries: group_data.entries.into_iter().map(|d| d.into()).collect(),
        }
    }
}
impl From<acolors_proto::NodeList> for crate::profile::ffi::NodeList {
    fn from(group_data: acolors_proto::NodeList) -> Self {
        crate::profile::ffi::NodeList {
            length: group_data.length,
            entries: group_data.entries.into_iter().map(|d| d.into()).collect(),
        }
    }
}
