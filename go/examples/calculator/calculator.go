package main

import (
	"math"
	"os"
	"strconv"
	"strings"
	"unicode"

	uimd "uimd"
)

const (
	calculatorDisplayZero  = "0"
	calculatorDisplayError = "Error"
	calculatorPercent      = 100.0
)

type expressionParser struct {
	input    string
	position int
}

func newExpressionParser(input string) *expressionParser {
	return &expressionParser{input: input}
}

func (parser *expressionParser) parse() (float64, bool) {
	value, ok := parser.expression()
	if !ok {
		return 0, false
	}
	parser.skipSpaces()
	return value, parser.position == len(parser.input)
}

func (parser *expressionParser) expression() (float64, bool) {
	value, ok := parser.term()
	if !ok {
		return 0, false
	}
	for {
		parser.skipSpaces()
		if parser.match('+') {
			next, ok := parser.term()
			if !ok {
				return 0, false
			}
			value += next
		} else if parser.match('-') {
			next, ok := parser.term()
			if !ok {
				return 0, false
			}
			value -= next
		} else {
			return value, true
		}
	}
}

func (parser *expressionParser) term() (float64, bool) {
	value, ok := parser.factor()
	if !ok {
		return 0, false
	}
	for {
		parser.skipSpaces()
		if parser.match('*') {
			next, ok := parser.factor()
			if !ok {
				return 0, false
			}
			value *= next
		} else if parser.match('/') {
			next, ok := parser.factor()
			if !ok {
				return 0, false
			}
			value /= next
		} else {
			return value, true
		}
	}
}

func (parser *expressionParser) factor() (float64, bool) {
	parser.skipSpaces()
	if parser.match('+') {
		return parser.factor()
	}
	if parser.match('-') {
		value, ok := parser.factor()
		return -value, ok
	}
	return parser.number()
}

func (parser *expressionParser) number() (float64, bool) {
	parser.skipSpaces()
	start := parser.position
	for parser.position < len(parser.input) {
		ch := rune(parser.input[parser.position])
		if !unicode.IsDigit(ch) && ch != '.' {
			break
		}
		parser.position++
	}
	if start == parser.position {
		return 0, false
	}
	value, err := strconv.ParseFloat(parser.input[start:parser.position], 64)
	return value, err == nil
}

func (parser *expressionParser) match(value byte) bool {
	if parser.position < len(parser.input) && parser.input[parser.position] == value {
		parser.position++
		return true
	}
	return false
}

func (parser *expressionParser) skipSpaces() {
	for parser.position < len(parser.input) && unicode.IsSpace(rune(parser.input[parser.position])) {
		parser.position++
	}
}

type Calculator struct {
	*CalculatorUI
	expression  string
	afterResult bool
}

func NewCalculator() *Calculator {
	app := &Calculator{
		CalculatorUI: NewCalculatorUI(),
		expression:   calculatorDisplayZero,
	}
	app.SetEventHandler(app)
	app.refresh()
	return app
}

func (app *Calculator) RuntimeOptions() uimd.GeneratedWindowRuntimeOptions {
	options := app.CalculatorUI.RuntimeOptions()
	options.OnKey = app.key
	return options
}

func (app *Calculator) OnN0Click() { app.button("n0") }
func (app *Calculator) OnN1Click() { app.button("n1") }
func (app *Calculator) OnN2Click() { app.button("n2") }
func (app *Calculator) OnN3Click() { app.button("n3") }
func (app *Calculator) OnN4Click() { app.button("n4") }
func (app *Calculator) OnN5Click() { app.button("n5") }
func (app *Calculator) OnN6Click() { app.button("n6") }
func (app *Calculator) OnN7Click() { app.button("n7") }
func (app *Calculator) OnN8Click() { app.button("n8") }
func (app *Calculator) OnN9Click() { app.button("n9") }
func (app *Calculator) OnDtClick() { app.button("dt") }
func (app *Calculator) OnAdClick() { app.button("ad") }
func (app *Calculator) OnMnClick() { app.button("mn") }
func (app *Calculator) OnMlClick() { app.button("ml") }
func (app *Calculator) OnDvClick() { app.button("dv") }
func (app *Calculator) OnSbClick() { app.button("sb") }
func (app *Calculator) OnAcClick() { app.button("ac") }
func (app *Calculator) OnDlClick() { app.button("dl") }
func (app *Calculator) OnPrClick() { app.button("pr") }

func (app *Calculator) button(name string) {
	switch name {
	case "n0":
		app.digit('0')
	case "n1":
		app.digit('1')
	case "n2":
		app.digit('2')
	case "n3":
		app.digit('3')
	case "n4":
		app.digit('4')
	case "n5":
		app.digit('5')
	case "n6":
		app.digit('6')
	case "n7":
		app.digit('7')
	case "n8":
		app.digit('8')
	case "n9":
		app.digit('9')
	case "dt":
		app.digit('.')
	case "ad":
		app.operation('+')
	case "mn":
		app.operation('-')
	case "ml":
		app.operation('*')
	case "dv":
		app.operation('/')
	case "sb":
		app.equalsPressed()
	case "ac":
		app.reset()
	case "dl":
		app.delete()
	case "pr":
		app.percent()
	}
}

func (app *Calculator) key(key string) bool {
	if len(key) == 1 && key[0] >= '0' && key[0] <= '9' {
		app.digit(rune(key[0]))
		return true
	}
	switch key {
	case ".":
		app.digit('.')
	case "+", "-", "*", "/":
		app.operation(rune(key[0]))
	case "=":
		app.equalsPressed()
	case "%":
		app.percent()
	case "Backspace":
		app.delete()
	default:
		return false
	}
	return true
}

func (app *Calculator) digit(digit rune) {
	if app.afterResult || app.expression == calculatorDisplayError {
		app.expression = ""
		app.afterResult = false
	}
	if digit == '.' {
		app.appendDecimalPoint()
	} else if app.expression == calculatorDisplayZero {
		app.expression = string(digit)
	} else if current := app.currentNumberText(); current == "0" || current == "-0" {
		app.replaceCurrentNumber(strings.Replace(current, calculatorDisplayZero, string(digit), 1))
	} else {
		app.expression += string(digit)
	}
	app.refresh()
}

func (app *Calculator) operation(operation rune) {
	if app.expression == calculatorDisplayError {
		app.expression = calculatorDisplayZero
	}
	if app.expression == "" {
		if operation == '-' {
			app.expression = string(operation)
			app.afterResult = false
			app.refresh()
		}
		return
	}
	if isOperator(rune(app.expression[len(app.expression)-1])) {
		if operation == '-' && app.expression[len(app.expression)-1] != '-' {
			app.expression += string(operation)
		} else {
			app.expression = strings.TrimRight(app.expression, "+-*/") + string(operation)
		}
	} else {
		app.expression += string(operation)
	}
	app.afterResult = false
	app.refresh()
}

func (app *Calculator) equalsPressed() {
	app.compute()
}

func (app *Calculator) delete() {
	if app.afterResult || app.expression == calculatorDisplayError {
		app.reset()
		return
	}
	if app.expression != "" {
		app.expression = app.expression[:len(app.expression)-1]
	}
	if app.expression == "" {
		app.expression = calculatorDisplayZero
	}
	app.refresh()
}

func (app *Calculator) percent() {
	current := app.currentNumberText()
	if current == "" {
		return
	}
	value, err := strconv.ParseFloat(current, 64)
	if err != nil {
		app.showError()
		return
	}
	app.replaceCurrentNumber(formatCalculator(value / calculatorPercent))
	app.afterResult = false
	app.refresh()
}

func (app *Calculator) compute() {
	result, ok := newExpressionParser(app.expression).parse()
	if !ok || math.IsInf(result, 0) || math.IsNaN(result) {
		app.showError()
		return
	}
	app.expression = formatCalculator(result)
	app.afterResult = true
	app.refresh()
}

func (app *Calculator) reset() {
	app.expression = calculatorDisplayZero
	app.afterResult = false
	app.refresh()
}

func (app *Calculator) refresh() {
	if app.expression == "" {
		app.Display.SetText(calculatorDisplayZero)
		return
	}
	app.Display.SetText(app.expression)
}

func (app *Calculator) showError() {
	app.expression = calculatorDisplayError
	app.afterResult = true
	app.refresh()
}

func (app *Calculator) appendDecimalPoint() {
	current := app.currentNumberText()
	if strings.Contains(current, ".") {
		return
	}
	if app.expression == "" || isOperator(rune(app.expression[len(app.expression)-1])) {
		app.expression += calculatorDisplayZero + "."
	} else {
		app.expression += "."
	}
}

func (app *Calculator) currentNumberSpan() (int, int) {
	end := len(app.expression)
	if end == 0 || isOperator(rune(app.expression[end-1])) {
		return end, end
	}
	start := end
	for start > 0 {
		ch := rune(app.expression[start-1])
		if !unicode.IsDigit(ch) && ch != '.' {
			break
		}
		start--
	}
	if start > 0 && app.expression[start-1] == '-' && (start == 1 || isOperator(rune(app.expression[start-2]))) {
		start--
	}
	return start, end
}

func (app *Calculator) currentNumberText() string {
	start, end := app.currentNumberSpan()
	return app.expression[start:end]
}

func (app *Calculator) replaceCurrentNumber(value string) {
	start, end := app.currentNumberSpan()
	app.expression = app.expression[:start] + value + app.expression[end:]
}

func isOperator(value rune) bool {
	return value == '+' || value == '-' || value == '*' || value == '/'
}

func formatCalculator(value float64) string {
	rounded := math.Round(value)
	if math.Abs(value-rounded) < math.SmallestNonzeroFloat64*math.Max(1.0, math.Abs(value)) {
		return strconv.FormatFloat(rounded, 'f', 0, 64)
	}
	return strconv.FormatFloat(value, 'g', 10, 64)
}

func main() {
	code := uimd.RunGeneratedAppMain(func() int {
		app := NewCalculator()
		return uimd.RunGeneratedWindow(app, app.RuntimeOptions(), os.Args)
	})
	os.Exit(code)
}
