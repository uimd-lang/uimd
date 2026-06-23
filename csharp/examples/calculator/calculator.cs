using System.Globalization;
using Uimd;

internal static class CalculatorConstants
{
    public const string DisplayZero = "0";
    public const string DisplayError = "Error";
    public const double PercentFactor = 100.0;
}

internal sealed class ExpressionParser
{
    private readonly string input;
    private int position;

    public ExpressionParser(string input)
    {
        this.input = input;
    }

    public double Parse()
    {
        double value = Expression();
        SkipSpaces();
        if (position != input.Length)
        {
            throw new InvalidOperationException("unexpected trailing calculator input");
        }
        return value;
    }

    private double Expression()
    {
        double value = Term();
        while (true)
        {
            SkipSpaces();
            if (Match('+'))
            {
                value += Term();
            }
            else if (Match('-'))
            {
                value -= Term();
            }
            else
            {
                return value;
            }
        }
    }

    private double Term()
    {
        double value = Factor();
        while (true)
        {
            SkipSpaces();
            if (Match('*'))
            {
                value *= Factor();
            }
            else if (Match('/'))
            {
                value /= Factor();
            }
            else
            {
                return value;
            }
        }
    }

    private double Factor()
    {
        SkipSpaces();
        if (Match('+'))
        {
            return Factor();
        }
        if (Match('-'))
        {
            return -Factor();
        }
        return Number();
    }

    private double Number()
    {
        SkipSpaces();
        int start = position;
        while (position < input.Length && (char.IsDigit(input[position]) || input[position] == '.'))
        {
            ++position;
        }
        if (start == position)
        {
            throw new InvalidOperationException("expected calculator number");
        }
        return double.Parse(input[start..position], CultureInfo.InvariantCulture);
    }

    private bool Match(char value)
    {
        if (position < input.Length && input[position] == value)
        {
            ++position;
            return true;
        }
        return false;
    }

    private void SkipSpaces()
    {
        while (position < input.Length && char.IsWhiteSpace(input[position]))
        {
            ++position;
        }
    }
}

public sealed class Calculator : CalculatorUI
{
    private string expression = CalculatorConstants.DisplayZero;
    private bool afterResult;

    public Calculator()
    {
        Refresh();
    }

    public override GeneratedWindowRuntimeOptions RuntimeOptions()
    {
        GeneratedWindowRuntimeOptions options = base.RuntimeOptions();
        options.OnKey = Key;
        return options;
    }

    protected override void onN0Click()
    {
        Button("n0");
    }

    protected override void onN1Click()
    {
        Button("n1");
    }

    protected override void onN2Click()
    {
        Button("n2");
    }

    protected override void onN3Click()
    {
        Button("n3");
    }

    protected override void onN4Click()
    {
        Button("n4");
    }

    protected override void onN5Click()
    {
        Button("n5");
    }

    protected override void onN6Click()
    {
        Button("n6");
    }

    protected override void onN7Click()
    {
        Button("n7");
    }

    protected override void onN8Click()
    {
        Button("n8");
    }

    protected override void onN9Click()
    {
        Button("n9");
    }

    protected override void onDtClick()
    {
        Button("dt");
    }

    protected override void onAdClick()
    {
        Button("ad");
    }

    protected override void onMnClick()
    {
        Button("mn");
    }

    protected override void onMlClick()
    {
        Button("ml");
    }

    protected override void onDvClick()
    {
        Button("dv");
    }

    protected override void onSbClick()
    {
        Button("sb");
    }

    protected override void onAcClick()
    {
        Button("ac");
    }

    protected override void onDlClick()
    {
        Button("dl");
    }

    protected override void onPrClick()
    {
        Button("pr");
    }

    private void Button(string name)
    {
        switch (name)
        {
            case "n0": Digit('0'); break;
            case "n1": Digit('1'); break;
            case "n2": Digit('2'); break;
            case "n3": Digit('3'); break;
            case "n4": Digit('4'); break;
            case "n5": Digit('5'); break;
            case "n6": Digit('6'); break;
            case "n7": Digit('7'); break;
            case "n8": Digit('8'); break;
            case "n9": Digit('9'); break;
            case "dt": Digit('.'); break;
            case "ad": Operation('+'); break;
            case "mn": Operation('-'); break;
            case "ml": Operation('*'); break;
            case "dv": Operation('/'); break;
            case "sb": EqualsPressed(); break;
            case "ac": Reset(); break;
            case "dl": Delete(); break;
            case "pr": Percent(); break;
        }
    }

    private bool Key(string key)
    {
        if (key.Length == 1 && char.IsDigit(key[0]))
        {
            Digit(key[0]);
            return true;
        }
        if (key == ".")
        {
            Digit('.');
            return true;
        }
        if (key is "+" or "-" or "*" or "/")
        {
            Operation(key[0]);
            return true;
        }
        if (key == "=")
        {
            EqualsPressed();
            return true;
        }
        if (key == "%")
        {
            Percent();
            return true;
        }
        if (key == "Backspace")
        {
            Delete();
            return true;
        }
        return false;
    }

    private void Digit(char digit)
    {
        if (afterResult || expression == CalculatorConstants.DisplayError)
        {
            expression = "";
            afterResult = false;
        }

        if (digit == '.')
        {
            AppendDecimalPoint();
        }
        else if (expression == CalculatorConstants.DisplayZero)
        {
            expression = digit.ToString();
        }
        else if (CurrentNumberText() is "0" or "-0")
        {
            ReplaceCurrentNumber(CurrentNumberText().Replace(CalculatorConstants.DisplayZero, digit.ToString(), StringComparison.Ordinal));
        }
        else
        {
            expression += digit;
        }
        Refresh();
    }

    private void Operation(char operation)
    {
        if (expression == CalculatorConstants.DisplayError)
        {
            expression = CalculatorConstants.DisplayZero;
        }

        if (expression.Length == 0)
        {
            if (operation == '-')
            {
                expression = operation.ToString();
                afterResult = false;
                Refresh();
            }
            return;
        }

        if (IsOperator(expression[^1]))
        {
            if (operation == '-' && expression[^1] != '-')
            {
                expression += operation;
            }
            else
            {
                expression = expression.TrimEnd('+', '-', '*', '/') + operation;
            }
        }
        else
        {
            expression += operation;
        }
        afterResult = false;
        Refresh();
    }

    private void EqualsPressed()
    {
        Compute();
    }

    private void Delete()
    {
        if (afterResult || expression == CalculatorConstants.DisplayError)
        {
            Reset();
            return;
        }
        expression = expression.Length > 0 ? expression[..^1] : "";
        if (expression.Length == 0)
        {
            expression = CalculatorConstants.DisplayZero;
        }
        Refresh();
    }

    private void Percent()
    {
        try
        {
            string current = CurrentNumberText();
            if (current.Length == 0)
            {
                return;
            }
            ReplaceCurrentNumber(Format(double.Parse(current, CultureInfo.InvariantCulture) / CalculatorConstants.PercentFactor));
            afterResult = false;
            Refresh();
        }
        catch (Exception)
        {
            ShowError();
        }
    }

    private void Compute()
    {
        try
        {
            double result = new ExpressionParser(expression).Parse();
            if (!double.IsFinite(result))
            {
                throw new InvalidOperationException("non-finite calculator result");
            }
            expression = Format(result);
            afterResult = true;
            Refresh();
        }
        catch (Exception)
        {
            ShowError();
        }
    }

    private void Reset()
    {
        expression = CalculatorConstants.DisplayZero;
        afterResult = false;
        Refresh();
    }

    private void Refresh()
    {
        display.SetText(string.IsNullOrEmpty(expression) ? CalculatorConstants.DisplayZero : expression);
    }

    private void ShowError()
    {
        expression = CalculatorConstants.DisplayError;
        afterResult = true;
        Refresh();
    }

    private void AppendDecimalPoint()
    {
        string current = CurrentNumberText();
        if (current.Contains('.', StringComparison.Ordinal))
        {
            return;
        }
        if (expression.Length == 0 || IsOperator(expression[^1]))
        {
            expression += CalculatorConstants.DisplayZero + ".";
        }
        else
        {
            expression += ".";
        }
    }

    private (int Start, int End) CurrentNumberSpan()
    {
        int end = expression.Length;
        if (end == 0 || IsOperator(expression[end - 1]))
        {
            return (end, end);
        }

        int start = end;
        while (start > 0 && (char.IsDigit(expression[start - 1]) || expression[start - 1] == '.'))
        {
            --start;
        }
        if (start > 0 && expression[start - 1] == '-')
        {
            if (start == 1 || IsOperator(expression[start - 2]))
            {
                --start;
            }
        }
        return (start, end);
    }

    private string CurrentNumberText()
    {
        (int start, int end) = CurrentNumberSpan();
        return expression[start..end];
    }

    private void ReplaceCurrentNumber(string value)
    {
        (int start, int end) = CurrentNumberSpan();
        expression = expression[..start] + value + expression[end..];
    }

    private static bool IsOperator(char value)
    {
        return value is '+' or '-' or '*' or '/';
    }

    private static string Format(double value)
    {
        double rounded = Math.Round(value);
        if (Math.Abs(value - rounded) < double.Epsilon * Math.Max(1.0, Math.Abs(value)))
        {
            return rounded.ToString("0", CultureInfo.InvariantCulture);
        }
        return value.ToString("G10", CultureInfo.InvariantCulture);
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            Calculator app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
