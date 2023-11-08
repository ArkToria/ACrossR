use std::sync::Arc;

use log::{error, SetLoggerError};
use spdlog::{
    error::EnvLevelError,
    formatter::{pattern, PatternFormatter},
    init_log_crate_proxy, Logger,
};

#[cxx::bridge(namespace = "across")]
pub mod ffi {
    extern "Rust" {
        fn init() -> Result<()>;
    }
}

#[derive(Debug)]
pub enum InitError {
    SetLog(SetLoggerError),
    EnvLevel(spdlog::error::EnvLevelError),
}
impl std::fmt::Display for InitError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            InitError::SetLog(_) => f.write_str("set logger error"),
            InitError::EnvLevel(_) => f.write_str("env level error"),
        }
    }
}
impl std::error::Error for InitError {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            InitError::SetLog(e) => Some(e),
            InitError::EnvLevel(e) => Some(e),
        }
    }
}
impl From<SetLoggerError> for InitError {
    fn from(err: SetLoggerError) -> Self {
        Self::SetLog(err)
    }
}
impl From<EnvLevelError> for InitError {
    fn from(err: EnvLevelError) -> Self {
        Self::EnvLevel(err)
    }
}

pub fn init() -> Result<(), InitError> {
    init_log()?;

    Ok(())
}

fn init_log() -> Result<(), InitError> {
    init_log_crate_proxy()?;
    spdlog::init_env_level()?;
    log::set_max_level(log::LevelFilter::Info);
    let proxy = spdlog::log_crate_proxy();
    if !cfg!(debug_assertions) {
        let formatter: Box<PatternFormatter<_>> = Box::new(PatternFormatter::new(pattern!(
            "[{date} {hour}:{minute}:{second}.{millisecond}] [{logger}] [{^{level}}] {payload}{eol}"
        )));
        spdlog::default_logger()
            .sinks()
            .iter()
            .for_each(|s| s.set_formatter(formatter.clone()));
    }
    let sinks = spdlog::default_logger().sinks().to_owned();
    let mut builder = Logger::builder();
    let builder = builder.sinks(sinks);
    let logger = builder.name("across-core").build();
    match logger {
        Ok(logger) => proxy.set_logger(Some(Arc::new(logger))),
        Err(e) => error!("Failed to build logger, using the default logger : {e}"),
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use log::{error, info};

    use super::{init, InitError};

    #[test]
    pub fn test_init() -> Result<(), InitError> {
        error!("before:Hello?");
        info!("before:Hello!");
        init()?;
        error!("after:Hello?");
        info!("after:Hello!");
        Ok(())
    }
}
