use anyhow::{anyhow, Result};
use lazy_static::lazy_static;
use std::{env::current_exe, path::PathBuf, process::Child, sync::Mutex};

lazy_static! {
    pub static ref ACROSS_QML_PROCESS: Mutex<QmlProcess> = Mutex::new(QmlProcess::new());
}

#[cxx::bridge(namespace = "across::qmlprocess")]
pub mod ffi {
    extern "Rust" {
        fn set_port(port: u16) -> Result<()>;
        fn start() -> Result<()>;
        fn close() -> Result<()>;
        fn force_close();
    }
}
pub struct QmlProcess {
    child: Option<Child>,
    port: Option<u16>,
}

impl Drop for QmlProcess {
    fn drop(&mut self) {
        self.close().unwrap_or_else(|e| println!("{e}"));
    }
}
pub fn set_port(port: u16) -> Result<()> {
    let mut lock = ACROSS_QML_PROCESS.lock().unwrap();
    lock.port = Some(port);
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
fn try_find_across_qml() -> PathBuf {
    const EXE_NAME: &str = "across-qml";
    if let Ok(mut path) = current_exe() {
        path.pop();
        path.push(EXE_NAME);
        if path.exists() {
            return path;
        }
    }
    PathBuf::from(EXE_NAME)
}

impl QmlProcess {
    pub fn start(&mut self) -> Result<()> {
        match self.child {
            Some(_) => return Err(anyhow!("QmlProcess already started")),
            None => {
                let program = try_find_across_qml();
                let mut command = std::process::Command::new(program);
                command.arg("-p").arg(
                    self.port
                        .ok_or(anyhow!("QmlProcess port not configured"))?
                        .to_string(),
                );
                let child = command.spawn()?;
                self.child = Some(child);
            }
        }

        Ok(())
    }
    pub fn close(&mut self) -> Result<()> {
        let mut child = self.child.take();
        match child.as_mut() {
            Some(c) => {
                c.kill()?;
                c.wait()?;
            }
            None => {
                return Err(anyhow!("QmlProcess not started"));
            }
        }

        Ok(())
    }
    pub fn new() -> Self {
        QmlProcess {
            child: None,
            port: None,
        }
    }
}
