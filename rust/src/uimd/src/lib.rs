extern crate self as uimd;

mod core;
mod dialogs;
mod elements;
mod image;
mod mcp;
mod runtime;
mod terminal;

pub use core::*;
pub use dialogs::*;
pub use elements::*;
pub use image::*;
pub use mcp::*;
pub use runtime::*;
pub use terminal::*;

pub const VERSION: &str = env!("CARGO_PKG_VERSION");

pub fn runtime_version() -> &'static str
{
    VERSION
}

#[cfg(test)]
mod tests
{
    #[test]
    fn public_runtime_version_is_the_cargo_package_version()
    {
        assert_eq!(super::runtime_version(), env!("CARGO_PKG_VERSION"));
    }
}
