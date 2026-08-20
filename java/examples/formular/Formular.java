import java.util.List;

import uimd.GeneratedWindowRuntime;

public final class Formular extends FormularUI
{
    private static final String ACTION_SAVE = "save";
    private static final String ACTION_CANCEL = "cancel";
    private static final int EXPECTED_LAYOUT_ENTRIES = 17;
    private static final int EXPECTED_COUNTRIES = 5;
    private static final int EXPECTED_ROLES = 7;

    private String action = "";
    private boolean closeRequested;

    public String action()
    {
        return action;
    }

    @Override
    protected void onSaveBtnClick()
    {
        finish(ACTION_SAVE);
    }

    @Override
    protected void onCancelBtnClick()
    {
        finish(ACTION_CANCEL);
    }

    @Override
    protected boolean shouldClose()
    {
        return closeRequested;
    }

    private void finish(String nextAction)
    {
        action = nextAction;
        closeRequested = true;
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() -> run(arguments));
        System.exit(exitCode);
    }

    private static int run(String[] arguments)
    {
        if (arguments.length > 0 && "--smoke".equals(arguments[0]))
        {
            smoke();
            return 0;
        }
        if (arguments.length > 0 && "--logic-test".equals(arguments[0]))
        {
            logicTest();
            return 0;
        }

        Formular application = new Formular();
        int result = GeneratedWindowRuntime.runGeneratedWindow(
            application,
            application.runtimeOptions(),
            arguments);
        if (!application.action().isEmpty())
        {
            printTerminalLines(formYaml(application, application.action()));
        }
        return result;
    }

    private static void smoke()
    {
        Formular window = new Formular();
        require("FormApp".equals(window.title()), "unexpected title");
        require(window.generatedLayout().size() == EXPECTED_LAYOUT_ENTRIES, "unexpected layout size");
        require(window.name_input != null, "name_input missing");
        require(window.email_input != null, "email_input missing");
        require(window.age_input != null, "age_input missing");
        require(window.country_combo.options().size() == EXPECTED_COUNTRIES, "unexpected country count");
        require(window.role_listbox.options().size() == EXPECTED_ROLES, "unexpected role count");
        require("Save".equals(window.save_btn.title()), "unexpected save title");
        require("Cancel".equals(window.cancel_btn.title()), "unexpected cancel title");
    }

    private static void logicTest()
    {
        Formular window = new Formular();
        window.name_input.setValue("Marek");
        window.email_input.setValue("marek@example.com");
        window.age_input.setValue(30.0);
        window.description_input.setValue("hello");
        window.country_combo.setSelectedIndex(2);
        window.role_listbox.setSelectedValues(List.of("QA"));
        window.notify_check.setChecked(true);
        window.terms_check.setChecked(true);
        window.onSaveBtnClick();

        String expected = "action: save\n"
            + "form:\n"
            + "  name: Marek\n"
            + "  email: marek@example.com\n"
            + "  age: 30\n"
            + "  description: hello\n"
            + "  country: Poland\n"
            + "  roles:\n"
            + "  - QA\n"
            + "  email_notifications: true\n"
            + "  accepted_terms: true\n";
        require(ACTION_SAVE.equals(window.action()), "save action was not recorded");
        require(window.shouldClose(), "save did not request close");
        require(expected.equals(formYaml(window, window.action())), "form YAML mismatch");

        Formular cancelled = new Formular();
        cancelled.onCancelBtnClick();
        require(ACTION_CANCEL.equals(cancelled.action()), "cancel action was not recorded");
        require(cancelled.shouldClose(), "cancel did not request close");
    }

    private static String formYaml(FormularUI window, String action)
    {
        StringBuilder output = new StringBuilder();
        output.append("action: ").append(action).append('\n');
        output.append("form:\n");
        output.append("  name: ").append(yamlScalar(window.name_input.value())).append('\n');
        output.append("  email: ").append(yamlScalar(window.email_input.value())).append('\n');
        output.append("  age: ").append(window.age_input.displayText()).append('\n');
        output.append("  description: ").append(yamlScalar(window.description_input.value())).append('\n');
        output.append("  country: ").append(yamlScalar(window.country_combo.selectedText())).append('\n');
        output.append("  roles:\n");
        for (String role : window.role_listbox.selectedValues())
        {
            output.append("  - ").append(yamlScalar(role)).append('\n');
        }
        output.append("  email_notifications: ").append(window.notify_check.checked()).append('\n');
        output.append("  accepted_terms: ").append(window.terms_check.checked()).append('\n');
        return output.toString();
    }

    private static String yamlScalar(String value)
    {
        return value.isEmpty() ? "\"\"" : value;
    }

    private static void printTerminalLines(String value)
    {
        String terminalText = "\r\n" + value.replace("\n", "\r\n");
        System.out.print(terminalText);
        System.out.flush();
    }

    private static void require(boolean condition, String message)
    {
        if (!condition)
        {
            throw new IllegalStateException(message);
        }
    }
}
