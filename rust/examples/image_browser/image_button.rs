mod imagebutton_ui;

use imagebutton_ui::*;

#[derive(Default)]
struct ImageButtonApp;

impl ImageButtonUIEvents for ImageButtonApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageButtonUI::new();
        let mut app = ImageButtonApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
