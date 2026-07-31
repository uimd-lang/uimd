mod settingsview_ui;

use settingsview_ui::*;

#[derive(Default)]
struct SettingsViewApp;

impl SettingsViewUIEvents for SettingsViewApp {}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = SettingsViewUI::new();
        let mut app = SettingsViewApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
