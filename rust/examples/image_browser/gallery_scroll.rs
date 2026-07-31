mod galleryscroll_ui;

use galleryscroll_ui::*;

#[derive(Default)]
struct GalleryScrollApp;

impl GalleryScrollUIEvents for GalleryScrollApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = GalleryScrollUI::new();
        let mut app = GalleryScrollApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
