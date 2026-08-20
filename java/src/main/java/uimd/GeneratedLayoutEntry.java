package uimd;

public final class GeneratedLayoutEntry
{
    private String name = "";
    private String type = "";
    private String cellName = "";
    private Rect relative = new Rect(0, 0, 1, 1);
    private Rect sourceCell = new Rect(0, 0, 1, 1);
    private AxisDimension width = AxisDimension.auto(1);
    private AxisDimension height = AxisDimension.auto(1);
    private AxisDimension cellWidth = AxisDimension.auto(1);
    private AxisDimension cellHeight = AxisDimension.auto(1);
    private Size charsSize = new Size(1, 1);
    private Size cellCharsSize = new Size(1, 1);
    private int marginRight;
    private int marginBottom;
    private Style cellStyle = new Style();
    private Style elementStyle = new Style();

    public String name()
    {
        return name;
    }

    public GeneratedLayoutEntry setName(String value)
    {
        name = value == null ? "" : value;
        return this;
    }

    public String type()
    {
        return type;
    }

    public GeneratedLayoutEntry setType(String value)
    {
        type = value == null ? "" : value;
        return this;
    }

    public String cellName()
    {
        return cellName;
    }

    public GeneratedLayoutEntry setCellName(String value)
    {
        cellName = value == null ? "" : value;
        return this;
    }

    public Rect relative()
    {
        return relative;
    }

    public GeneratedLayoutEntry setRelative(Rect value)
    {
        relative = value;
        return this;
    }

    public Rect sourceCell()
    {
        return sourceCell;
    }

    public GeneratedLayoutEntry setSourceCell(Rect value)
    {
        sourceCell = value;
        return this;
    }

    public AxisDimension width()
    {
        return width;
    }

    public GeneratedLayoutEntry setWidth(AxisDimension value)
    {
        width = value;
        return this;
    }

    public AxisDimension height()
    {
        return height;
    }

    public GeneratedLayoutEntry setHeight(AxisDimension value)
    {
        height = value;
        return this;
    }

    public AxisDimension cellWidth()
    {
        return cellWidth;
    }

    public GeneratedLayoutEntry setCellWidth(AxisDimension value)
    {
        cellWidth = value;
        return this;
    }

    public AxisDimension cellHeight()
    {
        return cellHeight;
    }

    public GeneratedLayoutEntry setCellHeight(AxisDimension value)
    {
        cellHeight = value;
        return this;
    }

    public Size charsSize()
    {
        return charsSize;
    }

    public GeneratedLayoutEntry setCharsSize(Size value)
    {
        charsSize = value;
        return this;
    }

    public Size cellCharsSize()
    {
        return cellCharsSize;
    }

    public GeneratedLayoutEntry setCellCharsSize(Size value)
    {
        cellCharsSize = value;
        return this;
    }

    public int marginRight()
    {
        return marginRight;
    }

    public GeneratedLayoutEntry setMarginRight(int value)
    {
        marginRight = value;
        return this;
    }

    public int marginBottom()
    {
        return marginBottom;
    }

    public GeneratedLayoutEntry setMarginBottom(int value)
    {
        marginBottom = value;
        return this;
    }

    public Style cellStyle()
    {
        return cellStyle;
    }

    public GeneratedLayoutEntry setCellStyle(Style value)
    {
        cellStyle = value == null ? new Style() : value;
        return this;
    }

    public Style elementStyle()
    {
        return elementStyle;
    }

    public GeneratedLayoutEntry setElementStyle(Style value)
    {
        elementStyle = value == null ? new Style() : value;
        return this;
    }
}
