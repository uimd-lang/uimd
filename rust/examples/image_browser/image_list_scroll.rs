mod imagelistscroll_ui;

use imagelistscroll_ui::*;

#[derive(Default)]
struct ImageListScrollApp;

impl ImageListScrollUIEvents for ImageListScrollApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageListScrollUI::new();
        let mut app = ImageListScrollApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
