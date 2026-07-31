mod special_elements_ui;

use special_elements_ui::*;

#[derive(Default)]
struct SpecialElementsApp;

impl SpecialElementsUIEvents for SpecialElementsApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = SpecialElementsUI::new();
        let mut app = SpecialElementsApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
