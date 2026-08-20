package uimd;

public final class LabelSpan
{
    private String text;
    private String foreground;
    private String background;

    public LabelSpan()
    {
        this("", "", "");
    }

    public LabelSpan(String text)
    {
        this(text, "", "");
    }

    public LabelSpan(String text, String foreground, String background)
    {
        this.text = text == null ? "" : text;
        this.foreground = foreground == null ? "" : foreground;
        this.background = background == null ? "" : background;
    }

    public String text()
    {
        return text;
    }

    public void setText(String value)
    {
        text = value == null ? "" : value;
    }

    public String foreground()
    {
        return foreground;
    }

    public void setForeground(String value)
    {
        foreground = value == null ? "" : value;
    }

    public String background()
    {
        return background;
    }

    public void setBackground(String value)
    {
        background = value == null ? "" : value;
    }
}
