use acolors_process::AcolorsManager;
use anyhow::anyhow;
use anyhow::Result;
use lazy_static::lazy_static;

use std::sync::Mutex;
use tonic::transport::{Channel, Endpoint};

use crate::acolors_process;
use crate::runtime::ACROSS_RUNTIME;

lazy_static! {
    pub static ref ACROSS_RPC: Mutex<AcrossRpc> = Mutex::new(match AcrossRpc::new() {
        Ok(a) => a,
        Err(e) => panic!("Failed to create AcrossRpc:{}", e),
    });
}
#[cxx::bridge(namespace = "across::acolors")]
pub mod ffi {
    extern "Rust" {
        fn set_channel(uri: &str) -> Result<()>;
        fn serve() -> Result<()>;
        fn serve_port() -> Result<u16>;
        fn close() -> Result<()>;
        fn force_close();
    }
}
pub fn close() -> Result<()> {
    let mut lock = ACROSS_RPC.lock().unwrap();
    let acolors = lock.acolors.as_mut();
    if let Some(a) = acolors {
        a.close()?;
    }
    Ok(())
}
pub fn force_close() {
    let mut lock = ACROSS_RPC.lock().unwrap();
    let acolors = lock.acolors.as_mut();
    if let Some(a) = acolors {
        a.force_close();
    }
}
pub fn serve_port() -> anyhow::Result<u16> {
    let lock = ACROSS_RPC.lock().unwrap();
    Ok(lock
        .acolors
        .as_ref()
        .ok_or(anyhow!("Process not found"))?
        .port())
}

pub fn set_channel(uri: &str) -> anyhow::Result<()> {
    let mut lock = ACROSS_RPC.lock().unwrap();
    lock.set_channel(Channel::builder(uri.parse().unwrap()));
    Ok(())
}
async fn async_serve() -> anyhow::Result<AcolorsManager> {
    let acolors = AcolorsManager::new().await?;
    Ok(acolors)
}
pub fn serve() -> anyhow::Result<()> {
    let mut lock = ACROSS_RPC.lock().unwrap();
    let acolors = ACROSS_RUNTIME.lock().unwrap().block_on(async_serve())?;
    let uri = format!("http://127.0.0.1:{}", acolors.port());
    lock.acolors = Some(acolors);
    lock.set_channel(Channel::builder(uri.parse().unwrap()));
    Ok(())
}

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
