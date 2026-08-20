import java.math.BigDecimal;
import java.math.MathContext;
import java.math.RoundingMode;

import uimd.GeneratedWindowRuntime;
import uimd.GeneratedWindowRuntimeOptions;

final class CalculatorConstants
{
    static final String DISPLAY_ZERO = "0";
    static final String DISPLAY_ERROR = "Error";
    static final double PERCENT_FACTOR = 100.0;

    private CalculatorConstants()
    {
    }
}

final class ExpressionParser
{
    private final String input;
    private int position;

    ExpressionParser(String input)
    {
        this.input = input;
    }

    double parse()
    {
        double value = expression();
        skipSpaces();
        if (position != input.length())
        {
            throw new IllegalStateException("unexpected trailing calculator input");
        }
        return value;
    }

    private double expression()
    {
        double value = term();
        while (true)
        {
            skipSpaces();
            if (match('+'))
            {
                value += term();
            }
            else if (match('-'))
            {
                value -= term();
            }
            else
            {
                return value;
            }
        }
    }

    private double term()
    {
        double value = factor();
        while (true)
        {
            skipSpaces();
            if (match('*'))
            {
                value *= factor();
            }
            else if (match('/'))
            {
                value /= factor();
            }
            else
            {
                return value;
            }
        }
    }

    private double factor()
    {
        skipSpaces();
        if (match('+'))
        {
            return factor();
        }
        if (match('-'))
        {
            return -factor();
        }
        return number();
    }

    private double number()
    {
        skipSpaces();
        int start = position;
        while (position < input.length()
            && (Character.isDigit(input.charAt(position)) || input.charAt(position) == '.'))
        {
            ++position;
        }
        if (start == position)
        {
            throw new IllegalStateException("expected calculator number");
        }
        return Double.parseDouble(input.substring(start, position));
    }

    private boolean match(char value)
    {
        if (position < input.length() && input.charAt(position) == value)
        {
            ++position;
            return true;
        }
        return false;
    }

    private void skipSpaces()
    {
        while (position < input.length() && Character.isWhitespace(input.charAt(position)))
        {
            ++position;
        }
    }
}

public final class Calculator extends CalculatorUI
{
    private static final MathContext DISPLAY_PRECISION = new MathContext(10, RoundingMode.HALF_EVEN);

    private String expression = CalculatorConstants.DISPLAY_ZERO;
    private boolean afterResult;

    public Calculator()
    {
        refresh();
    }

    @Override
    public GeneratedWindowRuntimeOptions runtimeOptions()
    {
        GeneratedWindowRuntimeOptions options = super.runtimeOptions();
        options.setOnKey(this::key);
        return options;
    }

    @Override
    protected void onN0Click()
    {
        button("n0");
    }

    @Override
    protected void onN1Click()
    {
        button("n1");
    }

    @Override
    protected void onN2Click()
    {
        button("n2");
    }

    @Override
    protected void onN3Click()
    {
        button("n3");
    }

    @Override
    protected void onN4Click()
    {
        button("n4");
    }

    @Override
    protected void onN5Click()
    {
        button("n5");
    }

    @Override
    protected void onN6Click()
    {
        button("n6");
    }

    @Override
    protected void onN7Click()
    {
        button("n7");
    }

    @Override
    protected void onN8Click()
    {
        button("n8");
    }

    @Override
    protected void onN9Click()
    {
        button("n9");
    }

    @Override
    protected void onDtClick()
    {
        button("dt");
    }

    @Override
    protected void onAdClick()
    {
        button("ad");
    }

    @Override
    protected void onMnClick()
    {
        button("mn");
    }

    @Override
    protected void onMlClick()
    {
        button("ml");
    }

    @Override
    protected void onDvClick()
    {
        button("dv");
    }

    @Override
    protected void onSbClick()
    {
        button("sb");
    }

    @Override
    protected void onAcClick()
    {
        button("ac");
    }

    @Override
    protected void onDlClick()
    {
        button("dl");
    }

    @Override
    protected void onPrClick()
    {
        button("pr");
    }

    private void button(String name)
    {
        switch (name)
        {
            case "n0": digit('0'); break;
            case "n1": digit('1'); break;
            case "n2": digit('2'); break;
            case "n3": digit('3'); break;
            case "n4": digit('4'); break;
            case "n5": digit('5'); break;
            case "n6": digit('6'); break;
            case "n7": digit('7'); break;
            case "n8": digit('8'); break;
            case "n9": digit('9'); break;
            case "dt": digit('.'); break;
            case "ad": operation('+'); break;
            case "mn": operation('-'); break;
            case "ml": operation('*'); break;
            case "dv": operation('/'); break;
            case "sb": equalsPressed(); break;
            case "ac": reset(); break;
            case "dl": delete(); break;
            case "pr": percent(); break;
            default: break;
        }
    }

    private boolean key(String key)
    {
        if (key.length() == 1 && Character.isDigit(key.charAt(0)))
        {
            digit(key.charAt(0));
            return true;
        }
        if (".".equals(key))
        {
            digit('.');
            return true;
        }
        if (key.length() == 1 && isOperator(key.charAt(0)))
        {
            operation(key.charAt(0));
            return true;
        }
        if ("=".equals(key))
        {
            equalsPressed();
            return true;
        }
        if ("%".equals(key))
        {
            percent();
            return true;
        }
        if ("Backspace".equals(key))
        {
            delete();
            return true;
        }
        return false;
    }

    private void digit(char digit)
    {
        if (afterResult || CalculatorConstants.DISPLAY_ERROR.equals(expression))
        {
            expression = "";
            afterResult = false;
        }

        if (digit == '.')
        {
            appendDecimalPoint();
        }
        else if (CalculatorConstants.DISPLAY_ZERO.equals(expression))
        {
            expression = Character.toString(digit);
        }
        else if ("0".equals(currentNumberText()) || "-0".equals(currentNumberText()))
        {
            replaceCurrentNumber(currentNumberText().replace(
                CalculatorConstants.DISPLAY_ZERO,
                Character.toString(digit)));
        }
        else
        {
            expression += digit;
        }
        refresh();
    }

    private void operation(char operation)
    {
        if (CalculatorConstants.DISPLAY_ERROR.equals(expression))
        {
            expression = CalculatorConstants.DISPLAY_ZERO;
        }

        if (expression.isEmpty())
        {
            if (operation == '-')
            {
                expression = Character.toString(operation);
                afterResult = false;
                refresh();
            }
            return;
        }

        if (isOperator(expression.charAt(expression.length() - 1)))
        {
            if (operation == '-' && expression.charAt(expression.length() - 1) != '-')
            {
                expression += operation;
            }
            else
            {
                expression = trimTrailingOperators(expression) + operation;
            }
        }
        else
        {
            expression += operation;
        }
        afterResult = false;
        refresh();
    }

    private void equalsPressed()
    {
        compute();
    }

    private void delete()
    {
        if (afterResult || CalculatorConstants.DISPLAY_ERROR.equals(expression))
        {
            reset();
            return;
        }
        if (!expression.isEmpty())
        {
            expression = expression.substring(0, expression.length() - 1);
        }
        if (expression.isEmpty())
        {
            expression = CalculatorConstants.DISPLAY_ZERO;
        }
        refresh();
    }

    private void percent()
    {
        try
        {
            String current = currentNumberText();
            if (current.isEmpty())
            {
                return;
            }
            replaceCurrentNumber(format(
                Double.parseDouble(current) / CalculatorConstants.PERCENT_FACTOR));
            afterResult = false;
            refresh();
        }
        catch (RuntimeException exception)
        {
            showError();
        }
    }

    private void compute()
    {
        try
        {
            double result = new ExpressionParser(expression).parse();
            if (!Double.isFinite(result))
            {
                throw new IllegalStateException("non-finite calculator result");
            }
            expression = format(result);
            afterResult = true;
            refresh();
        }
        catch (RuntimeException exception)
        {
            showError();
        }
    }

    private void reset()
    {
        expression = CalculatorConstants.DISPLAY_ZERO;
        afterResult = false;
        refresh();
    }

    private void refresh()
    {
        display.setText(expression.isEmpty() ? CalculatorConstants.DISPLAY_ZERO : expression);
    }

    private void showError()
    {
        expression = CalculatorConstants.DISPLAY_ERROR;
        afterResult = true;
        refresh();
    }

    private void appendDecimalPoint()
    {
        String current = currentNumberText();
        if (current.indexOf('.') >= 0)
        {
            return;
        }
        if (expression.isEmpty() || isOperator(expression.charAt(expression.length() - 1)))
        {
            expression += CalculatorConstants.DISPLAY_ZERO + ".";
        }
        else
        {
            expression += ".";
        }
    }

    private int[] currentNumberSpan()
    {
        int end = expression.length();
        if (end == 0 || isOperator(expression.charAt(end - 1)))
        {
            return new int[]{end, end};
        }

        int start = end;
        while (start > 0
            && (Character.isDigit(expression.charAt(start - 1)) || expression.charAt(start - 1) == '.'))
        {
            --start;
        }
        if (start > 0 && expression.charAt(start - 1) == '-'
            && (start == 1 || isOperator(expression.charAt(start - 2))))
        {
            --start;
        }
        return new int[]{start, end};
    }

    private String currentNumberText()
    {
        int[] span = currentNumberSpan();
        return expression.substring(span[0], span[1]);
    }

    private void replaceCurrentNumber(String value)
    {
        int[] span = currentNumberSpan();
        expression = expression.substring(0, span[0]) + value + expression.substring(span[1]);
    }

    private static String trimTrailingOperators(String value)
    {
        int end = value.length();
        while (end > 0 && isOperator(value.charAt(end - 1)))
        {
            --end;
        }
        return value.substring(0, end);
    }

    private static boolean isOperator(char value)
    {
        return value == '+' || value == '-' || value == '*' || value == '/';
    }

    private static String format(double value)
    {
        return BigDecimal.valueOf(value)
            .round(DISPLAY_PRECISION)
            .stripTrailingZeros()
            .toPlainString();
    }

    private static void require(boolean condition, String message)
    {
        if (!condition)
        {
            throw new IllegalStateException(message);
        }
    }

    private static void smoke()
    {
        CalculatorUI window = new CalculatorUI();
        require("Calculator".equals(window.title()), "calculator title mismatch");
        require(window.generatedLayout().size() == 20, "calculator layout mismatch");
        require(window.display != null, "calculator display is missing");
        require(window.n0 != null && "0".equals(window.n0.title()), "calculator digit button mismatch");
        require(window.sb != null && "=".equals(window.sb.title()), "calculator equals button mismatch");
    }

    private static void logicTest()
    {
        Calculator application = new Calculator();
        require(application.key("1"), "calculator key 1 was not handled");
        require(application.key("2"), "calculator key 2 was not handled");
        require(application.key("+"), "calculator key + was not handled");
        require(application.key("3"), "calculator key 3 was not handled");
        require(application.key("="), "calculator key = was not handled");
        require("15".equals(application.display.text()), "calculator addition failed");
        application.button("ml");
        application.button("n2");
        application.button("sb");
        require("30".equals(application.display.text()), "calculator multiplication failed");
        application.button("pr");
        require("0.3".equals(application.display.text()), "calculator percent failed");
        application.button("ac");
        require("0".equals(application.display.text()), "calculator reset failed");
    }

    public static void main(String[] arguments)
    {
        if (arguments.length > 0 && "--smoke".equals(arguments[0]))
        {
            smoke();
            return;
        }
        if (arguments.length > 0 && "--logic-test".equals(arguments[0]))
        {
            logicTest();
            return;
        }
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() ->
        {
            Calculator application = new Calculator();
            return GeneratedWindowRuntime.runGeneratedWindow(
                application,
                application.runtimeOptions(),
                arguments);
        });
        System.exit(exitCode);
    }
}
