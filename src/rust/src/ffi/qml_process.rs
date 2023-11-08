use log::{error, info};

use crate::{
    qml_process::{CloseError, StartError},
    ACROSS_QML_PROCESS,
};

#[cxx::bridge(namespace = "across::qmlprocess")]
pub mod ffi {
    extern "Rust" {
        fn set_port(port: u16);
        fn start() -> Result<()>;
        fn close() -> Result<()>;
        fn force_close();
    }
}
pub fn set_port(port: u16) {
    let mut lock = ACROSS_QML_PROCESS.lock().unwrap();
    lock.set_port(port);
    info!("QmlProcess port set: {port}");
}
pub fn start() -> Result<(), StartError> {
    let mut lock = ACROSS_QML_PROCESS.lock().unwrap();
    match lock.start() {
        Ok(res) => {
            info!("QmlProcess start: {:?}", &res);
            Ok(())
        }
        Err(e) => {
            error!("QmlProcess start error: {}", &e);
            Err(e)
        }
    }
}
pub fn force_close() {
    let lock = ACROSS_QML_PROCESS.lock();
    if let Ok(mut lock) = lock {
        if let Err(e) = lock.close() {
            error!("QML Process closing error: {e}");
        }
    }
}
pub fn close() -> Result<(), CloseError> {
    let mut lock = ACROSS_QML_PROCESS.lock().unwrap();
    match lock.close() {
        Ok(res) => {
            info!("QmlProcess close: {:?}", &res);
            Ok(())
        }
        Err(e) => {
            error!("QmlProcess close error: {}", &e);
            Err(e)
        }
    }
}
