package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class GeneratedWindowBase extends Window
{
    private List<GeneratedLayoutEntry> generatedLayout = new ArrayList<>();
    private Style generatedWindowStyle = new Style();
    private boolean generatedFocusable;
    private String generatedKind = "window";
    private boolean mcpEnabled = true;
    private String mcpClassName = "";
    private String mcpSourceMarkdown = "";
    private String mcpWindowDescription = "";
    private List<GeneratedElementMetadata> mcpElementMetadata = new ArrayList<>();
    private List<GeneratedAppToolMetadata> mcpAppTools = new ArrayList<>();
    private String mode = "";
    private GeneratedWindowStack runtimeWindowStack;
    private Element requestedFocusElement;

    public GeneratedWindowBase()
    {
        this("");
    }

    public GeneratedWindowBase(String title)
    {
        super(title);
    }

    public List<GeneratedLayoutEntry> generatedLayout()
    {
        return Collections.unmodifiableList(generatedLayout);
    }

    public void setGeneratedLayout(List<GeneratedLayoutEntry> value)
    {
        generatedLayout = new ArrayList<>(value == null ? List.of() : value);
    }

    public Style generatedWindowStyle()
    {
        return generatedWindowStyle;
    }

    public void setGeneratedWindowStyle(Style value)
    {
        generatedWindowStyle = value == null ? new Style() : value;
    }

    public boolean generatedFocusable()
    {
        return generatedFocusable;
    }

    public void setGeneratedFocusable(boolean value)
    {
        generatedFocusable = value;
    }

    public String generatedKind()
    {
        return generatedKind;
    }

    public void setGeneratedKind(String value)
    {
        generatedKind = value == null ? "window" : value;
    }

    public String mode()
    {
        return mode;
    }

    public void setMode(String value)
    {
        mode = value == null ? "" : value;
    }

    public void setMcpMetadata(
        boolean enabled,
        String sourceMarkdown)
    {
        setMcpMetadata(enabled, "", sourceMarkdown, "", List.of());
    }

    public void setMcpMetadata(
        boolean enabled,
        String className,
        String sourceMarkdown)
    {
        setMcpMetadata(enabled, className, sourceMarkdown, "", List.of());
    }

    public void setMcpMetadata(
        boolean enabled,
        String className,
        String sourceMarkdown,
        String windowDescription,
        List<GeneratedElementMetadata> elementMetadata)
    {
        mcpEnabled = enabled;
        mcpClassName = className == null ? "" : className;
        mcpSourceMarkdown = sourceMarkdown == null ? "" : sourceMarkdown;
        mcpWindowDescription = windowDescription == null ? "" : windowDescription;
        mcpElementMetadata = new ArrayList<>(elementMetadata == null ? List.of() : elementMetadata);
    }

    public boolean mcpEnabled()
    {
        return mcpEnabled;
    }

    public String mcpClassName()
    {
        return mcpClassName;
    }

    public String mcpSourceMarkdown()
    {
        return mcpSourceMarkdown;
    }

    public String mcpWindowDescription()
    {
        return mcpWindowDescription;
    }

    public List<GeneratedElementMetadata> mcpElementMetadata()
    {
        return Collections.unmodifiableList(mcpElementMetadata);
    }

    public GeneratedElementMetadata mcpElementMetadata(String name)
    {
        if (name == null)
        {
            return null;
        }
        for (GeneratedElementMetadata metadata : mcpElementMetadata)
        {
            if (name.equals(metadata.name()))
            {
                return metadata;
            }
        }
        return null;
    }

    public void setMcpAppTools(List<GeneratedAppToolMetadata> value)
    {
        mcpAppTools = new ArrayList<>(value == null ? List.of() : value);
    }

    public List<GeneratedAppToolMetadata> mcpAppTools()
    {
        return Collections.unmodifiableList(mcpAppTools);
    }

    public boolean handleGeneratedButton(String name)
    {
        return false;
    }

    public boolean handleGeneratedTextChanged(String name, String value)
    {
        return false;
    }

    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        return false;
    }

    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        return false;
    }

    public boolean handleGeneratedListBoxItemActivate(
        String name,
        String elementId,
        int index,
        String value)
    {
        return false;
    }

    public boolean onPreviewKey(KeyEvent event)
    {
        return false;
    }

    public boolean onKey(String key)
    {
        return false;
    }

    public String callAppTool(String name, String inputJson)
    {
        throw new IllegalStateException("App MCP tool is not implemented: " + name);
    }

    public boolean activateGeneratedControl()
    {
        return false;
    }

    public ScrollView generatedScrollView()
    {
        return null;
    }

    protected boolean shouldClose()
    {
        return false;
    }

    public GeneratedWindowRuntimeOptions runtimeOptions()
    {
        GeneratedWindowRuntimeOptions options = new GeneratedWindowRuntimeOptions();
        options.setClassName(mcpClassName);
        options.setOnButton(this::handleGeneratedButton);
        options.setOnTextChanged(this::handleGeneratedTextChanged);
        options.setOnTextConfirmed(this::handleGeneratedTextConfirmed);
        options.setOnSelectionChanged(this::handleGeneratedSelectionChanged);
        options.setShouldClose(this::shouldClose);
        return options;
    }

    public void setFocus(Element element)
    {
        Element resolved = element != null && element.enabled() ? element : null;
        if (runtimeWindowStack != null)
        {
            requestedFocusElement = null;
            runtimeWindowStack.setWindowFocus(this, resolved);
            return;
        }
        requestedFocusElement = resolved;
    }

    Element consumeRequestedFocusElement()
    {
        Element element = requestedFocusElement;
        requestedFocusElement = null;
        return element;
    }

    void attachRuntimeWindowStack(GeneratedWindowStack value)
    {
        runtimeWindowStack = value;
    }

    protected GeneratedWindowStack runtimeWindowStack()
    {
        return runtimeWindowStack;
    }

    protected void openWindow(GeneratedWindowBase window)
    {
        if (runtimeWindowStack != null)
        {
            runtimeWindowStack.push(window);
        }
    }

    protected void openWindow(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options)
    {
        if (runtimeWindowStack != null)
        {
            runtimeWindowStack.push(window, options);
        }
    }

    protected void closeWindow()
    {
        if (runtimeWindowStack != null)
        {
            runtimeWindowStack.pop();
        }
    }
}
