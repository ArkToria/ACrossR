pub mod acolors_proto {
    tonic::include_proto!("acolors");
}
#[cxx::bridge]
mod ffi {
    extern "Rust" {
        fn rusty_cxxbridge_integer() -> i32;
    }
}

pub fn rusty_cxxbridge_integer() -> i32 {
    42
}
