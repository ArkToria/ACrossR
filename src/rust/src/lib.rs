use core_protobuf::acolors_proto::profile_manager_client::ProfileManagerClient;
use core_protobuf::acolors_proto::CountGroupsRequest;
use core_protobuf::acolors_proto::ListAllGroupsRequest;
use lazy_static::lazy_static;
use std::sync::Mutex;
use tonic::transport::Channel;
use tonic::transport::Endpoint;
mod convert;

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
    extern "Rust" {
        type AcrossRpc;

        fn list_all_groups() -> Result<GroupList>;
        fn count_groups() -> Result<u64>;
        fn set_channel(uri: &str) -> Result<()>;
    }
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
#[cfg(test)]
mod tests {
    use crate::{count_groups, set_channel};
    use acolors_proto::profile_manager_client::ProfileManagerClient;
    use acolors_proto::ListAllGroupsRequest;
    use core_protobuf::acolors_proto;
    use tonic::transport::Channel;
    #[tokio::test]
    async fn my_test() -> Result<(), Box<dyn std::error::Error>> {
        let channel = Channel::builder("http://127.0.0.1:11451".parse().unwrap());
        let mut client = ProfileManagerClient::connect(channel).await?;
        let request = tonic::Request::new(ListAllGroupsRequest {});
        let response = client.list_all_groups(request).await?;
        print!("{:?}", response.into_inner());
        Ok(())
    }
    #[test]
    fn test_count_groups() -> Result<(), Box<dyn std::error::Error>> {
        set_channel("http://127.0.0.1:11451")?;

        dbg!(count_groups()?);
        Ok(())
    }
}
