package uimd;

public record FrameBufferCell(String text, String foreground, String background)
{
    public FrameBufferCell()
    {
        this(" ", "", "");
    }

    public FrameBufferCell(String text)
    {
        this(text, "", "");
    }
}
