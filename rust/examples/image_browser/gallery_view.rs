mod galleryview_ui;

use galleryview_ui::*;

#[derive(Default)]
struct GalleryViewApp;

impl GalleryViewUIEvents for GalleryViewApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = GalleryViewUI::new();
        let mut app = GalleryViewApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
