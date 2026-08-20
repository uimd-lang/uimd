package uimd;

public record VisualGlyph(String text, int sourceStart, int sourceEnd)
{
    public VisualGlyph()
    {
        this(" ", -1, -1);
    }
}
