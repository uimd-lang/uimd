package uimd;

import java.util.ArrayList;
import java.util.List;

public final class McpRuntimeConfig
{
    public static final int DEFAULT_PORT = 8765;
    public static final int DEFAULT_GUI_ACTION_DELAY_MILLISECONDS = 500;
    public static final int DEFAULT_GUI_TYPE_DELAY_MILLISECONDS = 50;
    public static final int DEFAULT_HEADLESS_ACTION_DELAY_MILLISECONDS = 0;
    public static final int DEFAULT_HEADLESS_TYPE_DELAY_MILLISECONDS = 0;
    public static final String DEFAULT_HOST = "127.0.0.1";
    public static final String TRANSPORT_TCP = "tcp";
    public static final String TRANSPORT_STDIO = "stdio";
    public static final String TRANSPORT_HTTP = "http";

    private boolean enabled;
    private boolean gui = true;
    private String transport = TRANSPORT_TCP;
    private String host = DEFAULT_HOST;
    private int port = DEFAULT_PORT;
    private int actionDelayMilliseconds = DEFAULT_GUI_ACTION_DELAY_MILLISECONDS;
    private int typeDelayMilliseconds = DEFAULT_GUI_TYPE_DELAY_MILLISECONDS;
    private boolean waitRender;
    private boolean controlledRender;
    private Rect viewport;

    public static McpRuntimeConfig parse(String[] arguments)
    {
        String[] args = arguments == null ? new String[0] : arguments;
        McpRuntimeConfig config = new McpRuntimeConfig();
        config.enabled = hasOption(args, "--mcp-server");
        config.gui = !hasOption(args, "--headless") || hasOption(args, "--gui");
        config.transport = optionValue(
            args,
            "--mcp-transport",
            config.gui ? TRANSPORT_TCP : TRANSPORT_STDIO);
        config.host = optionValue(args, "--mcp-host", DEFAULT_HOST);
        config.port = integerOption(args, "--mcp-port", DEFAULT_PORT);
        boolean fast = hasOption(args, "--mcp-fast");
        int defaultActionDelay = fast
            ? 0
            : config.gui
                ? DEFAULT_GUI_ACTION_DELAY_MILLISECONDS
                : DEFAULT_HEADLESS_ACTION_DELAY_MILLISECONDS;
        int defaultTypeDelay = fast
            ? 0
            : config.gui
                ? DEFAULT_GUI_TYPE_DELAY_MILLISECONDS
                : DEFAULT_HEADLESS_TYPE_DELAY_MILLISECONDS;
        config.actionDelayMilliseconds = integerOption(
            args,
            "--mcp-action-delay-ms",
            defaultActionDelay);
        config.typeDelayMilliseconds = integerOption(
            args,
            "--mcp-type-delay-ms",
            defaultTypeDelay);
        config.waitRender = hasOption(args, "--mcp-wait-render");
        config.controlledRender = hasOption(args, "--mcp-controlled-render");
        config.viewport = parseViewport(args);
        return config;
    }

    public boolean enabled()
    {
        return enabled;
    }

    public void setEnabled(boolean value)
    {
        enabled = value;
    }

    public boolean gui()
    {
        return gui;
    }

    public void setGui(boolean value)
    {
        gui = value;
    }

    public String transport()
    {
        return transport;
    }

    public void setTransport(String value)
    {
        transport = value == null ? "" : value;
    }

    public String host()
    {
        return host;
    }

    public void setHost(String value)
    {
        host = value == null || value.isEmpty() ? DEFAULT_HOST : value;
    }

    public int port()
    {
        return port;
    }

    public void setPort(int value)
    {
        port = value;
    }

    public int actionDelayMilliseconds()
    {
        return actionDelayMilliseconds;
    }

    public void setActionDelayMilliseconds(int value)
    {
        actionDelayMilliseconds = Math.max(0, value);
    }

    public int typeDelayMilliseconds()
    {
        return typeDelayMilliseconds;
    }

    public void setTypeDelayMilliseconds(int value)
    {
        typeDelayMilliseconds = Math.max(0, value);
    }

    public boolean waitRender()
    {
        return waitRender;
    }

    public void setWaitRender(boolean value)
    {
        waitRender = value;
    }

    public boolean controlledRender()
    {
        return controlledRender;
    }

    public void setControlledRender(boolean value)
    {
        controlledRender = value;
    }

    public Rect viewport()
    {
        return viewport;
    }

    public void setViewport(Rect value)
    {
        viewport = value;
    }

    private static Rect parseViewport(String[] arguments)
    {
        String value = optionValue(arguments, "--viewport", null);
        if (value != null)
        {
            String[] parts = value.split(",", -1);
            if (parts.length != 4)
            {
                throw new IllegalArgumentException(
                    "--viewport must have form row,col,width,height");
            }
            List<Integer> values = new ArrayList<>();
            for (String part : parts)
            {
                values.add(Integer.parseInt(part));
            }
            return new Rect(
                Math.max(0, values.get(0)),
                Math.max(0, values.get(1)),
                Math.max(1, values.get(2)),
                Math.max(1, values.get(3)));
        }
        boolean hasComponents = hasOption(arguments, "--viewport-row")
            || hasOption(arguments, "--viewport-col")
            || hasOption(arguments, "--viewport-width")
            || hasOption(arguments, "--viewport-height");
        if (!hasComponents)
        {
            return null;
        }
        return new Rect(
            Math.max(0, integerOption(arguments, "--viewport-row", 0)),
            Math.max(0, integerOption(arguments, "--viewport-col", 0)),
            Math.max(1, integerOption(arguments, "--viewport-width", 1)),
            Math.max(1, integerOption(arguments, "--viewport-height", 1)));
    }

    private static boolean hasOption(String[] arguments, String name)
    {
        for (String argument : arguments)
        {
            if (name.equals(argument))
            {
                return true;
            }
        }
        return false;
    }

    private static int integerOption(String[] arguments, String name, int fallback)
    {
        return Integer.parseInt(optionValue(arguments, name, Integer.toString(fallback)));
    }

    private static String optionValue(String[] arguments, String name, String fallback)
    {
        for (int index = 0; index + 1 < arguments.length; ++index)
        {
            if (name.equals(arguments[index]))
            {
                return arguments[index + 1];
            }
        }
        return fallback;
    }
}
