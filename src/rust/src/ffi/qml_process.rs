use crate::ACROSS_QML_PROCESS;
use anyhow::Result;

#[cxx::bridge(namespace = "across::qmlprocess")]
pub mod ffi {
    extern "Rust" {
        fn set_port(port: u16) -> Result<()>;
        fn start() -> Result<()>;
        fn close() -> Result<()>;
        fn force_close();
    }
}
pub fn set_port(port: u16) -> Result<()> {
    let mut lock = ACROSS_QML_PROCESS.lock().unwrap();
    lock.set_port(port);
    Ok(())
}
pub fn start() -> Result<()> {
    let mut lock = ACROSS_QML_PROCESS.lock().unwrap();
    lock.start()?;
    Ok(())
}
pub fn force_close() {
    let lock = ACROSS_QML_PROCESS.lock();
    if let Ok(mut lock) = lock {
        if let Err(e) = lock.close() {
            println!("QML Process closing error: {e}");
        }
    }
}
pub fn close() -> Result<()> {
    let mut lock = ACROSS_QML_PROCESS.lock().unwrap();
    lock.close()?;
    Ok(())
}
