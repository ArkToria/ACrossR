use std::borrow::BorrowMut;
use std::env::current_exe;
use std::fs::create_dir_all;
use std::path::PathBuf;
use std::time::Duration;

use anyhow::anyhow;
use anyhow::Result;
use core_protobuf::acolors_proto::manager_client::ManagerClient;
use core_protobuf::acolors_proto::ShutdownRequest;

use directories::ProjectDirs;
use tokio::time::sleep;
use tonic::transport::Channel;
use tonic::transport::Endpoint;
use utils::net::tcp_get_available_port;

use crate::runtime::ACROSS_RUNTIME;

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
    if let Ok(mut path) = current_exe() {
        path.push("acolors");
        if path.exists() {
            return path;
        }
    }
    PathBuf::from("acolors")
}
impl AcolorsManager {
    async fn async_close(manager: &mut ManagerClient<Channel>) -> Result<()> {
        let request = tonic::Request::new(ShutdownRequest {});
        let _response = manager.shutdown(request).await?;

        Ok(())
    }
    pub fn force_close(&mut self) {
        {
            match ACROSS_RUNTIME.lock() {
                Ok(lock) => {
                    lock.block_on(Self::async_close(self.manager.borrow_mut()))
                        .unwrap_or_else(|e| println!("{e}"));
                }
                Err(e) => {
                    println!("{e}");
                }
            }
        }
        self.child
            .kill()
            .unwrap_or_else(|e| println!("AcolorsManager child kill Failed : {e}"));
        if let Ok(exit) = self.child.wait() {
            println!("AcolorsManager exit: {exit}");
        }
    }
    pub fn close(&mut self) -> Result<()> {
        {
            ACROSS_RUNTIME
                .lock()
                .unwrap()
                .block_on(Self::async_close(self.manager.borrow_mut()))?;
        }
        self.child.wait()?;
        Ok(())
    }
    pub async fn new() -> Result<Self> {
        let port = tcp_get_available_port(11451..19198).ok_or(anyhow!("No ports available"))?;
        let project_dir = ProjectDirs::from("org", "Arktoria", "ACross")
            .ok_or(anyhow!("Config directory error"))?;
        let config_path = project_dir.config_dir().to_owned();
        let mut config = config_path.clone();
        create_dir_all(&config)?;
        config.push("acolors.json");
        let mut dbpath = project_dir.data_dir().to_owned();
        create_dir_all(&dbpath)?;
        dbpath.push("across.db");
        let program = try_find_acolors();
        let mut command = std::process::Command::new(program);
        command
            .arg("serve")
            .arg("-p")
            .arg(port.to_string())
            .arg("-c")
            .arg(config.as_os_str())
            .arg("-d")
            .arg(dbpath.as_os_str());
        let child = command.spawn()?;

        let uri = format!("http://127.0.0.1:{}", port);
        let endpoint = Endpoint::try_from(uri)?;

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
        let manager = match profile {
            Some(c) => c,
            None => return Err(anyhow!("process connecting failed")),
        };

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
}
#[cfg(test)]
mod tests {

    use crate::{across_rpc::serve, profile::list_all_groups};

    #[test]
    fn test_list_all_groups() -> anyhow::Result<()> {
        serve()?;
        let groups = list_all_groups()?;
        dbg!(&groups);

        Ok(())
    }
}
