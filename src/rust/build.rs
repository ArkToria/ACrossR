fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("cargo:rerun-if-changed=../../submodules/AColoRS/proto/acolors.proto");
    tonic_build::compile_protos("../../submodules/AColoRS/proto/acolors.proto")?;
    println!("cargo:rustc-link-lib=crypto");
    println!("cargo:rustc-link-lib=ssl");

    Ok(())
}
