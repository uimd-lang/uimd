mod calculator_ui;

use calculator_ui::*;

const DISPLAY_ZERO: &str = "0";
const DISPLAY_ERROR: &str = "Error";
const PERCENT_FACTOR: f64 = 100.0;

struct ExpressionParser<'a>
{
    input: &'a [u8],
    position: usize,
}

impl<'a> ExpressionParser<'a>
{
    fn new(input: &'a str) -> Self
    {
        Self { input: input.as_bytes(), position: 0 }
    }

    fn parse(mut self) -> Option<f64>
    {
        let value = self.expression()?;
        self.skip_spaces();
        (self.position == self.input.len()).then_some(value)
    }

    fn expression(&mut self) -> Option<f64>
    {
        let mut value = self.term()?;
        loop
        {
            self.skip_spaces();
            if self.matches(b'+')
            {
                value += self.term()?;
            }
            else if self.matches(b'-')
            {
                value -= self.term()?;
            }
            else
            {
                return Some(value);
            }
        }
    }

    fn term(&mut self) -> Option<f64>
    {
        let mut value = self.factor()?;
        loop
        {
            self.skip_spaces();
            if self.matches(b'*')
            {
                value *= self.factor()?;
            }
            else if self.matches(b'/')
            {
                value /= self.factor()?;
            }
            else
            {
                return Some(value);
            }
        }
    }

    fn factor(&mut self) -> Option<f64>
    {
        self.skip_spaces();
        if self.matches(b'+')
        {
            return self.factor();
        }
        if self.matches(b'-')
        {
            return self.factor().map(|value| -value);
        }
        self.number()
    }

    fn number(&mut self) -> Option<f64>
    {
        self.skip_spaces();
        let start = self.position;
        while self.position < self.input.len()
            && (self.input[self.position].is_ascii_digit() || self.input[self.position] == b'.')
        {
            self.position += 1;
        }
        if start == self.position
        {
            return None;
        }
        std::str::from_utf8(&self.input[start..self.position]).ok()?.parse().ok()
    }

    fn matches(&mut self, value: u8) -> bool
    {
        if self.input.get(self.position) == Some(&value)
        {
            self.position += 1;
            true
        }
        else
        {
            false
        }
    }

    fn skip_spaces(&mut self)
    {
        while self.input.get(self.position).is_some_and(u8::is_ascii_whitespace)
        {
            self.position += 1;
        }
    }
}

struct CalculatorApp
{
    expression: String,
    after_result: bool,
}

impl CalculatorApp
{
    fn new(ui: &mut CalculatorUI) -> Self
    {
        let app = Self { expression: DISPLAY_ZERO.to_string(), after_result: false };
        app.refresh(ui);
        app
    }

    fn digit(&mut self, ui: &mut CalculatorUI, digit: char)
    {
        if self.after_result || self.expression == DISPLAY_ERROR
        {
            self.expression.clear();
            self.after_result = false;
        }
        if digit == '.'
        {
            self.append_decimal_point();
        }
        else if self.expression == DISPLAY_ZERO
        {
            self.expression = digit.to_string();
        }
        else
        {
            let current = self.current_number_text();
            if current == "0" || current == "-0"
            {
                self.replace_current_number(&current.replacen(DISPLAY_ZERO, &digit.to_string(), 1));
            }
            else
            {
                self.expression.push(digit);
            }
        }
        self.refresh(ui);
    }

    fn operation(&mut self, ui: &mut CalculatorUI, operation: char)
    {
        if self.expression == DISPLAY_ERROR
        {
            self.expression = DISPLAY_ZERO.to_string();
        }
        if self.expression.is_empty()
        {
            if operation == '-'
            {
                self.expression.push(operation);
                self.after_result = false;
                self.refresh(ui);
            }
            return;
        }
        if self.expression.chars().last().is_some_and(is_operator)
        {
            if operation == '-' && !self.expression.ends_with('-')
            {
                self.expression.push(operation);
            }
            else
            {
                while self.expression.chars().last().is_some_and(is_operator)
                {
                    self.expression.pop();
                }
                self.expression.push(operation);
            }
        }
        else
        {
            self.expression.push(operation);
        }
        self.after_result = false;
        self.refresh(ui);
    }

    fn equals(&mut self, ui: &mut CalculatorUI)
    {
        match ExpressionParser::new(&self.expression).parse()
        {
            Some(result) if result.is_finite() =>
            {
                self.expression = format_calculator(result);
                self.after_result = true;
                self.refresh(ui);
            }
            _ => self.show_error(ui),
        }
    }

    fn reset(&mut self, ui: &mut CalculatorUI)
    {
        self.expression = DISPLAY_ZERO.to_string();
        self.after_result = false;
        self.refresh(ui);
    }

    fn delete(&mut self, ui: &mut CalculatorUI)
    {
        if self.after_result || self.expression == DISPLAY_ERROR
        {
            self.reset(ui);
            return;
        }
        self.expression.pop();
        if self.expression.is_empty()
        {
            self.expression = DISPLAY_ZERO.to_string();
        }
        self.refresh(ui);
    }

    fn percent(&mut self, ui: &mut CalculatorUI)
    {
        let current = self.current_number_text();
        let Ok(value) = current.parse::<f64>() else
        {
            self.show_error(ui);
            return;
        };
        self.replace_current_number(&format_calculator(value / PERCENT_FACTOR));
        self.after_result = false;
        self.refresh(ui);
    }

    fn refresh(&self, ui: &mut CalculatorUI)
    {
        ui.display.borrow_mut().set_text(if self.expression.is_empty()
        {
            DISPLAY_ZERO
        }
        else
        {
            &self.expression
        });
    }

    fn show_error(&mut self, ui: &mut CalculatorUI)
    {
        self.expression = DISPLAY_ERROR.to_string();
        self.after_result = true;
        self.refresh(ui);
    }

    fn append_decimal_point(&mut self)
    {
        if self.current_number_text().contains('.')
        {
            return;
        }
        if self.expression.is_empty() || self.expression.chars().last().is_some_and(is_operator)
        {
            self.expression.push_str("0.");
        }
        else
        {
            self.expression.push('.');
        }
    }

    fn current_number_span(&self) -> (usize, usize)
    {
        let bytes = self.expression.as_bytes();
        let end = bytes.len();
        if end == 0 || is_operator(bytes[end - 1] as char)
        {
            return (end, end);
        }
        let mut start = end;
        while start > 0 && (bytes[start - 1].is_ascii_digit() || bytes[start - 1] == b'.')
        {
            start -= 1;
        }
        if start > 0
            && bytes[start - 1] == b'-'
            && (start == 1 || is_operator(bytes[start - 2] as char))
        {
            start -= 1;
        }
        (start, end)
    }

    fn current_number_text(&self) -> String
    {
        let (start, end) = self.current_number_span();
        self.expression[start..end].to_string()
    }

    fn replace_current_number(&mut self, value: &str)
    {
        let (start, end) = self.current_number_span();
        self.expression.replace_range(start..end, value);
    }
}

impl CalculatorUIEvents for CalculatorApp
{
    fn on_n0_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '0'); }
    fn on_n1_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '1'); }
    fn on_n2_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '2'); }
    fn on_n3_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '3'); }
    fn on_n4_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '4'); }
    fn on_n5_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '5'); }
    fn on_n6_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '6'); }
    fn on_n7_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '7'); }
    fn on_n8_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '8'); }
    fn on_n9_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '9'); }
    fn on_dt_click(&mut self, ui: &mut CalculatorUI) { self.digit(ui, '.'); }
    fn on_ad_click(&mut self, ui: &mut CalculatorUI) { self.operation(ui, '+'); }
    fn on_mn_click(&mut self, ui: &mut CalculatorUI) { self.operation(ui, '-'); }
    fn on_ml_click(&mut self, ui: &mut CalculatorUI) { self.operation(ui, '*'); }
    fn on_dv_click(&mut self, ui: &mut CalculatorUI) { self.operation(ui, '/'); }
    fn on_sb_click(&mut self, ui: &mut CalculatorUI) { self.equals(ui); }
    fn on_ac_click(&mut self, ui: &mut CalculatorUI) { self.reset(ui); }
    fn on_dl_click(&mut self, ui: &mut CalculatorUI) { self.delete(ui); }
    fn on_pr_click(&mut self, ui: &mut CalculatorUI) { self.percent(ui); }

    fn handle_key(&mut self, ui: &mut CalculatorUI, key: &str) -> bool
    {
        if key.len() == 1 && key.as_bytes()[0].is_ascii_digit()
        {
            self.digit(ui, key.chars().next().unwrap_or_default());
            return true;
        }
        match key
        {
            "." => self.digit(ui, '.'),
            "+" => self.operation(ui, '+'),
            "-" => self.operation(ui, '-'),
            "*" => self.operation(ui, '*'),
            "/" => self.operation(ui, '/'),
            "=" => self.equals(ui),
            "%" => self.percent(ui),
            "Backspace" => self.delete(ui),
            _ => return false,
        }
        true
    }
}

fn is_operator(value: char) -> bool
{
    matches!(value, '+' | '-' | '*' | '/')
}

fn format_calculator(value: f64) -> String
{
    if value.fract() == 0.0
    {
        return format!("{value:.0}");
    }
    let text = format!("{value:.10}");
    text.trim_end_matches('0').trim_end_matches('.').to_string()
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = CalculatorUI::new();
        let mut app = CalculatorApp::new(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn calculator_domain_logic_matches_reference()
    {
        let mut ui = CalculatorUI::new();
        let mut app = CalculatorApp::new(&mut ui);
        for key in ["1", "2", "+", "3", "="]
        {
            assert!(app.handle_key(&mut ui, key));
        }
        assert_eq!(ui.display.borrow().text(), "15");
        app.on_ml_click(&mut ui);
        app.on_n2_click(&mut ui);
        app.on_sb_click(&mut ui);
        assert_eq!(ui.display.borrow().text(), "30");
        app.on_pr_click(&mut ui);
        assert_eq!(ui.display.borrow().text(), "0.3");
    }
}
