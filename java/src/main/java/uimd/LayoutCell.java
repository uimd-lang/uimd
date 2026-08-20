package uimd;

import java.util.ArrayList;
import java.util.List;

public final class LayoutCell
{
    public static final int DEFAULT_PADDING = 0;
    public static final int DEFAULT_GAP = 1;

    private String name = "";
    private AxisDimension width = AxisDimension.auto(0);
    private AxisDimension height = AxisDimension.auto(0);
    private Size sourceSize = new Size(0, 0);
    private Size fitContentSize = new Size(0, 0);
    private int padding = DEFAULT_PADDING;
    private int gap = DEFAULT_GAP;
    private LayoutDirection direction = LayoutDirection.COLUMN;
    private final List<LayoutElement> elements = new ArrayList<>();
    private final List<LayoutCell> children = new ArrayList<>();

    public String name()
    {
        return name;
    }

    public LayoutCell setName(String value)
    {
        name = value == null ? "" : value;
        return this;
    }

    public AxisDimension width()
    {
        return width;
    }

    public LayoutCell setWidth(AxisDimension value)
    {
        width = value == null ? AxisDimension.auto(0) : value;
        return this;
    }

    public AxisDimension height()
    {
        return height;
    }

    public LayoutCell setHeight(AxisDimension value)
    {
        height = value == null ? AxisDimension.auto(0) : value;
        return this;
    }

    public Size sourceSize()
    {
        return sourceSize;
    }

    public LayoutCell setSourceSize(Size value)
    {
        sourceSize = value == null ? new Size(0, 0) : value;
        return this;
    }

    public Size fitContentSize()
    {
        return fitContentSize;
    }

    public LayoutCell setFitContentSize(Size value)
    {
        fitContentSize = value == null ? new Size(0, 0) : value;
        return this;
    }

    public int padding()
    {
        return padding;
    }

    public LayoutCell setPadding(int value)
    {
        padding = value;
        return this;
    }

    public int gap()
    {
        return gap;
    }

    public LayoutCell setGap(int value)
    {
        gap = value;
        return this;
    }

    public LayoutDirection direction()
    {
        return direction;
    }

    public LayoutCell setDirection(LayoutDirection value)
    {
        direction = value == null ? LayoutDirection.COLUMN : value;
        return this;
    }

    public List<LayoutElement> elements()
    {
        return elements;
    }

    public LayoutCell addElement(LayoutElement value)
    {
        if (value == null)
        {
            throw new IllegalArgumentException("layout element must not be null");
        }
        elements.add(value);
        return this;
    }

    public List<LayoutCell> children()
    {
        return children;
    }

    public LayoutCell addChild(LayoutCell value)
    {
        if (value == null)
        {
            throw new IllegalArgumentException("layout cell must not be null");
        }
        children.add(value);
        return this;
    }
}
