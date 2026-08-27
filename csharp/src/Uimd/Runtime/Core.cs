using System.Globalization;
using System.Text;
using System.Text.Json;
using System.Text.Json.Nodes;

namespace Uimd;

public readonly record struct Rgba(byte Red, byte Green, byte Blue, byte Alpha);

public sealed class Color : IEquatable<Color>
{
    public string Text { get; }
    public Rgba? Rgba { get; }

    public Color(string? value = null)
    {
        Text = value ?? "";
        Rgba = ParseRgba(Text);
    }

    public static Color Rgb(byte red, byte green, byte blue, byte alpha = 255)
    {
        return new Color($"#{red:x2}{green:x2}{blue:x2}{alpha:x2}");
    }

    public bool IsTransparent => Text == "transparent";

    public Color BlendOver(Color background)
    {
        if (!Rgba.HasValue || !background.Rgba.HasValue)
        {
            return this;
        }
        Rgba foreground = Rgba.Value;
        Rgba baseColor = background.Rgba.Value;
        if (foreground.Alpha >= 255)
        {
            return this;
        }
        if (foreground.Alpha == 0)
        {
            return background;
        }
        double alpha = foreground.Alpha / 255.0;
        static byte Blend(double alpha, byte fg, byte bg)
        {
            return (byte)Math.Round(alpha * fg + (1.0 - alpha) * bg);
        }
        return Rgb(
            Blend(alpha, foreground.Red, baseColor.Red),
            Blend(alpha, foreground.Green, baseColor.Green),
            Blend(alpha, foreground.Blue, baseColor.Blue));
    }

    public override string ToString()
    {
        if (Text == "transparent")
        {
            return Text;
        }
        if (Rgba.HasValue)
        {
            Rgba rgba = Rgba.Value;
            return $"#{rgba.Red:x2}{rgba.Green:x2}{rgba.Blue:x2}";
        }
        return Text;
    }

    public bool Equals(Color? other)
    {
        if (other is null)
        {
            return false;
        }
        if (Rgba.HasValue && other.Rgba.HasValue)
        {
            return Rgba.Value.Equals(other.Rgba.Value);
        }
        return Text == other.Text;
    }

    public override bool Equals(object? obj)
    {
        return obj is Color other && Equals(other);
    }

    public override int GetHashCode()
    {
        return Rgba?.GetHashCode() ?? Text.GetHashCode(StringComparison.Ordinal);
    }

    private static Rgba? ParseRgba(string text)
    {
        if (string.IsNullOrWhiteSpace(text) || text == "transparent")
        {
            return text == "transparent" ? new Rgba(0, 0, 0, 0) : null;
        }
        if (!text.StartsWith('#'))
        {
            return null;
        }
        string hex = text[1..];
        if (hex.Length != 6 && hex.Length != 8)
        {
            return null;
        }
        try
        {
            byte red = byte.Parse(hex[..2], NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            byte green = byte.Parse(hex[2..4], NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            byte blue = byte.Parse(hex[4..6], NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            byte alpha = hex.Length == 8
                ? byte.Parse(hex[6..8], NumberStyles.HexNumber, CultureInfo.InvariantCulture)
                : byte.MaxValue;
            return new Rgba(red, green, blue, alpha);
        }
        catch (FormatException)
        {
            return null;
        }
    }
}

public sealed class TextGradient
{
    public int IntervalMs { get; set; }
    public int Step { get; set; }
    public int SegmentSize { get; set; }
    public List<Color> Colors { get; } = new();
}

public sealed class GradientRenderTime : IDisposable
{
    private readonly long? previous;

    public GradientRenderTime(long? timeMs)
    {
        previous = RenderHelpers.RenderTimeOverrideMs;
        RenderHelpers.RenderTimeOverrideMs = timeMs;
    }

    public void Dispose()
    {
        RenderHelpers.RenderTimeOverrideMs = previous;
    }
}

public sealed class Style
{
    public Color? Color { get; set; }
    public Color? Background { get; set; }
    public string? BackgroundTexture { get; set; }
    public Color? BackgroundTextureColor { get; set; }
    public Color? ScopeDimBackground { get; set; } = new("#00000066");
    public Color? BorderColor { get; set; }
    public int? BorderWidthHorizontal { get; set; }
    public int? BorderWidthVertical { get; set; }
    public int? Padding { get; set; }
    public int? PaddingTop { get; set; }
    public int? PaddingRight { get; set; }
    public int? PaddingBottom { get; set; }
    public int? PaddingLeft { get; set; }
    public int? Margin { get; set; }
    public int? MarginTop { get; set; }
    public int? MarginRight { get; set; }
    public int? MarginBottom { get; set; }
    public int? MarginLeft { get; set; }
    public int? Gap { get; set; }
    public string TextAlign { get; set; } = "";
    public string UserSelect { get; set; } = "";
    public bool? ScrollX { get; set; }
    public bool? ScrollY { get; set; }
    public TextGradient? TextColorGradient { get; set; }
    public TextGradient? TextBackgroundGradient { get; set; }

    public Style Clone()
    {
        return new Style
        {
            Color = Color,
            Background = Background,
            BackgroundTexture = BackgroundTexture,
            BackgroundTextureColor = BackgroundTextureColor,
            ScopeDimBackground = ScopeDimBackground,
            BorderColor = BorderColor,
            BorderWidthHorizontal = BorderWidthHorizontal,
            BorderWidthVertical = BorderWidthVertical,
            Padding = Padding,
            PaddingTop = PaddingTop,
            PaddingRight = PaddingRight,
            PaddingBottom = PaddingBottom,
            PaddingLeft = PaddingLeft,
            Margin = Margin,
            MarginTop = MarginTop,
            MarginRight = MarginRight,
            MarginBottom = MarginBottom,
            MarginLeft = MarginLeft,
            Gap = Gap,
            TextAlign = TextAlign,
            UserSelect = UserSelect,
            ScrollX = ScrollX,
            ScrollY = ScrollY,
            TextColorGradient = TextColorGradient,
            TextBackgroundGradient = TextBackgroundGradient,
        };
    }

    public void Merge(Style other)
    {
        Color = other.Color ?? Color;
        Background = other.Background ?? Background;
        BackgroundTexture = other.BackgroundTexture ?? BackgroundTexture;
        BackgroundTextureColor = other.BackgroundTextureColor ?? BackgroundTextureColor;
        ScopeDimBackground = other.ScopeDimBackground ?? ScopeDimBackground;
        BorderColor = other.BorderColor ?? BorderColor;
        BorderWidthHorizontal = other.BorderWidthHorizontal ?? BorderWidthHorizontal;
        BorderWidthVertical = other.BorderWidthVertical ?? BorderWidthVertical;
        Padding = other.Padding ?? Padding;
        PaddingTop = other.PaddingTop ?? PaddingTop;
        PaddingRight = other.PaddingRight ?? PaddingRight;
        PaddingBottom = other.PaddingBottom ?? PaddingBottom;
        PaddingLeft = other.PaddingLeft ?? PaddingLeft;
        Margin = other.Margin ?? Margin;
        MarginTop = other.MarginTop ?? MarginTop;
        MarginRight = other.MarginRight ?? MarginRight;
        MarginBottom = other.MarginBottom ?? MarginBottom;
        MarginLeft = other.MarginLeft ?? MarginLeft;
        Gap = other.Gap ?? Gap;
        TextAlign = string.IsNullOrEmpty(other.TextAlign) ? TextAlign : other.TextAlign;
        UserSelect = string.IsNullOrEmpty(other.UserSelect) ? UserSelect : other.UserSelect;
        ScrollX = other.ScrollX ?? ScrollX;
        ScrollY = other.ScrollY ?? ScrollY;
        TextColorGradient = other.TextColorGradient ?? TextColorGradient;
        TextBackgroundGradient = other.TextBackgroundGradient ?? TextBackgroundGradient;
    }
}

public readonly record struct Rect(int Row, int Col, int Width, int Height)
{
    public bool Contains(Point point)
    {
        return point.Row >= Row && point.Row < Row + Height &&
            point.Col >= Col && point.Col < Col + Width;
    }
}

public readonly record struct Size(int Width, int Height);

public readonly record struct Point(int Row, int Col);

public enum DimensionMode
{
    Auto,
    Fixed,
    Expanded,
    FitContent,
}

public readonly record struct AxisDimension(DimensionMode Mode, int Value)
{
    public static AxisDimension Auto(int value = 0)
    {
        return new AxisDimension(DimensionMode.Auto, value);
    }

    public static AxisDimension Fixed(int value)
    {
        return new AxisDimension(DimensionMode.Fixed, value);
    }

    public static AxisDimension Expanded()
    {
        return new AxisDimension(DimensionMode.Expanded, 0);
    }

    public static AxisDimension FitContent()
    {
        return new AxisDimension(DimensionMode.FitContent, 0);
    }
}

public sealed class TerminalCell
{
    public string Text { get; set; } = " ";
    public string Raw { get; set; } = "";
    public int RawWidth { get; set; } = 1;
    public int RawHeight { get; set; } = 1;
    public bool RawSkip { get; set; }
    public bool BackgroundFromImageSample { get; set; }
    public Color? Foreground { get; set; }
    public Color? Background { get; set; }

    public TerminalCell Clone()
    {
        return new TerminalCell
        {
            Text = Text,
            Raw = Raw,
            RawWidth = RawWidth,
            RawHeight = RawHeight,
            RawSkip = RawSkip,
            BackgroundFromImageSample = BackgroundFromImageSample,
            Foreground = Foreground,
            Background = Background,
        };
    }
}

public sealed class TerminalBuffer
{
    private const int AnsiBaseRow = 1;
    private const int AnsiBaseCol = 1;
    private const string AnsiReset = "\x1b[0m";
    private const string AnsiSyncUpdateBegin = "\x1b[?2026h";
    private const string AnsiSyncUpdateEnd = "\x1b[?2026l";
    private const string AnsiResetScrollRegion = "\x1b[r";
    private const int MinimumScrollRegionRows = 2;

    private readonly TerminalCell[,] cells;
    private readonly TerminalCell[,] previous;
    private bool forceFullRedraw = true;

    public int Width { get; }
    public int Height { get; }

    public TerminalBuffer(int width, int height)
    {
        Width = Math.Max(1, width);
        Height = Math.Max(1, height);
        cells = new TerminalCell[Height, Width];
        previous = new TerminalCell[Height, Width];
        Clear();
        FillPrevious();
    }

    public void Clear(TerminalCell? cell = null)
    {
        TerminalCell fill = cell ?? new TerminalCell();
        for (int row = 0; row < Height; ++row)
        {
            for (int col = 0; col < Width; ++col)
            {
                cells[row, col] = fill.Clone();
            }
        }
    }

    public TerminalCell Cell(int row, int col)
    {
        if (row < 0 || row >= Height || col < 0 || col >= Width)
        {
            throw new ArgumentOutOfRangeException(nameof(row));
        }
        return cells[row, col];
    }

    public void SetCell(int row, int col, TerminalCell cell)
    {
        if (row < 0 || row >= Height || col < 0 || col >= Width)
        {
            return;
        }
        cells[row, col] = cell;
    }

    public void RequestFullRedraw()
    {
        forceFullRedraw = true;
    }

    public string RenderDiff(int rowOffset = 0, int colOffset = 0)
    {
        return RenderDiffRegion(rowOffset, colOffset, 0, 0, Height, Width);
    }

    public string RenderDiffRegion(int rowOffset, int colOffset, int startRow, int startCol, int height, int width)
    {
        StringBuilder output = new();
        bool fullRedraw = forceFullRedraw;
        bool synchronizeUpdate = false;
        bool rawEmitted = false;
        int firstRow = Math.Max(0, startRow);
        int firstCol = Math.Max(0, startCol);
        int lastRow = Math.Min(Height, startRow + Math.Max(0, height));
        int lastCol = Math.Min(Width, startCol + Math.Max(0, width));
        for (int row = firstRow; row < lastRow; ++row)
        {
            int col = firstCol;
            while (col < lastCol)
            {
                TerminalCell start = cells[row, col];
                if (start.RawSkip)
                {
                    previous[row, col] = start.Clone();
                    ++col;
                    continue;
                }
                if (!fullRedraw && CellsEqual(start, previous[row, col]))
                {
                    ++col;
                    continue;
                }

                TerminalCell styleCell = start;
                if (!string.IsNullOrEmpty(styleCell.Raw))
                {
                    synchronizeUpdate = true;
                    int rawWidth = Math.Max(1, styleCell.RawWidth);
                    int rawHeight = Math.Max(1, styleCell.RawHeight);
                    int clearWidth = Math.Min(rawWidth, Width - col);
                    int clearHeight = Math.Min(rawHeight, Height - row);
                    for (int clearRow = row; clearRow < row + clearHeight; ++clearRow)
                    {
                        output.Append(CursorPosition(clearRow + rowOffset, col + colOffset));
                        output.Append(SgrForCell(styleCell));
                        output.Append(' ', clearWidth);
                    }
                    if (clearHeight >= rawHeight)
                    {
                        int anchorRow = row + rowOffset;
                        string noScrollRegion = RawNoScrollRegion(anchorRow, rawHeight, rowOffset + Height);
                        output.Append(noScrollRegion);
                        output.Append(CursorPosition(row + rowOffset, col + colOffset));
                        output.Append(styleCell.Raw);
                        if (!string.IsNullOrEmpty(noScrollRegion))
                        {
                            output.Append(AnsiResetScrollRegion);
                        }
                        rawEmitted = true;
                    }
                    for (int coveredRow = row; coveredRow < row + clearHeight; ++coveredRow)
                    {
                        for (int coveredCol = col; coveredCol < col + clearWidth; ++coveredCol)
                        {
                            previous[coveredRow, coveredCol] = cells[coveredRow, coveredCol].Clone();
                        }
                    }
                    col += clearWidth;
                    continue;
                }

                StringBuilder run = new();
                int runCol = col;
                while (col < lastCol)
                {
                    TerminalCell current = cells[row, col];
                    if (!fullRedraw && CellsEqual(current, previous[row, col]))
                    {
                        break;
                    }
                    if (current.RawSkip || !string.IsNullOrEmpty(current.Raw))
                    {
                        break;
                    }
                    if (!RenderHelpers.SameColor(current.Foreground, styleCell.Foreground) ||
                        !RenderHelpers.SameColor(current.Background, styleCell.Background))
                    {
                        break;
                    }
                    run.Append(RenderHelpers.SafeTerminalCellText(current.Text));
                    previous[row, col] = current.Clone();
                    ++col;
                }

                output.Append(CursorPosition(row + rowOffset, runCol + colOffset));
                output.Append(SgrForCell(styleCell));
                output.Append(run);
            }
        }

        if (output.Length > 0)
        {
            if (rawEmitted)
            {
                RepaintTextOverRaw(output, rowOffset, colOffset, firstRow, firstCol, lastRow, lastCol);
            }
            output.Append(AnsiReset);
        }
        forceFullRedraw = false;
        if (output.Length > 0 && synchronizeUpdate)
        {
            return AnsiSyncUpdateBegin + output + AnsiSyncUpdateEnd;
        }
        return output.ToString();
    }

    private static string RawNoScrollRegion(int anchorRow, int rawHeight, int bufferBottomExclusive)
    {
        if (anchorRow >= MinimumScrollRegionRows)
        {
            return $"\x1b[1;{anchorRow}r";
        }
        int rawBottomExclusive = anchorRow + Math.Max(1, rawHeight);
        if (bufferBottomExclusive - rawBottomExclusive >= MinimumScrollRegionRows)
        {
            return $"\x1b[{rawBottomExclusive + AnsiBaseRow};{bufferBottomExclusive}r";
        }
        return "";
    }

    public string RenderScrollRegion(int rowOffset, int startRow, int height, int delta)
    {
        int firstRow = Math.Max(0, startRow);
        int lastRow = Math.Min(Height, startRow + Math.Max(0, height));
        int regionHeight = lastRow - firstRow;
        int distance = Math.Min(Math.Abs(delta), regionHeight);
        if (forceFullRedraw || regionHeight <= 1 || distance <= 0 || distance >= regionHeight)
        {
            return "";
        }
        if (ScrollRegionHasRawCells(firstRow, lastRow))
        {
            return "";
        }

        TerminalCell[,] before = (TerminalCell[,])previous.Clone();
        if (delta > 0)
        {
            for (int row = lastRow - 1; row >= firstRow + distance; --row)
            {
                for (int col = 0; col < Width; ++col)
                {
                    previous[row, col] = before[row - distance, col];
                }
            }
            for (int row = firstRow; row < firstRow + distance; ++row)
            {
                for (int col = 0; col < Width; ++col)
                {
                    previous[row, col] = new TerminalCell();
                }
            }
        }
        else
        {
            for (int row = firstRow; row < lastRow - distance; ++row)
            {
                for (int col = 0; col < Width; ++col)
                {
                    previous[row, col] = before[row + distance, col];
                }
            }
            for (int row = lastRow - distance; row < lastRow; ++row)
            {
                for (int col = 0; col < Width; ++col)
                {
                    previous[row, col] = new TerminalCell();
                }
            }
        }

        char command = delta > 0 ? 'T' : 'S';
        return $"\x1b[{firstRow + rowOffset + AnsiBaseRow};{lastRow + rowOffset}r" +
            $"\x1b[{firstRow + rowOffset + AnsiBaseRow};1H" +
            $"\x1b[{distance}{command}" +
            "\x1b[r";
    }

    private bool ScrollRegionHasRawCells(int firstRow, int lastRow)
    {
        for (int row = firstRow; row < lastRow; ++row)
        {
            for (int col = 0; col < Width; ++col)
            {
                TerminalCell current = cells[row, col];
                TerminalCell prior = previous[row, col];
                if (!string.IsNullOrEmpty(current.Raw) || current.RawSkip ||
                    !string.IsNullOrEmpty(prior.Raw) || prior.RawSkip)
                {
                    return true;
                }
            }
        }
        return false;
    }

    private void RepaintTextOverRaw(
        StringBuilder output,
        int rowOffset,
        int colOffset,
        int firstRow,
        int firstCol,
        int lastRow,
        int lastCol)
    {
        for (int row = firstRow; row < lastRow; ++row)
        {
            int col = firstCol;
            while (col < lastCol)
            {
                TerminalCell cell = cells[row, col];
                if (cell.RawSkip || !string.IsNullOrEmpty(cell.Raw))
                {
                    ++col;
                    continue;
                }
                TerminalCell styleCell = cell;
                int runCol = col;
                StringBuilder run = new();
                while (col < lastCol)
                {
                    TerminalCell current = cells[row, col];
                    if (current.RawSkip || !string.IsNullOrEmpty(current.Raw))
                    {
                        break;
                    }
                    if (!RenderHelpers.SameColor(current.Foreground, styleCell.Foreground) ||
                        !RenderHelpers.SameColor(current.Background, styleCell.Background))
                    {
                        break;
                    }
                    run.Append(RenderHelpers.SafeTerminalCellText(current.Text));
                    ++col;
                }
                output.Append(CursorPosition(row + rowOffset, runCol + colOffset));
                output.Append(SgrForCell(styleCell));
                output.Append(run);
            }
        }
    }

    private void FillPrevious()
    {
        for (int row = 0; row < Height; ++row)
        {
            for (int col = 0; col < Width; ++col)
            {
                previous[row, col] = new TerminalCell();
            }
        }
    }

    private static string CursorPosition(int row, int col)
    {
        return $"\x1b[{row + AnsiBaseRow};{col + AnsiBaseCol}H";
    }

    private static string SgrForCell(TerminalCell cell)
    {
        return "\x1b[" + SgrForColor(cell.Foreground, foreground: true) + ";" +
            SgrForColor(cell.Background, foreground: false) + "m";
    }

    private static string SgrForColor(Color? color, bool foreground)
    {
        Color? visible = RenderHelpers.VisibleColor(color);
        if (visible is null || !visible.Rgba.HasValue)
        {
            return foreground ? "39" : "49";
        }
        Rgba rgba = visible.Rgba.Value;
        return (foreground ? "38" : "48") + $";2;{rgba.Red};{rgba.Green};{rgba.Blue}";
    }

    private static bool CellsEqual(TerminalCell lhs, TerminalCell rhs)
    {
        return lhs.Text == rhs.Text &&
            lhs.Raw == rhs.Raw &&
            lhs.RawWidth == rhs.RawWidth &&
            lhs.RawHeight == rhs.RawHeight &&
            lhs.RawSkip == rhs.RawSkip &&
            lhs.BackgroundFromImageSample == rhs.BackgroundFromImageSample &&
            RenderHelpers.SameColor(lhs.Foreground, rhs.Foreground) &&
            RenderHelpers.SameColor(lhs.Background, rhs.Background);
    }
}

public sealed class ElementRenderState
{
    public bool Focused { get; init; }
    public bool EditMode { get; init; }
    public bool PassiveFocus { get; init; }
    public Element? FocusedElement { get; init; }
    public bool SuppressActiveScrollViewScopeVisuals { get; init; }
    public int? ClipTop { get; init; }
    public int? ClipBottom { get; init; }
}

public static class RenderHelpers
{
    private const int TextTabWidth = 4;

    [ThreadStatic]
    private static long? renderTimeOverrideMs;

    public static long? RenderTimeOverrideMs
    {
        get => renderTimeOverrideMs;
        set => renderTimeOverrideMs = value;
    }

    internal readonly record struct VisualGlyph(string Text, int SourceStart, int SourceEnd);

    internal sealed class LabelVisualRow
    {
        public int Start { get; }
        public int End { get; }
        public List<VisualGlyph> Cells { get; }

        public LabelVisualRow(int start, int end, List<VisualGlyph> cells)
        {
            Start = start;
            End = end;
            Cells = cells;
        }
    }

    internal static int AlignedTextOffset(int contentWidth, int width, string? align)
    {
        int padding = Math.Max(0, width - contentWidth);
        return align switch
        {
            "center" => padding / 2,
            "right" => padding,
            _ => 0,
        };
    }

    public static List<List<TerminalCell>> RenderPlainText(string text, int width, int height, Style style)
    {
        text ??= "";
        width = Math.Max(1, width > 0 ? width : VisibleWidth(text));
        height = Math.Max(1, height);
        List<List<TerminalCell>> result = new();
        if (height > 1)
        {
            List<LabelVisualRow> rows = BuildLabelVisualRows(text, width);
            foreach (LabelVisualRow row in rows)
            {
                if (result.Count >= height)
                {
                    break;
                }
                result.Add(RenderGlyphRow(row.Cells, width, style));
            }
            while (result.Count < height)
            {
                result.Add(RenderGlyphRow(new List<VisualGlyph>(), width, style));
            }
            return result;
        }
        int newline = text.IndexOf('\n', StringComparison.Ordinal);
        string firstLine = newline >= 0 ? text[..newline] : text;
        result.Add(RenderGlyphRow(VisualGlyphs(firstLine, 0, 0), width, style));
        return result;
    }

    public static List<string> RenderedText(List<List<TerminalCell>> rendered)
    {
        return rendered.Select(row => string.Concat(row.Select(cell => cell.Text))).ToList();
    }

    public static List<string> RenderedAnsiText(List<List<TerminalCell>> rendered)
    {
        List<string> lines = new();
        foreach (List<TerminalCell> row in rendered)
        {
            StringBuilder builder = new();
            Color? currentForeground = null;
            Color? currentBackground = null;
            foreach (TerminalCell cell in row)
            {
                if (cell.RawSkip)
                {
                    continue;
                }
                if (!string.IsNullOrEmpty(cell.Raw))
                {
                    builder.Append("\x1b[0m");
                    builder.Append(cell.Raw);
                    currentForeground = null;
                    currentBackground = null;
                    continue;
                }
                Color? foreground = VisibleColor(cell.Foreground);
                Color? background = VisibleColor(cell.Background);
                if (!SameColor(foreground, currentForeground) || !SameColor(background, currentBackground))
                {
                    builder.Append("\x1b[0m");
                    if (foreground is not null)
                    {
                        builder.Append(AnsiForeground(foreground));
                    }
                    if (background is not null)
                    {
                        builder.Append(AnsiBackground(background));
                    }
                    currentForeground = foreground;
                    currentBackground = background;
                }
                builder.Append(cell.Text.Length == 0 ? " " : cell.Text);
            }
            builder.Append("\x1b[0m");
            lines.Add(builder.ToString());
        }
        return lines;
    }

    public static string JsonColor(Color? color)
    {
        return color is null || color.IsTransparent ? "null" : JsonSerializer.Serialize(color.ToString());
    }

    public static JsonArray CompactCells(List<List<TerminalCell>> content)
    {
        JsonArray rows = new();
        foreach (List<TerminalCell> row in content)
        {
            JsonArray cells = new();
            foreach (TerminalCell cell in row)
            {
                cells.Add(new JsonArray(
                    cell.Text.Length == 0 ? " " : cell.Text,
                    cell.Foreground is null || cell.Foreground.IsTransparent ? null : cell.Foreground.ToString(),
                    cell.Background is null || cell.Background.IsTransparent ? null : cell.Background.ToString()
                ));
            }
            rows.Add(cells);
        }
        return rows;
    }

    public static JsonArray SnapshotCells(List<List<TerminalCell>> content)
    {
        JsonArray rows = new();
        foreach (List<TerminalCell> row in content)
        {
            JsonArray cells = new();
            foreach (TerminalCell cell in row)
            {
                cells.Add(new JsonObject
                {
                    ["char"] = cell.Text.Length == 0 ? " " : cell.Text,
                    ["foreground"] = cell.Foreground is null || cell.Foreground.IsTransparent ? null : cell.Foreground.ToString(),
                    ["background"] = cell.Background is null || cell.Background.IsTransparent ? null : cell.Background.ToString(),
                    ["attributes"] = new JsonArray(),
                });
            }
            rows.Add(cells);
        }
        return rows;
    }

    public static List<string> WrapText(string text, int width)
    {
        width = Math.Max(1, width);
        List<string> rows = new();
        string normalized = text.Replace("\r\n", "\n").Replace('\r', '\n');
        foreach (string segment in normalized.Split('\n'))
        {
            if (segment.Length == 0)
            {
                rows.Add("");
                continue;
            }
            int offset = 0;
            while (offset < segment.Length)
            {
                int remaining = segment.Length - offset;
                int chunkSize = Math.Min(remaining, width);
                string chunk = segment.Substring(offset, chunkSize);
                if (chunkSize == width)
                {
                    int lastSpace = chunk.LastIndexOf(' ');
                    if (lastSpace > 0)
                    {
                        rows.Add(chunk[..lastSpace]);
                        offset += lastSpace + 1;
                        continue;
                    }
                }
                rows.Add(chunk);
                offset += chunkSize;
            }
        }
        if (rows.Count == 0)
        {
            rows.Add("");
        }
        return rows;
    }

    public static string Align(string text, int width, string align)
    {
        List<VisualGlyph> cells = VisualGlyphs(text ?? "", 0, 0);
        if (cells.Count > width)
        {
            cells = cells.Take(width).ToList();
        }
        int padding = Math.Max(0, width - cells.Count);
        string visible = string.Concat(cells.Select(cell => cell.Text));
        return align switch
        {
            "right" => new string(' ', padding) + visible,
            "center" => new string(' ', padding / 2) + visible + new string(' ', padding - padding / 2),
            _ => visible + new string(' ', padding),
        };
    }

    private static List<TerminalCell> RenderGlyphRow(List<VisualGlyph> glyphs, int width, Style style)
    {
        if (glyphs.Count > width)
        {
            glyphs = glyphs.Take(width).ToList();
        }
        int contentWidth = glyphs.Count;
        int offset = AlignedTextOffset(contentWidth, width, style.TextAlign);
        List<TerminalCell> row = new(width);
        for (int col = 0; col < offset && col < width; ++col)
        {
            row.Add(StyledCell(" ", style));
        }
        foreach (VisualGlyph glyph in glyphs)
        {
            if (row.Count >= width)
            {
                break;
            }
            Color? foreground = null;
            Color? background = null;
            if (glyph.SourceStart >= 0)
            {
                foreground = BlendGradientColor(GradientColorAt(style.TextColorGradient, glyph.SourceStart), style.Color);
                background = BlendGradientColor(GradientColorAt(style.TextBackgroundGradient, glyph.SourceStart), style.Background);
            }
            row.Add(StyledCell(glyph.Text, style, foreground, background));
        }
        while (row.Count < width)
        {
            row.Add(StyledCell(" ", style));
        }
        return row;
    }

    private static TerminalCell StyledCell(string text, Style style, Color? foreground = null, Color? background = null)
    {
        return new TerminalCell
        {
            Text = SafeTerminalCellText(text),
            Foreground = foreground ?? style.Color,
            Background = background ?? style.Background,
        };
    }

    private static LabelVisualRow MakeLabelVisualRow(int fallbackStart, List<VisualGlyph> cells)
    {
        if (cells.Count == 0)
        {
            return new LabelVisualRow(fallbackStart, fallbackStart, cells);
        }
        return new LabelVisualRow(cells[0].SourceStart, cells[^1].SourceEnd, cells);
    }

    internal static List<LabelVisualRow> BuildLabelVisualRows(string text, int width)
    {
        width = Math.Max(1, width);
        List<LabelVisualRow> rows = new();
        int segmentStart = 0;
        while (segmentStart <= text.Length)
        {
            int newline = text.IndexOf('\n', segmentStart);
            int segmentEnd = newline < 0 ? text.Length : newline;
            string segment = text.Substring(segmentStart, segmentEnd - segmentStart);
            if (segment.Length == 0)
            {
                rows.Add(new LabelVisualRow(segmentStart, segmentStart, new List<VisualGlyph>()));
            }
            else
            {
                List<VisualGlyph> glyphs = VisualGlyphs(segment, 0, segmentStart);
                int offset = 0;
                while (offset < glyphs.Count)
                {
                    int remaining = glyphs.Count - offset;
                    int chunkSize = Math.Min(remaining, width);
                    List<VisualGlyph> chunk = glyphs.GetRange(offset, chunkSize);
                    if (chunkSize == width)
                    {
                        int lastSpace = -1;
                        for (int index = chunk.Count; index > 1; --index)
                        {
                            VisualGlyph glyph = chunk[index - 1];
                            if (glyph.Text == " " &&
                                glyph.SourceStart >= 0 &&
                                glyph.SourceStart < text.Length &&
                                text[glyph.SourceStart] == ' ')
                            {
                                lastSpace = index - 1;
                                break;
                            }
                        }
                        if (lastSpace > 0)
                        {
                            chunk.RemoveRange(lastSpace, chunk.Count - lastSpace);
                            rows.Add(MakeLabelVisualRow(segmentStart, chunk));
                            offset += lastSpace + 1;
                            continue;
                        }
                    }
                    rows.Add(MakeLabelVisualRow(segmentStart, chunk));
                    offset += chunkSize;
                }
            }
            if (newline < 0)
            {
                break;
            }
            segmentStart = newline + 1;
        }
        if (rows.Count == 0)
        {
            rows.Add(new LabelVisualRow(0, 0, new List<VisualGlyph>()));
        }
        return rows;
    }

    internal static int VisualWidthForLabelRow(LabelVisualRow row)
    {
        return row.Cells.Count;
    }

    internal static int RawIndexForLabelVisualColumn(LabelVisualRow row, int col)
    {
        if (row.Cells.Count == 0)
        {
            return row.Start;
        }
        if (col >= row.Cells.Count)
        {
            return row.End;
        }
        col = Math.Clamp(col, 0, row.Cells.Count - 1);
        VisualGlyph glyph = row.Cells[col];
        int runStart = col;
        int runEnd = col + 1;
        while (runStart > 0 &&
            row.Cells[runStart - 1].SourceStart == glyph.SourceStart &&
            row.Cells[runStart - 1].SourceEnd == glyph.SourceEnd)
        {
            --runStart;
        }
        while (runEnd < row.Cells.Count &&
            row.Cells[runEnd].SourceStart == glyph.SourceStart &&
            row.Cells[runEnd].SourceEnd == glyph.SourceEnd)
        {
            ++runEnd;
        }
        double midpoint = runStart + (runEnd - runStart) / 2.0;
        return col >= midpoint ? glyph.SourceEnd : glyph.SourceStart;
    }

    internal static List<VisualGlyph> VisualGlyphs(string text, int startColumn, int sourceStart)
    {
        List<VisualGlyph> glyphs = new();
        int column = Math.Max(0, startColumn);
        int offset = 0;
        while (offset < text.Length)
        {
            (string glyphText, int codepoint, int length, bool valid) = DecodeGlyph(text, offset);
            int rawStart = sourceStart + offset;
            int rawEnd = rawStart + Math.Max(length, 1);
            if (valid && codepoint == '\r')
            {
                offset += Math.Max(length, 1);
                continue;
            }
            if (valid && codepoint == '\t')
            {
                int spaces = TabSpacesForColumn(column);
                for (int index = 0; index < spaces; ++index)
                {
                    glyphs.Add(new VisualGlyph(" ", rawStart, rawEnd));
                    ++column;
                }
            }
            else
            {
                string visible = !valid || IsUnsafeTerminalCodepoint(codepoint) ? "?" : glyphText;
                glyphs.Add(new VisualGlyph(string.IsNullOrEmpty(visible) ? " " : visible, rawStart, rawEnd));
                ++column;
            }
            offset += Math.Max(length, 1);
        }
        return glyphs;
    }

    private static int VisibleWidth(string text)
    {
        return VisualGlyphs(text, 0, 0).Count;
    }

    private static (string Text, int Codepoint, int Length, bool Valid) DecodeGlyph(string text, int offset)
    {
        if (offset >= text.Length)
        {
            return (" ", ' ', 0, false);
        }
        char first = text[offset];
        if (char.IsHighSurrogate(first))
        {
            if (offset + 1 < text.Length && char.IsLowSurrogate(text[offset + 1]))
            {
                return (text.Substring(offset, 2), char.ConvertToUtf32(first, text[offset + 1]), 2, true);
            }
            return ("?", '?', 1, false);
        }
        if (char.IsLowSurrogate(first))
        {
            return ("?", '?', 1, false);
        }
        return (first.ToString(), first, 1, true);
    }

    private static int TabSpacesForColumn(int column)
    {
        int normalized = Math.Max(0, column);
        return TextTabWidth - normalized % TextTabWidth;
    }

    private static bool IsUnsafeTerminalCodepoint(int codepoint)
    {
        static bool InRange(int value, int first, int last)
        {
            return value >= first && value <= last;
        }

        if (codepoint < 0x20 || codepoint == 0x7F)
        {
            return true;
        }
        if (InRange(codepoint, 0x0300, 0x036F) ||
            InRange(codepoint, 0x1AB0, 0x1AFF) ||
            InRange(codepoint, 0x1DC0, 0x1DFF) ||
            InRange(codepoint, 0x20D0, 0x20FF) ||
            InRange(codepoint, 0xFE20, 0xFE2F))
        {
            return true;
        }
        if (InRange(codepoint, 0x200B, 0x200F) ||
            InRange(codepoint, 0x202A, 0x202E) ||
            InRange(codepoint, 0x2060, 0x206F) ||
            InRange(codepoint, 0xFE00, 0xFE0F) ||
            InRange(codepoint, 0xE0100, 0xE01EF))
        {
            return true;
        }
        if (InRange(codepoint, 0x1100, 0x115F) ||
            InRange(codepoint, 0x2329, 0x232A) ||
            InRange(codepoint, 0x2E80, 0xA4CF) ||
            InRange(codepoint, 0xAC00, 0xD7A3) ||
            InRange(codepoint, 0xF900, 0xFAFF) ||
            InRange(codepoint, 0xFE10, 0xFE19) ||
            InRange(codepoint, 0xFE30, 0xFE6F) ||
            InRange(codepoint, 0xFF00, 0xFF60) ||
            InRange(codepoint, 0xFFE0, 0xFFE6) ||
            InRange(codepoint, 0x1F000, 0x1FAFF) ||
            InRange(codepoint, 0x20000, 0x3FFFD))
        {
            return true;
        }
        return false;
    }

    internal static string SafeTerminalCellText(string text)
    {
        if (string.IsNullOrEmpty(text))
        {
            return " ";
        }
        (string glyphText, int codepoint, int _, bool valid) = DecodeGlyph(text, 0);
        if (!valid || codepoint == '\t')
        {
            return codepoint == '\t' ? " " : "?";
        }
        if (IsUnsafeTerminalCodepoint(codepoint))
        {
            return "?";
        }
        return string.IsNullOrEmpty(glyphText) ? " " : glyphText;
    }

    private static long GradientFrame(TextGradient gradient)
    {
        int interval = Math.Max(1, gradient.IntervalMs);
        long timeMs = RenderTimeOverrideMs ?? DateTimeOffset.UtcNow.ToUnixTimeMilliseconds();
        return timeMs / interval * gradient.Step;
    }

    private static Color? GradientColorAt(TextGradient? gradient, int sourceIndex)
    {
        if (gradient is null || gradient.Colors.Count == 0)
        {
            return null;
        }
        int segmentSize = Math.Max(1, gradient.SegmentSize);
        long frame = GradientFrame(gradient);
        long size = gradient.Colors.Count;
        long colorIndex = (sourceIndex / segmentSize - frame) % size;
        if (colorIndex < 0)
        {
            colorIndex += size;
        }
        return gradient.Colors[(int)colorIndex];
    }

    private static Color? BlendGradientColor(Color? overlay, Color? baseColor)
    {
        if (overlay is null)
        {
            return baseColor;
        }
        if (!overlay.Rgba.HasValue)
        {
            return overlay;
        }
        if (overlay.Rgba.Value.Alpha == 0)
        {
            return baseColor;
        }
        if (overlay.Rgba.Value.Alpha < 255 && baseColor is not null && baseColor.Rgba.HasValue)
        {
            return overlay.BlendOver(baseColor);
        }
        return overlay;
    }

    internal static Color? VisibleColor(Color? color)
    {
        return color is null || color.IsTransparent ? null : color;
    }

    internal static bool SameColor(Color? lhs, Color? rhs)
    {
        if (lhs is null || rhs is null)
        {
            return lhs is null && rhs is null;
        }
        return lhs.Equals(rhs);
    }

    internal static string AnsiForeground(Color color)
    {
        return color.Rgba is Rgba rgba ? $"\x1b[38;2;{rgba.Red};{rgba.Green};{rgba.Blue}m" : "";
    }

    internal static string AnsiBackground(Color color)
    {
        return color.Rgba is Rgba rgba ? $"\x1b[48;2;{rgba.Red};{rgba.Green};{rgba.Blue}m" : "";
    }
}
