import Foundation
import Uimd

private enum CalculatorConstants
{
    static let displayZero = "0"
    static let displayError = "Error"
    static let percentFactor = 100.0
}

private final class ExpressionParser
{
    private let input: String
    private var position: String.Index

    init(_ input: String)
    {
        self.input = input
        self.position = input.startIndex
    }

    func parse() throws -> Double
    {
        let value = try expression()
        skipSpaces()
        if position != input.endIndex
        {
            throw RuntimeError("unexpected trailing calculator input")
        }
        return value
    }

    private func expression() throws -> Double
    {
        var value = try term()
        while true
        {
            skipSpaces()
            if match("+")
            {
                value += try term()
            }
            else if match("-")
            {
                value -= try term()
            }
            else
            {
                return value
            }
        }
    }

    private func term() throws -> Double
    {
        var value = try factor()
        while true
        {
            skipSpaces()
            if match("*")
            {
                value *= try factor()
            }
            else if match("/")
            {
                value /= try factor()
            }
            else
            {
                return value
            }
        }
    }

    private func factor() throws -> Double
    {
        skipSpaces()
        if match("+")
        {
            return try factor()
        }
        if match("-")
        {
            return -(try factor())
        }
        return try number()
    }

    private func number() throws -> Double
    {
        skipSpaces()
        let start = position
        while position < input.endIndex
            && (input[position].isNumber || input[position] == ".")
        {
            position = input.index(after: position)
        }
        if start == position
        {
            throw RuntimeError("expected calculator number")
        }
        guard let value = Double(input[start..<position]) else
        {
            throw RuntimeError("invalid calculator number")
        }
        return value
    }

    private func match(_ value: Character) -> Bool
    {
        if position < input.endIndex && input[position] == value
        {
            position = input.index(after: position)
            return true
        }
        return false
    }

    private func skipSpaces()
    {
        while position < input.endIndex && input[position].isWhitespace
        {
            position = input.index(after: position)
        }
    }
}

public final class CalculatorApp: CalculatorUI
{
    private var expression = CalculatorConstants.displayZero
    private var afterResult = false

    public override init()
    {
        super.init()
        refresh()
    }

    public override func runtimeOptions() -> GeneratedWindowRuntimeOptions
    {
        var options = super.runtimeOptions()
        options.onKey = { [weak self] key in
            self?.key(key) ?? false
        }
        return options
    }

    override public func onN0Click() { button("n0") }
    override public func onN1Click() { button("n1") }
    override public func onN2Click() { button("n2") }
    override public func onN3Click() { button("n3") }
    override public func onN4Click() { button("n4") }
    override public func onN5Click() { button("n5") }
    override public func onN6Click() { button("n6") }
    override public func onN7Click() { button("n7") }
    override public func onN8Click() { button("n8") }
    override public func onN9Click() { button("n9") }
    override public func onDtClick() { button("dt") }
    override public func onAdClick() { button("ad") }
    override public func onMnClick() { button("mn") }
    override public func onMlClick() { button("ml") }
    override public func onDvClick() { button("dv") }
    override public func onSbClick() { button("sb") }
    override public func onAcClick() { button("ac") }
    override public func onDlClick() { button("dl") }
    override public func onPrClick() { button("pr") }

    private func button(_ name: String)
    {
        switch name
        {
        case "n0": digit("0")
        case "n1": digit("1")
        case "n2": digit("2")
        case "n3": digit("3")
        case "n4": digit("4")
        case "n5": digit("5")
        case "n6": digit("6")
        case "n7": digit("7")
        case "n8": digit("8")
        case "n9": digit("9")
        case "dt": digit(".")
        case "ad": operation("+")
        case "mn": operation("-")
        case "ml": operation("*")
        case "dv": operation("/")
        case "sb": equals()
        case "ac": reset()
        case "dl": erase()
        case "pr": percent()
        default: break
        }
    }

    private func key(_ key: String) -> Bool
    {
        if key.count == 1, let ch = key.first, ch.isNumber
        {
            digit(ch)
            return true
        }
        if key == "."
        {
            digit(".")
            return true
        }
        if ["+", "-", "*", "/"].contains(key), let ch = key.first
        {
            operation(ch)
            return true
        }
        if key == "="
        {
            equals()
            return true
        }
        if key == "%"
        {
            percent()
            return true
        }
        if key == "Backspace"
        {
            erase()
            return true
        }
        return false
    }

    private func digit(_ value: Character)
    {
        if afterResult || expression == CalculatorConstants.displayZero || expression == CalculatorConstants.displayError
        {
            expression = value == "." ? "0." : String(value)
            afterResult = false
        }
        else if value == "."
        {
            let current = currentNumber()
            if !current.contains(".")
            {
                expression.append(value)
            }
        }
        else
        {
            expression.append(value)
        }
        refresh()
    }

    private func operation(_ value: Character)
    {
        if expression == CalculatorConstants.displayError
        {
            return
        }
        afterResult = false
        if let last = expression.last, ["+", "-", "*", "/"].contains(last)
        {
            expression.removeLast()
        }
        expression.append(value)
        refresh()
    }

    private func equals()
    {
        do
        {
            let value = try ExpressionParser(expression).parse()
            expression = format(value)
            afterResult = true
        }
        catch
        {
            expression = CalculatorConstants.displayError
            afterResult = true
        }
        refresh()
    }

    private func reset()
    {
        expression = CalculatorConstants.displayZero
        afterResult = false
        refresh()
    }

    private func erase()
    {
        if expression == CalculatorConstants.displayError || expression.count <= 1
        {
            expression = CalculatorConstants.displayZero
        }
        else
        {
            expression.removeLast()
        }
        afterResult = false
        refresh()
    }

    private func percent()
    {
        do
        {
            let value = try ExpressionParser(expression).parse() / CalculatorConstants.percentFactor
            expression = format(value)
            afterResult = true
        }
        catch
        {
            expression = CalculatorConstants.displayError
            afterResult = true
        }
        refresh()
    }

    private func currentNumber() -> String
    {
        let separators: Set<Character> = ["+", "-", "*", "/"]
        var result = ""
        for ch in expression.reversed()
        {
            if separators.contains(ch)
            {
                break
            }
            result.insert(ch, at: result.startIndex)
        }
        return result
    }

    private func format(_ value: Double) -> String
    {
        if value.rounded() == value
        {
            return String(Int(value))
        }
        return String(value)
    }

    private func refresh()
    {
        display.setText(expression)
    }
}

@main
enum Program
{
    static func main()
    {
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            let app = CalculatorApp()
            return GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
        }
        Foundation.exit(Int32(code))
    }
}
