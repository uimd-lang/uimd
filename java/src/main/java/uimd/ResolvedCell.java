package uimd;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class ResolvedCell
{
    private final String name;
    private final Rect rect;
    private final List<ResolvedElement> elements = new ArrayList<>();
    private final List<ResolvedCell> children = new ArrayList<>();

    public ResolvedCell(String name, Rect rect)
    {
        this.name = name == null ? "" : name;
        this.rect = rect == null ? new Rect(0, 0, 0, 0) : rect;
    }

    public String name()
    {
        return name;
    }

    public Rect rect()
    {
        return rect;
    }

    public List<ResolvedElement> elements()
    {
        return Collections.unmodifiableList(elements);
    }

    public List<ResolvedCell> children()
    {
        return Collections.unmodifiableList(children);
    }

    void addElement(ResolvedElement value)
    {
        elements.add(value);
    }

    void addChild(ResolvedCell value)
    {
        children.add(value);
    }
}
