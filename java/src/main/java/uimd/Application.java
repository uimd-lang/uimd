package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class Application
{
    private final List<Window> windows = new ArrayList<>();

    public Window open(Window window)
    {
        if (window == null)
        {
            throw new IllegalArgumentException("window must not be null");
        }
        windows.add(window);
        return window;
    }

    public Window activeWindow()
    {
        return windows.isEmpty() ? null : windows.get(windows.size() - 1);
    }

    public List<Window> windows()
    {
        return Collections.unmodifiableList(windows);
    }
}
