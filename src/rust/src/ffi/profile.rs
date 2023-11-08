use crate::{ACROSS_RPC, ACROSS_RUNTIME};
use core_protobuf::acolors_proto::profile_manager_client::ProfileManagerClient;
use core_protobuf::acolors_proto::*;

use tonic::transport::Endpoint;

#[cxx::bridge(namespace = "across::profile")]
pub mod ffi {
    #[derive(Clone, Debug)]
    pub struct GroupData {
        pub id: i64,
        pub name: String,
        pub is_subscription: bool,
        pub group_type: i32,
        pub url: String,
        pub cycle_time: i32,
        pub created_at: i64,
        pub modified_at: i64,
    }
    #[derive(Clone, Debug)]
    pub struct GroupList {
        pub length: u64,
        pub entries: Vec<GroupData>,
    }
    #[derive(Clone, Debug)]
    pub struct NodeList {
        pub length: u64,
        pub entries: Vec<NodeData>,
    }
    #[derive(Clone, Debug)]
    pub struct NodeData {
        pub id: i64,
        pub name: String,
        pub group_id: i64,
        pub group_name: String,
        pub routing_id: i32,
        pub routing_name: String,
        pub protocol: String,
        pub address: String,
        pub port: i32,
        pub password: String,
        pub raw: String,
        pub url: String,
        pub latency: i32,
        pub upload: i64,
        pub download: i64,
        pub created_at: i64,
        pub modified_at: i64,
    }

    extern "Rust" {
        fn count_groups() -> Result<u64>;
        fn list_all_groups() -> Result<GroupList>;

        fn count_nodes(group_id: i64) -> Result<u64>;
        fn list_all_nodes(group_id: i64) -> Result<NodeList>;

        fn get_group_by_id(group_id: i64) -> Result<GroupData>;
        fn get_node_by_id(node_id: i64) -> Result<NodeData>;

        fn set_group_by_id(group_id: i64, group_data: GroupData) -> Result<()>;
        fn set_node_by_id(node_id: i64, node_data: NodeData) -> Result<()>;
        fn set_node_by_url(node_id: i64, url: String) -> Result<()>;

        fn remove_group_by_id(group_id: i64) -> Result<()>;
        fn remove_node_by_id(node_id: i64) -> Result<()>;

        fn append_group(group_data: GroupData) -> Result<i64>;
        fn append_node(group_id: i64, node_data: NodeData) -> Result<i64>;
        fn append_node_by_url(group_id: i64, url: String) -> Result<i64>;

        fn update_group_by_id(group_id: i64, use_proxy: bool) -> Result<()>;

        fn empty_group_by_id(group_id: i64) -> Result<()>;

    }
}
#[derive(Debug)]
pub enum ProfileError {
    Connect(tonic::transport::Error),
    Request(Box<tonic::Status>),
    NoEndpoint,
}
impl std::fmt::Display for ProfileError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            ProfileError::Connect(_) => f.write_str("failed to connect"),
            ProfileError::Request(_) => f.write_str("failed to request"),
            ProfileError::NoEndpoint => f.write_str("no endpoint"),
        }
    }
}
impl std::error::Error for ProfileError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            ProfileError::Connect(e) => Some(e),
            ProfileError::Request(e) => Some(e),
            ProfileError::NoEndpoint => None,
        }
    }
}
impl From<tonic::transport::Error> for ProfileError {
    fn from(err: tonic::transport::Error) -> Self {
        Self::Connect(err)
    }
}
impl From<tonic::Status> for ProfileError {
    fn from(err: tonic::Status) -> Self {
        Self::Request(Box::new(err))
    }
}
async fn async_empty_group_by_id(channel: Endpoint, group_id: i64) -> Result<(), ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(EmptyGroupByIdRequest { group_id });
    let _response = client.empty_group_by_id(request).await?;
    Ok(())
}
pub fn empty_group_by_id(group_id: i64) -> Result<(), ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_empty_group_by_id(channel, group_id));
    result
}
async fn async_update_group_by_id(
    channel: Endpoint,
    group_id: i64,
    use_proxy: bool,
) -> Result<(), ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(UpdateGroupByIdRequest {
        group_id,
        use_proxy,
    });
    let _response = client.update_group_by_id(request).await?;
    Ok(())
}
pub fn update_group_by_id(group_id: i64, use_proxy: bool) -> Result<(), ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_update_group_by_id(channel, group_id, use_proxy));
    result
}
async fn async_append_node_by_url(
    channel: Endpoint,
    group_id: i64,
    url: String,
) -> Result<i64, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(AppendNodeByUrlRequest { url, group_id });
    let response = client.append_node_by_url(request).await?;
    Ok(response.into_inner().node_id)
}
pub fn append_node_by_url(group_id: i64, url: String) -> Result<i64, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_append_node_by_url(channel, group_id, url));
    result
}
async fn async_append_node(
    channel: Endpoint,
    group_id: i64,
    node_data: NodeData,
) -> Result<i64, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(AppendNodeRequest {
        data: Some(node_data),
        group_id,
    });
    let response = client.append_node(request).await?;
    Ok(response.into_inner().node_id)
}
pub fn append_node(group_id: i64, node_data: ffi::NodeData) -> Result<i64, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME.lock().unwrap().block_on(async_append_node(
        channel,
        group_id,
        node_data.into(),
    ));
    result
}
async fn async_append_group(channel: Endpoint, group_data: GroupData) -> Result<i64, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(AppendGroupRequest {
        data: Some(group_data),
    });
    let response = client.append_group(request).await?;
    Ok(response.into_inner().group_id)
}
pub fn append_group(group_data: ffi::GroupData) -> Result<i64, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_append_group(channel, group_data.into()));
    result
}
async fn async_remove_group_by_id(channel: Endpoint, group_id: i64) -> Result<(), ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(RemoveGroupByIdRequest { group_id });
    let _response = client.remove_group_by_id(request).await?;
    Ok(())
}
pub fn remove_group_by_id(group_id: i64) -> Result<(), ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_remove_group_by_id(channel, group_id));
    result
}
async fn async_remove_node_by_id(channel: Endpoint, node_id: i64) -> Result<(), ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(RemoveNodeByIdRequest { node_id });
    let _response = client.remove_node_by_id(request).await?;
    Ok(())
}
pub fn remove_node_by_id(node_id: i64) -> Result<(), ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_remove_node_by_id(channel, node_id));
    result
}
async fn async_set_node_by_url(
    channel: Endpoint,
    node_id: i64,
    url: String,
) -> Result<(), ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(SetNodeByUrlRequest { node_id, url });
    let _response = client.set_node_by_url(request).await?;
    Ok(())
}
pub fn set_node_by_url(node_id: i64, url: String) -> Result<(), ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_set_node_by_url(channel, node_id, url));
    result
}
async fn async_set_node_by_id(
    channel: Endpoint,
    node_id: i64,
    node_data: ffi::NodeData,
) -> Result<(), ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(SetNodeByIdRequest {
        node_id,
        data: Some(node_data.into()),
    });
    let _response = client.set_node_by_id(request).await?;
    Ok(())
}
pub fn set_node_by_id(node_id: i64, node_data: ffi::NodeData) -> Result<(), ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_set_node_by_id(channel, node_id, node_data));
    result
}
async fn async_set_group_by_id(
    channel: Endpoint,
    group_id: i64,
    group_data: ffi::GroupData,
) -> Result<(), ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(SetGroupByIdRequest {
        group_id,
        data: Some(group_data.into()),
    });
    let _response = client.set_group_by_id(request).await?;
    Ok(())
}
pub fn set_group_by_id(group_id: i64, group_data: ffi::GroupData) -> Result<(), ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let result = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_set_group_by_id(channel, group_id, group_data));
    result
}
async fn async_get_node_by_id(
    channel: Endpoint,
    node_id: i64,
) -> Result<ffi::NodeData, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(GetNodeByIdRequest { node_id });
    let response = client.get_node_by_id(request).await?;
    Ok(response.into_inner().into())
}
pub fn get_node_by_id(node_id: i64) -> Result<ffi::NodeData, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let node_data = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_get_node_by_id(channel, node_id));
    node_data
}
async fn async_get_group_by_id(
    channel: Endpoint,
    group_id: i64,
) -> Result<ffi::GroupData, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(GetGroupByIdRequest { group_id });
    let response = client.get_group_by_id(request).await?;
    Ok(response.into_inner().into())
}
pub fn get_group_by_id(group_id: i64) -> Result<ffi::GroupData, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let group_data = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_get_group_by_id(channel, group_id));
    group_data
}
async fn async_list_all_nodes(
    channel: Endpoint,
    group_id: i64,
) -> Result<ffi::NodeList, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(ListAllNodesRequest { group_id });
    let response = client.list_all_nodes(request).await?;
    Ok(response.into_inner().into())
}
pub fn list_all_nodes(group_id: i64) -> Result<ffi::NodeList, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let node_list = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_list_all_nodes(channel, group_id));
    node_list
}
async fn async_count_nodes(channel: Endpoint, group_id: i64) -> Result<u64, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(CountNodesRequest { group_id });
    let response = client.count_nodes(request).await?;
    Ok(response.into_inner().count)
}
pub fn count_nodes(group_id: i64) -> Result<u64, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let count = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_count_nodes(channel, group_id));
    count
}
async fn async_list_all_groups(channel: Endpoint) -> Result<ffi::GroupList, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(ListAllGroupsRequest {});
    let response = client.list_all_groups(request).await?;
    Ok(response.into_inner().into())
}
pub fn list_all_groups() -> Result<ffi::GroupList, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let group_list = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_list_all_groups(channel));
    group_list
}

async fn async_count_groups(channel: Endpoint) -> Result<u64, ProfileError> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(CountGroupsRequest {});
    let response = client.count_groups(request).await?;
    Ok(response.into_inner().count)
}
pub fn count_groups() -> Result<u64, ProfileError> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(ProfileError::NoEndpoint),
    };
    let count = ACROSS_RUNTIME
        .lock()
        .unwrap()
        .block_on(async_count_groups(channel));
    count
}
