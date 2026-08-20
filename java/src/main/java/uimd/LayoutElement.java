package uimd;

public final class LayoutElement
{
    private String name = "";
    private Rect relative = new Rect(0, 0, 0, 0);
    private AxisDimension width = AxisDimension.auto(0);
    private AxisDimension height = AxisDimension.auto(0);
    private Size fitContentSize = new Size(0, 0);

    public String name()
    {
        return name;
    }

    public LayoutElement setName(String value)
    {
        name = value == null ? "" : value;
        return this;
    }

    public Rect relative()
    {
        return relative;
    }

    public LayoutElement setRelative(Rect value)
    {
        relative = value == null ? new Rect(0, 0, 0, 0) : value;
        return this;
    }

    public AxisDimension width()
    {
        return width;
    }

    public LayoutElement setWidth(AxisDimension value)
    {
        width = value == null ? AxisDimension.auto(0) : value;
        return this;
    }

    public AxisDimension height()
    {
        return height;
    }

    public LayoutElement setHeight(AxisDimension value)
    {
        height = value == null ? AxisDimension.auto(0) : value;
        return this;
    }

    public Size fitContentSize()
    {
        return fitContentSize;
    }

    public LayoutElement setFitContentSize(Size value)
    {
        fitContentSize = value == null ? new Size(0, 0) : value;
        return this;
    }
}
