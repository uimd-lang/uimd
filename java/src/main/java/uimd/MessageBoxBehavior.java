package uimd;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.function.Consumer;

final class MessageBoxBehavior
{
    private final Map<String, String> resultByButton;
    private final String defaultResult;
    private final Consumer<String> onClose;
    private String result = "";
    private boolean closed;

    MessageBoxBehavior(
        Label headerElement,
        Label messageElement,
        String header,
        String message,
        Consumer<String> onClose,
        Map<String, String> resultByButton,
        String defaultResult)
    {
        headerElement.setText(header == null ? "" : header);
        messageElement.setText(message == null ? "" : message);
        this.onClose = onClose;
        this.resultByButton = new LinkedHashMap<>(resultByButton);
        this.defaultResult = defaultResult == null ? "" : defaultResult;
    }

    GeneratedWindowRuntimeOptions configure(GeneratedWindowRuntimeOptions options)
    {
        options.setInitialFocusName(buttonNameForResult(defaultResult));
        options.setOnKey(key ->
        {
            if (!"Escape".equals(key) || defaultResult.isEmpty())
            {
                return false;
            }
            close(defaultResult);
            return true;
        });
        options.setShouldClose(this::closed);
        return options;
    }

    boolean handleButton(String name)
    {
        String nextResult = resultByButton.get(name);
        if (nextResult == null)
        {
            return false;
        }
        close(nextResult);
        return true;
    }

    String result()
    {
        return result;
    }

    boolean closed()
    {
        return closed;
    }

    private String buttonNameForResult(String targetResult)
    {
        for (Map.Entry<String, String> entry : resultByButton.entrySet())
        {
            if (entry.getValue().equals(targetResult))
            {
                return entry.getKey();
            }
        }
        return "";
    }

    private void close(String value)
    {
        result = value;
        closed = true;
        if (onClose != null)
        {
            onClose.accept(result);
        }
    }
}
