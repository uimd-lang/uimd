#include "calculator_ui.hpp"

#include "ui/generated/GeneratedWindowRuntime.hpp"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cctype>
#include <stdexcept>
#include <string>
#include <string_view>

namespace
{

constexpr std::string_view kDisplayZero = "0";
constexpr std::string_view kDisplayError = "Error";
constexpr double kPercentFactor = 100.0;

class ExpressionParser
{
public:
    explicit ExpressionParser(std::string_view input)
        : input_(input)
    {
    }

    [[nodiscard]] double parse()
    {
        const double value = expression();
        skipSpaces();
        if (pos_ != input_.size())
        {
            throw std::runtime_error("unexpected trailing calculator input");
        }
        return value;
    }

private:
    [[nodiscard]] double expression()
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

    [[nodiscard]] double term()
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

    [[nodiscard]] double factor()
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

    [[nodiscard]] double number()
    {
        skipSpaces();
        const std::size_t start = pos_;
        while (pos_ < input_.size() && (std::isdigit(static_cast<unsigned char>(input_[pos_])) || input_[pos_] == '.'))
        {
            ++pos_;
        }
        if (start == pos_)
        {
            throw std::runtime_error("expected calculator number");
        }
        return std::stod(std::string(input_.substr(start, pos_ - start)));
    }

    [[nodiscard]] bool match(char ch)
    {
        if (pos_ < input_.size() && input_[pos_] == ch)
        {
            ++pos_;
            return true;
        }
        return false;
    }

    void skipSpaces()
    {
        while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_])))
        {
            ++pos_;
        }
    }

    std::string_view input_;
    std::size_t pos_ = 0;
};

class CalculatorApp : public CalculatorUI
{
public:
    CalculatorApp()
    {
        refresh();
    }

    [[nodiscard]] CalculatorUI& window()
    {
        return *this;
    }

    void button(std::string_view name)
    {
        if (name == "n0") digit('0');
        else if (name == "n1") digit('1');
        else if (name == "n2") digit('2');
        else if (name == "n3") digit('3');
        else if (name == "n4") digit('4');
        else if (name == "n5") digit('5');
        else if (name == "n6") digit('6');
        else if (name == "n7") digit('7');
        else if (name == "n8") digit('8');
        else if (name == "n9") digit('9');
        else if (name == "dt") digit('.');
        else if (name == "ad") operation('+');
        else if (name == "mn") operation('-');
        else if (name == "ml") operation('*');
        else if (name == "dv") operation('/');
        else if (name == "sb") equals();
        else if (name == "ac") reset();
        else if (name == "dl") erase();
        else if (name == "pr") percent();
    }

    [[nodiscard]] bool key(std::string_view key)
    {
        if (key.size() == 1 && std::isdigit(static_cast<unsigned char>(key.front())))
        {
            digit(key.front());
            return true;
        }
        if (key == ".")
        {
            digit('.');
            return true;
        }
        if (key == "+" || key == "-" || key == "*" || key == "/")
        {
            operation(key.front());
            return true;
        }
        if (key == "=")
        {
            equals();
            return true;
        }
        if (key == "%")
        {
            percent();
            return true;
        }
        if (key == "Backspace")
        {
            erase();
            return true;
        }
        return false;
    }

    [[nodiscard]] ui::GeneratedWindowRuntimeOptions runtimeOptions() override
    {
        ui::GeneratedWindowRuntimeOptions options = CalculatorUI::runtimeOptions();
        options.onKey = [this](std::string_view key)
        {
            return this->key(key);
        };
        return options;
    }

protected:
    void onN0Click() override
    {
        button("n0");
    }
    void onN1Click() override
    {
        button("n1");
    }
    void onN2Click() override
    {
        button("n2");
    }
    void onN3Click() override
    {
        button("n3");
    }
    void onN4Click() override
    {
        button("n4");
    }
    void onN5Click() override
    {
        button("n5");
    }
    void onN6Click() override
    {
        button("n6");
    }
    void onN7Click() override
    {
        button("n7");
    }
    void onN8Click() override
    {
        button("n8");
    }
    void onN9Click() override
    {
        button("n9");
    }
    void onDtClick() override
    {
        button("dt");
    }
    void onAdClick() override
    {
        button("ad");
    }
    void onMnClick() override
    {
        button("mn");
    }
    void onMlClick() override
    {
        button("ml");
    }
    void onDvClick() override
    {
        button("dv");
    }
    void onSbClick() override
    {
        button("sb");
    }
    void onAcClick() override
    {
        button("ac");
    }
    void onDlClick() override
    {
        button("dl");
    }
    void onPrClick() override
    {
        button("pr");
    }

private:
    void digit(char value)
    {
        if (afterResult_ || expression_ == kDisplayError)
        {
            expression_.clear();
            afterResult_ = false;
        }
        if (value == '.')
        {
            appendDecimalPoint();
        }
        else if (expression_ == kDisplayZero)
        {
            expression_ = value;
        }
        else
        {
            expression_ += value;
        }
        refresh();
    }

    void operation(char op)
    {
        if (expression_ == kDisplayError)
        {
            expression_ = kDisplayZero;
        }
        if (expression_.empty())
        {
            if (op == '-')
            {
                expression_ = op;
            }
            refresh();
            return;
        }
        const char last = expression_.back();
        if (last == '+' || last == '-' || last == '*' || last == '/')
        {
            expression_.back() = op;
        }
        else
        {
            expression_ += op;
        }
        afterResult_ = false;
        refresh();
    }

    void equals()
    {
        try
        {
            const double result = ExpressionParser(expression_).parse();
            if (!std::isfinite(result))
            {
                throw std::runtime_error("non-finite calculator result");
            }
            expression_ = format(result);
            afterResult_ = true;
            refresh();
        }
        catch (const std::exception&)
        {
            expression_ = kDisplayError;
            afterResult_ = true;
            refresh();
        }
    }

    void reset()
    {
        expression_ = kDisplayZero;
        afterResult_ = false;
        refresh();
    }

    void erase()
    {
        if (afterResult_ || expression_ == kDisplayError)
        {
            reset();
            return;
        }
        if (!expression_.empty())
        {
            expression_.pop_back();
        }
        if (expression_.empty())
        {
            expression_ = kDisplayZero;
        }
        refresh();
    }

    void percent()
    {
        try
        {
            const double result = std::stod(expression_) / kPercentFactor;
            expression_ = format(result);
            afterResult_ = false;
            refresh();
        }
        catch (const std::exception&)
        {
            expression_ = kDisplayError;
            afterResult_ = true;
            refresh();
        }
    }

    void appendDecimalPoint()
    {
        const std::size_t pos = expression_.find_last_of("+-*/");
        const std::string_view current = pos == std::string::npos
            ? std::string_view(expression_)
            : std::string_view(expression_).substr(pos + 1);
        if (current.find('.') != std::string_view::npos)
        {
            return;
        }
        if (expression_.empty() || expression_.back() == '+' || expression_.back() == '-' ||
            expression_.back() == '*' || expression_.back() == '/')
        {
            expression_ += "0.";
        }
        else
        {
            expression_ += '.';
        }
    }

    [[nodiscard]] static std::string format(double value)
    {
        if (std::trunc(value) == value)
        {
            return std::to_string(static_cast<long long>(value));
        }
        std::string text = std::to_string(value);
        while (!text.empty() && text.back() == '0')
        {
            text.pop_back();
        }
        if (!text.empty() && text.back() == '.')
        {
            text.pop_back();
        }
        return text;
    }

    void refresh()
    {
        display->setText(expression_.empty() ? std::string(kDisplayZero) : expression_);
    }

    std::string expression_ = std::string(kDisplayZero);
    bool afterResult_ = false;
};

void smoke()
{
    constexpr std::size_t kExpectedLayoutEntries = 20;

    CalculatorUI window;

    assert(window.title() == "Calculator");
    assert(window.generatedLayout().size() == kExpectedLayoutEntries);
    assert(window.display != nullptr);
    assert(window.n0 != nullptr);
    assert(window.n0->title() == "0");
    assert(window.sb != nullptr);
    assert(window.sb->title() == "=");
}

void logicTest()
{
    CalculatorApp app;

    assert(app.key("1"));
    assert(app.key("2"));
    assert(app.key("+"));
    assert(app.key("3"));
    assert(app.key("="));
    assert(app.window().display->text() == "15");

    app.button("ml");
    app.button("n2");
    app.button("sb");
    assert(app.window().display->text() == "30");

    app.button("pr");
    assert(app.window().display->text() == "0.3");

    app.button("ac");
    assert(app.window().display->text() == "0");
}

}  // namespace

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view(argv[1]) == "--smoke")
    {
        smoke();
        return 0;
    }
    if (argc > 1 && std::string_view(argv[1]) == "--logic-test")
    {
        logicTest();
        return 0;
    }

    CalculatorApp app;
    return ui::runGeneratedWindow(app, app.runtimeOptions(), argc, argv);
}
