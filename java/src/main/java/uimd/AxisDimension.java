package uimd;

public record AxisDimension(DimensionMode mode, int value)
{
    public static AxisDimension auto(int value)
    {
        return new AxisDimension(DimensionMode.AUTO, value);
    }

    public static AxisDimension fixed(int value)
    {
        return new AxisDimension(DimensionMode.FIXED, value);
    }

    public static AxisDimension expanded()
    {
        return new AxisDimension(DimensionMode.EXPANDED, 0);
    }

    public static AxisDimension fitContent()
    {
        return new AxisDimension(DimensionMode.FIT_CONTENT, 0);
    }
}
