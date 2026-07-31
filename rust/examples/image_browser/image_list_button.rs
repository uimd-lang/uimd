mod imagelistbutton_ui;

use imagelistbutton_ui::*;

#[derive(Default)]
struct ImageListButtonApp;

impl ImageListButtonUIEvents for ImageListButtonApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageListButtonUI::new();
        let mut app = ImageListButtonApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
