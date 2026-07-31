mod source_separator_row_ui;
mod source_separator_scroll_ui;

use source_separator_row_ui::SourceSeparatorRowUI;
use source_separator_scroll_ui::*;

#[derive(Default)]
struct SourceSeparatorScrollApp;

impl SourceSeparatorScrollUIEvents for SourceSeparatorScrollApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = SourceSeparatorScrollUI::new();
        let row = SourceSeparatorRowUI::new();
        ui.base.add_named_child_window("[0]", row.base);
        let mut app = SourceSeparatorScrollApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
