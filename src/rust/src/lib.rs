mod acolors_process;
mod across_rpc;
mod convert;
pub mod ffi;
mod qml_process;

#[cfg(test)]
mod tests;

use lazy_static::lazy_static;
use std::sync::Mutex;
use tokio::runtime::Runtime;

pub use across_rpc::AcrossRpc;
use qml_process::QmlProcess;

lazy_static! {
    pub static ref ACROSS_RPC: Mutex<AcrossRpc> = Mutex::new(match AcrossRpc::new() {
        Ok(a) => a,
        Err(e) => panic!("Failed to create AcrossRpc:{}", e),
    });
    pub static ref ACROSS_RUNTIME: Mutex<Runtime> =
        Mutex::new(match tokio::runtime::Runtime::new() {
            Ok(a) => a,
            Err(e) => panic!("Failed to create AcrossRuntime:{}", e),
        });
    pub static ref ACROSS_QML_PROCESS: Mutex<QmlProcess> = Mutex::new(QmlProcess::new());
}
