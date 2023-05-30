use core_protobuf::acolors_proto::profile_manager_client::ProfileManagerClient;
use core_protobuf::acolors_proto::*;
use lazy_static::lazy_static;
use std::sync::Mutex;
use tonic::transport::Channel;
use tonic::transport::Endpoint;
mod convert;
mod test_fun;

lazy_static! {
    static ref ACROSS_RPC: Mutex<AcrossRpc> = Mutex::new(match AcrossRpc::new() {
        Ok(a) => a,
        Err(e) => panic!("Failed to create AcrossRpc:{}", e),
    });
}
#[cxx::bridge(namespace = "across::core")]
mod ffi {

    #[derive(Clone, Debug)]
    pub struct GroupData {
        pub id: i64,
        pub name: String,
        pub is_subscription: bool,
        pub group_type: i32,
        pub url: String,
        pub cycle_time: i32,
        pub create_at: i64,
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
        pub create_at: i64,
        pub modified_at: i64,
    }

    extern "Rust" {
        fn set_channel(uri: &str) -> Result<()>;

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
async fn async_empty_group_by_id(channel: Endpoint, group_id: i64) -> anyhow::Result<()> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(EmptyGroupByIdRequest { group_id });
    let _response = client.empty_group_by_id(request).await?;
    Ok(())
}
fn empty_group_by_id(group_id: i64) -> anyhow::Result<()> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_empty_group_by_id(channel, group_id));
    result
}
async fn async_update_group_by_id(
    channel: Endpoint,
    group_id: i64,
    use_proxy: bool,
) -> anyhow::Result<()> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(UpdateGroupByIdRequest {
        group_id,
        use_proxy,
    });
    let _response = client.update_group_by_id(request).await?;
    Ok(())
}
fn update_group_by_id(group_id: i64, use_proxy: bool) -> anyhow::Result<()> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_update_group_by_id(channel, group_id, use_proxy));
    result
}
async fn async_append_node_by_url(
    channel: Endpoint,
    group_id: i64,
    url: String,
) -> anyhow::Result<i64> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(AppendNodeByUrlRequest { url, group_id });
    let response = client.append_node_by_url(request).await?;
    Ok(response.into_inner().node_id)
}
fn append_node_by_url(group_id: i64, url: String) -> anyhow::Result<i64> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_append_node_by_url(channel, group_id, url));
    result
}
async fn async_append_node(
    channel: Endpoint,
    group_id: i64,
    node_data: NodeData,
) -> anyhow::Result<i64> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(AppendNodeRequest {
        data: Some(node_data),
        group_id,
    });
    let response = client.append_node(request).await?;
    Ok(response.into_inner().node_id)
}
fn append_node(group_id: i64, node_data: ffi::NodeData) -> anyhow::Result<i64> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_append_node(channel, group_id, node_data.into()));
    result
}
async fn async_append_group(channel: Endpoint, group_data: GroupData) -> anyhow::Result<i64> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(AppendGroupRequest {
        data: Some(group_data),
    });
    let response = client.append_group(request).await?;
    Ok(response.into_inner().group_id)
}
fn append_group(group_data: ffi::GroupData) -> anyhow::Result<i64> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_append_group(channel, group_data.into()));
    result
}
async fn async_remove_group_by_id(channel: Endpoint, group_id: i64) -> anyhow::Result<()> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(RemoveGroupByIdRequest { group_id });
    let _response = client.remove_group_by_id(request).await?;
    Ok(())
}
fn remove_group_by_id(group_id: i64) -> anyhow::Result<()> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_remove_group_by_id(channel, group_id));
    result
}
async fn async_remove_node_by_id(channel: Endpoint, node_id: i64) -> anyhow::Result<()> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(RemoveNodeByIdRequest { node_id });
    let _response = client.remove_node_by_id(request).await?;
    Ok(())
}
fn remove_node_by_id(node_id: i64) -> anyhow::Result<()> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_remove_node_by_id(channel, node_id));
    result
}
async fn async_set_node_by_url(channel: Endpoint, node_id: i64, url: String) -> anyhow::Result<()> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(SetNodeByUrlRequest { node_id, url });
    let _response = client.set_node_by_url(request).await?;
    Ok(())
}
fn set_node_by_url(node_id: i64, url: String) -> anyhow::Result<()> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_set_node_by_url(channel, node_id, url));
    result
}
async fn async_set_node_by_id(
    channel: Endpoint,
    node_id: i64,
    node_data: ffi::NodeData,
) -> anyhow::Result<()> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(SetNodeByIdRequest {
        node_id,
        data: Some(node_data.into()),
    });
    let _response = client.set_node_by_id(request).await?;
    Ok(())
}
fn set_node_by_id(node_id: i64, node_data: ffi::NodeData) -> anyhow::Result<()> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_set_node_by_id(channel, node_id, node_data));
    result
}
async fn async_set_group_by_id(
    channel: Endpoint,
    group_id: i64,
    group_data: ffi::GroupData,
) -> anyhow::Result<()> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(SetGroupByIdRequest {
        group_id,
        data: Some(group_data.into()),
    });
    let _response = client.set_group_by_id(request).await?;
    Ok(())
}
fn set_group_by_id(group_id: i64, group_data: ffi::GroupData) -> anyhow::Result<()> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let result = lock
        .runtime
        .block_on(async_set_group_by_id(channel, group_id, group_data));
    result
}
async fn async_get_node_by_id(channel: Endpoint, node_id: i64) -> anyhow::Result<ffi::NodeData> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(GetNodeByIdRequest { node_id });
    let response = client.get_node_by_id(request).await?;
    Ok(response.into_inner().into())
}
fn get_node_by_id(node_id: i64) -> anyhow::Result<ffi::NodeData> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let node_data = lock
        .runtime
        .block_on(async_get_node_by_id(channel, node_id));
    node_data
}
async fn async_get_group_by_id(channel: Endpoint, group_id: i64) -> anyhow::Result<ffi::GroupData> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(GetGroupByIdRequest { group_id });
    let response = client.get_group_by_id(request).await?;
    Ok(response.into_inner().into())
}
fn get_group_by_id(group_id: i64) -> anyhow::Result<ffi::GroupData> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let group_data = lock
        .runtime
        .block_on(async_get_group_by_id(channel, group_id));
    group_data
}
async fn async_list_all_nodes(channel: Endpoint, group_id: i64) -> anyhow::Result<ffi::NodeList> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(ListAllNodesRequest { group_id });
    let response = client.list_all_nodes(request).await?;
    Ok(response.into_inner().into())
}
fn list_all_nodes(group_id: i64) -> anyhow::Result<ffi::NodeList> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let node_list = lock
        .runtime
        .block_on(async_list_all_nodes(channel, group_id));
    node_list
}
async fn async_count_nodes(channel: Endpoint, group_id: i64) -> anyhow::Result<u64> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(CountNodesRequest { group_id });
    let response = client.count_nodes(request).await?;
    Ok(response.into_inner().count)
}
fn count_nodes(group_id: i64) -> anyhow::Result<u64> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let count = lock.runtime.block_on(async_count_nodes(channel, group_id));
    count
}
async fn async_list_all_groups(channel: Endpoint) -> anyhow::Result<ffi::GroupList> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(ListAllGroupsRequest {});
    let response = client.list_all_groups(request).await?;
    Ok(response.into_inner().into())
}
pub fn list_all_groups() -> anyhow::Result<ffi::GroupList> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let group_list = lock.runtime.block_on(async_list_all_groups(channel));
    group_list
}
pub fn set_channel(uri: &str) -> anyhow::Result<()> {
    let mut lock = ACROSS_RPC.lock().unwrap();
    lock.set_channel(Channel::builder(uri.parse().unwrap()));
    Ok(())
}

async fn async_count_groups(channel: Endpoint) -> anyhow::Result<u64> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(CountGroupsRequest {});
    let response = client.count_groups(request).await?;
    Ok(response.into_inner().count)
}
pub fn count_groups() -> anyhow::Result<u64> {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return Err(anyhow::anyhow!("No Endpoint")),
    };
    let count = lock.runtime.block_on(async_count_groups(channel));
    count
}
pub fn rusty_cxxbridge_integer() -> i32 {
    42
}

pub struct AcrossRpc {
    runtime: tokio::runtime::Runtime,
    endpoint: Option<tonic::transport::Endpoint>,
}

impl AcrossRpc {
    pub fn new() -> std::io::Result<Self> {
        let runtime = tokio::runtime::Runtime::new()?;
        Ok(AcrossRpc {
            runtime,
            endpoint: None,
        })
    }
    pub fn set_channel(&mut self, channel: Endpoint) {
        self.endpoint = Some(channel);
    }
}
