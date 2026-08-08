mod formular_ui;

use formular_ui::*;
use std::io::{self, Write};

#[derive(Default)]
struct FormularApp
{
    action: String,
}

impl FormularUIEvents for FormularApp
{
    fn on_save_btn_click(&mut self, ui: &mut FormularUI)
    {
        self.action = "save".to_string();
        ui.base.request_close();
    }

    fn on_cancel_btn_click(&mut self, ui: &mut FormularUI)
    {
        self.action = "cancel".to_string();
        ui.base.request_close();
    }
}

fn yaml_scalar(value: &str) -> String
{
    if value.is_empty()
    {
        "\"\"".to_string()
    }
    else
    {
        value.to_string()
    }
}

fn yaml_bool(value: bool) -> &'static str
{
    if value { "true" } else { "false" }
}

fn selected_combo_text(ui: &FormularUI) -> String
{
    ui.country_combo
        .options()
        .get(ui.country_combo.selected_index().max(0) as usize)
        .cloned()
        .unwrap_or_default()
}

fn selected_role_text(ui: &FormularUI) -> String
{
    ui.role_listbox
        .options()
        .get(ui.role_listbox.selected_index().max(0) as usize)
        .cloned()
        .unwrap_or_default()
}

fn form_yaml(ui: &FormularUI, action: &str) -> String
{
    format!(
        concat!(
            "action: {}\n",
            "form:\n",
            "  name: {}\n",
            "  email: {}\n",
            "  age: {}\n",
            "  description: {}\n",
            "  country: {}\n",
            "  roles:\n",
            "  - {}\n",
            "  email_notifications: {}\n",
            "  accepted_terms: {}\n",
        ),
        action,
        yaml_scalar(&ui.name_input.value()),
        yaml_scalar(&ui.email_input.value()),
        ui.age_input.display_value(),
        yaml_scalar(&ui.description_input.value()),
        yaml_scalar(&selected_combo_text(ui)),
        yaml_scalar(&selected_role_text(ui)),
        yaml_bool(ui.notify_check.checked()),
        yaml_bool(ui.terms_check.checked()),
    )
}

fn print_terminal_lines(text: &str) -> io::Result<()>
{
    let mut output = io::stdout().lock();
    output.write_all(b"\r\n")?;
    output.write_all(text.replace('\n', "\r\n").as_bytes())?;
    output.flush()
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = FormularUI::new();
        let mut app = FormularApp::default();
        let result = ui.run(&mut app, &std::env::args().collect::<Vec<_>>());
        if !app.action.is_empty()
            && let Err(error) = print_terminal_lines(&form_yaml(&ui, &app.action))
        {
            eprintln!("Failed to write form result: {error}");
            return 1;
        }
        result
    });
    std::process::exit(code);
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn save_and_cancel_request_close_and_form_yaml_matches_cpp()
    {
        let mut ui = FormularUI::new();
        ui.name_input.set_value("Marek");
        ui.email_input.set_value("marek@example.com");
        ui.age_input.set_value(30.0);
        ui.description_input.set_value("hello");
        ui.country_combo.set_selected_index(2);
        ui.role_listbox.set_selected_values(&["QA".to_string()]);
        ui.notify_check.set_checked(true);
        ui.terms_check.set_checked(true);
        let mut app = FormularApp::default();

        app.on_save_btn_click(&mut ui);

        assert_eq!(app.action, "save");
        assert!(ui.base.should_close());
        assert_eq!(
            form_yaml(&ui, &app.action),
            concat!(
                "action: save\n",
                "form:\n",
                "  name: Marek\n",
                "  email: marek@example.com\n",
                "  age: 30\n",
                "  description: hello\n",
                "  country: Poland\n",
                "  roles:\n",
                "  - QA\n",
                "  email_notifications: true\n",
                "  accepted_terms: true\n",
            )
        );

        let mut cancel_ui = FormularUI::new();
        let mut cancel_app = FormularApp::default();
        cancel_app.on_cancel_btn_click(&mut cancel_ui);
        assert_eq!(cancel_app.action, "cancel");
        assert!(cancel_ui.base.should_close());
    }
}
