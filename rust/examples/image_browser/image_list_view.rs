mod imagelistview_ui;

use imagelistview_ui::*;

#[derive(Default)]
struct ImageListViewApp;

impl ImageListViewUIEvents for ImageListViewApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageListViewUI::new();
        let mut app = ImageListViewApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
