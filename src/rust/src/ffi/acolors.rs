use std::fmt::Display;

use acolors_process::AcolorsManager;
use log::error;
use log::info;
use tonic::transport::Channel;

use crate::acolors_process;
use crate::acolors_process::AcolorsClosingError;
use crate::acolors_process::AcolorsExecError;
use crate::ACROSS_RPC;
use crate::ACROSS_RUNTIME;

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
pub fn close() -> Result<(), AcolorsClosingError> {
    let mut lock = ACROSS_RPC.lock().unwrap();
    let acolors = lock.acolors.as_mut();
    if let Some(a) = acolors {
        ACROSS_RUNTIME.lock().unwrap().block_on(a.close())?;
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
#[derive(Debug)]
pub struct NotServedError {}
impl Display for NotServedError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str("acolors not served")
    }
}
impl std::error::Error for NotServedError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        None
    }
}
pub fn serve_port() -> Result<u16, NotServedError> {
    let lock = ACROSS_RPC.lock().unwrap();
    lock.acolors
        .as_ref()
        .map(AcolorsManager::port)
        .ok_or(NotServedError {})
}

pub fn set_channel(uri: &str) -> Result<(), http::uri::InvalidUri> {
    let mut lock = ACROSS_RPC.lock().unwrap();
    let uri = match uri.parse() {
        Ok(uri) => {
            info!("Acolors set channel: {}", uri);
            Ok(uri)
        }
        Err(e) => {
            error!("Acolors set channel failed: {}", e);
            Err(e)
        }
    };
    lock.set_channel(Channel::builder(uri?));
    Ok(())
}
async fn async_serve() -> Result<AcolorsManager, AcolorsExecError> {
    let acolors = match AcolorsManager::new().await {
        Ok(a) => {
            info!("Acolors serves");
            Ok(a)
        }
        Err(e) => {
            info!("Failed to serve acolors: {e}");
            Err(e)
        }
    }?;
    Ok(acolors)
}
pub fn serve() -> Result<(), AcolorsExecError> {
    let mut lock = ACROSS_RPC.lock().unwrap();
    let acolors = ACROSS_RUNTIME.lock().unwrap().block_on(async_serve())?;
    let uri = format!("http://127.0.0.1:{}", acolors.port());
    lock.acolors = Some(acolors);
    lock.set_channel(Channel::builder(uri.parse().unwrap()));
    Ok(())
}
