use lazy_static::lazy_static;
use std::sync::Mutex;
use tonic::transport::Endpoint;
mod convert;
mod profile;
mod test_fun;

lazy_static! {
    static ref ACROSS_RPC: Mutex<AcrossRpc> = Mutex::new(match AcrossRpc::new() {
        Ok(a) => a,
        Err(e) => panic!("Failed to create AcrossRpc:{}", e),
    });
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
