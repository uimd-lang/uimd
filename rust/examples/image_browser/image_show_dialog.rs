mod imageshowdialog_ui;

use imageshowdialog_ui::*;

#[derive(Default)]
struct ImageShowDialogApp;

impl ImageShowDialogUIEvents for ImageShowDialogApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = ImageShowDialogUI::new();
        let mut app = ImageShowDialogApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
