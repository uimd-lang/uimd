package uimd;

import java.util.Objects;

public final class TerminalCell
{
    private String text = " ";
    private String raw = "";
    private int rawWidth = 1;
    private int rawHeight = 1;
    private boolean rawSkip;
    private Color foreground;
    private Color background;

    public TerminalCell copy()
    {
        TerminalCell result = new TerminalCell();
        result.text = text;
        result.raw = raw;
        result.rawWidth = rawWidth;
        result.rawHeight = rawHeight;
        result.rawSkip = rawSkip;
        result.foreground = foreground;
        result.background = background;
        return result;
    }

    public String text()
    {
        return text;
    }

    public void setText(String value)
    {
        text = value == null ? " " : value;
    }

    public String raw()
    {
        return raw;
    }

    public void setRaw(String value)
    {
        raw = value == null ? "" : value;
    }

    public int rawWidth()
    {
        return rawWidth;
    }

    public void setRawWidth(int value)
    {
        rawWidth = value;
    }

    public int rawHeight()
    {
        return rawHeight;
    }

    public void setRawHeight(int value)
    {
        rawHeight = value;
    }

    public boolean rawSkip()
    {
        return rawSkip;
    }

    public void setRawSkip(boolean value)
    {
        rawSkip = value;
    }

    public Color foreground()
    {
        return foreground;
    }

    public void setForeground(Color value)
    {
        foreground = value;
    }

    public Color background()
    {
        return background;
    }

    public void setBackground(Color value)
    {
        background = value;
    }

    @Override
    public boolean equals(Object value)
    {
        if (!(value instanceof TerminalCell other))
        {
            return false;
        }
        return rawWidth == other.rawWidth
            && rawHeight == other.rawHeight
            && rawSkip == other.rawSkip
            && text.equals(other.text)
            && raw.equals(other.raw)
            && Objects.equals(foreground, other.foreground)
            && Objects.equals(background, other.background);
    }

    @Override
    public int hashCode()
    {
        return Objects.hash(
            text,
            raw,
            rawWidth,
            rawHeight,
            rawSkip,
            foreground,
            background);
    }
}
