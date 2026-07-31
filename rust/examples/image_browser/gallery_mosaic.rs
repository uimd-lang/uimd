mod gallerymosaic_ui;

use gallerymosaic_ui::*;

#[derive(Default)]
struct GalleryMosaicApp;

impl GalleryMosaicUIEvents for GalleryMosaicApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = GalleryMosaicUI::new();
        let mut app = GalleryMosaicApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
