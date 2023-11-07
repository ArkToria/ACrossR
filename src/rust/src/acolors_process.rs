use std::borrow::BorrowMut;
use std::env::current_exe;
use std::fmt::Display;
use std::fs::create_dir_all;
use std::io;
use std::path::Path;
use std::path::PathBuf;
use std::time::Duration;

use core_protobuf::acolors_proto::manager_client::ManagerClient;
use core_protobuf::acolors_proto::ShutdownRequest;

use directories::ProjectDirs;
use tokio::time::sleep;
use tonic::transport::Channel;
use tonic::transport::Endpoint;
use utils::net::tcp_get_available_port;

#[derive(Debug)]
pub struct AcolorsManager {
    port: u16,
    config: PathBuf,
    dbpath: PathBuf,
    child: std::process::Child,
    manager: ManagerClient<Channel>,
}
impl Drop for AcolorsManager {
    fn drop(&mut self) {
        self.force_close();
    }
}
fn try_find_acolors() -> PathBuf {
    const EXE_NAME: &str = "acolors";
    if let Ok(mut path) = current_exe() {
        path.pop();
        path.push(EXE_NAME);
        if path.exists() {
            return path;
        }
    }
    PathBuf::from(EXE_NAME)
}
#[derive(Debug)]
pub enum AcolorsExecError {
    Args(ArgsError),
    CommandStart(std::io::Error),
    Endpoint(tonic::transport::Error),
    Connect,
}
impl Display for AcolorsExecError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::Args(_) => write!(f, "failed to process acolors exec args"),
            Self::CommandStart(_) => write!(f, "failed to execute acolors command"),
            Self::Endpoint(_) => write!(f, "failed to create Endpoint from uri"),
            Self::Connect => write!(f, "failed to connect acolors"),
        }
    }
}
impl std::error::Error for AcolorsExecError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            Self::Args(e) => Some(e),
            Self::CommandStart(e) => Some(e),
            Self::Endpoint(e) => Some(e),
            Self::Connect => None,
        }
    }
}
impl From<ArgsError> for AcolorsExecError {
    fn from(err: ArgsError) -> Self {
        Self::Args(err)
    }
}
#[derive(Debug)]
pub enum AcolorsClosingError {
    ShutdownError(Box<tonic::Status>),
    ChildWaitError(io::Error),
}
impl Display for AcolorsClosingError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            AcolorsClosingError::ShutdownError(_) => write!(f, "failed to shutdown acolors"),
            AcolorsClosingError::ChildWaitError(_) => write!(f, "failed to wait child process"),
        }
    }
}
impl std::error::Error for AcolorsClosingError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            AcolorsClosingError::ShutdownError(e) => Some(e),
            AcolorsClosingError::ChildWaitError(e) => Some(e),
        }
    }
}
impl AcolorsManager {
    async fn async_close(manager: &mut ManagerClient<Channel>) -> tonic::Result<()> {
        let request = tonic::Request::new(ShutdownRequest {});
        let _response = manager.shutdown(request).await?;

        Ok(())
    }
    pub fn force_close(&mut self) {
        self.child
            .kill()
            .unwrap_or_else(|e| println!("AcolorsManager child kill Failed : {e}"));
        if let Ok(exit) = self.child.wait() {
            println!("AcolorsManager exit: {exit}");
        }
    }
    pub async fn close(&mut self) -> Result<(), AcolorsClosingError> {
        Self::async_close(self.manager.borrow_mut())
            .await
            .map_err(|e| AcolorsClosingError::ShutdownError(Box::new(e)))?;
        self.child
            .wait()
            .map_err(AcolorsClosingError::ChildWaitError)?;
        Ok(())
    }

    pub async fn new() -> Result<Self, AcolorsExecError> {
        let (port, config, dbpath) = acolors_args()?;

        let mut command = acolors_command(try_find_acolors(), port, &config, &dbpath);
        let child = command.spawn().map_err(AcolorsExecError::CommandStart)?;

        let endpoint = Endpoint::try_from(format!("http://127.0.0.1:{}", port))
            .map_err(AcolorsExecError::Endpoint)?;

        let manager = acolors_try_connect(endpoint).await?;

        Ok(AcolorsManager {
            port,
            config,
            dbpath,
            child,
            manager,
        })
    }

    pub fn port(&self) -> u16 {
        self.port
    }

    pub fn config(&self) -> &Path {
        &self.config
    }

    pub fn dbpath(&self) -> &Path {
        &self.dbpath
    }
}

#[derive(Debug)]
pub enum ArgsError {
    NoPortsAvailable,
    ProjectDirsNotFound,
    CreatingDirError(io::Error),
}
impl Display for ArgsError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Self::NoPortsAvailable => write!(f, "no ports available"),
            Self::ProjectDirsNotFound => write!(f, "project dirs not found"),
            Self::CreatingDirError(_) => write!(f, "failed to create dir"),
        }
    }
}
impl std::error::Error for ArgsError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            Self::NoPortsAvailable => None,
            Self::ProjectDirsNotFound => None,
            Self::CreatingDirError(e) => Some(e),
        }
    }
}

fn acolors_args() -> Result<(u16, PathBuf, PathBuf), ArgsError> {
    let port = tcp_get_available_port(11451..19198).ok_or(ArgsError::NoPortsAvailable)?;
    let project_dir =
        ProjectDirs::from("org", "Arktoria", "ACross").ok_or(ArgsError::ProjectDirsNotFound)?;
    let config_path = project_dir.config_dir().to_owned();
    let mut config = config_path.clone();
    create_dir_all(&config).map_err(ArgsError::CreatingDirError)?;
    config.push("acolors.json");
    let mut dbpath = project_dir.data_dir().to_owned();
    create_dir_all(&dbpath).map_err(ArgsError::CreatingDirError)?;
    dbpath.push("across.db");
    Ok((port, config, dbpath))
}

async fn acolors_try_connect(
    endpoint: Endpoint,
) -> Result<ManagerClient<Channel>, AcolorsExecError> {
    let mut profile = None;
    const TRY_TIMES: usize = 10;
    const TRY_DURATION: Duration = Duration::from_millis(100);
    for _ in 0..TRY_TIMES {
        sleep(TRY_DURATION).await;
        let manager = ManagerClient::connect(endpoint.clone()).await;
        if let Ok(c) = manager {
            profile = Some(c);
            break;
        }
    }
    profile.map(Ok).unwrap_or(Err(AcolorsExecError::Connect))
}

fn acolors_command(
    program: PathBuf,
    port: u16,
    config: &Path,
    dbpath: &Path,
) -> std::process::Command {
    let mut command = std::process::Command::new(program);
    command
        .arg("serve")
        .arg("-p")
        .arg(port.to_string())
        .arg("-c")
        .arg(config.as_os_str())
        .arg("-d")
        .arg(dbpath.as_os_str());
    command
}
#[cfg(test)]
mod tests {

    use crate::{ffi::acolors::serve, ffi::profile::list_all_groups};

    #[test]
    fn test_list_all_groups() -> anyhow::Result<()> {
        serve()?;
        let groups = list_all_groups()?;
        dbg!(&groups);

        Ok(())
    }
}
