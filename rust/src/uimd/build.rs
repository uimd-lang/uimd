use std::env;
use std::path::{Path, PathBuf};

fn stb_include_directory(manifest_directory: &Path) -> PathBuf
{
    let candidates = [
        manifest_directory.join("vendor/stb"),
        manifest_directory.join("../../../cpp/third_party/stb"),
    ];
    candidates
        .into_iter()
        .find(|candidate| candidate.join("stb_image.h").is_file())
        .unwrap_or_else(||
        {
            panic!(
                "cannot find canonical stb_image.h; expected vendor/stb or cpp/third_party/stb"
            )
        })
}

fn main()
{
    let manifest_directory = PathBuf::from(
        env::var_os("CARGO_MANIFEST_DIR")
            .expect("Cargo must define CARGO_MANIFEST_DIR"),
    );
    let include_directory = stb_include_directory(&manifest_directory);
    let header = include_directory.join("stb_image.h");
    let adapter = manifest_directory.join("src/stb_image_adapter.c");
    println!("cargo:rerun-if-changed={}", header.display());
    println!("cargo:rerun-if-changed={}", adapter.display());
    cc::Build::new()
        .include(include_directory)
        .file(adapter)
        .compile("uimd_stb_image");
}
