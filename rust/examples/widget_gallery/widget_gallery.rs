mod widget_gallery_ui;

use widget_gallery_ui::*;

const DEFAULT_NAME: &str = "Ada Lovelace";
const DEFAULT_COUNT: f64 = 3.0;

#[derive(Default)]
struct WidgetGalleryApp;

impl WidgetGalleryApp
{
    fn summary_text(ui: &WidgetGalleryUI) -> String
    {
        let name = ui.name_input.borrow().value().to_string();
        let count = ui.count_input.borrow().number_value();
        let theme = ui.theme_combo.borrow().selected_value();
        let mode = ui.mode_list.borrow().selected_value();
        let enabled = ui.enabled_check.borrow().checked();
        let count = if count.fract() == 0.0
        {
            format!("{count:.0}")
        }
        else
        {
            format!("{count}")
        };
        format!(
            "values:\n  name: {name}\n  count: {count}\n  theme: {theme}\n  mode: {mode}\n  enabled: {}",
            if enabled { "True" } else { "False" },
        )
    }

    fn refresh_summary(ui: &mut WidgetGalleryUI)
    {
        ui.spans.borrow_mut().set_spans(vec![
            uimd::LabelSpan::new("SpanLabel", "#facc15", ""),
            uimd::LabelSpan::new(" can mix ", "", ""),
            uimd::LabelSpan::new("foreground", "#38bdf8", ""),
            uimd::LabelSpan::new(" and ", "", ""),
            uimd::LabelSpan::new("background", "#111827", "#f97316"),
            uimd::LabelSpan::new(" colors.", "", ""),
        ]);
        let summary = Self::summary_text(ui);
        ui.summary.borrow_mut().set_text(summary);
    }

    fn reset_values(ui: &mut WidgetGalleryUI)
    {
        ui.name_input.borrow_mut().set_value(DEFAULT_NAME);
        ui.count_input.borrow_mut().set_number_value(DEFAULT_COUNT);
        ui.enabled_check.borrow_mut().set_checked(true);
        ui.theme_combo.borrow_mut().set_selected_index(0);
        ui.mode_list.borrow_mut().set_selected_index(0);
        Self::refresh_summary(ui);
    }
}

impl WidgetGalleryUIEvents for WidgetGalleryApp
{
    fn on_close_btn_click(&mut self, ui: &mut WidgetGalleryUI)
    {
        ui.base.request_close();
    }

    fn on_apply_btn_click(&mut self, ui: &mut WidgetGalleryUI)
    {
        Self::refresh_summary(ui);
    }

    fn on_reset_btn_click(&mut self, ui: &mut WidgetGalleryUI)
    {
        Self::reset_values(ui);
    }
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = WidgetGalleryUI::new();
        WidgetGalleryApp::refresh_summary(&mut ui);
        let mut app = WidgetGalleryApp;
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}
