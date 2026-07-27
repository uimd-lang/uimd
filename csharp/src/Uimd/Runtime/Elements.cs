using System.Text;
using System.Runtime.InteropServices;
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
    public Style? ActiveStyle { get; private set; }
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

    public void SetActiveStyle(Style style)
    {
        ActiveStyle = style;
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
    private int? selectionStart;
    private int? selectionEnd;

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

    public void SelectRange(int start, int end)
    {
        int textLength = Text.Length;
        selectionStart = Math.Clamp(start, 0, textLength);
        selectionEnd = Math.Clamp(end, 0, textLength);
    }

    public void ClearSelection()
    {
        selectionStart = null;
        selectionEnd = null;
    }

    public bool HasSelection()
    {
        return selectionStart.HasValue &&
            selectionEnd.HasValue &&
            selectionStart.Value != selectionEnd.Value;
    }

    public string SelectedText()
    {
        if (!HasSelection())
        {
            return "";
        }
        int low = Math.Min(selectionStart!.Value, selectionEnd!.Value);
        int high = Math.Max(selectionStart.Value, selectionEnd.Value);
        int from = Math.Clamp(low, 0, Text.Length);
        int to = Math.Clamp(high, 0, Text.Length);
        return Text.Substring(from, to - from);
    }

    public int TextPositionFromPoint(int localRow, int localCol, Size size)
    {
        int width = Math.Max(1, size.Width > 0 ? size.Width : Text.Length);
        int textLength = Text.Length;
        Style style = EffectiveStyle(false, false);
        if (size.Height == 1)
        {
            int newline = Text.IndexOf('\n', StringComparison.Ordinal);
            int segmentEnd = newline < 0 ? Text.Length : newline;
            List<RenderHelpers.VisualGlyph> glyphs = RenderHelpers.VisualGlyphs(Text[..segmentEnd], 0, 0);
            RenderHelpers.LabelVisualRow visible = new(
                glyphs.Count == 0 ? 0 : glyphs[0].SourceStart,
                glyphs.Count == 0 ? 0 : glyphs[^1].SourceEnd,
                glyphs);
            int offset = style.TextAlign switch
            {
                "center" => Math.Max(0, (width - RenderHelpers.VisualWidthForLabelRow(visible)) / 2),
                "right" => Math.Max(0, width - RenderHelpers.VisualWidthForLabelRow(visible)),
                _ => 0,
            };
            int raw = RenderHelpers.RawIndexForLabelVisualColumn(visible, localCol - offset);
            return Math.Clamp(raw, 0, textLength);
        }

        List<RenderHelpers.LabelVisualRow> rows = RenderHelpers.BuildLabelVisualRows(Text, width);
        if (rows.Count == 0 || localRow < 0)
        {
            return 0;
        }
        if (localRow >= rows.Count)
        {
            return textLength;
        }
        RenderHelpers.LabelVisualRow row = rows[localRow];
        int rowOffset = style.TextAlign switch
        {
            "center" => Math.Max(0, (width - RenderHelpers.VisualWidthForLabelRow(row)) / 2),
            "right" => Math.Max(0, width - RenderHelpers.VisualWidthForLabelRow(row)),
            _ => 0,
        };
        int rowRaw = RenderHelpers.RawIndexForLabelVisualColumn(row, localCol - rowOffset);
        return Math.Min(textLength, rowRaw);
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        bool activeSelection = HasSelection();
        int selectionLow = activeSelection ? Math.Min(selectionStart!.Value, selectionEnd!.Value) : 0;
        int selectionHigh = activeSelection ? Math.Max(selectionStart!.Value, selectionEnd!.Value) : 0;
        if (spans.Count == 0 && !activeSelection)
        {
            return RenderHelpers.RenderPlainText(Text, size.Width, size.Height, style);
        }
        Style cursorStyle = style.Clone();
        if (CursorStyle is not null)
        {
            cursorStyle.Merge(CursorStyle);
        }
        int width = Math.Max(1, size.Width > 0 ? size.Width : Text.Length);
        int height = Math.Max(1, size.Height);
        List<List<TerminalCell>> rendered = new();
        List<TerminalCell> row = new(width);
        bool clippingLine = false;
        IEnumerable<(char Ch, Color? Foreground, Color? Background)> styledChars = StyledChars();
        int sourceIndex = 0;
        foreach ((char ch, Color? foreground, Color? background) in styledChars)
        {
            if (ch == '\r')
            {
                ++sourceIndex;
                continue;
            }
            if (ch == '\n')
            {
                AppendBlankCells(row, width, style);
                rendered.Add(row);
                ++sourceIndex;
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
                bool selected = activeSelection && sourceIndex >= selectionLow && sourceIndex < selectionHigh;
                row.Add(StyledSpanCell(ch, selected ? cursorStyle : style, foreground, background));
            }
            ++sourceIndex;
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

    private IEnumerable<(char Ch, Color? Foreground, Color? Background)> StyledChars()
    {
        if (spans.Count == 0)
        {
            foreach (char ch in Text)
            {
                yield return (ch, null, null);
            }
            yield break;
        }
        foreach (LabelSpan span in spans)
        {
            Color? foreground = string.IsNullOrEmpty(span.Foreground) ? null : new Color(span.Foreground);
            Color? background = string.IsNullOrEmpty(span.Background) ? null : new Color(span.Background);
            foreach (char ch in span.Text)
            {
                yield return (ch, foreground, background);
            }
        }
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

public sealed class ImageRenderInfo
{
    public string Source { get; set; } = "";
    public string Fit { get; set; } = "";
    public string ConfiguredRenderMode { get; set; } = "";
    public string ResolvedRenderMode { get; set; } = "";
    public bool SourceLoaded { get; set; }
    public int SourceWidth { get; set; }
    public int SourceHeight { get; set; }
    public int ElementWidth { get; set; }
    public int ElementHeight { get; set; }
    public int CellPixelWidth { get; set; }
    public int CellPixelHeight { get; set; }
    public int ImageLeft { get; set; }
    public int ImageTop { get; set; }
    public int ImageWidth { get; set; }
    public int ImageHeight { get; set; }
    public int VisibleLeft { get; set; }
    public int VisibleTop { get; set; }
    public int VisibleWidth { get; set; }
    public int VisibleHeight { get; set; }
    public bool RawExpected { get; set; }
    public bool RawPresent { get; set; }
    public List<string> SampleSignature { get; } = new();
}

public sealed class Image : Element
{
    private const int DefaultImageCellPixelWidth = 8;
    private const int DefaultImageCellPixelHeight = 16;
    private const int FallbackVerticalSamplesPerCell = 2;
    private const int SixelBitsPerGlyph = 6;
    private const int SixelColorComponentScale = 100;
    private const int SixelColorLevels = 6;
    private const int SixelMaxColors = 256;
    private const int SixelFalseStatusMask = 0x1000;
    private const int SixelPixelFormatRgb888 = 0x03;
    private const int SixelLargeAuto = 0x0;
    private const int SixelRepAuto = 0x0;
    private const int SixelQualityHigh = 0x1;
    private const int TestFallbackBlendDenominator = 255;
    private const int TestFallbackCheckerTilePixels = 4;
    private const int TestFallbackCheckerLightAlpha = 160;
    private const int TestFallbackCheckerDarkAlpha = 0;
    private const int TestFallbackColorQuantum = 32;
    private const int ImageInfoSampleGridSize = 3;
    private const int ImageInfoColorQuantum = 64;
    private const string FallbackUpperHalfBlock = "▀";
    private const string FallbackFullBlock = "█";
    private const string DeterministicImageFallbackEnv = "UIMD_DETERMINISTIC_IMAGE_FALLBACK";
    private const string ForceSixelEnv = "UIMD_FORCE_SIXEL";
    private const string DisableSixelEnv = "UIMD_DISABLE_SIXEL";
    private const string DefaultImageFit = "contain";
    private const string DefaultImageRenderMode = "auto";
    private const string DefaultImageAlign = "center";
    private const string DefaultImageVerticalAlign = "middle";
    private const string MissingImagePlaceholder = "image";

    [ThreadStatic]
    private static int forceCellBackgroundRenderingDepth;

    private static readonly object RasterCacheLock = new();
    private static readonly Dictionary<string, Raster> RasterCache = new();
    private static readonly object SixelPayloadCacheLock = new();
    private static readonly Dictionary<ImageRenderCacheKey, string> SixelPayloadCache = new();

    private static Size? terminalCellPixelOverride;

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
            string imagePath = ImagePath();
            Raster raster = LoadRaster(imagePath);
            if (raster.Width <= 0 || raster.Height <= 0)
            {
                return PlaceholderContent(width, height, style);
            }
            if (forceCellBackgroundRenderingDepth > 0)
            {
                return CellBackgroundImageContent(raster, width, height, style);
            }
            if (DeterministicImageFallbackEnabled())
            {
                return DeterministicImageContent(raster, width, height, style, RenderMode != "fallback");
            }
            if (ShouldRenderSixel())
            {
                return SixelImageContent(imagePath, raster, width, height, style, state);
            }
            return FallbackImageContent(raster, width, height, style);
        }
        catch
        {
            return PlaceholderContent(width, height, style);
        }
    }

    internal static IDisposable ForceCellBackgroundRendering()
    {
        forceCellBackgroundRenderingDepth += 1;
        return new ForceCellBackgroundRenderingScope();
    }

    internal static void SetTerminalCellPixels(Size size)
    {
        if (size.Width > 0 && size.Height > 0)
        {
            terminalCellPixelOverride = size;
        }
    }

    public ImageRenderInfo RenderInfo(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        int width = Math.Max(1, size.Width);
        int height = Math.Max(1, size.Height);
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        Size cellPixels = TerminalCellPixels();
        ImageRenderInfo info = new()
        {
            Source = Source,
            Fit = Fit,
            ConfiguredRenderMode = RenderMode,
            ResolvedRenderMode = ResolvedRenderMode(),
            ElementWidth = width,
            ElementHeight = height,
            CellPixelWidth = cellPixels.Width,
            CellPixelHeight = cellPixels.Height,
        };

        Raster raster = LoadRaster(ImagePath());
        if (raster.Width <= 0 || raster.Height <= 0)
        {
            info.ResolvedRenderMode = "placeholder";
            return info;
        }

        (int cols, int rows, int colOffset, int rowOffset) =
            ImageCellRegion(width, height, raster.Width, raster.Height);
        int visibleTop = rowOffset;
        int visibleBottom = rowOffset + rows;
        if (state.ClipTop.HasValue || state.ClipBottom.HasValue)
        {
            visibleTop = Math.Max(visibleTop, state.ClipTop ?? 0);
            visibleBottom = Math.Min(visibleBottom, state.ClipBottom ?? height);
        }
        int visibleRows = Math.Max(0, visibleBottom - visibleTop);
        string regionFit = Fit == "contain" ? "cover" : Fit;
        int signatureRows = info.ResolvedRenderMode == "sixel" ? Math.Max(1, visibleRows) : rows;

        info.SourceLoaded = true;
        info.SourceWidth = raster.Width;
        info.SourceHeight = raster.Height;
        info.ImageLeft = colOffset;
        info.ImageTop = rowOffset;
        info.ImageWidth = cols;
        info.ImageHeight = rows;
        info.VisibleLeft = colOffset;
        info.VisibleTop = visibleTop;
        info.VisibleWidth = visibleRows > 0 ? cols : 0;
        info.VisibleHeight = visibleRows;
        info.RawExpected = info.ResolvedRenderMode == "sixel" && visibleRows > 0;
        info.RawPresent = info.RawExpected;
        info.SampleSignature.AddRange(ImageInfoSampleSignature(
            raster,
            cols,
            Math.Max(1, signatureRows),
            regionFit,
            Align,
            VerticalAlign,
            LetterboxRgb(style)));
        return info;
    }

    private string ResolvedRenderMode()
    {
        if (forceCellBackgroundRenderingDepth > 0)
        {
            return "cell_background";
        }
        if (DeterministicImageFallbackEnabled())
        {
            return "deterministic";
        }
        return ShouldRenderSixel() ? "sixel" : "fallback";
    }

    private static Size TerminalCellPixels()
    {
        return terminalCellPixelOverride ?? new Size(DefaultImageCellPixelWidth, DefaultImageCellPixelHeight);
    }

    private static bool DeterministicImageFallbackEnabled()
    {
        string value = (Environment.GetEnvironmentVariable(DeterministicImageFallbackEnv) ?? "").Trim().ToLowerInvariant();
        return value is "1" or "true" or "yes" or "on";
    }

    private static bool TruthyEnv(string name)
    {
        string value = (Environment.GetEnvironmentVariable(name) ?? "").Trim().ToLowerInvariant();
        return value is "1" or "true" or "yes" or "on";
    }

    private static bool SixelDisabled()
    {
        return TruthyEnv(DisableSixelEnv);
    }

    private static bool TerminalSupportsSixel()
    {
        if (TruthyEnv(ForceSixelEnv))
        {
            return true;
        }
        if (SixelDisabled())
        {
            return false;
        }
        string termProgram = (Environment.GetEnvironmentVariable("TERM_PROGRAM") ?? "").Trim().ToLowerInvariant();
        string term = (Environment.GetEnvironmentVariable("TERM") ?? "").Trim().ToLowerInvariant();
        string colorTerm = (Environment.GetEnvironmentVariable("COLORTERM") ?? "").Trim().ToLowerInvariant();
        string itermSession = (Environment.GetEnvironmentVariable("ITERM_SESSION_ID") ?? "").Trim().ToLowerInvariant();
        string lcTerminal = (Environment.GetEnvironmentVariable("LC_TERMINAL") ?? "").Trim().ToLowerInvariant();
        if (termProgram.Contains("apple_terminal", StringComparison.Ordinal))
        {
            return false;
        }
        if (!string.IsNullOrEmpty(itermSession) || lcTerminal.Contains("iterm", StringComparison.Ordinal))
        {
            return true;
        }
        if (termProgram.Contains("iterm", StringComparison.Ordinal) ||
            termProgram.Contains("wezterm", StringComparison.Ordinal) ||
            termProgram.Contains("mlterm", StringComparison.Ordinal) ||
            termProgram.Contains("foot", StringComparison.Ordinal) ||
            termProgram.Contains("contour", StringComparison.Ordinal))
        {
            return true;
        }
        if (term.Contains("sixel", StringComparison.Ordinal) || colorTerm.Contains("sixel", StringComparison.Ordinal))
        {
            return true;
        }
        return term.Contains("mlterm", StringComparison.Ordinal) ||
            term.Contains("foot", StringComparison.Ordinal) ||
            term.Contains("contour", StringComparison.Ordinal);
    }

    private bool ShouldRenderSixel()
    {
        return (RenderMode == "sixel" || RenderMode == "auto") && TerminalSupportsSixel();
    }

    public bool ShouldWarnSixelFallback()
    {
        if (DeterministicImageFallbackEnabled() || SixelDisabled() || TruthyEnv(ForceSixelEnv))
        {
            return false;
        }
        return RenderMode != "fallback" && !TerminalSupportsSixel();
    }

    private List<List<TerminalCell>> FallbackImageContent(
        Raster raster,
        int width,
        int height,
        Style style)
    {
        List<List<TerminalCell>> content = BlankContent(width, height, style);
        (int cols, int rows, int colOffset, int rowOffset) =
            ImageCellRegion(width, height, raster.Width, raster.Height);
        string regionFit = Fit == "contain" ? "cover" : Fit;
        RgbSample background = LetterboxRgb(style);
        Raster image = ResizeRaster(
            raster,
            cols,
            rows * FallbackVerticalSamplesPerCell,
            regionFit,
            Align,
            VerticalAlign,
            background);
        if (image.Width <= 0 || image.Height <= 0)
        {
            return content;
        }

        for (int row = 0; row < rows; ++row)
        {
            int targetRow = rowOffset + row;
            if (targetRow < 0 || targetRow >= height)
            {
                continue;
            }
            int topY = row * FallbackVerticalSamplesPerCell;
            int bottomY = Math.Min(image.Height - 1, topY + 1);
            for (int col = 0; col < cols; ++col)
            {
                int targetCol = colOffset + col;
                if (targetCol < 0 || targetCol >= width)
                {
                    continue;
                }
                RgbaSample top = image.PixelAt(col, topY);
                RgbaSample bottom = image.PixelAt(col, bottomY);
                content[targetRow][targetCol] = new TerminalCell
                {
                    Text = FallbackUpperHalfBlock,
                    Foreground = ColorFromRgb(new RgbSample(top.Red, top.Green, top.Blue)),
                    Background = ColorFromRgb(new RgbSample(bottom.Red, bottom.Green, bottom.Blue)),
                    BackgroundFromImageSample = true,
                };
            }
        }

        return content;
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

    private List<List<TerminalCell>> CellBackgroundImageContent(
        Raster raster,
        int width,
        int height,
        Style style)
    {
        List<List<TerminalCell>> content = BlankContent(width, height, style);
        (int cols, int rows, int colOffset, int rowOffset) =
            ImageCellRegion(width, height, raster.Width, raster.Height);
        string regionFit = Fit == "contain" ? "cover" : Fit;
        RgbSample background = LetterboxRgb(style);
        Raster image = ResizeRaster(
            raster,
            cols,
            rows,
            regionFit,
            Align,
            VerticalAlign,
            background);
        if (image.Width <= 0 || image.Height <= 0)
        {
            return content;
        }

        for (int row = 0; row < rows; ++row)
        {
            int targetRow = rowOffset + row;
            if (targetRow < 0 || targetRow >= height)
            {
                continue;
            }
            for (int col = 0; col < cols; ++col)
            {
                int targetCol = colOffset + col;
                if (targetCol < 0 || targetCol >= width)
                {
                    continue;
                }
                RgbaSample pixel = image.PixelAt(col, row);
                Color color = ColorFromRgb(new RgbSample(pixel.Red, pixel.Green, pixel.Blue));
                content[targetRow][targetCol] = new TerminalCell
                {
                    Text = FallbackFullBlock,
                    Foreground = color,
                    Background = color,
                };
            }
        }

        return content;
    }

    private List<List<TerminalCell>> SixelImageContent(
        string source,
        Raster raster,
        int width,
        int height,
        Style style,
        ElementRenderState state)
    {
        List<List<TerminalCell>> content = BlankContent(width, height, style);
        (int cols, int rows, int colOffset, int rowOffset) =
            ImageCellRegion(width, height, raster.Width, raster.Height);
        int visibleTop = rowOffset;
        int visibleBottom = rowOffset + rows;
        if (state.ClipTop.HasValue || state.ClipBottom.HasValue)
        {
            visibleTop = Math.Max(visibleTop, state.ClipTop ?? 0);
            visibleBottom = Math.Min(visibleBottom, state.ClipBottom ?? height);
        }
        int visibleRows = Math.Max(0, visibleBottom - visibleTop);
        if (visibleRows <= 0)
        {
            return content;
        }
        string regionFit = Fit == "contain" ? "cover" : Fit;
        RgbSample background = LetterboxRgb(style);
        string raw = CachedSixelPayload(
            source,
            raster,
            cols,
            visibleRows,
            regionFit,
            Align,
            VerticalAlign,
            background,
            rows,
            visibleTop - rowOffset);
        if (string.IsNullOrEmpty(raw))
        {
            return FallbackImageContent(raster, width, height, style);
        }

        for (int row = visibleTop; row < visibleBottom; ++row)
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
                content[row][col].RawSkip = true;
            }
        }
        int anchorRow = Math.Clamp(visibleTop, 0, height - 1);
        int anchorCol = Math.Clamp(colOffset, 0, width - 1);
        TerminalCell anchor = content[anchorRow][anchorCol];
        anchor.Raw = raw;
        anchor.RawWidth = cols;
        anchor.RawHeight = visibleRows;
        anchor.RawSkip = false;
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
        Size cellPixels = TerminalCellPixels();
        double scale = Math.Min(
            width * cellPixels.Width / (double)sourceWidth,
            height * cellPixels.Height / (double)sourceHeight);
        int cols = Math.Max(1, Math.Min(width, RoundLikePython(sourceWidth * scale / cellPixels.Width)));
        int rows = Math.Max(1, Math.Min(height, RoundLikePython(sourceHeight * scale / cellPixels.Height)));
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

    private static int SixelComponent(int value)
    {
        return Math.Clamp((int)Math.Round(value * SixelColorComponentScale / 255.0), 0, SixelColorComponentScale);
    }

    private static int QuantizeChannel(int value)
    {
        int index = (int)Math.Round(value * (SixelColorLevels - 1) / 255.0);
        return Math.Clamp(index * 255 / (SixelColorLevels - 1), 0, 255);
    }

    private static Raster QuantizeRaster(Raster raster)
    {
        byte[] data = new byte[Math.Max(0, raster.Width * raster.Height * 4)];
        for (int y = 0; y < raster.Height; ++y)
        {
            for (int x = 0; x < raster.Width; ++x)
            {
                RgbaSample pixel = raster.PixelAt(x, y);
                int offset = (y * raster.Width + x) * 4;
                data[offset] = (byte)QuantizeChannel(pixel.Red);
                data[offset + 1] = (byte)QuantizeChannel(pixel.Green);
                data[offset + 2] = (byte)QuantizeChannel(pixel.Blue);
                data[offset + 3] = (byte)Math.Clamp(pixel.Alpha, 0, 255);
            }
        }
        return new Raster(raster.Width, raster.Height, data);
    }

    private static void AppendSixelRun(StringBuilder output, char ch, int count)
    {
        if (count <= 0)
        {
            return;
        }
        if (count >= 4)
        {
            output.Append('!');
            output.Append(count);
            output.Append(ch);
        }
        else
        {
            output.Append(ch, count);
        }
    }

    private static string SixelPayload(Raster raster)
    {
        if (raster.Width <= 0 || raster.Height <= 0)
        {
            return "";
        }
        List<RgbSample> colors = new();
        Dictionary<RgbSample, int> colorIndexes = new();
        for (int y = 0; y < raster.Height; ++y)
        {
            for (int x = 0; x < raster.Width; ++x)
            {
                RgbaSample pixel = raster.PixelAt(x, y);
                if (pixel.Alpha == 0)
                {
                    continue;
                }
                RgbSample color = new(pixel.Red, pixel.Green, pixel.Blue);
                if (colorIndexes.ContainsKey(color))
                {
                    continue;
                }
                colorIndexes[color] = colors.Count;
                colors.Add(color);
            }
        }

        StringBuilder output = new();
        output.Append("\x1bPq");
        foreach (RgbSample color in colors)
        {
            int index = colorIndexes[color];
            output.Append('#');
            output.Append(index);
            output.Append(";2;");
            output.Append(SixelComponent(color.Red));
            output.Append(';');
            output.Append(SixelComponent(color.Green));
            output.Append(';');
            output.Append(SixelComponent(color.Blue));
        }
        for (int y = 0; y < raster.Height; y += SixelBitsPerGlyph)
        {
            foreach (RgbSample color in colors)
            {
                StringBuilder run = new();
                bool hasPixels = false;
                char previous = '\0';
                int count = 0;
                for (int x = 0; x < raster.Width; ++x)
                {
                    int bits = 0;
                    for (int bit = 0; bit < SixelBitsPerGlyph; ++bit)
                    {
                        int py = y + bit;
                        if (py < raster.Height)
                        {
                            RgbaSample pixel = raster.PixelAt(x, py);
                            if (pixel.Alpha > 0 &&
                                new RgbSample(pixel.Red, pixel.Green, pixel.Blue).Equals(color))
                            {
                                bits |= 1 << bit;
                            }
                        }
                    }
                    if (bits != 0)
                    {
                        hasPixels = true;
                    }
                    char ch = (char)(63 + bits);
                    if (ch == previous)
                    {
                        ++count;
                    }
                    else
                    {
                        AppendSixelRun(run, previous, count);
                        previous = ch;
                        count = 1;
                    }
                }
                if (!hasPixels)
                {
                    continue;
                }
                AppendSixelRun(run, previous, count);
                output.Append('#');
                output.Append(colorIndexes[color]);
                output.Append(run);
                output.Append('$');
            }
            output.Append('-');
        }
        output.Append("\x1b\\");
        return output.ToString();
    }

    private static string CachedSixelPayload(
        string source,
        Raster sourceRaster,
        int width,
        int height,
        string fit,
        string align,
        string verticalAlign,
        RgbSample background,
        int sourceHeight = 0,
        int cropTop = 0)
    {
        sourceHeight = sourceHeight > 0 ? sourceHeight : height;
        cropTop = Math.Max(0, cropTop);
        ImageRenderCacheKey key = new(source, width, height, fit, align, verticalAlign, background, sourceHeight, cropTop);
        lock (SixelPayloadCacheLock)
        {
            if (SixelPayloadCache.TryGetValue(key, out string? cached))
            {
                return cached;
            }
        }

        Size cellPixels = TerminalCellPixels();
        Raster fitted = ResizeRaster(
            sourceRaster,
            width * cellPixels.Width,
            sourceHeight * cellPixels.Height,
            fit,
            align,
            verticalAlign,
            background);
        Raster payloadRaster = CropRasterRows(
            fitted,
            cropTop * cellPixels.Height,
            height * cellPixels.Height);
        string raw = LibsixelEncode(payloadRaster);
        if (string.IsNullOrEmpty(raw))
        {
            raw = SixelPayload(QuantizeRaster(payloadRaster));
        }

        lock (SixelPayloadCacheLock)
        {
            SixelPayloadCache[key] = raw;
        }
        return raw;
    }

    private static string LibsixelEncode(Raster raster)
    {
        if (raster.Width <= 0 || raster.Height <= 0)
        {
            return "";
        }
        SixelApi? api = LoadSixelApi();
        if (api is null)
        {
            return "";
        }

        byte[] rgb = raster.ToRgbBytes();
        StringBuilder outputText = new();
        GCHandle outputHandle = GCHandle.Alloc(outputText);
        GCHandle rgbHandle = GCHandle.Alloc(rgb, GCHandleType.Pinned);
        IntPtr output = IntPtr.Zero;
        IntPtr dither = IntPtr.Zero;
        try
        {
            IntPtr rgbData = rgbHandle.AddrOfPinnedObject();
            if (!SixelStatusSucceeded(api.OutputNew(out output, SixelWriteCallback, GCHandle.ToIntPtr(outputHandle), IntPtr.Zero)))
            {
                return "";
            }
            if (!SixelStatusSucceeded(api.DitherNew(out dither, SixelMaxColors, IntPtr.Zero)))
            {
                api.OutputUnref(output);
                output = IntPtr.Zero;
                return "";
            }
            int status = api.DitherInitialize(
                dither,
                rgbData,
                raster.Width,
                raster.Height,
                SixelPixelFormatRgb888,
                SixelLargeAuto,
                SixelRepAuto,
                SixelQualityHigh);
            if (SixelStatusSucceeded(status))
            {
                _ = api.Encode(rgbData, raster.Width, raster.Height, 3, dither, output);
            }
            return outputText.ToString();
        }
        finally
        {
            if (dither != IntPtr.Zero)
            {
                api.DitherUnref(dither);
            }
            if (output != IntPtr.Zero)
            {
                api.OutputUnref(output);
            }
            rgbHandle.Free();
            outputHandle.Free();
        }
    }

    private static bool SixelStatusSucceeded(int status)
    {
        return (status & SixelFalseStatusMask) == 0;
    }

    private static readonly SixelWriteFunction SixelWriteCallback = SixelWrite;

    private static int SixelWrite(IntPtr data, int size, IntPtr priv)
    {
        if (size <= 0 || data == IntPtr.Zero || priv == IntPtr.Zero)
        {
            return size;
        }
        GCHandle handle = GCHandle.FromIntPtr(priv);
        if (handle.Target is not StringBuilder builder)
        {
            return size;
        }
        byte[] bytes = new byte[size];
        Marshal.Copy(data, bytes, 0, size);
        builder.Append(Encoding.ASCII.GetString(bytes));
        return size;
    }

    private static Raster ResizeRaster(
        Raster raster,
        int targetWidth,
        int targetHeight,
        string fit,
        string align,
        string verticalAlign,
        RgbSample background)
    {
        targetWidth = Math.Max(1, targetWidth);
        targetHeight = Math.Max(1, targetHeight);
        byte[] data = new byte[targetWidth * targetHeight * 4];
        if (raster.Width <= 0 || raster.Height <= 0)
        {
            return new Raster(targetWidth, targetHeight, data);
        }
        fit = NormalizedMode(fit, DefaultImageFit);
        bool stretch = fit == "stretch";
        bool cover = fit == "cover";
        double scale = stretch
            ? 0.0
            : cover
                ? Math.Max(targetWidth / (double)raster.Width, targetHeight / (double)raster.Height)
                : Math.Min(targetWidth / (double)raster.Width, targetHeight / (double)raster.Height);
        double drawnWidth = stretch ? targetWidth : Math.Max(1.0, raster.Width * scale);
        double drawnHeight = stretch ? targetHeight : Math.Max(1.0, raster.Height * scale);
        double xOffset = stretch ? 0.0 : cover
            ? AlignmentOffsetFloat(drawnWidth, targetWidth, align, "left", "right")
            : AlignmentOffsetFloat(targetWidth, drawnWidth, align, "left", "right");
        double yOffset = stretch ? 0.0 : cover
            ? AlignmentOffsetFloat(drawnHeight, targetHeight, verticalAlign, "top", "bottom")
            : AlignmentOffsetFloat(targetHeight, drawnHeight, verticalAlign, "top", "bottom");
        for (int y = 0; y < targetHeight; ++y)
        {
            for (int x = 0; x < targetWidth; ++x)
            {
                double sourceLeft;
                double sourceRight;
                double sourceTop;
                double sourceBottom;
                if (stretch)
                {
                    sourceLeft = x * raster.Width / (double)targetWidth;
                    sourceRight = (x + 1) * raster.Width / (double)targetWidth;
                    sourceTop = y * raster.Height / (double)targetHeight;
                    sourceBottom = (y + 1) * raster.Height / (double)targetHeight;
                }
                else if (cover)
                {
                    sourceLeft = (x + xOffset) / scale;
                    sourceRight = (x + 1 + xOffset) / scale;
                    sourceTop = (y + yOffset) / scale;
                    sourceBottom = (y + 1 + yOffset) / scale;
                }
                else
                {
                    sourceLeft = (x - xOffset) / scale;
                    sourceRight = (x + 1 - xOffset) / scale;
                    sourceTop = (y - yOffset) / scale;
                    sourceBottom = (y + 1 - yOffset) / scale;
                }
                RgbSample color = SampleRasterArea(raster, sourceLeft, sourceTop, sourceRight, sourceBottom, background);
                int offset = (y * targetWidth + x) * 4;
                data[offset] = (byte)color.Red;
                data[offset + 1] = (byte)color.Green;
                data[offset + 2] = (byte)color.Blue;
                data[offset + 3] = byte.MaxValue;
            }
        }
        return new Raster(targetWidth, targetHeight, data);
    }

    private static int ChannelFromSample(double value)
    {
        return Math.Clamp((int)Math.Round(value), 0, byte.MaxValue);
    }

    private static double IntervalOverlap(double firstStart, double firstEnd, double secondStart, double secondEnd)
    {
        return Math.Max(0.0, Math.Min(firstEnd, secondEnd) - Math.Max(firstStart, secondStart));
    }

    private static RgbSample SampleRasterArea(
        Raster raster,
        double left,
        double top,
        double right,
        double bottom,
        RgbSample background)
    {
        double fullArea = Math.Max(0.000001, (right - left) * (bottom - top));
        if (raster.Width <= 0 ||
            raster.Height <= 0 ||
            right <= 0.0 ||
            bottom <= 0.0 ||
            left >= raster.Width ||
            top >= raster.Height)
        {
            return background;
        }

        double red = background.Red * fullArea;
        double green = background.Green * fullArea;
        double blue = background.Blue * fullArea;
        int startX = Math.Max(0, (int)Math.Floor(left));
        int endX = Math.Min(raster.Width, (int)Math.Ceiling(right));
        int startY = Math.Max(0, (int)Math.Floor(top));
        int endY = Math.Min(raster.Height, (int)Math.Ceiling(bottom));
        for (int y = startY; y < endY; ++y)
        {
            double yWeight = IntervalOverlap(top, bottom, y, y + 1);
            if (yWeight <= 0.0)
            {
                continue;
            }
            for (int x = startX; x < endX; ++x)
            {
                double xWeight = IntervalOverlap(left, right, x, x + 1);
                if (xWeight <= 0.0)
                {
                    continue;
                }
                double weight = xWeight * yWeight;
                RgbaSample pixel = raster.PixelAt(x, y);
                double alpha = pixel.Alpha / (double)byte.MaxValue;
                red += (pixel.Red - background.Red) * alpha * weight;
                green += (pixel.Green - background.Green) * alpha * weight;
                blue += (pixel.Blue - background.Blue) * alpha * weight;
            }
        }

        return new RgbSample(
            ChannelFromSample(red / fullArea),
            ChannelFromSample(green / fullArea),
            ChannelFromSample(blue / fullArea));
    }

    private static IEnumerable<string> ImageInfoSampleSignature(
        Raster raster,
        int targetWidth,
        int targetHeight,
        string fit,
        string align,
        string verticalAlign,
        RgbSample background)
    {
        targetWidth = Math.Max(1, targetWidth);
        targetHeight = Math.Max(1, targetHeight);
        Raster fitted = ResizeRaster(raster, targetWidth, targetHeight, fit, align, verticalAlign, background);
        foreach (int row in ImageInfoSamplePositions(targetHeight))
        {
            foreach (int col in ImageInfoSamplePositions(targetWidth))
            {
                RgbaSample pixel = fitted.PixelAt(col, row);
                yield return RgbHex(new RgbSample(
                    QuantizeImageInfoChannel(pixel.Red),
                    QuantizeImageInfoChannel(pixel.Green),
                    QuantizeImageInfoChannel(pixel.Blue)));
            }
        }
    }

    private static IEnumerable<int> ImageInfoSamplePositions(int extent)
    {
        extent = Math.Max(1, extent);
        for (int index = 0; index < ImageInfoSampleGridSize; ++index)
        {
            int position = RoundLikePython(index * (extent - 1) / (double)(ImageInfoSampleGridSize - 1));
            yield return Math.Clamp(position, 0, extent - 1);
        }
    }

    private static int QuantizeImageInfoChannel(int value)
    {
        return Math.Clamp((value / ImageInfoColorQuantum) * ImageInfoColorQuantum, 0, 255);
    }

    private static string RgbHex(RgbSample color)
    {
        return $"#{color.Red:x2}{color.Green:x2}{color.Blue:x2}";
    }

    private static Raster CropRasterRows(Raster raster, int top, int height)
    {
        top = Math.Max(0, top);
        height = Math.Max(1, height);
        if (raster.Width <= 0 || raster.Height <= 0 || top <= 0 && height >= raster.Height)
        {
            return raster;
        }
        int bottom = Math.Min(raster.Height, top + height);
        if (bottom <= top)
        {
            return new Raster(raster.Width, 0, Array.Empty<byte>());
        }
        byte[] data = new byte[raster.Width * (bottom - top) * 4];
        for (int y = top; y < bottom; ++y)
        {
            for (int x = 0; x < raster.Width; ++x)
            {
                RgbaSample pixel = raster.PixelAt(x, y);
                int offset = ((y - top) * raster.Width + x) * 4;
                data[offset] = (byte)pixel.Red;
                data[offset + 1] = (byte)pixel.Green;
                data[offset + 2] = (byte)pixel.Blue;
                data[offset + 3] = (byte)pixel.Alpha;
            }
        }
        return new Raster(raster.Width, bottom - top, data);
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

    private static SixelApi? LoadSixelApi()
    {
        return SixelApiLoader.Load();
    }

    private static Raster LoadRaster(string path)
    {
        lock (RasterCacheLock)
        {
            if (RasterCache.TryGetValue(path, out Raster? cached))
            {
                return cached;
            }
        }

        Raster raster;
        try
        {
            using FileStream stream = File.OpenRead(path);
            ImageResult image = ImageResult.FromStream(stream, ColorComponents.RedGreenBlueAlpha);
            raster = image.Width <= 0 || image.Height <= 0 || image.Data.Length == 0
                ? Raster.Empty
                : new Raster(image.Width, image.Height, image.Data);
        }
        catch
        {
            raster = Raster.Empty;
        }

        lock (RasterCacheLock)
        {
            RasterCache[path] = raster;
        }
        return raster;
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

    private readonly record struct ImageRenderCacheKey(
        string Source,
        int Width,
        int Height,
        string Fit,
        string Align,
        string VerticalAlign,
        RgbSample Background,
        int SourceHeight,
        int CropTop);

    private readonly record struct RgbSample(int Red, int Green, int Blue);

    private readonly record struct RgbaSample(int Red, int Green, int Blue, int Alpha);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int SixelWriteFunction(IntPtr data, int size, IntPtr priv);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int SixelOutputNewFunction(out IntPtr output, SixelWriteFunction write, IntPtr priv, IntPtr allocator);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int SixelDitherNewFunction(out IntPtr dither, int colors, IntPtr allocator);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int SixelDitherInitializeFunction(
        IntPtr dither,
        IntPtr pixels,
        int width,
        int height,
        int format,
        int largest,
        int representative,
        int quality);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate int SixelEncodeFunction(IntPtr pixels, int width, int height, int depth, IntPtr dither, IntPtr output);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void SixelOutputUnrefFunction(IntPtr output);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void SixelDitherUnrefFunction(IntPtr dither);

    private sealed class SixelApi
    {
        public SixelApi(
            IntPtr handle,
            SixelOutputNewFunction outputNew,
            SixelDitherNewFunction ditherNew,
            SixelDitherInitializeFunction ditherInitialize,
            SixelEncodeFunction encode,
            SixelOutputUnrefFunction outputUnref,
            SixelDitherUnrefFunction ditherUnref)
        {
            Handle = handle;
            OutputNew = outputNew;
            DitherNew = ditherNew;
            DitherInitialize = ditherInitialize;
            Encode = encode;
            OutputUnref = outputUnref;
            DitherUnref = ditherUnref;
        }

        public IntPtr Handle { get; }
        public SixelOutputNewFunction OutputNew { get; }
        public SixelDitherNewFunction DitherNew { get; }
        public SixelDitherInitializeFunction DitherInitialize { get; }
        public SixelEncodeFunction Encode { get; }
        public SixelOutputUnrefFunction OutputUnref { get; }
        public SixelDitherUnrefFunction DitherUnref { get; }
    }

    private static class SixelApiLoader
    {
        private static readonly object LoadLock = new();
        private static bool loaded;
        private static SixelApi? api;

        public static SixelApi? Load()
        {
            lock (LoadLock)
            {
                if (loaded)
                {
                    return api;
                }
                loaded = true;
                api = LoadFromCandidates();
                return api;
            }
        }

        private static SixelApi? LoadFromCandidates()
        {
            foreach (string candidate in SixelLibraryCandidates())
            {
                if (!NativeLibrary.TryLoad(candidate, out IntPtr handle))
                {
                    continue;
                }
                SixelApi? loadedApi = ApiFromHandle(handle);
                if (loadedApi is not null)
                {
                    return loadedApi;
                }
                NativeLibrary.Free(handle);
            }
            return null;
        }

        private static IEnumerable<string> SixelLibraryCandidates()
        {
            foreach (string name in SixelLibraryNames())
            {
                yield return name;
            }
            foreach (string directory in SixelSearchDirectories())
            {
                foreach (string name in SixelLibraryNames())
                {
                    yield return Path.Combine(directory, name);
                }
            }
            string configuredPath = Environment.GetEnvironmentVariable("UIMD_LIBSIXEL_PATH") ?? "";
            if (!string.IsNullOrWhiteSpace(configuredPath))
            {
                yield return configuredPath;
            }
            foreach (string directory in EnvironmentSearchDirectories("UIMD_LIBSIXEL_DIR", Path.PathSeparator))
            {
                foreach (string name in SixelLibraryNames())
                {
                    yield return Path.Combine(directory, name);
                }
            }
        }

        private static IEnumerable<string> SixelLibraryNames()
        {
            if (OperatingSystem.IsWindows())
            {
                yield return "libsixel.dll";
                yield return "sixel.dll";
                yield return "libsixel-1.dll";
                yield break;
            }
            if (OperatingSystem.IsMacOS() || OperatingSystem.IsMacCatalyst())
            {
                yield return "libsixel.1.dylib";
                yield return "libsixel.dylib";
                yield break;
            }
            yield return "libsixel.so.1";
            yield return "libsixel.so";
        }

        private static IEnumerable<string> SixelSearchDirectories()
        {
            HashSet<string> directories = new(StringComparer.Ordinal);
            if (OperatingSystem.IsWindows())
            {
                AppendEnvironmentSearchDirectories(directories, "PATH", ';');
                AppendEnvironmentSearchDirectories(directories, "LIB", ';');
            }
            else
            {
                AppendEnvironmentSearchDirectories(directories, "LD_LIBRARY_PATH", ':');
                if (OperatingSystem.IsMacOS() || OperatingSystem.IsMacCatalyst())
                {
                    AppendEnvironmentSearchDirectories(directories, "DYLD_LIBRARY_PATH", ':');
                    AppendEnvironmentSearchDirectories(directories, "DYLD_FALLBACK_LIBRARY_PATH", ':');
                }
            }

            string homebrewPrefix = Environment.GetEnvironmentVariable("HOMEBREW_PREFIX") ?? "";
            if (!string.IsNullOrWhiteSpace(homebrewPrefix))
            {
                AppendSearchDirectory(directories, Path.Combine(homebrewPrefix, "lib"));
                AppendSearchDirectory(directories, Path.Combine(homebrewPrefix, "opt", "libsixel", "lib"));
            }
            string macportsPrefix = Environment.GetEnvironmentVariable("MACPORTS_PREFIX") ?? "";
            if (!string.IsNullOrWhiteSpace(macportsPrefix))
            {
                AppendSearchDirectory(directories, Path.Combine(macportsPrefix, "lib"));
            }

            string baseDirectory = AppContext.BaseDirectory;
            if (!string.IsNullOrWhiteSpace(baseDirectory))
            {
                AppendSearchDirectory(directories, baseDirectory);
                AppendSearchDirectory(directories, Path.Combine(baseDirectory, "lib"));
                AppendSearchDirectory(directories, Path.Combine(baseDirectory, "..", "lib"));
                AppendSearchDirectory(directories, Path.Combine(baseDirectory, "..", "..", "lib"));
            }

            if (OperatingSystem.IsMacOS() || OperatingSystem.IsMacCatalyst())
            {
                AppendSearchDirectory(directories, "/opt/homebrew/opt/libsixel/lib");
                AppendSearchDirectory(directories, "/opt/homebrew/lib");
                AppendSearchDirectory(directories, "/usr/local/opt/libsixel/lib");
                AppendSearchDirectory(directories, "/usr/local/lib");
                AppendSearchDirectory(directories, "/opt/local/lib");
            }
            else if (OperatingSystem.IsWindows())
            {
                AppendSearchDirectory(directories, "C:/Program Files/libsixel/bin");
                AppendSearchDirectory(directories, "C:/Program Files/libsixel/lib");
                AppendSearchDirectory(directories, "C:/Program Files (x86)/libsixel/bin");
                AppendSearchDirectory(directories, "C:/Program Files (x86)/libsixel/lib");
            }
            else
            {
                AppendSearchDirectory(directories, "/usr/local/lib");
                AppendSearchDirectory(directories, "/usr/lib");
                AppendSearchDirectory(directories, "/usr/lib64");
                AppendSearchDirectory(directories, "/lib");
                AppendSearchDirectory(directories, "/lib64");
                AppendSearchDirectory(directories, "/usr/lib/x86_64-linux-gnu");
                AppendSearchDirectory(directories, "/usr/lib/aarch64-linux-gnu");
                AppendSearchDirectory(directories, "/usr/lib/arm-linux-gnueabihf");
            }

            return directories;
        }

        private static IEnumerable<string> EnvironmentSearchDirectories(string variable, char delimiter)
        {
            string value = Environment.GetEnvironmentVariable(variable) ?? "";
            if (string.IsNullOrWhiteSpace(value))
            {
                yield break;
            }
            foreach (string item in value.Split(delimiter))
            {
                string normalized = NormalizeDirectory(item);
                if (!string.IsNullOrEmpty(normalized))
                {
                    yield return normalized;
                }
            }
        }

        private static void AppendEnvironmentSearchDirectories(HashSet<string> directories, string variable, char delimiter)
        {
            foreach (string directory in EnvironmentSearchDirectories(variable, delimiter))
            {
                AppendSearchDirectory(directories, directory);
            }
        }

        private static void AppendSearchDirectory(HashSet<string> directories, string directory)
        {
            string normalized = NormalizeDirectory(directory);
            if (!string.IsNullOrEmpty(normalized))
            {
                directories.Add(normalized);
            }
        }

        private static string NormalizeDirectory(string directory)
        {
            if (string.IsNullOrWhiteSpace(directory))
            {
                return "";
            }
            return Path.GetFullPath(directory);
        }

        private static SixelApi? ApiFromHandle(IntPtr handle)
        {
            if (!TryGet(handle, "sixel_output_new", out SixelOutputNewFunction? outputNew) ||
                !TryGet(handle, "sixel_dither_new", out SixelDitherNewFunction? ditherNew) ||
                !TryGet(handle, "sixel_dither_initialize", out SixelDitherInitializeFunction? ditherInitialize) ||
                !TryGet(handle, "sixel_encode", out SixelEncodeFunction? encode) ||
                !TryGet(handle, "sixel_output_unref", out SixelOutputUnrefFunction? outputUnref) ||
                !TryGet(handle, "sixel_dither_unref", out SixelDitherUnrefFunction? ditherUnref))
            {
                return null;
            }
            return new SixelApi(
                handle,
                outputNew!,
                ditherNew!,
                ditherInitialize!,
                encode!,
                outputUnref!,
                ditherUnref!);
        }

        private static bool TryGet<T>(IntPtr handle, string name, out T? function) where T : Delegate
        {
            if (NativeLibrary.TryGetExport(handle, name, out IntPtr pointer))
            {
                function = Marshal.GetDelegateForFunctionPointer<T>(pointer);
                return true;
            }
            function = null;
            return false;
        }
    }

    private sealed class ForceCellBackgroundRenderingScope : IDisposable
    {
        private bool disposed;

        public void Dispose()
        {
            if (disposed)
            {
                return;
            }
            disposed = true;
            forceCellBackgroundRenderingDepth = Math.Max(0, forceCellBackgroundRenderingDepth - 1);
        }
    }

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

        public static Raster Empty { get; } = new(0, 0, Array.Empty<byte>());

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

        public byte[] ToRgbBytes()
        {
            byte[] rgb = new byte[Math.Max(0, Width * Height * 3)];
            int target = 0;
            for (int offset = 0; offset + 3 < Data.Length && target + 2 < rgb.Length; offset += 4)
            {
                rgb[target++] = Data[offset];
                rgb[target++] = Data[offset + 1];
                rgb[target++] = Data[offset + 2];
            }
            return rgb;
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
    private const int TextInputOptionHorizontalSteps = 5;
    private const int TextInputOptionVerticalSteps = 3;

    public string Value { get; private set; }
    public int Cursor { get; private set; }
    public int? SelectionStart { get; private set; }
    public int? SelectionEnd { get; private set; }
    public int MaxLength { get; }
    public bool Multiline { get; }
    private int colScrollOffset;
    private int rowScrollOffset;
    private bool manualRowScroll;

    public TextInput(string name, string value = "", int maxLength = 0, bool multiline = false) : base(name)
    {
        Value = NormalizeTextInputValue(value, multiline);
        MaxLength = maxLength;
        Multiline = multiline;
        Cursor = Value.Length;
    }

    public void SetValue(string value)
    {
        Value = NormalizeTextInputValue(value, Multiline);
        if (MaxLength > 0 && Value.Length > MaxLength)
        {
            Value = Value[..MaxLength];
        }
        SetCursor(Cursor);
    }

    public void SetCursor(int cursor)
    {
        manualRowScroll = false;
        Cursor = Math.Clamp(cursor, 0, Value.Length);
        ClearSelection();
    }

    private void SetCursorKeepingSelection(int cursor)
    {
        Cursor = Math.Clamp(cursor, 0, Value.Length);
    }

    public void SetSelection(int start, int end)
    {
        SelectionStart = Math.Clamp(start, 0, Value.Length);
        SelectionEnd = Math.Clamp(end, 0, Value.Length);
        Cursor = SelectionEnd.Value;
    }

    public void SelectRange(int start, int end)
    {
        manualRowScroll = false;
        int clampedStart = Math.Clamp(start, 0, Value.Length);
        int clampedEnd = Math.Clamp(end, 0, Value.Length);
        if (clampedStart == clampedEnd)
        {
            SelectionStart = null;
            SelectionEnd = null;
            Cursor = clampedEnd;
            return;
        }
        SelectionStart = clampedStart;
        SelectionEnd = clampedEnd;
        Cursor = clampedEnd;
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

    private bool HasSelection()
    {
        return SelectionStart.HasValue && SelectionEnd.HasValue && SelectionStart.Value != SelectionEnd.Value;
    }

    private void DeleteSelection()
    {
        if (!HasSelection())
        {
            return;
        }
        int start = Math.Min(SelectionStart!.Value, SelectionEnd!.Value);
        int end = Math.Max(SelectionStart.Value, SelectionEnd.Value);
        Value = Value.Remove(start, end - start);
        Cursor = start;
        ClearSelection();
    }

    public void InsertText(string text)
    {
        text = text.Replace("\r\n", "\n").Replace('\r', '\n');
        if (!Multiline)
        {
            text = text.Replace('\n', ' ');
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

    public int CursorForPoint(int row, int col, Size size)
    {
        int width = SafeWidth(size, Value);
        if (!Multiline)
        {
            RenderHelpers.LabelVisualRow inputVisualRow = MakeVisualTextRow(0, RenderHelpers.VisualGlyphs(Value, 0, 0));
            return Math.Clamp(
                RawIndexForVisualColumn(inputVisualRow, colScrollOffset + col),
                0,
                Value.Length);
        }

        int height = SafeHeight(size);
        List<RenderHelpers.LabelVisualRow> rows = BuildVisualRows(Value, width);
        int targetRow = Math.Clamp(row, 0, height - 1) + rowScrollOffset;
        if (targetRow >= rows.Count)
        {
            return Value.Length;
        }
        RenderHelpers.LabelVisualRow rowAtPoint = rows[targetRow];
        return Math.Clamp(RawIndexForVisualColumn(rowAtPoint, col), rowAtPoint.Start, rowAtPoint.End);
    }

    public bool ScrollByRows(int delta, int viewportHeight, bool manual = true)
    {
        if (!Multiline)
        {
            return false;
        }
        int height = SafeHeight(new Size(1, viewportHeight));
        if (height <= 1)
        {
            return false;
        }
        int width = SafeWidth(new Size(Frame.Width, viewportHeight), Value);
        List<RenderHelpers.LabelVisualRow> rows = BuildVisualRows(Value, width);
        int maxOffset = Math.Max(0, rows.Count - height);
        int nextOffset = Math.Clamp(rowScrollOffset + delta, 0, maxOffset);
        if (nextOffset == rowScrollOffset)
        {
            return false;
        }
        rowScrollOffset = nextOffset;
        manualRowScroll = manual;
        return true;
    }

    public override bool HandleKey(string key)
    {
        manualRowScroll = false;
        bool hasSelection = HasSelection();
        switch (key)
        {
            case "Shift+Left":
                SelectionStart ??= Cursor;
                SetCursorKeepingSelection(Cursor - 1);
                SelectionEnd = Cursor;
                return true;
            case "Shift+Right":
                SelectionStart ??= Cursor;
                SetCursorKeepingSelection(Cursor + 1);
                SelectionEnd = Cursor;
                return true;
            case "Shift+Home":
                SelectionStart ??= Cursor;
                SetCursorKeepingSelection(LineStartForCursor());
                SelectionEnd = Cursor;
                return true;
            case "Shift+End":
                SelectionStart ??= Cursor;
                SetCursorKeepingSelection(LineEndForCursor());
                SelectionEnd = Cursor;
                return true;
            case "Shift+Up":
                if (!Multiline)
                {
                    return false;
                }
                SelectionStart ??= Cursor;
                MoveCursorVerticalKeepingSelection(-1);
                SelectionEnd = Cursor;
                return true;
            case "Shift+Down":
                if (!Multiline)
                {
                    return false;
                }
                SelectionStart ??= Cursor;
                MoveCursorVerticalKeepingSelection(1);
                SelectionEnd = Cursor;
                return true;
            case "Left":
                if (hasSelection)
                {
                    Cursor = Math.Min(SelectionStart!.Value, SelectionEnd!.Value);
                    ClearSelection();
                }
                else
                {
                    SetCursor(Cursor - 1);
                }
                return true;
            case "Right":
                if (hasSelection)
                {
                    Cursor = Math.Max(SelectionStart!.Value, SelectionEnd!.Value);
                    ClearSelection();
                }
                else
                {
                    SetCursor(Cursor + 1);
                }
                return true;
            case "Alt+Left":
                ClearSelection();
                for (int step = 0; step < TextInputOptionHorizontalSteps; ++step)
                {
                    SetCursor(Cursor - 1);
                }
                return true;
            case "Alt+Right":
                ClearSelection();
                for (int step = 0; step < TextInputOptionHorizontalSteps; ++step)
                {
                    SetCursor(Cursor + 1);
                }
                return true;
            case "Up":
                if (!Multiline)
                {
                    return false;
                }
                MoveCursorVertical(-1);
                return true;
            case "Down":
                if (!Multiline)
                {
                    return false;
                }
                MoveCursorVertical(1);
                return true;
            case "Alt+Up":
                if (!Multiline)
                {
                    return false;
                }
                ClearSelection();
                for (int step = 0; step < TextInputOptionVerticalSteps; ++step)
                {
                    MoveCursorVertical(-1);
                }
                return true;
            case "Alt+Down":
                if (!Multiline)
                {
                    return false;
                }
                ClearSelection();
                for (int step = 0; step < TextInputOptionVerticalSteps; ++step)
                {
                    MoveCursorVertical(1);
                }
                return true;
            case "Home":
                SetCursor(LineStartForCursor());
                return true;
            case "End":
                SetCursor(LineEndForCursor());
                return true;
            case "Backspace":
                if (hasSelection)
                {
                    DeleteSelection();
                }
                else if (Cursor > 0)
                {
                    Value = Value.Remove(Cursor - 1, 1);
                    --Cursor;
                }
                return true;
            case "Delete":
                if (hasSelection)
                {
                    DeleteSelection();
                }
                else if (Cursor < Value.Length)
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

    private int LineStartForCursor()
    {
        if (Cursor <= 0)
        {
            return 0;
        }
        int index = Value.LastIndexOf('\n', Math.Max(0, Cursor - 1));
        return index < 0 ? 0 : index + 1;
    }

    private int LineEndForCursor()
    {
        int index = Value.IndexOf('\n', Cursor);
        return index < 0 ? Value.Length : index;
    }

    private void MoveCursorVertical(int delta)
    {
        MoveCursorVertical(delta, clearSelection: true);
    }

    private void MoveCursorVerticalKeepingSelection(int delta)
    {
        MoveCursorVertical(delta, clearSelection: false);
    }

    private void MoveCursorVertical(int delta, bool clearSelection)
    {
        if (Multiline && Frame.Width > 0)
        {
            int width = SafeWidth(new Size(Frame.Width, Frame.Height), Value);
            List<RenderHelpers.LabelVisualRow> rows = BuildVisualRows(Value, width);
            int currentRow = VisualRowForCursor(rows, width, Cursor);
            int targetRow = currentRow + delta;
            if (targetRow < 0 || targetRow >= rows.Count)
            {
                return;
            }
            RenderHelpers.LabelVisualRow current = rows[currentRow];
            RenderHelpers.LabelVisualRow target = rows[targetRow];
            int currentVisualCol = Math.Clamp(
                VisualColumnForCursor(current, Cursor, width),
                0,
                current.Cells.Count);
            int nextCursor = RawIndexForVisualColumn(target, currentVisualCol);
            if (clearSelection)
            {
                SetCursor(nextCursor);
            }
            else
            {
                SetCursorKeepingSelection(nextCursor);
            }
            return;
        }

        int currentStart = LineStartForCursor();
        int currentTextCol = Cursor - currentStart;
        int targetStart = currentStart;
        if (delta < 0)
        {
            if (currentStart == 0)
            {
                return;
            }
            int previousNewline = Value.LastIndexOf('\n', Math.Max(0, currentStart - 2));
            targetStart = previousNewline < 0 ? 0 : previousNewline + 1;
        }
        else if (delta > 0)
        {
            int nextNewline = Value.IndexOf('\n', Cursor);
            if (nextNewline < 0)
            {
                return;
            }
            targetStart = nextNewline + 1;
        }

        int targetEndPos = Value.IndexOf('\n', targetStart);
        int targetEnd = targetEndPos < 0 ? Value.Length : targetEndPos;
        int next = Math.Min(targetStart + currentTextCol, targetEnd);
        if (clearSelection)
        {
            SetCursor(next);
        }
        else
        {
            SetCursorKeepingSelection(next);
        }
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        int width = SafeWidth(size, Value) + (size.Width > 0 ? 0 : 1);
        int height = Math.Max(1, Multiline ? size.Height : 1);
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        Style cursorStyle = style.Clone();
        if (CursorStyle is not null)
        {
            cursorStyle.Merge(CursorStyle);
        }

        if (!Multiline)
        {
            RenderHelpers.LabelVisualRow inputVisualRow = MakeVisualTextRow(0, RenderHelpers.VisualGlyphs(Value, 0, 0));
            int textWidth = inputVisualRow.Cells.Count;
            colScrollOffset = Math.Max(0, colScrollOffset);
            if (textWidth <= width)
            {
                colScrollOffset = 0;
            }
            int cursorVisualCol = VisualColumnForCursor(inputVisualRow, Cursor, Math.Max(width, textWidth + 1));
            if (state.EditMode)
            {
                if (cursorVisualCol < colScrollOffset)
                {
                    colScrollOffset = cursorVisualCol;
                }
                else if (cursorVisualCol >= colScrollOffset + width)
                {
                    colScrollOffset = Math.Max(0, cursorVisualCol - width + 1);
                }
            }

            List<RenderHelpers.VisualGlyph> visibleCells = new();
            if (colScrollOffset < textWidth)
            {
                int end = Math.Min(textWidth, colScrollOffset + width);
                visibleCells = inputVisualRow.Cells.GetRange(colScrollOffset, end - colScrollOffset);
            }
            List<TerminalCell> renderedRow = GlyphRow(visibleCells, width, style);
            if (!state.EditMode && width > 0 && textWidth > colScrollOffset + width)
            {
                renderedRow[width - 1].Text = ">";
            }
            List<List<TerminalCell>> single = new() { renderedRow };
            if (state.EditMode && HasSelection())
            {
                int singleSelectionLow = SelectionLow();
                int singleSelectionHigh = SelectionHigh();
                for (int col = 0; col < width; ++col)
                {
                    int source = col < visibleCells.Count ? visibleCells[col].SourceStart : -1;
                    if (source >= singleSelectionLow && source < singleSelectionHigh)
                    {
                        single[0][col].Foreground = cursorStyle.Color;
                        single[0][col].Background = cursorStyle.Background;
                    }
                }
            }
            else if (state.EditMode)
            {
                int visibleCol = Math.Clamp(cursorVisualCol - colScrollOffset, 0, width - 1);
                single[0][visibleCol].Foreground = cursorStyle.Color;
                single[0][visibleCol].Background = cursorStyle.Background;
            }
            return single;
        }

        List<RenderHelpers.LabelVisualRow> rows = BuildVisualRows(Value, width);
        int cursorRow = VisualRowForCursor(rows, width, Cursor);
        rowScrollOffset = Math.Clamp(rowScrollOffset, 0, Math.Max(0, rows.Count - height));
        if (state.EditMode && !manualRowScroll)
        {
            if (cursorRow < rowScrollOffset)
            {
                rowScrollOffset = cursorRow;
            }
            else if (cursorRow >= rowScrollOffset + height)
            {
                rowScrollOffset = cursorRow - height + 1;
            }
        }

        bool hasAbove = rowScrollOffset > 0;
        bool hasBelow = rowScrollOffset + height < rows.Count;
        bool selectionActive = state.EditMode && HasSelection();
        int selectionLow = selectionActive ? SelectionLow() : 0;
        int selectionHigh = selectionActive ? SelectionHigh() : 0;
        List<List<TerminalCell>> rendered = new();

        for (int row = 0; row < height; ++row)
        {
            int rowIndex = rowScrollOffset + row;
            bool hasVisualRow = rowIndex < rows.Count;
            RenderHelpers.LabelVisualRow visualRow = hasVisualRow
                ? rows[rowIndex]
                : new RenderHelpers.LabelVisualRow(0, 0, new List<RenderHelpers.VisualGlyph>());
            List<TerminalCell> renderedRow = GlyphRow(visualRow.Cells, width, style);
            rendered.Add(renderedRow);

            int cursorCol = -1;
            if (state.EditMode && hasVisualRow)
            {
                cursorCol = VisualColumnForCursor(visualRow, Cursor, width);
            }

            bool firstVisibleRow = row == 0;
            bool lastVisibleRow = row == height - 1;
            bool indicatorVisible = width > 0 &&
                visualRow.Cells.Count < width &&
                ((firstVisibleRow && hasAbove) || (lastVisibleRow && hasBelow));
            int indicatorCol = width - 1;
            bool indicatorOverlapsCursor = cursorCol == indicatorCol;
            if (indicatorVisible && !indicatorOverlapsCursor)
            {
                rendered[^1][indicatorCol].Text = firstVisibleRow && hasAbove ? "^" : "v";
            }

            if (selectionActive)
            {
                for (int col = 0; col < Math.Min(width, visualRow.Cells.Count); ++col)
                {
                    int source = visualRow.Cells[col].SourceStart;
                    if (source < selectionLow || source >= selectionHigh)
                    {
                        continue;
                    }
                    rendered[^1][col].Foreground = cursorStyle.Color;
                    rendered[^1][col].Background = cursorStyle.Background;
                }
            }
            else if (state.EditMode && cursorCol >= 0 && cursorCol < width)
            {
                rendered[^1][cursorCol].Foreground = cursorStyle.Color;
                rendered[^1][cursorCol].Background = cursorStyle.Background;
            }
        }
        return rendered;
    }

    private static string NormalizeTextInputValue(string value, bool multiline)
    {
        return multiline ? value : value.Replace('\r', ' ').Replace('\n', ' ');
    }

    private static int SafeWidth(Size size, string text)
    {
        return Math.Max(1, size.Width > 0 ? size.Width : RenderHelpers.VisualGlyphs(text, 0, 0).Count);
    }

    private static int SafeHeight(Size size)
    {
        return Math.Max(1, size.Height > 0 ? size.Height : 1);
    }

    private static RenderHelpers.LabelVisualRow MakeVisualTextRow(
        int fallbackStart,
        List<RenderHelpers.VisualGlyph> cells)
    {
        if (cells.Count == 0)
        {
            return new RenderHelpers.LabelVisualRow(fallbackStart, fallbackStart, cells);
        }
        return new RenderHelpers.LabelVisualRow(cells[0].SourceStart, cells[^1].SourceEnd, cells);
    }

    private static List<RenderHelpers.LabelVisualRow> BuildVisualRows(string text, int width)
    {
        return RenderHelpers.BuildLabelVisualRows(text, width);
    }

    private static int VisualRowForCursor(List<RenderHelpers.LabelVisualRow> rows, int width, int cursor)
    {
        for (int index = 0; index < rows.Count; ++index)
        {
            RenderHelpers.LabelVisualRow row = rows[index];
            if (cursor <= row.End && (row.Cells.Count < width || cursor < row.End))
            {
                return index;
            }
        }
        return Math.Max(0, rows.Count - 1);
    }

    private static int VisualColumnForCursor(RenderHelpers.LabelVisualRow row, int cursor, int width)
    {
        if (cursor < row.Start || cursor > row.End)
        {
            return -1;
        }
        for (int index = 0; index < row.Cells.Count; ++index)
        {
            RenderHelpers.VisualGlyph glyph = row.Cells[index];
            if (glyph.SourceStart >= 0 && cursor <= glyph.SourceStart)
            {
                return index;
            }
            if (glyph.SourceStart >= 0 &&
                glyph.SourceEnd >= 0 &&
                cursor > glyph.SourceStart &&
                cursor < glyph.SourceEnd)
            {
                return index;
            }
        }
        if (cursor >= row.Start && cursor <= row.End)
        {
            return Math.Min(row.Cells.Count, Math.Max(0, width - 1));
        }
        return -1;
    }

    private static int RawIndexForVisualColumn(RenderHelpers.LabelVisualRow row, int col)
    {
        return RenderHelpers.RawIndexForLabelVisualColumn(row, col);
    }

    private int SelectionLow()
    {
        return HasSelection() ? Math.Min(SelectionStart!.Value, Cursor) : Cursor;
    }

    private int SelectionHigh()
    {
        return HasSelection() ? Math.Max(SelectionStart!.Value, Cursor) : Cursor;
    }

    private static List<TerminalCell> GlyphRow(
        List<RenderHelpers.VisualGlyph> glyphs,
        int width,
        Style style)
    {
        List<TerminalCell> row = new(width);
        for (int index = 0; index < Math.Min(width, glyphs.Count); ++index)
        {
            row.Add(StyledCell(glyphs[index].Text, style));
        }
        while (row.Count < width)
        {
            row.Add(StyledCell(" ", style));
        }
        return row;
    }

    private static TerminalCell StyledCell(string text, Style style)
    {
        return new TerminalCell
        {
            Text = string.IsNullOrEmpty(text) ? " " : text,
            Foreground = style.Color,
            Background = style.Background,
        };
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
    private double editOriginalValue;
    private bool replaceOnFirstTextInput;

    public NumberInput(string name, double value = 0.0, double stepSize = 1.0) : base(name)
    {
        Value = value;
        StepSize = stepSize;
    }

    public void SetValue(double value)
    {
        bool wasEditing = editing;
        Value = value;
        editOriginalValue = value;
        editText = wasEditing ? DisplayValue() : "";
        editCursor = wasEditing ? editText.Length : 0;
        editing = wasEditing;
        replaceOnFirstTextInput = false;
    }

    public string DisplayText()
    {
        return DisplayValue();
    }

    public void BeginEdit()
    {
        if (editing)
        {
            return;
        }
        editOriginalValue = Value;
        editText = DisplayValue();
        editCursor = editText.Length;
        editing = true;
        replaceOnFirstTextInput = Value == 0.0;
    }

    public void CancelEdit()
    {
        if (!editing)
        {
            return;
        }
        Value = editOriginalValue;
        editText = "";
        editCursor = 0;
        editing = false;
        replaceOnFirstTextInput = false;
    }

    public void CommitEdit()
    {
        EnsureEditText();
        if (string.IsNullOrEmpty(editText))
        {
            Value = 0.0;
        }
        else if (double.TryParse(
            editText,
            System.Globalization.NumberStyles.Float,
            System.Globalization.CultureInfo.InvariantCulture,
            out double parsed))
        {
            Value = parsed;
        }
        else
        {
            Value = editOriginalValue;
        }
        editText = "";
        editCursor = 0;
        editing = false;
        replaceOnFirstTextInput = false;
    }

    public void SetEditCursor(int cursor, bool preserveReplaceOnFirstTextInput = false)
    {
        EnsureEditText();
        editCursor = Math.Clamp(cursor, 0, editText.Length);
        if (!preserveReplaceOnFirstTextInput)
        {
            replaceOnFirstTextInput = false;
        }
    }

    public void SetEditText(string text)
    {
        editText = text;
        editCursor = editText.Length;
        editing = true;
        replaceOnFirstTextInput = false;
    }

    public override bool HandleKey(string key)
    {
        EnsureEditText();
        if (key == "Up")
        {
            Value += StepSize;
            editText = DisplayValue();
            editCursor = editText.Length;
            replaceOnFirstTextInput = false;
            return true;
        }
        if (key == "Down")
        {
            Value -= StepSize;
            editText = DisplayValue();
            editCursor = editText.Length;
            replaceOnFirstTextInput = false;
            return true;
        }
        if (key == "Left")
        {
            SetEditCursor(editCursor - 1);
            return true;
        }
        if (key == "Right")
        {
            SetEditCursor(editCursor + 1);
            return true;
        }
        if (key == "Home")
        {
            SetEditCursor(0);
            return true;
        }
        if (key == "End")
        {
            SetEditCursor(editText.Length);
            return true;
        }
        if (key == "Backspace")
        {
            replaceOnFirstTextInput = false;
            if (editCursor > 0)
            {
                editText = editText.Remove(editCursor - 1, 1);
                --editCursor;
            }
            return true;
        }
        if (key == "Enter")
        {
            CommitEdit();
            return true;
        }
        if (key.Length == 1 && (char.IsDigit(key[0]) || key == "." || key == "-"))
        {
            if (replaceOnFirstTextInput)
            {
                editText = "";
                editCursor = 0;
                replaceOnFirstTextInput = false;
            }
            editText = editText.Insert(editCursor, key);
            ++editCursor;
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
            if (replaceOnFirstTextInput)
            {
                for (int col = 0; col < Math.Min(editText.Length, rendered[0].Count); ++col)
                {
                    rendered[0][col].Foreground = cursorStyle.Color;
                    rendered[0][col].Background = cursorStyle.Background;
                }
            }
            else
            {
                int cursorCol = Math.Clamp(editCursor, 0, Math.Max(0, rendered[0].Count - 1));
                rendered[0][cursorCol].Foreground = cursorStyle.Color;
                rendered[0][cursorCol].Background = cursorStyle.Background;
            }
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
        BeginEdit();
    }
}

public sealed class ComboBox : Element
{
    public List<string> Options { get; }
    public int SelectedIndex { get; private set; }

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

    public override bool HandleKey(string key)
    {
        if (Options.Count == 0)
        {
            return false;
        }
        if (key == "Down")
        {
            SetSelectedIndex(SelectedIndex + 1);
            return true;
        }
        if (key == "Up")
        {
            SetSelectedIndex(SelectedIndex - 1);
            return true;
        }
        return false;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        int width = Math.Max(1, size.Width);
        int height = state.EditMode ? Options.Count + 1 : 1;
        Style baseStyle = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> rendered = new();

        string closed = FitText(SelectedText, width);
        if (width > 0 && (state.Focused || state.EditMode))
        {
            char[] chars = closed.ToCharArray();
            chars[width - 1] = state.EditMode ? 'v' : '>';
            closed = new string(chars);
        }
        rendered.Add(RenderRow(closed, width, baseStyle));

        if (state.EditMode)
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
    public IReadOnlyList<string> DisabledValues => disabledValues;
    public int SelectedIndex { get; private set; }
    public int ActiveIndex { get; private set; }
    public bool Multiple { get; private set; }
    public bool ActiveItemVisible { get; private set; }
    private readonly List<int> selectedIndices = new();
    private readonly List<string> disabledValues = new();
    private int scrollOffset;
    private int lastViewportHeight;

    public ListBox(string name, IEnumerable<string>? options = null) : base(name)
    {
        Options = options?.ToList() ?? new List<string>();
        SetSelectedIndex(SelectedIndex);
    }

    public IReadOnlyCollection<string> SelectedValues =>
        selectedIndices
            .Where(index => index >= 0 && index < Options.Count)
            .Select(index => Options[index])
            .ToList();
    public int ScrollOffset => scrollOffset;

    public void SetOptions(IEnumerable<string> options)
    {
        Options.Clear();
        Options.AddRange(options);
        if (selectedIndices.Count == 0)
        {
            SetSelectedIndex(SelectedIndex);
        }
        else
        {
            SetSelectedIndices(selectedIndices.ToList());
        }
        if (Options.Count == 0)
        {
            scrollOffset = 0;
        }
    }

    public void SetDisabledValues(IEnumerable<string> values)
    {
        disabledValues.Clear();
        disabledValues.AddRange(values);
    }

    public void SetMultiple(bool multiple)
    {
        Multiple = multiple;
        if (!Multiple && selectedIndices.Count > 1)
        {
            SetSelectedIndex(SelectedIndex);
        }
    }

    public void SetSelectedIndex(int index)
    {
        if (Options.Count == 0)
        {
            SelectedIndex = 0;
            ActiveIndex = 0;
            selectedIndices.Clear();
            scrollOffset = 0;
            return;
        }
        SelectedIndex = Math.Clamp(index, 0, Options.Count - 1);
        ActiveIndex = SelectedIndex;
        if (!Multiple)
        {
            selectedIndices.Clear();
            selectedIndices.Add(SelectedIndex);
        }
        if (lastViewportHeight > 0)
        {
            EnsureSelectedVisible(lastViewportHeight);
        }
    }

    public void SetSelectedIndices(IEnumerable<int> indices)
    {
        selectedIndices.Clear();
        if (Options.Count == 0)
        {
            SelectedIndex = 0;
            ActiveIndex = 0;
            scrollOffset = 0;
            return;
        }
        foreach (int index in indices)
        {
            int clamped = Math.Clamp(index, 0, Options.Count - 1);
            if (!selectedIndices.Contains(clamped))
            {
                selectedIndices.Add(clamped);
            }
            if (!Multiple)
            {
                break;
            }
        }
        if (selectedIndices.Count == 0)
        {
            SelectedIndex = Math.Clamp(SelectedIndex, 0, Options.Count - 1);
            ActiveIndex = SelectedIndex;
            return;
        }
        SelectedIndex = selectedIndices[^1];
        ActiveIndex = SelectedIndex;
        if (lastViewportHeight > 0)
        {
            EnsureSelectedVisible(lastViewportHeight);
        }
    }

    public void ToggleSelectedIndex(int index)
    {
        if (Options.Count == 0)
        {
            SelectedIndex = 0;
            ActiveIndex = 0;
            selectedIndices.Clear();
            scrollOffset = 0;
            return;
        }
        SetActiveIndex(index);
        int existing = selectedIndices.IndexOf(ActiveIndex);
        if (existing < 0)
        {
            selectedIndices.Add(ActiveIndex);
        }
        else
        {
            selectedIndices.RemoveAt(existing);
        }
        if (lastViewportHeight > 0)
        {
            EnsureSelectedVisible(lastViewportHeight);
        }
    }

    public void SetSelectedValues(IEnumerable<string> values)
    {
        List<int> indices = new();
        foreach (string value in values)
        {
            int index = Options.IndexOf(value);
            if (index >= 0)
            {
                indices.Add(index);
            }
        }
        SetSelectedIndices(indices);
    }

    public void SetActiveIndex(int index)
    {
        if (Options.Count == 0)
        {
            SelectedIndex = 0;
            ActiveIndex = 0;
            scrollOffset = 0;
            return;
        }
        ActiveIndex = Math.Clamp(index, 0, Options.Count - 1);
        if (lastViewportHeight > 0)
        {
            EnsureIndexVisible(ActiveIndex, lastViewportHeight);
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

    public void ShowActiveItem()
    {
        ActiveItemVisible = true;
    }

    public void HideActiveItem()
    {
        ActiveItemVisible = false;
    }

    private static Style MergeListBoxRowLayer(Style baseStyle, Style? layer)
    {
        if (layer is null)
        {
            return baseStyle;
        }
        Color? inheritedBackground = baseStyle.Background;
        baseStyle.Merge(layer);
        if (layer.Background?.Rgba is { Alpha: < 255 } && inheritedBackground is not null)
        {
            baseStyle.Background = layer.Background.BlendOver(inheritedBackground);
        }
        return baseStyle;
    }

    public override bool HandleKey(string key)
    {
        if (key == "Up")
        {
            SetActiveIndex(ActiveIndex - 1);
            ShowActiveItem();
            return true;
        }
        if (key == "Down")
        {
            SetActiveIndex(ActiveIndex + 1);
            ShowActiveItem();
            return true;
        }
        if (key == "Enter" && Multiple && ActiveIndex >= 0 && ActiveIndex < Options.Count)
        {
            int existing = selectedIndices.IndexOf(ActiveIndex);
            if (existing < 0)
            {
                selectedIndices.Add(ActiveIndex);
            }
            else
            {
                selectedIndices.RemoveAt(existing);
            }
            return true;
        }
        if (key == "Enter")
        {
            SetSelectedIndex(ActiveIndex);
            HideActiveItem();
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
        if (state.EditMode && ActiveItemVisible)
        {
            EnsureIndexVisible(ActiveIndex, height);
        }
        else
        {
            EnsureSelectedVisible(height);
        }
        bool hasAbove = scrollOffset > 0;
        bool hasBelow = scrollOffset + height < Options.Count;
        for (int row = 0; row < height; ++row)
        {
            int optionIndex = scrollOffset + row;
            string text = optionIndex < Options.Count ? Options[optionIndex] : "";
            bool selected = optionIndex < Options.Count && selectedIndices.Contains(optionIndex);
            bool active = optionIndex < Options.Count && state.EditMode && ActiveItemVisible && optionIndex == ActiveIndex;
            bool disabled = optionIndex < Options.Count && disabledValues.Contains(Options[optionIndex]);
            Style rowStyle = style.Clone();
            if (selected)
            {
                rowStyle = MergeListBoxRowLayer(rowStyle, SelectedStyle);
            }
            if (active)
            {
                rowStyle = ActiveStyle is not null
                    ? MergeListBoxRowLayer(rowStyle, ActiveStyle)
                    : selected ? rowStyle : MergeListBoxRowLayer(rowStyle, SelectedStyle);
            }
            if (disabled)
            {
                rowStyle = style.Clone();
                if (DisabledStyle is not null)
                {
                    rowStyle.Merge(DisabledStyle);
                }
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

    private void EnsureIndexVisible(int index, int height)
    {
        if (index < scrollOffset)
        {
            scrollOffset = index;
        }
        else if (index >= scrollOffset + height)
        {
            scrollOffset = index - height + 1;
        }
    }

    private void EnsureSelectedVisible(int height)
    {
        EnsureIndexVisible(SelectedIndex, height);
    }
}

public readonly record struct ScrollViewPosition(int ScrollOffset, int ViewOffset, bool AutoScroll);

public readonly record struct ScrollViewChildView(
    Element? Element,
    Rect Frame,
    bool Visible,
    int Index,
    bool Clipped);

public class ScrollView : Element
{
    private const int DefaultWheelScrollLines = 4;
    private const int WheelScrollViewportFraction = 3;

    public int Gap { get; private set; }
    public bool AutoScroll { get; private set; }
    public int ScrollOffset { get; private set; }
    public int ViewOffset { get; private set; }
    public int HViewOffset { get; private set; }
    public Style? DescendantFocusStyle { get; private set; }
    private readonly List<List<List<TerminalCell>>> renderedChildren = new();
    private Func<int, List<List<List<TerminalCell>>>>? dynamicChildrenRenderer;
    private int? dynamicChildrenWidth;
    private int? cachedChildHeightsWidth;
    private List<int>? cachedChildHeights;
    private int lastNaturalSkip;
    private int pendingTerminalScrollDelta;

    public ScrollView(string name, int gap = 0) : base(name)
    {
        Gap = gap;
    }

    public void SetGap(int gap)
    {
        Gap = Math.Max(0, gap);
        InvalidateHeightCache();
    }

    public void SetAutoScroll(bool autoScroll)
    {
        AutoScroll = autoScroll;
        if (AutoScroll)
        {
            ViewOffset = 0;
        }
    }

    public void SetDescendantFocusStyle(Style style)
    {
        DescendantFocusStyle = style;
    }

    public ScrollViewPosition ScrollPosition()
    {
        return new ScrollViewPosition(ScrollOffset, ViewOffset, AutoScroll);
    }

    public void RestoreScrollPosition(ScrollViewPosition position)
    {
        _ = DynamicRenderedChildren(Math.Max(1, Frame.Width));
        int total = renderedChildren.Count + Children.Count;
        ScrollOffset = Math.Clamp(position.ScrollOffset, 0, Math.Max(0, total - 1));
        ViewOffset = Math.Max(0, position.ViewOffset);
        AutoScroll = position.AutoScroll;
        InvalidateHeightCache();
        Size frameViewport = new(
            Math.Max(1, Frame.Width),
            Math.Max(1, Frame.Height));
        Size viewport = PaddedViewportSize(frameViewport, Style);
        int naturalSkip = Math.Max(
            0,
            ContentHeight(Math.Max(1, viewport.Width)) - Math.Max(1, viewport.Height));
        ViewOffset = AutoScroll ? 0 : Math.Min(ViewOffset, naturalSkip);
        lastNaturalSkip = naturalSkip;
        pendingTerminalScrollDelta = 0;
    }

    public void ClearChildren()
    {
        Children.Clear();
        renderedChildren.Clear();
        dynamicChildrenWidth = null;
        InvalidateHeightCache();
        ScrollOffset = 0;
        ViewOffset = 0;
        lastNaturalSkip = 0;
    }

    public T AddChild<T>(T child) where T : Element
    {
        Children.Add(child);
        InvalidateHeightCache();
        if (AutoScroll)
        {
            ViewOffset = 0;
        }
        return child;
    }

    public void AddChild(List<List<TerminalCell>> child)
    {
        renderedChildren.Add(child);
        InvalidateHeightCache();
        if (AutoScroll)
        {
            ViewOffset = 0;
        }
    }

    public void SetDynamicChildrenRenderer(Func<int, List<List<List<TerminalCell>>>> renderer)
    {
        dynamicChildrenRenderer = renderer;
        InvalidateDynamicChildren();
    }

    public void InvalidateDynamicChildren()
    {
        dynamicChildrenWidth = null;
        InvalidateHeightCache();
    }

    public bool ScrollBy(int delta, Size viewport)
    {
        Style style = EffectiveStyle();
        Size paddedViewport = PaddedViewportSize(viewport, style);
        return ScrollTo(ScrollOffset + delta, paddedViewport);
    }

    public bool ScrollLines(int delta, Size viewport)
    {
        Style style = EffectiveStyle();
        Size paddedViewport = PaddedViewportSize(viewport, style);
        int maxOffset = MaxViewOffset(paddedViewport);
        int previous = Math.Clamp(ViewOffset, 0, maxOffset);
        ViewOffset = Math.Clamp(previous + delta, 0, maxOffset);
        AutoScroll = ViewOffset == 0;
        if (ViewOffset != previous)
        {
            pendingTerminalScrollDelta += ViewOffset - previous;
        }
        return ViewOffset != previous;
    }

    public bool ScrollToTop(Size viewport)
    {
        Style style = EffectiveStyle();
        Size paddedViewport = PaddedViewportSize(viewport, style);
        int previous = ViewOffset;
        ViewOffset = MaxViewOffset(paddedViewport);
        AutoScroll = false;
        return ViewOffset != previous;
    }

    public bool ScrollToBottom(Size viewport)
    {
        Style style = EffectiveStyle();
        Size paddedViewport = PaddedViewportSize(viewport, style);
        _ = MaxViewOffset(paddedViewport);
        bool changed = ScrollOffset != 0 || ViewOffset != 0 || !AutoScroll;
        if (ViewOffset != 0)
        {
            pendingTerminalScrollDelta -= ViewOffset;
        }
        ScrollOffset = 0;
        ViewOffset = 0;
        AutoScroll = true;
        return changed;
    }

    public bool ScrollPageUp(Size viewport)
    {
        Style style = EffectiveStyle();
        Size paddedViewport = PaddedViewportSize(viewport, style);
        return ScrollLines(Math.Max(1, paddedViewport.Height), viewport);
    }

    public bool ScrollPageDown(Size viewport)
    {
        Style style = EffectiveStyle();
        Size paddedViewport = PaddedViewportSize(viewport, style);
        return ScrollLines(-Math.Max(1, paddedViewport.Height), viewport);
    }

    public bool ScrollHorizontal(int delta)
    {
        int previous = HViewOffset;
        HViewOffset = Math.Max(0, HViewOffset + delta);
        return HViewOffset != previous;
    }

    public bool HandleWheel(int wheelDelta, Size viewport)
    {
        if (wheelDelta == 0)
        {
            return false;
        }
        int step = WheelScrollLines(viewport) * Math.Max(1, Math.Abs(wheelDelta));
        return wheelDelta > 0
            ? ScrollLines(step, viewport)
            : ScrollLines(-step, viewport);
    }

    public bool EnsureVisibleRange(int targetTop, int targetBottom, Size size)
    {
        Style style = EffectiveStyle();
        Size viewport = PaddedViewportSize(size, style);
        int naturalSkip = MaxViewOffset(viewport);
        int currentActualSkip = naturalSkip - Math.Min(ViewOffset, naturalSkip);
        int nextActualSkip = currentActualSkip;
        if (targetTop < currentActualSkip)
        {
            nextActualSkip = targetTop;
        }
        else if (targetBottom > currentActualSkip + viewport.Height)
        {
            nextActualSkip = targetBottom - viewport.Height;
        }
        nextActualSkip = Math.Clamp(nextActualSkip, 0, naturalSkip);
        int nextViewOffset = naturalSkip - nextActualSkip;
        int previous = ViewOffset;
        ViewOffset = nextViewOffset;
        AutoScroll = ViewOffset == 0;
        if (ViewOffset != previous)
        {
            pendingTerminalScrollDelta += ViewOffset - previous;
        }
        return ViewOffset != previous;
    }

    public int ConsumeTerminalScrollDelta()
    {
        int delta = pendingTerminalScrollDelta;
        pendingTerminalScrollDelta = 0;
        return delta;
    }

    public int ContentHeight(int width)
    {
        EnsureHeightCache(Math.Max(1, width));
        if (cachedChildHeights is null || cachedChildHeights.Count == 0)
        {
            return 0;
        }
        int height = 0;
        int itemCount = 0;
        for (int index = Math.Max(0, ScrollOffset); index < cachedChildHeights.Count; ++index)
        {
            height += cachedChildHeights[index];
            ++itemCount;
        }
        if (itemCount > 0)
        {
            height += Math.Max(0, Gap) * (itemCount - 1);
        }
        return height;
    }

    public int ContentHeight()
    {
        return ContentHeight(Math.Max(1, Frame.Width));
    }

    public List<ScrollViewChildView> ChildViews(Size size)
    {
        Style style = Style;
        int paddingTop = ConstrainedPaddingTop(size, style);
        int paddingLeft = ConstrainedPaddingLeft(size, style);
        Size viewport = PaddedViewportSize(size, style);
        _ = DynamicRenderedChildren(viewport.Width);
        EnsureHeightCache(viewport.Width);
        int skip = ActualSkip(viewport);

        List<ScrollViewChildView> views = new();
        int globalRow = 0;
        int totalItems = renderedChildren.Count + Children.Count;
        int itemIndex = 0;

        void AppendGap()
        {
            if (itemIndex >= ScrollOffset && itemIndex + 1 < totalItems)
            {
                globalRow += Gap;
            }
            ++itemIndex;
        }

        foreach (List<List<TerminalCell>> child in renderedChildren)
        {
            if (itemIndex >= ScrollOffset)
            {
                int childHeight = Math.Max(0, child.Count);
                int localRow = paddingTop + globalRow - skip;
                int visibleTop = Math.Max(localRow, paddingTop);
                int visibleBottom = Math.Min(localRow + childHeight, paddingTop + viewport.Height);
                bool visible = visibleBottom > visibleTop;
                bool clipped = visible && (visibleTop > localRow || visibleBottom < localRow + childHeight);
                int visibleHeight = visible ? visibleBottom - visibleTop : 0;
                views.Add(new ScrollViewChildView(
                    null,
                    new Rect(visibleTop, paddingLeft, Math.Max(1, viewport.Width), visibleHeight),
                    visible,
                    itemIndex,
                    clipped));
                globalRow += childHeight;
            }
            AppendGap();
        }

        foreach (Element child in Children)
        {
            int childWidth = viewport.Width;
            int childHeight = itemIndex >= 0 &&
                cachedChildHeights is not null &&
                itemIndex < cachedChildHeights.Count
                    ? cachedChildHeights[itemIndex]
                    : NativeChildHeight(child, childWidth);
            if (itemIndex >= ScrollOffset)
            {
                int localRow = paddingTop + globalRow - skip;
                int visibleTop = Math.Max(localRow, paddingTop);
                int visibleBottom = Math.Min(localRow + childHeight, paddingTop + viewport.Height);
                bool visible = visibleBottom > visibleTop;
                bool clipped = visible && (visibleTop > localRow || visibleBottom < localRow + childHeight);
                views.Add(new ScrollViewChildView(
                    child,
                    new Rect(localRow, paddingLeft, Math.Max(1, childWidth), childHeight),
                    visible,
                    itemIndex,
                    clipped));
                globalRow += childHeight;
            }
            AppendGap();
        }

        return views;
    }

    public override bool HandleKey(string key)
    {
        Style style = EffectiveStyle();
        bool scrollY = style.ScrollY ?? true;
        bool scrollX = style.ScrollX ?? false;
        if (scrollY)
        {
            if (key is "ArrowUp" or "Up")
            {
                return ScrollLines(1, new Size(Frame.Width, Frame.Height));
            }
            if (key is "ArrowDown" or "Down")
            {
                return ScrollLines(-1, new Size(Frame.Width, Frame.Height));
            }
            if (key == "PageUp")
            {
                return ScrollPageUp(new Size(Frame.Width, Frame.Height));
            }
            if (key == "PageDown")
            {
                return ScrollPageDown(new Size(Frame.Width, Frame.Height));
            }
            if (key == "Home")
            {
                return ScrollToTop(new Size(Frame.Width, Frame.Height));
            }
            if (key == "End")
            {
                return ScrollToBottom(new Size(Frame.Width, Frame.Height));
            }
        }
        if (scrollX)
        {
            if (key is "ArrowLeft" or "Left")
            {
                return ScrollHorizontal(-1);
            }
            if (key is "ArrowRight" or "Right")
            {
                return ScrollHorizontal(1);
            }
        }
        return false;
    }

    public override List<List<TerminalCell>> Render(Size size, ElementRenderState? state = null)
    {
        state ??= new ElementRenderState();
        Style style = EffectiveStyle(state.Focused, state.EditMode);
        List<List<TerminalCell>> canvas = RenderHelpers.RenderPlainText("", size.Width, size.Height, style);
        int paddingTop = ConstrainedPaddingTop(size, style);
        int paddingLeft = ConstrainedPaddingLeft(size, style);
        Size viewport = PaddedViewportSize(size, style);
        int hOffset = Math.Max(0, HViewOffset);
        int renderWidth = Math.Max(1, viewport.Width + hOffset);
        List<List<List<TerminalCell>>> dynamicChildren = DynamicRenderedChildren(renderWidth);
        int naturalSkip = MaxViewOffset(new Size(renderWidth, viewport.Height));
        int clampedViewOffset = Math.Min(ViewOffset, naturalSkip);
        int skip = naturalSkip - clampedViewOffset;
        int cursor = paddingTop - skip;
        int totalItems = dynamicChildren.Count + Children.Count;
        int itemIndex = 0;
        foreach (List<List<TerminalCell>> rendered in dynamicChildren)
        {
            int childHeight = Math.Max(1, rendered.Count);
            if (itemIndex >= ScrollOffset)
            {
                for (int row = 0; row < rendered.Count; ++row)
                {
                    int targetRow = cursor + row;
                    if (targetRow < paddingTop || targetRow >= paddingTop + viewport.Height || targetRow >= canvas.Count)
                    {
                        continue;
                    }
                    for (int col = 0; col < Math.Min(viewport.Width, rendered[row].Count - hOffset); ++col)
                    {
                        int targetCol = paddingLeft + col;
                        int sourceCol = hOffset + col;
                        if (targetCol < 0 || targetCol >= canvas[targetRow].Count || sourceCol < 0 || sourceCol >= rendered[row].Count)
                        {
                            continue;
                        }
                        canvas[targetRow][targetCol] = rendered[row][sourceCol].Clone();
                    }
                }
                cursor += childHeight;
            }
            if (itemIndex >= ScrollOffset && itemIndex + 1 < totalItems)
            {
                cursor += Gap;
            }
            ++itemIndex;
        }
        foreach (Element child in Children)
        {
            int childHeight = NativeChildHeight(child, renderWidth);
            if (itemIndex >= ScrollOffset)
            {
                cursor += childHeight;
            }
            if (itemIndex >= ScrollOffset && itemIndex + 1 < totalItems)
            {
                cursor += Gap;
            }
            ++itemIndex;
        }
        int indicatorCol = Math.Min(canvas[0].Count, paddingLeft + Math.Max(1, viewport.Width)) - 1;
        int topIndicatorRow = Math.Clamp(paddingTop, 0, Math.Max(0, canvas.Count - 1));
        int bottomIndicatorRow = Math.Clamp(paddingTop + Math.Max(1, viewport.Height) - 1, 0, Math.Max(0, canvas.Count - 1));
        if (skip > 0 && canvas.Count > 0 && canvas[topIndicatorRow].Count > 0)
        {
            ApplyScrollIndicator(canvas[topIndicatorRow], "^", indicatorCol);
        }
        if (clampedViewOffset > 0 && canvas.Count > 0 && canvas[bottomIndicatorRow].Count > 0)
        {
            ApplyScrollIndicator(canvas[bottomIndicatorRow], "v", indicatorCol);
        }
        bool viewportFocused =
            !state.SuppressActiveScrollViewScopeVisuals &&
            (state.Focused ||
                Children.Any(child => state.FocusedElement is not null && ElementTreeContains(child, state.FocusedElement)));
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

    private bool ScrollTo(int index, Size paddedViewport)
    {
        _ = DynamicRenderedChildren(Math.Max(1, paddedViewport.Width));
        int total = renderedChildren.Count + Children.Count;
        if (total == 0)
        {
            int previous = ScrollOffset;
            ScrollOffset = 0;
            return ScrollOffset != previous;
        }

        int previousOffset = ScrollOffset;
        int maxVisible = MaxChildrenInViewport(paddedViewport);
        int maxOffset = Math.Max(0, total - maxVisible);
        ScrollOffset = Math.Clamp(index, 0, maxOffset);
        if (ScrollOffset == 0)
        {
            AutoScroll = false;
        }
        return ScrollOffset != previousOffset;
    }

    public int MaxViewOffset(Size viewport)
    {
        int naturalSkip = Math.Max(0, ContentHeight(Math.Max(1, viewport.Width)) - Math.Max(1, viewport.Height));
        SyncViewOffsetForNaturalSkip(naturalSkip);
        lastNaturalSkip = naturalSkip;
        return naturalSkip;
    }

    private int ActualSkip(Size viewport)
    {
        int naturalSkip = MaxViewOffset(viewport);
        int clampedViewOffset = Math.Min(ViewOffset, naturalSkip);
        return naturalSkip - clampedViewOffset;
    }

    private void SyncViewOffsetForNaturalSkip(int naturalSkip)
    {
        int previousNaturalSkip = Math.Max(0, lastNaturalSkip);
        naturalSkip = Math.Max(0, naturalSkip);
        if (AutoScroll)
        {
            ViewOffset = 0;
        }
        else if (ViewOffset >= previousNaturalSkip)
        {
            ViewOffset = naturalSkip;
        }
        else if (ViewOffset > naturalSkip)
        {
            ViewOffset = naturalSkip;
        }
    }

    private int MaxChildrenInViewport(Size viewport)
    {
        _ = DynamicRenderedChildren(Math.Max(1, viewport.Width));
        int total = renderedChildren.Count + Children.Count;
        if (total == 0)
        {
            return 0;
        }

        int totalHeight = Math.Max(1, viewport.Height);
        int sampleSize = Math.Min(WheelScrollViewportFraction + 2, total);
        if (sampleSize == 0)
        {
            return 1;
        }

        EnsureHeightCache(Math.Max(1, viewport.Width));
        double measured = 0.0;
        int measuredCount = 0;
        if (cachedChildHeights is not null)
        {
            for (int index = 0; index < sampleSize && index < cachedChildHeights.Count; ++index)
            {
                measured += cachedChildHeights[index];
                ++measuredCount;
            }
        }
        double averageHeight = Math.Max(1.0, measuredCount == 0 ? 1.0 : measured / measuredCount);
        int denominator = (int)averageHeight + Math.Max(0, Gap);
        if (denominator <= 0)
        {
            return 1;
        }
        return Math.Max(1, (totalHeight + Math.Max(0, Gap)) / denominator);
    }

    private int WheelScrollLines(Size viewport)
    {
        Style style = EffectiveStyle();
        Size paddedViewport = PaddedViewportSize(viewport, style);
        int viewportLimited = Math.Max(1, paddedViewport.Height / WheelScrollViewportFraction);
        return Math.Max(1, Math.Min(DefaultWheelScrollLines, viewportLimited));
    }

    private void EnsureHeightCache(int width)
    {
        width = Math.Max(1, width);
        if (cachedChildHeights is not null &&
            cachedChildHeightsWidth.HasValue &&
            cachedChildHeightsWidth.Value == width)
        {
            return;
        }

        _ = DynamicRenderedChildren(width);
        if (cachedChildHeights is not null &&
            cachedChildHeightsWidth.HasValue &&
            cachedChildHeightsWidth.Value == width)
        {
            return;
        }

        List<int> heights = new(renderedChildren.Count + Children.Count);
        foreach (List<List<TerminalCell>> child in renderedChildren)
        {
            heights.Add(Math.Max(0, child.Count));
        }
        foreach (Element child in Children)
        {
            heights.Add(NativeChildHeight(child, width));
        }
        cachedChildHeightsWidth = width;
        cachedChildHeights = heights;
    }

    private void InvalidateHeightCache()
    {
        cachedChildHeightsWidth = null;
        cachedChildHeights = null;
    }

    private static int NativeChildHeight(Element child, int width)
    {
        if (child is ReusableElement reusable && reusable.Child is not null)
        {
            Size resolved = GeneratedWindowRuntime.GeneratedWindowContentSizeForWidth(reusable.Child, Math.Max(1, width));
            return Math.Max(1, resolved.Height);
        }
        return Math.Max(1, child.Frame.Height);
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
        InvalidateHeightCache();
        if (AutoScroll)
        {
            ViewOffset = 0;
        }
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
