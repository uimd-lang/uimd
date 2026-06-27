using System.Globalization;
using System.Text;
using Uimd;

public sealed class FormApp : FormularUI
{
    private bool closeRequested;
    private string action = "";

    public string Action => action;

    protected override void onSaveBtnClick()
    {
        action = "save";
        closeRequested = true;
    }

    protected override void onCancelBtnClick()
    {
        action = "cancel";
        closeRequested = true;
    }

    protected override bool shouldClose()
    {
        return closeRequested;
    }
}

public static class Program
{
    private const string NumberFormat = "0.##########";

    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            FormApp app = new();
            int result = GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
            if (app.Action.Length > 0)
            {
                PrintTerminalLines(FormYaml(app, app.Action));
            }
            return result;
        });
    }

    private static string FormYaml(FormularUI window, string action)
    {
        StringBuilder output = new();
        output.Append("action: ").Append(action).Append('\n');
        output.Append("form:\n");
        output.Append("  name: ").Append(YamlScalar(window.name_input.Value)).Append('\n');
        output.Append("  email: ").Append(YamlScalar(window.email_input.Value)).Append('\n');
        output.Append("  age: ").Append(window.age_input.Value.ToString(NumberFormat, CultureInfo.InvariantCulture)).Append('\n');
        output.Append("  description: ").Append(YamlScalar(window.description_input.Value)).Append('\n');
        output.Append("  country: ").Append(YamlScalar(window.country_combo.SelectedText)).Append('\n');
        output.Append("  roles:\n");
        output.Append("  - ").Append(YamlScalar(SelectedRoleText(window))).Append('\n');
        output.Append("  email_notifications: ").Append(YamlBool(window.notify_check.Checked)).Append('\n');
        output.Append("  accepted_terms: ").Append(YamlBool(window.terms_check.Checked)).Append('\n');
        return output.ToString();
    }

    private static string YamlScalar(string value)
    {
        return value.Length == 0 ? "\"\"" : value;
    }

    private static string YamlBool(bool value)
    {
        return value ? "true" : "false";
    }

    private static string SelectedRoleText(FormularUI window)
    {
        return window.role_listbox.SelectedValues.FirstOrDefault() ?? "";
    }

    private static void PrintTerminalLines(string text)
    {
        Console.Write("\r\n");
        foreach (char character in text)
        {
            if (character == '\n')
            {
                Console.Write("\r\n");
            }
            else
            {
                Console.Write(character);
            }
        }
        Console.Out.Flush();
    }
}
