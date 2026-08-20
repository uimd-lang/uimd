package uimd;

public record FrameBufferRegion(int row, int col, int width, int height)
{
    public FrameBufferRegion()
    {
        this(0, 0, 1, 1);
    }
}
