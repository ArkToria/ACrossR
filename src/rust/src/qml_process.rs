use anyhow::{anyhow, Result};
use std::{env::current_exe, path::PathBuf, process::Child};

#[derive(Default)]
pub struct QmlProcess {
    child: Option<Child>,
    port: Option<u16>,
}

impl Drop for QmlProcess {
    fn drop(&mut self) {
        self.close().unwrap_or_else(|e| println!("{e}"));
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
fn qml_command(port: u16, program: PathBuf) -> Result<std::process::Command> {
    let mut command = std::process::Command::new(program);
    command.arg("-p").arg(port.to_string());
    Ok(command)
}
impl QmlProcess {
    pub fn start(&mut self) -> Result<()> {
        match self.child {
            Some(_) => return Err(anyhow!("QmlProcess already started")),
            None => {
                let program = try_find_across_qml();
                let mut command = qml_command(
                    self.port.ok_or(anyhow!("QmlProcess port not configured"))?,
                    program,
                )?;
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

    pub fn set_port(&mut self, port: u16) {
        self.port = Some(port);
    }
}
