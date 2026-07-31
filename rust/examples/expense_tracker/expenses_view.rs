mod expensesview_ui;

use expensesview_ui::*;

#[derive(Default)]
struct ExpensesViewApp;

impl ExpensesViewUIEvents for ExpensesViewApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ExpensesViewUI::new();
        let mut app = ExpensesViewApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
