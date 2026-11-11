mod compatibility_ui;

use compatibility_ui::*;

#[derive(Default)]
struct CompatibilityApp;

impl CompatibilityUIEvents for CompatibilityApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = CompatibilityUI::new();
        let mut app = CompatibilityApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
