package uimd;

public record Rgba(int red, int green, int blue, int alpha)
{
    public Rgba
    {
        red = clamp(red);
        green = clamp(green);
        blue = clamp(blue);
        alpha = clamp(alpha);
    }

    private static int clamp(int value)
    {
        return Math.max(0, Math.min(255, value));
    }
}
