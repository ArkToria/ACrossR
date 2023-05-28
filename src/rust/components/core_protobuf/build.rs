fn main() -> Result<(), Box<dyn std::error::Error>> {
    println!("cargo:rerun-if-changed=../../proto/acolors.proto");
    tonic_build::compile_protos("../../proto/acolors.proto")?;

    Ok(())
}
