package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Window
{
    private final String title;
    private final List<Element> elements = new ArrayList<>();

    public Window()
    {
        this("");
    }

    public Window(String title)
    {
        this.title = title == null ? "" : title;
    }

    public String title()
    {
        return title;
    }

    public List<Element> elements()
    {
        return Collections.unmodifiableList(elements);
    }

    public <T extends Element> T addElement(T element)
    {
        if (element == null)
        {
            throw new IllegalArgumentException("element must not be null");
        }
        elements.add(element);
        return element;
    }
}
