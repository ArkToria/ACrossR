use std::{env::current_exe, fmt::Display, path::PathBuf, process::Child};

use log::error;

#[derive(Default)]
pub struct QmlProcess {
    child: Option<Child>,
    port: Option<u16>,
}

impl Drop for QmlProcess {
    fn drop(&mut self) {
        self.close().unwrap_or_else(|e| error!("{e}"));
    }
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
fn qml_command(port: u16, program: PathBuf) -> std::process::Command {
    let mut command = std::process::Command::new(program);
    command.arg("-p").arg(port.to_string());
    command
}
#[derive(Debug)]
pub enum StartError {
    AlreadyStarted,
    NotConfigured,
    ProcessSpawnFailed(std::io::Error),
}
impl Display for StartError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            StartError::AlreadyStarted => f.write_str("qml process already started"),
            StartError::NotConfigured => f.write_str("process arguments not configured"),
            StartError::ProcessSpawnFailed(_) => f.write_str("failed to spawn process"),
        }
    }
}
impl std::error::Error for StartError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            StartError::AlreadyStarted => None,
            StartError::NotConfigured => None,
            StartError::ProcessSpawnFailed(e) => Some(e),
        }
    }
}
#[derive(Debug)]
pub enum CloseError {
    NotStarted,
    KillFailed(std::io::Error),
    WaitFailed(std::io::Error),
}
impl Display for CloseError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            CloseError::NotStarted => f.write_str("qml process not started"),
            CloseError::KillFailed(_) => f.write_str("failed to kill qml process"),
            CloseError::WaitFailed(_) => f.write_str("failed to wait qml process exit"),
        }
    }
}
impl std::error::Error for CloseError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            CloseError::NotStarted => None,
            CloseError::KillFailed(e) => Some(e),
            CloseError::WaitFailed(e) => Some(e),
        }
    }
}
impl QmlProcess {
    pub fn check_closed(&mut self) {
        let is_closed = match self.child.as_mut() {
            Some(c) => c.try_wait().is_ok(),
            None => true,
        };
        if is_closed && self.child.is_some() {
            self.child.take();
        }
    }
    pub fn start(&mut self) -> Result<(), StartError> {
        self.check_closed();
        match self.child {
            Some(_) => return Err(StartError::AlreadyStarted),
            None => {
                let program = try_find_across_qml();
                let mut command = qml_command(self.port.ok_or(StartError::NotConfigured)?, program);
                let child = command.spawn().map_err(StartError::ProcessSpawnFailed)?;
                self.child = Some(child);
            }
        }

        Ok(())
    }

    pub fn close(&mut self) -> Result<(), CloseError> {
        let mut child = self.child.take();
        match child.as_mut() {
            Some(c) => {
                c.kill().map_err(CloseError::KillFailed)?;
                c.wait().map_err(CloseError::WaitFailed)?;
            }
            None => {
                return Err(CloseError::NotStarted);
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

    pub fn set_port(&mut self, port: u16) {
        self.port = Some(port);
    }
}
