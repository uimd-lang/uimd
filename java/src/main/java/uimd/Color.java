package uimd;

import java.util.Locale;
import java.util.Objects;
import java.util.Optional;

public final class Color
{
    public enum Kind
    {
        UNSET,
        TRANSPARENT,
        RGB,
        NAMED
    }

    private static final int OPAQUE_ALPHA = 255;
    private static final int SHORT_RGB_LENGTH = 3;
    private static final int SHORT_RGBA_LENGTH = 4;
    private static final int RGB_LENGTH = 6;
    private static final int RGBA_LENGTH = 8;

    private final Kind kind;
    private final String value;
    private final Rgba rgba;

    public Color()
    {
        this(Kind.UNSET, "", null);
    }

    public Color(String colorValue)
    {
        String normalized = normalize(colorValue);
        if (normalized.isEmpty())
        {
            kind = Kind.UNSET;
            value = "";
            rgba = null;
        }
        else if ("transparent".equals(normalized))
        {
            kind = Kind.TRANSPARENT;
            value = normalized;
            rgba = null;
        }
        else if (normalized.charAt(0) != '#')
        {
            kind = Kind.NAMED;
            value = normalized;
            rgba = null;
        }
        else
        {
            kind = Kind.RGB;
            value = normalized;
            rgba = parseHexRgba(normalized.substring(1));
        }
    }

    private Color(Kind colorKind, String colorValue, Rgba colorRgba)
    {
        kind = Objects.requireNonNull(colorKind, "color kind");
        value = colorValue == null ? "" : colorValue;
        rgba = colorRgba;
    }

    public static Color transparent()
    {
        return new Color(Kind.TRANSPARENT, "transparent", null);
    }

    public static Color rgb(int red, int green, int blue)
    {
        return rgb(red, green, blue, OPAQUE_ALPHA);
    }

    public static Color rgb(int red, int green, int blue, int alpha)
    {
        return new Color(Kind.RGB, "", new Rgba(red, green, blue, alpha));
    }

    public static Color named(String colorValue)
    {
        return new Color(Kind.NAMED, normalize(colorValue), null);
    }

    public Kind kind()
    {
        return kind;
    }

    public String value()
    {
        return value;
    }

    public String text()
    {
        return value();
    }

    public Optional<Rgba> rgba()
    {
        return Optional.ofNullable(rgba);
    }

    public boolean isSet()
    {
        return kind != Kind.UNSET;
    }

    public boolean isTransparent()
    {
        return kind == Kind.TRANSPARENT;
    }

    public Color blendOver(Color background)
    {
        if (kind != Kind.RGB || rgba == null)
        {
            return this;
        }
        if (rgba.alpha() >= OPAQUE_ALPHA)
        {
            return this;
        }
        if (rgba.alpha() == 0)
        {
            return background;
        }
        if (background == null || background.kind != Kind.RGB || background.rgba == null)
        {
            return this;
        }
        double alpha = rgba.alpha() / (double) OPAQUE_ALPHA;
        return rgb(
            blend(alpha, rgba.red(), background.rgba.red()),
            blend(alpha, rgba.green(), background.rgba.green()),
            blend(alpha, rgba.blue(), background.rgba.blue()));
    }

    @Override
    public String toString()
    {
        if (kind == Kind.UNSET)
        {
            return "";
        }
        if (kind == Kind.TRANSPARENT || kind == Kind.NAMED)
        {
            return value;
        }
        if (rgba == null)
        {
            return "";
        }
        return String.format(
            Locale.ROOT,
            "#%02x%02x%02x",
            rgba.red(),
            rgba.green(),
            rgba.blue());
    }

    @Override
    public boolean equals(Object otherValue)
    {
        if (!(otherValue instanceof Color other))
        {
            return false;
        }
        if (kind != other.kind)
        {
            return false;
        }
        if (kind == Kind.RGB)
        {
            return Objects.equals(rgba, other.rgba);
        }
        return value.equals(other.value);
    }

    @Override
    public int hashCode()
    {
        return kind == Kind.RGB
            ? Objects.hash(kind, rgba)
            : Objects.hash(kind, value);
    }

    private static int blend(double alpha, int foreground, int background)
    {
        return (int) Math.round(alpha * foreground + (1.0 - alpha) * background);
    }

    private static String normalize(String colorValue)
    {
        if (colorValue == null)
        {
            return "";
        }
        String result = colorValue.strip();
        if (result.endsWith(";"))
        {
            result = result.substring(0, result.length() - 1).stripTrailing();
        }
        return result.toLowerCase(Locale.ROOT);
    }

    private static Rgba parseHexRgba(String hex)
    {
        if (!hex.matches("[0-9a-f]+"))
        {
            throw new IllegalArgumentException("invalid color hex text");
        }
        return switch (hex.length())
        {
            case SHORT_RGB_LENGTH -> new Rgba(
                parseShortHex(hex.charAt(0)),
                parseShortHex(hex.charAt(1)),
                parseShortHex(hex.charAt(2)),
                OPAQUE_ALPHA);
            case SHORT_RGBA_LENGTH -> new Rgba(
                parseShortHex(hex.charAt(0)),
                parseShortHex(hex.charAt(1)),
                parseShortHex(hex.charAt(2)),
                parseShortHex(hex.charAt(3)));
            case RGB_LENGTH, RGBA_LENGTH -> new Rgba(
                parseHexByte(hex, 0),
                parseHexByte(hex, 2),
                parseHexByte(hex, 4),
                hex.length() == RGBA_LENGTH ? parseHexByte(hex, 6) : OPAQUE_ALPHA);
            default -> throw new IllegalArgumentException("unsupported color hex length");
        };
    }

    private static int parseShortHex(char value)
    {
        return Integer.parseInt(new String(new char[] {value, value}), 16);
    }

    private static int parseHexByte(String hex, int offset)
    {
        return Integer.parseInt(hex.substring(offset, offset + 2), 16);
    }
}
