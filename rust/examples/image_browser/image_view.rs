mod imageview_ui;

use imageview_ui::*;

#[derive(Default)]
struct ImageViewApp;

impl ImageViewUIEvents for ImageViewApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageViewUI::new();
        let mut app = ImageViewApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
