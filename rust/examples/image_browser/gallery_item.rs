mod galleryitem_ui;

use galleryitem_ui::*;

#[derive(Default)]
struct GalleryItemApp;

impl GalleryItemUIEvents for GalleryItemApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = GalleryItemUI::new();
        let mut app = GalleryItemApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
