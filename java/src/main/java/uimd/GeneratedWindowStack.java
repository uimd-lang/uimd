package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class GeneratedWindowStack
{
    private final List<GeneratedWindowStackFrame> frames = new ArrayList<>();
    private GeneratedWindowStackFrame rootFrame;

    public GeneratedWindowStackFrame push(GeneratedWindowBase window)
    {
        return push(window, window.runtimeOptions());
    }

    public GeneratedWindowStackFrame push(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options)
    {
        if (window == null)
        {
            throw new IllegalArgumentException("window must not be null");
        }
        GeneratedWindowRuntimeOptions resolvedOptions = options == null
            ? window.runtimeOptions()
            : options;
        GeneratedWindowStackFrame frame = GeneratedWindowRuntime.createFrame(
            window,
            resolvedOptions);
        window.attachRuntimeWindowStack(this);
        frames.add(frame);
        return frame;
    }

    public void pop()
    {
        if (!frames.isEmpty())
        {
            frames.remove(frames.size() - 1);
        }
    }

    public void remove(GeneratedWindowBase window)
    {
        frames.removeIf(frame -> frame.window() == window);
    }

    public void clear()
    {
        frames.clear();
    }

    public boolean empty()
    {
        return frames.isEmpty();
    }

    public int size()
    {
        return frames.size();
    }

    public GeneratedWindowBase topWindow()
    {
        GeneratedWindowStackFrame frame = topFrame();
        return frame == null ? null : frame.window();
    }

    public List<GeneratedWindowStackFrame> frames()
    {
        return Collections.unmodifiableList(frames);
    }

    public GeneratedWindowStackFrame topFrame()
    {
        return frames.isEmpty() ? null : frames.get(frames.size() - 1);
    }

    GeneratedWindowStackFrame activeFrame()
    {
        GeneratedWindowStackFrame top = topFrame();
        return top == null ? rootFrame : top;
    }

    GeneratedWindowStackFrame rootFrame()
    {
        return rootFrame;
    }

    void setRootFrame(GeneratedWindowStackFrame value)
    {
        rootFrame = value;
        if (rootFrame != null)
        {
            rootFrame.window().attachRuntimeWindowStack(this);
        }
    }

    void setWindowFocus(GeneratedWindowBase window, Element element)
    {
        GeneratedWindowStackFrame frame = frameForWindow(window);
        if (frame != null)
        {
            GeneratedWindowRuntime.focusFrameElement(frame, element);
        }
    }

    private GeneratedWindowStackFrame frameForWindow(GeneratedWindowBase window)
    {
        for (int index = frames.size() - 1; index >= 0; --index)
        {
            GeneratedWindowStackFrame frame = frames.get(index);
            if (frame.window() == window)
            {
                return frame;
            }
        }
        return rootFrame != null && rootFrame.window() == window ? rootFrame : null;
    }
}
