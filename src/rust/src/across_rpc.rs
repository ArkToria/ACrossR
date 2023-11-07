use tonic::transport::Endpoint;

use crate::acolors_process::AcolorsManager;

pub struct AcrossRpc {
    pub acolors: Option<AcolorsManager>,
    pub endpoint: Option<tonic::transport::Endpoint>,
}

impl AcrossRpc {
    pub fn new() -> std::io::Result<Self> {
        Ok(AcrossRpc {
            acolors: None,
            endpoint: None,
        })
    }

    pub fn set_channel(&mut self, channel: Endpoint) {
        self.endpoint = Some(channel);
    }
}
