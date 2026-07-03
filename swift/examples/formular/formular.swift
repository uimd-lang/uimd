import Foundation
import Uimd

public final class FormApp: FormularUI
{
    private var wantsClose = false
    private(set) var action = ""

    override public func onSaveBtnClick()
    {
        action = "save"
        wantsClose = true
        requestClose()
    }

    override public func onCancelBtnClick()
    {
        action = "cancel"
        wantsClose = true
        requestClose()
    }

    override public func shouldClose() -> Bool
    {
        wantsClose
    }
}

@main
enum Program
{
    static func main()
    {
        let app = FormApp()
        let code = GeneratedWindowRuntime.runGeneratedAppMain
        {
            let result = GeneratedWindowRuntime.runGeneratedWindow(app, app.runtimeOptions(), CommandLine.arguments)
            if !app.action.isEmpty
            {
                printTerminalLines(formYaml(app, app.action))
            }
            return result
        }
        Foundation.exit(Int32(code))
    }

    private static func formYaml(_ window: FormularUI, _ action: String) -> String
    {
        var output = ""
        output += "action: \(action)\n"
        output += "form:\n"
        output += "  name: \(yamlScalar(window.name_input.value))\n"
        output += "  email: \(yamlScalar(window.email_input.value))\n"
        output += "  age: \(formatNumber(window.age_input.numberValue))\n"
        output += "  description: \(yamlScalar(window.description_input.value))\n"
        output += "  country: \(yamlScalar(window.country_combo.terminalText))\n"
        output += "  roles:\n"
        output += "  - \(yamlScalar(window.role_listbox.selectedValues.first ?? ""))\n"
        output += "  email_notifications: \(yamlBool(window.notify_check.checked))\n"
        output += "  accepted_terms: \(yamlBool(window.terms_check.checked))\n"
        return output
    }

    private static func yamlScalar(_ value: String) -> String
    {
        value.isEmpty ? "\"\"" : value
    }

    private static func yamlBool(_ value: Bool) -> String
    {
        value ? "true" : "false"
    }

    private static func formatNumber(_ value: Double) -> String
    {
        if value.rounded() == value
        {
            return String(Int(value))
        }
        return String(value)
    }

    private static func printTerminalLines(_ text: String)
    {
        FileHandle.standardOutput.write(Data("\r\n".utf8))
        for character in text
        {
            if character == "\n"
            {
                FileHandle.standardOutput.write(Data("\r\n".utf8))
            }
            else
            {
                FileHandle.standardOutput.write(Data(String(character).utf8))
            }
        }
    }
}
