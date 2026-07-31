mod cells_ui;

use cells_ui::*;

#[derive(Default)]
struct CellsApp;

impl CellsUIEvents for CellsApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = CellsUI::new();
        let mut app = CellsApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
