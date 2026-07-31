mod imagelistitem_ui;

use imagelistitem_ui::*;

#[derive(Default)]
struct ImageListItemApp;

impl ImageListItemUIEvents for ImageListItemApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageListItemUI::new();
        let mut app = ImageListItemApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
