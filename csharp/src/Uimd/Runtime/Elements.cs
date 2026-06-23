using StbImageSharp;

namespace Uimd;

public class Element
{
    public const string CommitModeStandard = "standard";
    public const string CommitModeLeave = "leave";

    [ThreadStatic]
    private static Color? renderingParentBackground;

    public string Name { get; }
    public string CommitMode { get; private set; } = CommitModeStandard;
    public bool Focusable { get; set; } = true;
    public bool Enabled { get; set; } = true;
    public Rect Frame { get; set; }
    public Style Style { get; private set; } = new();
    public Style? FocusStyle { get; private set; }
    public Style? EditStyle { get; private set; }
    public Style? CursorStyle { get; private set; }
    public Style? SelectedStyle { get; private set; }
    public Style? CheckedStyle { get; private set; }
    public Style? UncheckedStyle { get; private set; }
    public Style? DisabledStyle { get; private set; }
    public List<Element> Children { get; } = new();

    public Element(string name)
    {
        Name = name;
    }

    internal static Color? RenderingParentBackground
    {
        get => renderingParentBackground;
        set => renderingParentBackground = value;
    }

    public void SetCommitMode(string mode)
    {
        CommitMode = mode;
    }

    public void SetStyle(Style style)
    {
        Style = style;
    }

    public void SetFocusStyle(Style style)
    {
        FocusStyle = style;
    }

    public void SetEditStyle(Style style)
    {
        EditStyle = style;
    }

    public void SetCursorStyle(Style style)
    {
        CursorStyle = style;
    }

    public void SetSelectedStyle(Style style)
    {
        SelectedStyle = style;
    }

    public void SetCheckedStyle(Style style)
    {
        CheckedStyle = style;
    }

    public void SetUncheckedStyle(Style style)
    {
        UncheckedStyle = style;
    }

    public void SetDisabledStyle(Style style)
    {
        DisabledStyle = style;
    }

    public Style EffectiveStyle(bool focused = false, bool editMode = false)
    {
        Style result = Style.Clone();
        if (!Enabled && DisabledStyle is not null)
        {
            return MergedStateStyle(result, DisabledStyle);
        }
        if (editMode && EditStyle is not null)
        {
            return MergedStateStyle(result, EditStyle);
        }
        if (focused && FocusStyle is not null)
        {
            return MergedStateStyle(result, FocusStyle);
        }
        return result;
    }

    private static bool HasPartialAlpha(Color? color)
    {
        return color?.Rgba is Rgba rgba && rgba.Alpha < 255;
    }

    private static Style MergedStateStyle(Style baseStyle, Style state)
    {
        Color? baseBackground = baseStyle.Background;
        Color? stateBackground = state.Background;
        if (HasPartialAlpha(baseBackground) &&
            renderingParentBackground?.Rgba is not null)
        {
            baseStyle.Background = baseBackground!.BlendOver(renderingParentBackground);
            baseBackground = baseStyle.Background;
        }
        baseStyle.Merge(state);
        if (HasPartialAlpha(stateBackground) &&
            baseBackground?.Rgba is Rgba &&
            stateBackground!.Rgba!.Value.Alpha == 0)
        {
            baseStyle.Background = baseBackground;
        }
        else if (HasPartialAlpha(stateBackground) &&
            baseBackground?.Rgba is Rgba)
        {
            baseStyle.Background = stateBackground!.BlendOver(baseBackground);
        }
        return baseStyle;
    }

    public virtual bool HandleKey(string key)
    {
        return false;
    }

    public virtual List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        return RenderHelpers.RenderPlainText(Name, size.Width, size.Height, EffectiveStyle(state.Focused, state.EditMode));
    }
}

public sealed class ScopedElementParentBackground : IDisposable
{
    private readonly Color? previous;

    public ScopedElementParentBackground(Color? background)
    {
        previous = Element.RenderingParentBackground;
        Element.RenderingParentBackground = background;
    }

    public void Dispose()
    {
        Element.RenderingParentBackground = previous;
    }
}

public class Label : Element
{
    public string Text { get; private set; }
    private readonly List<LabelSpan> spans = new();

    public Label(string name, string text = "") : base(name)
    {
        Text = text;
        Focusable = false;
    }

    public void SetText(string text)
    {
        Text = text;
        spans.Clear();
    }

    public void SetSpans(IEnumerable<LabelSpan> nextSpans)
    {
        spans.Clear();
        spans.AddRange(nextSpans);
        Text = string.Concat(spans.Select(span => span.Text));
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        if (spans.Count == 0)
        {
            return RenderHelpers.RenderPlainText(Text, size.Width, size.Height, style);
        }
        int width = Math.Max(1, size.Width > 0 ? size.Width : Text.Length);
        int height = Math.Max(1, size.Height);
        List<List<TerminalCell>> rendered = new();
        List<TerminalCell> row = new(width);
        bool clippingLine = false;
        foreach (LabelSpan span in spans)
        {
            Color? foreground = string.IsNullOrEmpty(span.Foreground) ? null : new Color(span.Foreground);
            Color? background = string.IsNullOrEmpty(span.Background) ? null : new Color(span.Background);
            foreach (char ch in span.Text)
            {
                if (ch == '\r')
                {
                    continue;
                }
                if (ch == '\n')
                {
                    AppendBlankCells(row, width, style);
                    rendered.Add(row);
                    if (rendered.Count >= height)
                    {
                        return rendered;
                    }
                    row = new List<TerminalCell>(width);
                    clippingLine = false;
                    continue;
                }
                if (row.Count >= width)
                {
                    clippingLine = true;
                }
                if (!clippingLine)
                {
                    row.Add(StyledSpanCell(ch, style, foreground, background));
                }
            }
        }
        if (rendered.Count < height)
        {
            AppendBlankCells(row, width, style);
            rendered.Add(row);
        }
        while (rendered.Count < height)
        {
            rendered.Add(RenderHelpers.RenderPlainText("", width, 1, style)[0]);
        }
        return rendered;
    }

    private static TerminalCell StyledSpanCell(char ch, Style style, Color? foreground, Color? background)
    {
        TerminalCell cell = RenderHelpers.RenderPlainText(ch.ToString(), 1, 1, style)[0][0];
        cell.Foreground = foreground ?? cell.Foreground;
        cell.Background = background ?? cell.Background;
        return cell;
    }

    private static void AppendBlankCells(List<TerminalCell> row, int width, Style style)
    {
        while (row.Count < width)
        {
            row.Add(RenderHelpers.RenderPlainText("", 1, 1, style)[0][0]);
        }
    }
}

public sealed class LabelSpan
{
    public string Text { get; set; } = "";
    public string Foreground { get; set; } = "";
    public string Background { get; set; } = "";

    public LabelSpan()
    {
    }

    public LabelSpan(string text, string foreground = "", string background = "")
    {
        Text = text;
        Foreground = foreground;
        Background = background;
    }
}

public sealed class InfoLabel : Label
{
    public InfoLabel(string name, string text = "") : base(name, text)
    {
    }
}

public sealed class FrameBufferView : Element
{
    private List<List<TerminalCell>> content = new();

    public FrameBufferView(string name) : base(name)
    {
        Focusable = false;
    }

    public void SetContent(List<List<TerminalCell>> rows)
    {
        content = rows;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> result = RenderHelpers.RenderPlainText("", size.Width, size.Height, style);
        for (int row = 0; row < Math.Min(size.Height, content.Count); ++row)
        {
            for (int col = 0; col < Math.Min(size.Width, content[row].Count); ++col)
            {
                result[row][col] = content[row][col].Clone();
            }
        }
        return result;
    }
}

public sealed class MessageTable : Label
{
    private enum TableAlignment
    {
        Left,
        Center,
        Right,
    }

    private sealed record ParsedTable(
        List<string> Headers,
        List<TableAlignment> Alignments,
        List<List<string>> Rows);

    public MessageTable(string name, string text = "") : base(name, text)
    {
    }

    public int FitContentHeight()
    {
        ParsedTable? parsed = ParseMarkdownTable(Text);
        if (parsed is null)
        {
            return 1;
        }
        return parsed.Rows.Count + 4;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        int width = Math.Max(1, size.Width);
        ParsedTable? parsed = ParseMarkdownTable(Text);
        if (parsed is null)
        {
            return RenderHelpers.RenderPlainText("", width, 1, style);
        }

        List<int> widths = ExpandColumnWidths(ColumnWidths(parsed), width);
        Style borderStyle = style.Clone();
        List<List<TerminalCell>> rendered = new()
        {
            SeparatorRow(widths, borderStyle, "┌", "┬", "┐"),
            DataRow(parsed.Headers, widths, parsed.Alignments, style, borderStyle),
            SeparatorRow(widths, borderStyle, "├", "┼", "┤"),
        };
        foreach (List<string> row in parsed.Rows)
        {
            rendered.Add(DataRow(row, widths, parsed.Alignments, style, borderStyle));
        }
        rendered.Add(SeparatorRow(widths, borderStyle, "└", "┴", "┘"));
        return rendered;
    }

    private static ParsedTable? ParseMarkdownTable(string text)
    {
        if (string.IsNullOrWhiteSpace(text))
        {
            return null;
        }
        string[] lines = text.Trim().Replace("\r\n", "\n").Replace('\r', '\n').Split('\n');
        if (lines.Length < 3)
        {
            return null;
        }

        List<string> headers = SplitTableCells(lines[0]);
        List<string> separator = SplitTableCells(lines[1]);
        if (headers.Count == 0 || headers.Count != separator.Count || !IsMarkdownSeparatorRow(separator))
        {
            return null;
        }

        ParsedTable table = new(headers, ParseAlignments(separator), new List<List<string>>());
        for (int index = 2; index < lines.Length; ++index)
        {
            List<string> row = SplitTableCells(lines[index]);
            if (row.Count != 0 && row.Count == headers.Count)
            {
                table.Rows.Add(row);
            }
        }
        return table.Rows.Count == 0 ? null : table;
    }

    private static List<string> SplitTableCells(string line)
    {
        string trimmed = line.Trim();
        if (trimmed.StartsWith('|'))
        {
            trimmed = trimmed[1..];
        }
        if (trimmed.EndsWith('|'))
        {
            trimmed = trimmed[..^1];
        }
        return trimmed.Split('|').Select(cell => cell.Trim()).ToList();
    }

    private static bool IsMarkdownSeparatorRow(List<string> cells)
    {
        if (cells.Count == 0)
        {
            return false;
        }
        foreach (string cell in cells)
        {
            string compact = cell.Trim();
            if (compact.Length == 0)
            {
                return false;
            }
            foreach (char ch in compact)
            {
                if (ch != '-' && ch != ':')
                {
                    return false;
                }
            }
        }
        return true;
    }

    private static List<TableAlignment> ParseAlignments(List<string> cells)
    {
        List<TableAlignment> alignments = new(cells.Count);
        foreach (string cell in cells)
        {
            string compact = cell.Trim();
            bool left = compact.StartsWith(':');
            bool right = compact.EndsWith(':');
            if (left && right)
            {
                alignments.Add(TableAlignment.Center);
            }
            else if (right)
            {
                alignments.Add(TableAlignment.Right);
            }
            else
            {
                alignments.Add(TableAlignment.Left);
            }
        }
        return alignments;
    }

    private static List<int> ColumnWidths(ParsedTable table)
    {
        List<int> widths = table.Headers.Select(TextWidth).ToList();
        foreach (List<string> row in table.Rows)
        {
            for (int column = 0; column < row.Count && column < widths.Count; ++column)
            {
                widths[column] = Math.Max(widths[column], TextWidth(row[column]));
            }
        }
        return widths;
    }

    private static int RenderedTableWidth(List<int> widths)
    {
        if (widths.Count == 0)
        {
            return 0;
        }
        return 1 + widths.Sum(width => width + 2) + (widths.Count - 1) + 1;
    }

    private static List<int> ExpandColumnWidths(List<int> widths, int targetWidth)
    {
        if (widths.Count == 0)
        {
            return widths;
        }
        int extraWidth = targetWidth - RenderedTableWidth(widths);
        if (extraWidth <= 0)
        {
            return widths;
        }
        int perColumn = extraWidth / widths.Count;
        int remainder = extraWidth % widths.Count;
        for (int index = 0; index < widths.Count; ++index)
        {
            widths[index] += perColumn + (index < remainder ? 1 : 0);
        }
        return widths;
    }

    private static List<TerminalCell> SeparatorRow(List<int> widths, Style borderStyle, string left, string middle, string right)
    {
        List<TerminalCell> row = new();
        AppendGlyph(row, left, borderStyle);
        for (int column = 0; column < widths.Count; ++column)
        {
            for (int index = 0; index < widths[column] + 2; ++index)
            {
                AppendGlyph(row, "─", borderStyle);
            }
            if (column + 1 < widths.Count)
            {
                AppendGlyph(row, middle, borderStyle);
            }
        }
        AppendGlyph(row, right, borderStyle);
        return row;
    }

    private static List<TerminalCell> DataRow(
        List<string> cells,
        List<int> widths,
        List<TableAlignment> alignments,
        Style cellStyle,
        Style borderStyle)
    {
        List<TerminalCell> row = new();
        AppendGlyph(row, "│", borderStyle);
        for (int column = 0; column < widths.Count; ++column)
        {
            string value = column < cells.Count
                ? AlignText(cells[column], widths[column], alignments[column])
                : new string(' ', widths[column]);
            AppendText(row, " ", cellStyle);
            AppendText(row, value, cellStyle);
            AppendText(row, " ", cellStyle);
            if (column + 1 < widths.Count)
            {
                AppendGlyph(row, "│", borderStyle);
            }
        }
        AppendGlyph(row, "│", borderStyle);
        return row;
    }

    private static string AlignText(string text, int width, TableAlignment alignment)
    {
        string visible = VisibleText(text);
        int textWidth = TextWidth(visible);
        if (textWidth >= width)
        {
            return FitVisibleText(visible, width);
        }
        int padding = width - textWidth;
        return alignment switch
        {
            TableAlignment.Right => new string(' ', padding) + visible,
            TableAlignment.Center => new string(' ', padding / 2) + visible + new string(' ', padding - padding / 2),
            _ => visible + new string(' ', padding),
        };
    }

    private static string FitVisibleText(string text, int width)
    {
        List<string> glyphs = TextGlyphs(text).Take(Math.Max(0, width)).ToList();
        return string.Concat(glyphs);
    }

    private static string VisibleText(string text)
    {
        return string.Concat(TextGlyphs(text));
    }

    private static int TextWidth(string text)
    {
        return TextGlyphs(text).Count;
    }

    private static List<string> TextGlyphs(string text)
    {
        List<string> glyphs = new();
        System.Globalization.TextElementEnumerator enumerator =
            System.Globalization.StringInfo.GetTextElementEnumerator(text ?? "");
        while (enumerator.MoveNext())
        {
            glyphs.Add(enumerator.GetTextElement());
        }
        return glyphs;
    }

    private static void AppendGlyph(List<TerminalCell> row, string glyph, Style style)
    {
        row.Add(new TerminalCell
        {
            Text = glyph,
            Foreground = style.Color,
            Background = style.Background,
        });
    }

    private static void AppendText(List<TerminalCell> row, string text, Style style)
    {
        foreach (string glyph in TextGlyphs(text))
        {
            row.Add(new TerminalCell
            {
                Text = glyph,
                Foreground = style.Color,
                Background = style.Background,
            });
        }
    }
}

public sealed class Image : Element
{
    private const int ImageCellPixelWidth = 8;
    private const int ImageCellPixelHeight = 16;
    private const int FallbackVerticalSamplesPerCell = 2;
    private const int TestFallbackBlendDenominator = 255;
    private const int TestFallbackCheckerTilePixels = 4;
    private const int TestFallbackCheckerLightAlpha = 160;
    private const int TestFallbackCheckerDarkAlpha = 0;
    private const int TestFallbackColorQuantum = 32;
    private const string FallbackUpperHalfBlock = "▀";
    private const string DefaultImageFit = "contain";
    private const string DefaultImageRenderMode = "auto";
    private const string DefaultImageAlign = "center";
    private const string DefaultImageVerticalAlign = "middle";
    private const string MissingImagePlaceholder = "image";

    public string Source { get; private set; }
    public string Alt { get; private set; }
    public string Fit { get; private set; }
    public string RenderMode { get; private set; }
    public string Align { get; private set; }
    public string VerticalAlign { get; private set; }

    public Image(
        string name,
        string source = "",
        string alt = "",
        string fit = DefaultImageFit,
        string renderMode = DefaultImageRenderMode,
        string align = DefaultImageAlign,
        string verticalAlign = DefaultImageVerticalAlign) : base(name)
    {
        Source = source ?? "";
        Alt = alt ?? "";
        Fit = NormalizedMode(fit, DefaultImageFit);
        RenderMode = NormalizedMode(renderMode, DefaultImageRenderMode);
        Align = NormalizedMode(align, DefaultImageAlign);
        VerticalAlign = NormalizedMode(verticalAlign, DefaultImageVerticalAlign);
    }

    public void SetSource(string source)
    {
        Source = source ?? "";
    }

    public void SetAlt(string alt)
    {
        Alt = alt ?? "";
    }

    public void SetFit(string fit)
    {
        Fit = NormalizedMode(fit, DefaultImageFit);
    }

    public void SetRenderMode(string renderMode)
    {
        RenderMode = NormalizedMode(renderMode, DefaultImageRenderMode);
    }

    public void SetAlign(string align)
    {
        Align = NormalizedMode(align, DefaultImageAlign);
    }

    public void SetVerticalAlign(string verticalAlign)
    {
        VerticalAlign = NormalizedMode(verticalAlign, DefaultImageVerticalAlign);
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        int width = Math.Max(1, size.Width);
        int height = Math.Max(1, size.Height);
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        try
        {
            Raster raster = LoadRaster(ImagePath());
            return DeterministicImageContent(raster, width, height, style, RenderMode != "fallback");
        }
        catch
        {
            return PlaceholderContent(width, height, style);
        }
    }

    private List<List<TerminalCell>> DeterministicImageContent(
        Raster raster,
        int width,
        int height,
        Style style,
        bool checker)
    {
        List<List<TerminalCell>> content = BlankContent(width, height, style);
        (int cols, int rows, int colOffset, int rowOffset) =
            ImageCellRegion(width, height, raster.Width, raster.Height);
        string regionFit = Fit == "contain" ? "cover" : Fit;
        int sampleHeight = rows * FallbackVerticalSamplesPerCell;
        RgbSample background = LetterboxRgb(style);

        for (int row = rowOffset; row < rowOffset + rows; ++row)
        {
            if (row < 0 || row >= height)
            {
                continue;
            }
            for (int col = colOffset; col < colOffset + cols; ++col)
            {
                if (col < 0 || col >= width)
                {
                    continue;
                }
                int sampleCol = col - colOffset;
                int sampleTop = (row - rowOffset) * FallbackVerticalSamplesPerCell;
                content[row][col] = new TerminalCell
                {
                    Text = FallbackUpperHalfBlock,
                    Foreground = ColorFromRgb(TestFallbackSampleColor(
                        raster,
                        sampleCol,
                        sampleTop,
                        cols,
                        sampleHeight,
                        regionFit,
                        Align,
                        VerticalAlign,
                        background,
                        checker)),
                    Background = ColorFromRgb(TestFallbackSampleColor(
                        raster,
                        sampleCol,
                        sampleTop + 1,
                        cols,
                        sampleHeight,
                        regionFit,
                        Align,
                        VerticalAlign,
                        background,
                        checker)),
                    BackgroundFromImageSample = true,
                };
            }
        }

        return content;
    }

    private (int Cols, int Rows, int ColOffset, int RowOffset) ImageCellRegion(
        int width,
        int height,
        int sourceWidth,
        int sourceHeight)
    {
        if (Fit != "contain" || sourceWidth <= 0 || sourceHeight <= 0)
        {
            return (width, height, 0, 0);
        }
        double scale = Math.Min(
            width * ImageCellPixelWidth / (double)sourceWidth,
            height * ImageCellPixelHeight / (double)sourceHeight);
        int cols = Math.Max(1, Math.Min(width, RoundLikePython(sourceWidth * scale / ImageCellPixelWidth)));
        int rows = Math.Max(1, Math.Min(height, RoundLikePython(sourceHeight * scale / ImageCellPixelHeight)));
        int colOffset = AlignmentOffset(width, cols, Align, "left", "right");
        int rowOffset = AlignmentOffset(height, rows, VerticalAlign, "top", "bottom");
        return (cols, rows, colOffset, rowOffset);
    }

    private static RgbSample TestFallbackSampleColor(
        Raster raster,
        int x,
        int y,
        int targetWidth,
        int targetHeight,
        string fit,
        string align,
        string verticalAlign,
        RgbSample background,
        bool checker)
    {
        targetWidth = Math.Max(1, targetWidth);
        targetHeight = Math.Max(1, targetHeight);
        x = Math.Clamp(x, 0, targetWidth - 1);
        y = Math.Clamp(y, 0, targetHeight - 1);
        RgbSample baseColor = checker ? TestFallbackCheckerTint(background, x, y) : background;
        if (raster.Width <= 0 || raster.Height <= 0)
        {
            return QuantizeTestFallbackColor(baseColor);
        }

        int sourceX;
        int sourceY;
        fit = NormalizedMode(fit, DefaultImageFit);
        if (fit == "stretch")
        {
            sourceX = (int)(x * raster.Width / (double)targetWidth);
            sourceY = (int)(y * raster.Height / (double)targetHeight);
        }
        else
        {
            double scale = fit == "cover"
                ? Math.Max(targetWidth / (double)raster.Width, targetHeight / (double)raster.Height)
                : Math.Min(targetWidth / (double)raster.Width, targetHeight / (double)raster.Height);
            double drawnWidth = raster.Width * scale;
            double drawnHeight = raster.Height * scale;
            double xOffset = AlignmentOffsetFloat(targetWidth, drawnWidth, align, "left", "right");
            double yOffset = AlignmentOffsetFloat(targetHeight, drawnHeight, verticalAlign, "top", "bottom");
            double sourceXFloat = (x - xOffset) / scale;
            double sourceYFloat = (y - yOffset) / scale;
            if (sourceXFloat < 0.0 ||
                sourceYFloat < 0.0 ||
                sourceXFloat >= raster.Width ||
                sourceYFloat >= raster.Height)
            {
                return QuantizeTestFallbackColor(baseColor);
            }
            sourceX = (int)sourceXFloat;
            sourceY = (int)sourceYFloat;
        }

        sourceX = Math.Clamp(sourceX, 0, raster.Width - 1);
        sourceY = Math.Clamp(sourceY, 0, raster.Height - 1);
        RgbaSample pixel = raster.PixelAt(sourceX, sourceY);
        RgbSample color = BlendRgb(new RgbSample(pixel.Red, pixel.Green, pixel.Blue), baseColor, pixel.Alpha);
        if (checker)
        {
            color = TestFallbackCheckerTint(color, x, y);
        }
        return QuantizeTestFallbackColor(color);
    }

    private static bool TestFallbackCheckerLightSquare(int x, int y)
    {
        return ((x / TestFallbackCheckerTilePixels) + (y / TestFallbackCheckerTilePixels)) % 2 == 0;
    }

    private static RgbSample TestFallbackCheckerTint(RgbSample color, int x, int y)
    {
        int alpha = TestFallbackCheckerLightSquare(x, y)
            ? TestFallbackCheckerLightAlpha
            : TestFallbackCheckerDarkAlpha;
        return BlendRgb(new RgbSample(255, 255, 255), color, alpha);
    }

    private static RgbSample BlendRgb(RgbSample source, RgbSample destination, int alpha)
    {
        alpha = Math.Clamp(alpha, 0, TestFallbackBlendDenominator);
        return new RgbSample(
            (source.Red * alpha + destination.Red * (TestFallbackBlendDenominator - alpha)) /
                TestFallbackBlendDenominator,
            (source.Green * alpha + destination.Green * (TestFallbackBlendDenominator - alpha)) /
                TestFallbackBlendDenominator,
            (source.Blue * alpha + destination.Blue * (TestFallbackBlendDenominator - alpha)) /
                TestFallbackBlendDenominator);
    }

    private static RgbSample QuantizeTestFallbackColor(RgbSample color)
    {
        static int Quantize(int channel)
        {
            return Math.Clamp((channel / TestFallbackColorQuantum) * TestFallbackColorQuantum, 0, 255);
        }
        return new RgbSample(Quantize(color.Red), Quantize(color.Green), Quantize(color.Blue));
    }

    private RgbSample LetterboxRgb(Style style)
    {
        Color? background = style.Background;
        if (background is null || background.IsTransparent || !IsOpaque(background))
        {
            background = Element.RenderingParentBackground ?? background;
        }
        if (background?.Rgba is Rgba rgba)
        {
            return new RgbSample(rgba.Red, rgba.Green, rgba.Blue);
        }
        return new RgbSample(0, 0, 0);
    }

    private List<List<TerminalCell>> PlaceholderContent(int width, int height, Style style)
    {
        string text = string.IsNullOrEmpty(Alt)
            ? Path.GetFileName(Source)
            : Alt;
        if (string.IsNullOrEmpty(text))
        {
            text = MissingImagePlaceholder;
        }

        List<List<TerminalCell>> content = BlankContent(width, height, style);
        int row = height / 2;
        int col = Math.Max(0, (width - text.Length) / 2);
        for (int index = 0; index < text.Length && index < width; ++index)
        {
            int targetCol = col + index;
            if (targetCol >= width)
            {
                break;
            }
            content[row][targetCol] = new TerminalCell
            {
                Text = text[index].ToString(),
                Foreground = style.Color,
                Background = style.Background,
            };
        }
        return content;
    }

    private static List<List<TerminalCell>> BlankContent(int width, int height, Style style)
    {
        List<List<TerminalCell>> content = new();
        for (int row = 0; row < height; ++row)
        {
            List<TerminalCell> line = new();
            for (int col = 0; col < width; ++col)
            {
                line.Add(new TerminalCell
                {
                    Text = " ",
                    Foreground = style.Color,
                    Background = style.Background,
                });
            }
            content.Add(line);
        }
        return content;
    }

    private string ImagePath()
    {
        if (string.IsNullOrEmpty(Source) || Path.IsPathRooted(Source))
        {
            return Source;
        }
        return Path.GetFullPath(Source);
    }

    private static Raster LoadRaster(string path)
    {
        using FileStream stream = File.OpenRead(path);
        ImageResult image = ImageResult.FromStream(stream, ColorComponents.RedGreenBlueAlpha);
        if (image.Width <= 0 || image.Height <= 0 || image.Data.Length == 0)
        {
            throw new InvalidOperationException("Image could not be decoded.");
        }
        return new Raster(image.Width, image.Height, image.Data);
    }

    private static Color ColorFromRgb(RgbSample color)
    {
        return Color.Rgb((byte)color.Red, (byte)color.Green, (byte)color.Blue);
    }

    private static string NormalizedMode(string? text, string fallback)
    {
        string normalized = (text ?? "").Trim().ToLowerInvariant();
        return string.IsNullOrEmpty(normalized) ? fallback : normalized;
    }

    private static int AlignmentOffset(int outer, int inner, string value, string startValue, string endValue)
    {
        int span = Math.Max(0, outer - inner);
        value = NormalizedMode(value, "");
        if (value == startValue)
        {
            return 0;
        }
        if (value == endValue)
        {
            return span;
        }
        return span / 2;
    }

    private static double AlignmentOffsetFloat(
        double outer,
        double inner,
        string value,
        string startValue,
        string endValue)
    {
        double span = Math.Max(0.0, outer - inner);
        value = NormalizedMode(value, "");
        if (value == startValue)
        {
            return 0.0;
        }
        if (value == endValue)
        {
            return span;
        }
        return span / 2.0;
    }

    private static int RoundLikePython(double value)
    {
        return (int)Math.Round(value, MidpointRounding.ToEven);
    }

    private static bool IsOpaque(Color color)
    {
        return color.Rgba is Rgba rgba && rgba.Alpha >= byte.MaxValue;
    }

    private readonly record struct RgbSample(int Red, int Green, int Blue);

    private readonly record struct RgbaSample(int Red, int Green, int Blue, int Alpha);

    private sealed class Raster
    {
        public Raster(int width, int height, byte[] data)
        {
            Width = width;
            Height = height;
            Data = data;
        }

        public int Width { get; }
        public int Height { get; }
        private byte[] Data { get; }

        public RgbaSample PixelAt(int x, int y)
        {
            x = Math.Clamp(x, 0, Width - 1);
            y = Math.Clamp(y, 0, Height - 1);
            int offset = (y * Width + x) * 4;
            return new RgbaSample(
                Data[offset],
                Data[offset + 1],
                Data[offset + 2],
                Data[offset + 3]);
        }
    }
}

public sealed class Button : Element
{
    public string Title { get; private set; }

    public Button(string name, string title = "Button") : base(name)
    {
        Title = title;
    }

    public void SetTitle(string title)
    {
        Title = title;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        string align = string.IsNullOrEmpty(style.TextAlign) ? "center" : style.TextAlign;
        int width = Math.Max(1, size.Width);
        string body = Title;
        int titleWidth = Title.Length;
        if (state.Focused && width >= titleWidth + 2)
        {
            if (width >= titleWidth + 6)
            {
                int innerWidth = Math.Max(1, width - " [] ".Length);
                body = " [" + RenderHelpers.Align(" " + Title + " ", innerWidth, align)[..innerWidth] + "] ";
            }
            else if (width >= titleWidth + 4)
            {
                int innerWidth = Math.Max(1, width - "  []".Length);
                body = " [" + RenderHelpers.Align(Title, innerWidth, align)[..innerWidth] + "] ";
            }
            else
            {
                body = "[" + Title + "]";
            }
        }
        string text = RenderHelpers.Align(body, width, align);
        return RenderHelpers.RenderPlainText(text, width, 1, style);
    }
}

public sealed class CheckBox : Element
{
    public string Title { get; private set; }
    public bool Checked { get; private set; }

    public CheckBox(string name, string title = "", bool isChecked = false) : base(name)
    {
        Title = title;
        Checked = isChecked;
    }

    public void SetChecked(bool isChecked)
    {
        Checked = isChecked;
    }

    public override bool HandleKey(string key)
    {
        if (key != "Enter" && key != " ")
        {
            return false;
        }
        Checked = !Checked;
        return true;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        string indicator = Checked ? "[x]" : "[ ]";
        string text = string.IsNullOrEmpty(Title) ? indicator : indicator + " " + Title;
        int width = Math.Max(1, size.Width);
        Style baseStyle = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> rendered = RenderHelpers.RenderPlainText(text, width, 1, baseStyle);
        Style indicatorStyle = baseStyle.Clone();
        Color? inheritedBackground = indicatorStyle.Background;
        Style? stateStyle = Checked ? CheckedStyle : UncheckedStyle;
        if (stateStyle is not null)
        {
            indicatorStyle.Merge(stateStyle);
        }
        if ((state.Focused || state.EditMode) &&
            indicatorStyle.Background is not null &&
            indicatorStyle.Background.IsTransparent &&
            inheritedBackground is not null)
        {
            indicatorStyle.Background = inheritedBackground;
        }
        for (int index = 0; index < Math.Min(3, width); ++index)
        {
            rendered[0][index].Foreground = indicatorStyle.Color;
            rendered[0][index].Background = indicatorStyle.Background;
        }
        return rendered;
    }
}

public class TextInput : Element
{
    public string Value { get; private set; }
    public int Cursor { get; private set; }
    public int? SelectionStart { get; private set; }
    public int? SelectionEnd { get; private set; }
    public int MaxLength { get; }
    public bool Multiline { get; }

    public TextInput(string name, string value = "", int maxLength = 0, bool multiline = false) : base(name)
    {
        Value = value;
        MaxLength = maxLength;
        Multiline = multiline;
        Cursor = Value.Length;
    }

    public void SetValue(string value)
    {
        Value = value;
        if (MaxLength > 0 && Value.Length > MaxLength)
        {
            Value = Value[..MaxLength];
        }
        Cursor = Value.Length;
        ClearSelection();
    }

    public void SetCursor(int cursor)
    {
        Cursor = Math.Clamp(cursor, 0, Value.Length);
        ClearSelection();
    }

    public void SetSelection(int start, int end)
    {
        SelectionStart = Math.Clamp(start, 0, Value.Length);
        SelectionEnd = Math.Clamp(end, 0, Value.Length);
        Cursor = SelectionEnd.Value;
    }

    public void ClearSelection()
    {
        SelectionStart = null;
        SelectionEnd = null;
    }

    public string SelectionText()
    {
        if (!SelectionStart.HasValue || !SelectionEnd.HasValue)
        {
            return "";
        }
        int start = Math.Min(SelectionStart.Value, SelectionEnd.Value);
        int end = Math.Max(SelectionStart.Value, SelectionEnd.Value);
        return Value[start..end];
    }

    public void InsertText(string text)
    {
        text = text.Replace("\r\n", "\n").Replace('\r', '\n');
        if (!Multiline)
        {
            text = text.Replace("\n", "");
        }
        string prefix = Value[..Cursor];
        string suffix = Value[Cursor..];
        if (SelectionStart.HasValue && SelectionEnd.HasValue)
        {
            int start = Math.Min(SelectionStart.Value, SelectionEnd.Value);
            int end = Math.Max(SelectionStart.Value, SelectionEnd.Value);
            prefix = Value[..start];
            suffix = Value[end..];
            Cursor = start;
        }
        string next = prefix + text + suffix;
        if (MaxLength > 0 && next.Length > MaxLength)
        {
            next = next[..MaxLength];
        }
        Cursor = Math.Min(next.Length, Cursor + text.Length);
        Value = next;
        ClearSelection();
    }

    public override bool HandleKey(string key)
    {
        switch (key)
        {
            case "Left":
                SetCursor(Cursor - 1);
                return true;
            case "Right":
                SetCursor(Cursor + 1);
                return true;
            case "Home":
                SetCursor(0);
                return true;
            case "End":
                SetCursor(Value.Length);
                return true;
            case "Backspace":
                if (Cursor > 0)
                {
                    Value = Value.Remove(Cursor - 1, 1);
                    --Cursor;
                }
                return true;
            case "Delete":
                if (Cursor < Value.Length)
                {
                    Value = Value.Remove(Cursor, 1);
                }
                return true;
            case "Enter":
                if (Multiline)
                {
                    InsertText("\n");
                    return true;
                }
                return false;
            default:
                if (key.Length == 1)
                {
                    InsertText(key);
                    return true;
                }
                return false;
        }
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        int width = Math.Max(1, size.Width);
        int height = Math.Max(1, Multiline ? size.Height : 1);
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> rendered = RenderHelpers.RenderPlainText(Value, width, height, style);
        if (Multiline)
        {
            List<string> rows = RenderHelpers.WrapText(Value, width);
            if (rows.Count > height && rows[Math.Min(height - 1, rows.Count - 1)].Length < width)
            {
                rendered[height - 1][width - 1].Text = "v";
            }
        }
        if (!state.EditMode)
        {
            return rendered;
        }

        Style cursorStyle = style.Clone();
        if (CursorStyle is not null)
        {
            cursorStyle.Merge(CursorStyle);
        }
        ApplySelectionOrCursor(rendered, width, height, cursorStyle);
        return rendered;
    }

    private void ApplySelectionOrCursor(List<List<TerminalCell>> rendered, int width, int height, Style cursorStyle)
    {
        if (SelectionStart.HasValue && SelectionEnd.HasValue && SelectionStart.Value != SelectionEnd.Value)
        {
            int start = Math.Min(SelectionStart.Value, SelectionEnd.Value);
            int end = Math.Max(SelectionStart.Value, SelectionEnd.Value);
            for (int index = start; index < end; ++index)
            {
                (int row, int col) = VisualPosition(index, width);
                if (row >= 0 && row < height && col >= 0 && col < width)
                {
                    rendered[row][col].Foreground = cursorStyle.Color;
                    rendered[row][col].Background = cursorStyle.Background;
                }
            }
            return;
        }

        (int cursorRow, int cursorCol) = VisualPosition(Cursor, width);
        cursorRow = Math.Clamp(cursorRow, 0, height - 1);
        cursorCol = Math.Clamp(cursorCol, 0, width - 1);
        rendered[cursorRow][cursorCol].Foreground = cursorStyle.Color;
        rendered[cursorRow][cursorCol].Background = cursorStyle.Background;
    }

    private (int Row, int Col) VisualPosition(int rawIndex, int width)
    {
        rawIndex = Math.Clamp(rawIndex, 0, Value.Length);
        width = Math.Max(1, width);
        int row = 0;
        int col = 0;
        for (int index = 0; index < rawIndex; ++index)
        {
            char ch = Value[index];
            if (ch == '\r')
            {
                continue;
            }
            if (ch == '\n')
            {
                ++row;
                col = 0;
                continue;
            }
            ++col;
            if (col >= width)
            {
                ++row;
                col = 0;
            }
        }
        return (row, col);
    }
}

public sealed class TextArea : TextInput
{
    public TextArea(string name, string value = "", int maxLength = 0) : base(name, value, maxLength, true)
    {
        SetCommitMode(CommitModeLeave);
    }
}

public sealed class NumberInput : Element
{
    public double Value { get; private set; }
    public double StepSize { get; }
    private string editText = "";
    private int editCursor;
    private bool editing;

    public NumberInput(string name, double value = 0.0, double stepSize = 1.0) : base(name)
    {
        Value = value;
        StepSize = stepSize;
    }

    public void SetValue(double value)
    {
        Value = value;
        editText = "";
        editCursor = 0;
        editing = false;
    }

    public override bool HandleKey(string key)
    {
        if (key == "Up")
        {
            Value += StepSize;
            return true;
        }
        if (key == "Down")
        {
            Value -= StepSize;
            return true;
        }
        return false;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        if (state.EditMode)
        {
            EnsureEditText();
        }
        string text = state.EditMode ? editText : DisplayValue();
        int width = Math.Max(1, size.Width);
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> rendered = RenderHelpers.RenderPlainText(text, width, 1, style);
        if (state.EditMode)
        {
            Style cursorStyle = style.Clone();
            if (CursorStyle is not null)
            {
                cursorStyle.Merge(CursorStyle);
            }
            int cursorCol = Math.Clamp(editCursor, 0, Math.Max(0, rendered[0].Count - 1));
            rendered[0][cursorCol].Foreground = cursorStyle.Color;
            rendered[0][cursorCol].Background = cursorStyle.Background;
        }
        return rendered;
    }

    private string DisplayValue()
    {
        return Value.ToString("0.##########", System.Globalization.CultureInfo.InvariantCulture);
    }

    private void EnsureEditText()
    {
        if (editing)
        {
            return;
        }
        editText = DisplayValue();
        editCursor = editText.Length;
        editing = true;
    }
}

public sealed class ComboBox : Element
{
    public List<string> Options { get; }
    public int SelectedIndex { get; private set; }
    public bool MenuOpen { get; private set; }

    public ComboBox(string name, IEnumerable<string>? options = null) : base(name)
    {
        Options = options?.ToList() ?? new List<string>();
        SelectedIndex = Options.Count > 0 ? 0 : -1;
    }

    public string SelectedText => SelectedIndex >= 0 && SelectedIndex < Options.Count ? Options[SelectedIndex] : "";

    public void SetOptions(IEnumerable<string> options)
    {
        Options.Clear();
        Options.AddRange(options);
        SelectedIndex = Options.Count > 0 ? Math.Clamp(SelectedIndex, 0, Options.Count - 1) : -1;
    }

    public void SetSelectedIndex(int index)
    {
        if (Options.Count == 0)
        {
            SelectedIndex = -1;
            return;
        }
        SelectedIndex = Math.Clamp(index, 0, Options.Count - 1);
    }

    public void CloseMenu()
    {
        MenuOpen = false;
    }

    public override bool HandleKey(string key)
    {
        if (!MenuOpen)
        {
            if (key == "Enter")
            {
                MenuOpen = true;
                return true;
            }
            return false;
        }
        if (key == "Escape")
        {
            MenuOpen = false;
            return true;
        }
        if (key == "Up")
        {
            SetSelectedIndex(SelectedIndex - 1);
            return true;
        }
        if (key == "Down")
        {
            SetSelectedIndex(SelectedIndex + 1);
            return true;
        }
        if (key == "Enter")
        {
            MenuOpen = false;
            return true;
        }
        return false;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        int width = Math.Max(1, size.Width);
        int height = MenuOpen ? Options.Count + 1 : 1;
        Style baseStyle = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> rendered = new();

        string closed = FitText(SelectedText, width);
        if (width > 0 && (state.Focused || state.EditMode || MenuOpen))
        {
            char[] chars = closed.ToCharArray();
            chars[width - 1] = MenuOpen ? 'v' : '>';
            closed = new string(chars);
        }
        rendered.Add(RenderRow(closed, width, baseStyle));

        if (MenuOpen)
        {
            for (int row = 1; row < height; ++row)
            {
                int optionIndex = row - 1;
                bool selectedRow = optionIndex == SelectedIndex;
                string text = optionIndex < Options.Count ? "  " + Options[optionIndex] : "";
                Style rowStyle = baseStyle.Clone();
                if (selectedRow && SelectedStyle is not null)
                {
                    rowStyle.Merge(SelectedStyle);
                }
                rendered.Add(RenderRow(text, width, rowStyle));
            }
        }
        return rendered;
    }

    private static List<TerminalCell> RenderRow(string text, int width, Style style)
    {
        return RenderHelpers.RenderPlainText(text, width, 1, style)[0];
    }

    private static string FitText(string text, int width)
    {
        width = Math.Max(1, width);
        if (text.Length > width)
        {
            return text[..width];
        }
        return text.PadRight(width);
    }
}

public sealed class ListBox : Element
{
    public List<string> Options { get; }
    public int SelectedIndex { get; private set; }
    public bool Multiple { get; private set; }
    private readonly HashSet<string> selectedValues = new();
    private int scrollOffset;
    private int lastViewportHeight;

    public ListBox(string name, IEnumerable<string>? options = null) : base(name)
    {
        Options = options?.ToList() ?? new List<string>();
        SelectedIndex = Options.Count > 0 ? 0 : -1;
    }

    public IReadOnlyCollection<string> SelectedValues => selectedValues.Count > 0 ? selectedValues : SelectedIndex >= 0 && SelectedIndex < Options.Count ? new[] { Options[SelectedIndex] } : Array.Empty<string>();

    public void SetOptions(IEnumerable<string> options)
    {
        Options.Clear();
        Options.AddRange(options);
        selectedValues.Clear();
        SelectedIndex = Options.Count > 0 ? Math.Clamp(SelectedIndex, 0, Options.Count - 1) : -1;
        EnsureSelectedVisible();
    }

    public void SetMultiple(bool multiple)
    {
        Multiple = multiple;
    }

    public void SetSelectedIndex(int index)
    {
        if (Options.Count == 0)
        {
            SelectedIndex = -1;
            return;
        }
        SelectedIndex = Math.Clamp(index, 0, Options.Count - 1);
        EnsureSelectedVisible();
    }

    public void SetSelectedValues(IEnumerable<string> values)
    {
        selectedValues.Clear();
        foreach (string value in values)
        {
            if (Options.Contains(value))
            {
                selectedValues.Add(value);
            }
        }
        if (selectedValues.Count == 1)
        {
            SelectedIndex = Options.IndexOf(selectedValues.First());
            EnsureSelectedVisible();
        }
    }

    public bool ScrollLines(int delta)
    {
        int viewportHeight = lastViewportHeight > 0 ? lastViewportHeight : Math.Max(1, Frame.Height);
        int maxOffset = Math.Max(0, Options.Count - viewportHeight);
        int next = Math.Clamp(scrollOffset - delta, 0, maxOffset);
        if (next == scrollOffset)
        {
            return false;
        }
        scrollOffset = next;
        return true;
    }

    public override bool HandleKey(string key)
    {
        if (key == "Up")
        {
            SetSelectedIndex(SelectedIndex - 1);
            return true;
        }
        if (key == "Down")
        {
            SetSelectedIndex(SelectedIndex + 1);
            return true;
        }
        if (key == " " && Multiple && SelectedIndex >= 0 && SelectedIndex < Options.Count)
        {
            string value = Options[SelectedIndex];
            if (!selectedValues.Add(value))
            {
                selectedValues.Remove(value);
            }
            return true;
        }
        return false;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> rows = new();
        int height = Math.Max(1, size.Height);
        int width = Math.Max(1, size.Width);
        lastViewportHeight = height;
        int maxOffset = Math.Max(0, Options.Count - height);
        scrollOffset = Math.Clamp(scrollOffset, 0, maxOffset);
        bool hasAbove = scrollOffset > 0;
        bool hasBelow = scrollOffset + height < Options.Count;
        for (int row = 0; row < height; ++row)
        {
            int optionIndex = scrollOffset + row;
            string text = optionIndex < Options.Count ? Options[optionIndex] : "";
            bool selected = optionIndex < Options.Count && (selectedValues.Count > 0 ? selectedValues.Contains(Options[optionIndex]) : optionIndex == SelectedIndex);
            Style rowStyle = style.Clone();
            if (selected && SelectedStyle is not null)
            {
                rowStyle.Merge(SelectedStyle);
            }
            List<TerminalCell> rendered = RenderHelpers.RenderPlainText(text, width, 1, rowStyle)[0];
            if (row == 0 && hasAbove && width > 0)
            {
                rendered[width - 1].Text = "^";
            }
            else if (row == height - 1 && hasBelow && width > 0)
            {
                rendered[width - 1].Text = "v";
            }
            rows.Add(rendered);
        }
        return rows;
    }

    private void EnsureSelectedVisible()
    {
        if (lastViewportHeight <= 0 || SelectedIndex < 0)
        {
            return;
        }
        if (SelectedIndex < scrollOffset)
        {
            scrollOffset = SelectedIndex;
        }
        else if (SelectedIndex >= scrollOffset + lastViewportHeight)
        {
            scrollOffset = SelectedIndex - lastViewportHeight + 1;
        }
        scrollOffset = Math.Clamp(scrollOffset, 0, Math.Max(0, Options.Count - lastViewportHeight));
    }
}

public class ScrollView : Element
{
    public int Gap { get; private set; }
    public bool AutoScroll { get; private set; }
    public int ScrollOffset { get; private set; }
    public Style? DescendantFocusStyle { get; private set; }
    private readonly List<List<List<TerminalCell>>> renderedChildren = new();
    private Func<int, List<List<List<TerminalCell>>>>? dynamicChildrenRenderer;
    private int? dynamicChildrenWidth;

    public ScrollView(string name, int gap = 0) : base(name)
    {
        Gap = gap;
    }

    public void SetGap(int gap)
    {
        Gap = Math.Max(0, gap);
    }

    public void SetAutoScroll(bool autoScroll)
    {
        AutoScroll = autoScroll;
    }

    public void SetDescendantFocusStyle(Style style)
    {
        DescendantFocusStyle = style;
    }

    public void ClearChildren()
    {
        Children.Clear();
        renderedChildren.Clear();
        dynamicChildrenWidth = null;
        ScrollOffset = 0;
    }

    public T AddChild<T>(T child) where T : Element
    {
        Children.Add(child);
        dynamicChildrenWidth = null;
        return child;
    }

    public void AddChild(List<List<TerminalCell>> child)
    {
        renderedChildren.Add(child);
        dynamicChildrenWidth = null;
    }

    public void SetDynamicChildrenRenderer(Func<int, List<List<List<TerminalCell>>>> renderer)
    {
        dynamicChildrenRenderer = renderer;
        InvalidateDynamicChildren();
    }

    public void InvalidateDynamicChildren()
    {
        dynamicChildrenWidth = null;
    }

    public bool ScrollBy(int delta, Size viewport)
    {
        Style style = EffectiveStyle();
        Size paddedViewport = PaddedViewportSize(viewport, style);
        int max = Math.Max(0, ContentHeight(Math.Max(1, paddedViewport.Width)) - Math.Max(1, paddedViewport.Height));
        int next = Math.Clamp(ScrollOffset + delta, 0, max);
        bool changed = next != ScrollOffset;
        ScrollOffset = next;
        return changed;
    }

    public int ContentHeight(int width)
    {
        int renderedHeight = DynamicRenderedChildren(Math.Max(1, width)).Sum(child => Math.Max(1, child.Count));
        int elementHeight = Children.Sum(child => Math.Max(1, child.Frame.Height));
        int childCount = renderedChildren.Count + Children.Count;
        if (childCount == 0)
        {
            return 0;
        }
        return renderedHeight + elementHeight + Math.Max(0, childCount - 1) * Gap;
    }

    public int ContentHeight()
    {
        return ContentHeight(Math.Max(1, Frame.Width));
    }

    public override bool HandleKey(string key)
    {
        return key switch
        {
            "Up" => ScrollBy(-1, new Size(Frame.Width, Frame.Height)),
            "Down" => ScrollBy(1, new Size(Frame.Width, Frame.Height)),
            _ => false,
        };
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> canvas = RenderHelpers.RenderPlainText("", size.Width, size.Height, style);
        int paddingTop = ConstrainedPaddingTop(size, style);
        int paddingLeft = ConstrainedPaddingLeft(size, style);
        Size viewport = PaddedViewportSize(size, style);
        List<List<List<TerminalCell>>> dynamicChildren = DynamicRenderedChildren(Math.Max(1, viewport.Width));
        if (AutoScroll)
        {
            ScrollOffset = Math.Max(0, ContentHeight(Math.Max(1, viewport.Width)) - Math.Max(1, viewport.Height));
        }
        int cursor = paddingTop - ScrollOffset;
        foreach (List<List<TerminalCell>> rendered in dynamicChildren)
        {
            for (int row = 0; row < rendered.Count; ++row)
            {
                int targetRow = cursor + row;
                if (targetRow < paddingTop || targetRow >= paddingTop + viewport.Height || targetRow >= canvas.Count)
                {
                    continue;
                }
                for (int col = 0; col < Math.Min(viewport.Width, rendered[row].Count); ++col)
                {
                    int targetCol = paddingLeft + col;
                    if (targetCol < 0 || targetCol >= canvas[targetRow].Count)
                    {
                        continue;
                    }
                    canvas[targetRow][targetCol] = rendered[row][col].Clone();
                }
            }
            cursor += Math.Max(1, rendered.Count) + Gap;
        }
        (ComboBox ComboBox, int RowStart, int ColStart)? openComboBoxOverlay = null;
        foreach (Element child in Children)
        {
            int childTop = cursor;
            int childHeight = Math.Max(1, child.Frame.Height);
            bool childHasFocusedDescendant = state.FocusedElement is not null && ElementTreeContains(child, state.FocusedElement);
            bool applyViewportFocusBackground = state.Focused || childHasFocusedDescendant;
            List<List<TerminalCell>> rendered = child.Render(
                new Size(viewport.Width, childHeight),
                new ElementRenderState
                {
                    FocusedElement = state.FocusedElement,
                    EditMode = state.EditMode,
                });
            if (childHasFocusedDescendant)
            {
                ApplyFocusedDescendantBackground(rendered, child, state.Focused);
            }
            for (int row = 0; row < rendered.Count; ++row)
            {
                int targetRow = cursor + row;
                if (targetRow < paddingTop || targetRow >= paddingTop + viewport.Height || targetRow >= canvas.Count)
                {
                    continue;
                }
                for (int col = 0; col < Math.Min(viewport.Width, rendered[row].Count); ++col)
                {
                    int targetCol = paddingLeft + col;
                    if (targetCol < 0 || targetCol >= canvas[targetRow].Count)
                    {
                        continue;
                    }
                    canvas[targetRow][targetCol] = rendered[row][col].Clone();
                }
                if (applyViewportFocusBackground)
                {
                    ApplyFocusedDescendantBackground(canvas[targetRow], child, state.Focused || childHasFocusedDescendant);
                }
            }
            if (state.FocusedElement is ComboBox comboBox &&
                comboBox.MenuOpen &&
                ElementTreeContains(child, comboBox))
            {
                openComboBoxOverlay = (comboBox, childTop + comboBox.Frame.Row, paddingLeft + comboBox.Frame.Col);
            }
            int childFrameRow = Frame.Row + childTop;
            int childFrameCol = Frame.Col + paddingLeft;
            child.Frame = new Rect(childFrameRow, childFrameCol, Math.Max(1, viewport.Width), childHeight);
            if (child is ReusableElement reusable && reusable.Child is not null)
            {
                GeneratedWindowRuntime.OffsetWindowElementFrames(reusable.Child, childFrameRow, childFrameCol);
            }
            cursor += childHeight + Gap;
        }
        if (openComboBoxOverlay.HasValue)
        {
            OverlayOpenComboBox(
                canvas,
                openComboBoxOverlay.Value.ComboBox,
                openComboBoxOverlay.Value.RowStart,
                openComboBoxOverlay.Value.ColStart,
                paddingTop,
                paddingLeft,
                viewport);
            if (state.Focused)
            {
                int top = Math.Max(0, paddingTop);
                int bottom = Math.Min(canvas.Count, paddingTop + Math.Max(1, viewport.Height));
                for (int row = top; row < bottom; ++row)
                {
                    ApplyFocusedDescendantBackground(canvas[row], this, true);
                }
            }
        }
        int maxOffset = Math.Max(0, ContentHeight(Math.Max(1, viewport.Width)) - Math.Max(1, viewport.Height));
        int indicatorCol = Math.Min(canvas[0].Count, paddingLeft + Math.Max(1, viewport.Width)) - 1;
        int topIndicatorRow = Math.Clamp(paddingTop, 0, Math.Max(0, canvas.Count - 1));
        int bottomIndicatorRow = Math.Clamp(paddingTop + Math.Max(1, viewport.Height) - 1, 0, Math.Max(0, canvas.Count - 1));
        if (ScrollOffset > 0 && canvas.Count > 0 && canvas[topIndicatorRow].Count > 0)
        {
            ApplyScrollIndicator(canvas[topIndicatorRow], "^", indicatorCol);
        }
        if (ScrollOffset < maxOffset && canvas.Count > 0 && canvas[bottomIndicatorRow].Count > 0)
        {
            ApplyScrollIndicator(canvas[bottomIndicatorRow], "v", indicatorCol);
        }
        bool viewportFocused =
            state.Focused ||
            Children.Any(child => state.FocusedElement is not null && ElementTreeContains(child, state.FocusedElement));
        if (viewportFocused)
        {
            int top = 0;
            int bottom = canvas.Count;
            for (int row = top; row < bottom; ++row)
            {
                ApplyFocusedDescendantBackground(canvas[row], this, true);
            }
        }
        return canvas;
    }

    private void ApplyFocusedDescendantBackground(List<List<TerminalCell>> rendered, Element child, bool includeBlankRows)
    {
        foreach (List<TerminalCell> row in rendered)
        {
            ApplyFocusedDescendantBackground(row, child, includeBlankRows);
        }
    }

    private void ApplyFocusedDescendantBackground(List<TerminalCell> row, Element child, bool includeBlankRows)
    {
        Color? focusBackground = FocusStyle?.Background;
        if (focusBackground is null || (!includeBlankRows && !RowHasRenderedContent(row)))
        {
            return;
        }
        Color? fallbackBackground = child.Style.Background ?? Style.Background;
        Color? focusedFallbackBackground = fallbackBackground is null
            ? null
            : focusBackground.BlendOver(fallbackBackground);
        for (int col = 0; col < row.Count; ++col)
        {
            TerminalCell cell = row[col];
            Color? background = cell.Background ?? fallbackBackground;
            if (background is null || background.IsTransparent)
            {
                continue;
            }
            if (background.Rgba is Rgba rgba && rgba.Alpha < byte.MaxValue)
            {
                continue;
            }
            if (fallbackBackground is not null &&
                !background.Equals(fallbackBackground) &&
                !background.Equals(focusedFallbackBackground))
            {
                continue;
            }
            cell.Background = focusedFallbackBackground ?? focusBackground.BlendOver(background);
        }
    }

    private static bool RowHasRenderedContent(List<TerminalCell> row)
    {
        return row.Any(cell => !string.IsNullOrEmpty(cell.Text) && cell.Text != " ");
    }

    private static void OverlayOpenComboBox(
        List<List<TerminalCell>> canvas,
        ComboBox comboBox,
        int rowStart,
        int colStart,
        int paddingTop,
        int paddingLeft,
        Size viewport)
    {
        int width = Math.Max(1, comboBox.Frame.Width);
        List<List<TerminalCell>> rendered = comboBox.Render(
            new Size(width, Math.Max(1, comboBox.Options.Count + 1)),
            new ElementRenderState
            {
                Focused = true,
                EditMode = true,
            });
        int viewportTop = paddingTop;
        int viewportBottom = paddingTop + Math.Max(1, viewport.Height);
        int viewportLeft = paddingLeft;
        int viewportRight = paddingLeft + Math.Max(1, viewport.Width);
        for (int row = 0; row < rendered.Count; ++row)
        {
            int targetRow = rowStart + row;
            if (targetRow < viewportTop || targetRow >= viewportBottom || targetRow < 0 || targetRow >= canvas.Count)
            {
                continue;
            }
            for (int col = 0; col < rendered[row].Count; ++col)
            {
                int targetCol = colStart + col;
                if (targetCol < viewportLeft || targetCol >= viewportRight ||
                    targetCol < 0 || targetCol >= canvas[targetRow].Count)
                {
                    continue;
                }
                canvas[targetRow][targetCol] = rendered[row][col].Clone();
            }
        }
    }

    private static bool ElementTreeContains(Element root, Element target)
    {
        if (ReferenceEquals(root, target))
        {
            return true;
        }
        if (root is ReusableElement reusable && reusable.Child is not null)
        {
            foreach (Element element in reusable.Child.Elements)
            {
                if (ElementTreeContains(element, target))
                {
                    return true;
                }
            }
        }
        foreach (Element child in root.Children)
        {
            if (ElementTreeContains(child, target))
            {
                return true;
            }
        }
        return false;
    }

    private static Size PaddedViewportSize(Size size, Style style)
    {
        int paddingTop = ConstrainedPaddingTop(size, style);
        int paddingBottom = ConstrainedPaddingBottom(size, style, paddingTop);
        int paddingLeft = ConstrainedPaddingLeft(size, style);
        int paddingRight = ConstrainedPaddingRight(size, style, paddingLeft);
        return new Size(
            Math.Max(1, size.Width - paddingLeft - paddingRight),
            Math.Max(1, size.Height - paddingTop - paddingBottom));
    }

    private static int StylePaddingTop(Style style)
    {
        return Math.Max(0, style.PaddingTop ?? style.Padding ?? 0);
    }

    private static int StylePaddingRight(Style style)
    {
        return Math.Max(0, style.PaddingRight ?? style.Padding ?? 0);
    }

    private static int StylePaddingBottom(Style style)
    {
        return Math.Max(0, style.PaddingBottom ?? style.Padding ?? 0);
    }

    private static int StylePaddingLeft(Style style)
    {
        return Math.Max(0, style.PaddingLeft ?? style.Padding ?? 0);
    }

    private static int ConstrainedPaddingTop(Size size, Style style)
    {
        return Math.Min(StylePaddingTop(style), Math.Max(0, size.Height - 1));
    }

    private static int ConstrainedPaddingBottom(Size size, Style style, int paddingTop)
    {
        return Math.Min(StylePaddingBottom(style), Math.Max(0, size.Height - paddingTop - 1));
    }

    private static int ConstrainedPaddingLeft(Size size, Style style)
    {
        return Math.Min(StylePaddingLeft(style), Math.Max(0, size.Width - 1));
    }

    private static int ConstrainedPaddingRight(Size size, Style style, int paddingLeft)
    {
        return Math.Min(StylePaddingRight(style), Math.Max(0, size.Width - paddingLeft - 1));
    }

    private static void ApplyScrollIndicator(List<TerminalCell> row, string indicator, int col)
    {
        if (row.Count == 0)
        {
            return;
        }
        col = Math.Clamp(col, 0, row.Count - 1);
        TerminalCell cell = row[col];
        cell.Text = indicator;
        if (cell.Foreground is null || cell.Foreground.IsTransparent)
        {
            cell.Foreground = InferScrollIndicatorForeground(row, col);
        }
    }

    private static Color? InferScrollIndicatorForeground(List<TerminalCell> row, int col)
    {
        Color? foreground = null;
        for (int index = 0; index <= col && index < row.Count; ++index)
        {
            if (row[index].Foreground is not null && !row[index].Foreground!.IsTransparent)
            {
                foreground = row[index].Foreground;
            }
        }
        return foreground;
    }

    private List<List<List<TerminalCell>>> DynamicRenderedChildren(int width)
    {
        if (dynamicChildrenRenderer is null)
        {
            return renderedChildren;
        }
        if (dynamicChildrenWidth.HasValue && dynamicChildrenWidth.Value == width)
        {
            return renderedChildren;
        }
        renderedChildren.Clear();
        renderedChildren.AddRange(dynamicChildrenRenderer(width));
        dynamicChildrenWidth = width;
        return renderedChildren;
    }
}

public class ReusableElement : Element
{
    public GeneratedWindowBase? Child { get; private set; }

    public ReusableElement(string name) : base(name)
    {
    }

    public void SetChild(GeneratedWindowBase child)
    {
        Child = child;
        Child.Mode = "fullscreen";
    }

    public override bool HandleKey(string key)
    {
        if (Child is GeneratedScrollViewBase scrollView)
        {
            return scrollView.ScrollView().HandleKey(key);
        }
        return false;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        if (Child is null)
        {
            return base.Render(size, state);
        }
        int focusedIndex = -1;
        if (state?.FocusedElement is not null)
        {
            List<Element> focusable = GeneratedWindowRuntime.FocusableElements(Child);
            focusedIndex = focusable.IndexOf(state.FocusedElement);
        }
        return GeneratedWindowRuntime.RenderGeneratedWindowContent(
            Child,
            size,
            focusedIndex,
            state?.FocusedElement,
            focusedIndex >= 0 && state?.EditMode == true);
    }
}

public sealed class ViewHost : ReusableElement
{
    public ViewHost(string name) : base(name)
    {
    }
}
