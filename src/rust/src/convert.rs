use core_protobuf::acolors_proto;

impl From<crate::ffi::GroupData> for acolors_proto::GroupData {
    fn from(group_data: crate::ffi::GroupData) -> Self {
        acolors_proto::GroupData {
            id: group_data.id,
            name: group_data.name,
            is_subscription: group_data.is_subscription,
            group_type: group_data.group_type,
            url: group_data.url,
            cycle_time: group_data.cycle_time,
            create_at: group_data.create_at,
            modified_at: group_data.modified_at,
        }
    }
}
impl From<acolors_proto::GroupData> for crate::ffi::GroupData {
    fn from(group_data: acolors_proto::GroupData) -> Self {
        crate::ffi::GroupData {
            id: group_data.id,
            name: group_data.name,
            is_subscription: group_data.is_subscription,
            group_type: group_data.group_type,
            url: group_data.url,
            cycle_time: group_data.cycle_time,
            create_at: group_data.create_at,
            modified_at: group_data.modified_at,
        }
    }
}

impl From<crate::ffi::GroupList> for acolors_proto::GroupList {
    fn from(group_data: crate::ffi::GroupList) -> Self {
        acolors_proto::GroupList {
            length: group_data.length,
            entries: group_data.entries.into_iter().map(|d| d.into()).collect(),
        }
    }
}
impl From<acolors_proto::GroupList> for crate::ffi::GroupList {
    fn from(group_data: acolors_proto::GroupList) -> Self {
        crate::ffi::GroupList {
            length: group_data.length,
            entries: group_data.entries.into_iter().map(|d| d.into()).collect(),
        }
    }
}
