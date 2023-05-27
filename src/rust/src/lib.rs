use acolors_proto::profile_manager_client::ProfileManagerClient;
use acolors_proto::CountGroupsRequest;
use lazy_static::lazy_static;
use std::sync::Mutex;
use tonic::transport::Endpoint;
use tonic::transport::Channel;

pub mod acolors_proto {
    tonic::include_proto!("acolors");
}
#[cxx::bridge(namespace = "across::core")]
mod ffi {
    extern "Rust" {
        type AcrossRpc;
        fn count_groups() -> u64;
        fn set_default_endpoint();
    }
}

pub fn set_default_endpoint() {
    let mut lock = ACROSS_RPC.lock().unwrap();
    lock.set_channel(Channel::builder("http://127.0.0.1:11451".parse().unwrap()));
}

lazy_static! {
    static ref ACROSS_RPC: Mutex<AcrossRpc> = Mutex::new(match AcrossRpc::new() {
        Ok(a) => a,
        Err(e) => panic!("Failed to create AcrossRpc:{}", e),
    });
}
async fn async_count_groups(channel: Endpoint) -> anyhow::Result<u64> {
    let mut client = ProfileManagerClient::connect(channel).await?;
    let request = tonic::Request::new(CountGroupsRequest {});
    let response = client.count_groups(request).await?;
    Ok(response.into_inner().count)
}
pub fn count_groups() -> u64 {
    let lock = ACROSS_RPC.lock().unwrap();
    let channel = match &lock.endpoint {
        Some(c) => c.clone(),
        None => return 0,
    };
    let count = lock.runtime.block_on(async_count_groups(channel));
    count.unwrap_or_default()
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
    use crate::{acolors_proto, count_groups, ACROSS_RPC};
    use acolors_proto::profile_manager_client::ProfileManagerClient;
    use acolors_proto::ListAllGroupsRequest;
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
    fn set_default_endpoint() {
        let mut lock = ACROSS_RPC.lock().unwrap();
        lock.set_channel(Channel::builder("http://127.0.0.1:11451".parse().unwrap()));
    }
    #[test]
    fn test_count_groups() -> Result<(), Box<dyn std::error::Error>> {
        set_default_endpoint();
        dbg!(count_groups());
        Ok(())
    }
}
