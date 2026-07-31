mod image_gallery_ui;

use image_gallery_ui::*;

#[derive(Default)]
struct ImageGalleryApp;

impl ImageGalleryUIEvents for ImageGalleryApp
{
    fn on_close_btn_click(&mut self, ui: &mut ImageGalleryUI)
    {
        ui.base.request_close();
    }
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageGalleryUI::new();
        let mut app = ImageGalleryApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
