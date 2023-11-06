use std::sync::Mutex;

use lazy_static::lazy_static;
use tokio::runtime::Runtime;
lazy_static! {
    pub static ref ACROSS_RUNTIME: Mutex<Runtime> =
        Mutex::new(match tokio::runtime::Runtime::new() {
            Ok(a) => a,
            Err(e) => panic!("Failed to create AcrossRuntime:{}", e),
        });
}
