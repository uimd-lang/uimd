mod categoriesview_ui;

use categoriesview_ui::*;

#[derive(Default)]
struct CategoriesViewApp;

impl CategoriesViewUIEvents for CategoriesViewApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = CategoriesViewUI::new();
        let mut app = CategoriesViewApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
