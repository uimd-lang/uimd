mod formular_ui;

use formular_ui::*;

#[derive(Default)]
struct FormularApp;

impl FormularUIEvents for FormularApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = FormularUI::new();
        let mut app = FormularApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
