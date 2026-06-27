using Uimd;

public sealed record MarkdownBlock(string Kind, int Level, string Text, string Language);

public sealed class Paragraph : ParagraphUI
{
    public void SetTextValue(string value)
    {
        text.SetText(value);
    }
}

public sealed class Heading : HeadingUI
{
    public void SetHeading(int level, string value)
    {
        int clamped = Math.Max(1, Math.Min(3, level));
        string color = clamped == 1 ? "#facc15" : clamped == 2 ? "#93c5fd" : "#a7f3d0";
        heading_text.SetSpans(new[]
        {
            new LabelSpan(new string('#', clamped), "#64748b"),
            new LabelSpan(" "),
            new LabelSpan(value, color),
        });
    }
}

public sealed class CodeBlock : CodeBlockUI
{
    private static readonly HashSet<string> PythonKeywords = new(StringComparer.Ordinal)
    {
        "False", "None", "True", "and", "as", "assert", "async", "await", "break", "class",
        "continue", "def", "del", "elif", "else", "except", "finally", "for", "from",
        "global", "if", "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass",
        "raise", "return", "try", "while", "with", "yield",
    };

    public void SetCode(string language, string source)
    {
        lang.SetText(string.IsNullOrEmpty(language) ? "text" : language);
        code.SetSpans(Highlight(source));
    }

    private static List<LabelSpan> Highlight(string source)
    {
        List<LabelSpan> spans = new();
        int cursor = 0;
        for (int index = 0; index < source.Length;)
        {
            if (!IsIdentifierChar(source[index]))
            {
                ++index;
                continue;
            }
            int start = index;
            while (index < source.Length && IsIdentifierChar(source[index]))
            {
                ++index;
            }
            string word = source[start..index];
            if (!PythonKeywords.Contains(word))
            {
                continue;
            }
            if (start > cursor)
            {
                spans.Add(new LabelSpan(source[cursor..start]));
            }
            spans.Add(new LabelSpan(word, "#c084fc"));
            cursor = index;
        }
        if (cursor < source.Length)
        {
            spans.Add(new LabelSpan(source[cursor..]));
        }
        return spans;
    }

    private static bool IsIdentifierChar(char ch)
    {
        return char.IsLetterOrDigit(ch) || ch == '_';
    }
}

public sealed class TableBlock : TableBlockUI
{
    public void SetTable(string value)
    {
        table.SetText(value);
    }
}

public sealed class DocumentView : DocumentViewUI
{
    private readonly List<MarkdownBlock> blocks = new();

    public DocumentView()
    {
        SetAutoScroll(false);
        SetDynamicChildrenRenderer(RenderBlocks);
    }

    public void SetBlocks(IEnumerable<MarkdownBlock> nextBlocks)
    {
        blocks.Clear();
        blocks.AddRange(nextBlocks);
        InvalidateDynamicChildren();
        SetAutoScroll(false);
        ScrollToTop();
    }

    private static List<List<TerminalCell>> RenderBlock(GeneratedWindowBase block, int width)
    {
        Size baseSize = GeneratedWindowRuntime.GeneratedWindowContentSize(block);
        int renderWidth = Math.Max(baseSize.Width, width);
        Size resolved = GeneratedWindowRuntime.GeneratedWindowResolvedContentSize(block, new Size(renderWidth, baseSize.Height));
        return GeneratedWindowRuntime.RenderGeneratedWindowContent(
            block,
            new Size(renderWidth, Math.Max(baseSize.Height, resolved.Height)));
    }

    private List<List<List<TerminalCell>>> RenderBlocks(int width)
    {
        int blockWidth = Math.Max(1, width);
        List<List<List<TerminalCell>>> rendered = new();
        foreach (MarkdownBlock block in blocks)
        {
            rendered.Add(RenderBlock(MakeChild(block), blockWidth));
        }
        return rendered;
    }

    private static GeneratedWindowBase MakeChild(MarkdownBlock block)
    {
        if (block.Kind == "heading")
        {
            Heading child = new();
            child.SetHeading(block.Level, block.Text);
            return child;
        }
        if (block.Kind == "code")
        {
            CodeBlock child = new();
            child.SetCode(block.Language, block.Text);
            return child;
        }
        if (block.Kind == "table")
        {
            TableBlock child = new();
            child.SetTable(block.Text);
            return child;
        }
        Paragraph paragraph = new();
        paragraph.SetTextValue(block.Text);
        return paragraph;
    }

}

public sealed class MarkdownViewer : MarkdownViewerUI
{
    private static readonly string[] PreferredDocuments =
    {
        "overview.markdown",
        "styles.markdown",
        "tables.markdown",
        "code.markdown",
    };

    private readonly Dictionary<string, string> documents;
    private bool finished;

    public MarkdownViewer()
    {
        viewer.SetChild(new DocumentView());
        documents = LoadDocuments();
        List<string> names = new();
        foreach (string preferred in PreferredDocuments)
        {
            if (documents.ContainsKey(preferred))
            {
                names.Add(preferred);
            }
        }
        foreach (string name in documents.Keys.OrderBy(name => name, StringComparer.Ordinal))
        {
            if (!names.Contains(name))
            {
                names.Add(name);
            }
        }
        docs.SetOptions(names);
        docs.SetSelectedIndex(0);
        ShowSelected();
    }

    public override GeneratedWindowRuntimeOptions RuntimeOptions()
    {
        GeneratedWindowRuntimeOptions options = base.RuntimeOptions();
        options.InitialFocusName = "docs";
        options.StartInEditMode = true;
        options.OnFocusChanged = (name, focused) =>
        {
            if (!focused)
            {
                return;
            }
            if (name == "viewer")
            {
                status.SetText("viewer focus - Enter scroll mode, Esc navigation, arrows/PageUp/PageDown/Home/End scroll");
            }
            else if (name is "up_btn" or "down_btn")
            {
                status.SetText("buttons scroll the viewer; Right focuses viewer");
            }
        };
        return options;
    }

    protected override void onUpBtnClick()
    {
        Scroll(1);
    }

    protected override void onDownBtnClick()
    {
        Scroll(-1);
    }

    protected override void onCloseBtnClick()
    {
        finished = true;
    }

    protected override void onDocsSelectionChange(List<string> value)
    {
        ShowSelected();
    }

    protected override bool shouldClose()
    {
        return finished;
    }

    public void ShowSelected()
    {
        if (docs.Options.Count == 0)
        {
            return;
        }
        string name = docs.Options[Math.Clamp(docs.SelectedIndex, 0, docs.Options.Count - 1)];
        documents.TryGetValue(name, out string? source);
        List<MarkdownBlock> blocks = ParseMarkdown(source ?? "");
        DocumentView().SetBlocks(blocks);
        status.SetText(name + " - " + blocks.Count + " blocks - arrows preview");
    }

    private void Scroll(int direction)
    {
        string key = direction > 0 ? "Up" : "Down";
        DocumentView().ScrollView().HandleKey(key);
    }

    private DocumentView DocumentView()
    {
        return viewer.Child as DocumentView ?? throw new InvalidOperationException("Document view child is not initialized.");
    }

    private static Dictionary<string, string> LoadDocuments()
    {
        Dictionary<string, string> result = new(StringComparer.Ordinal);
        string docsDir = DocsDir();
        foreach (string preferred in PreferredDocuments)
        {
            string path = Path.Combine(docsDir, preferred);
            if (File.Exists(path))
            {
                result[preferred] = File.ReadAllText(path);
            }
        }
        if (Directory.Exists(docsDir))
        {
            foreach (string path in Directory.EnumerateFiles(docsDir, "*.markdown"))
            {
                string name = Path.GetFileName(path);
                if (!result.ContainsKey(name))
                {
                    result[name] = File.ReadAllText(path);
                }
            }
        }
        return result;
    }

    private static string DocsDir()
    {
        return Path.Combine(AppContext.BaseDirectory, "docs");
    }

    private static List<MarkdownBlock> ParseMarkdown(string text)
    {
        string[] lines = SplitLines(text);
        List<MarkdownBlock> blocks = new();
        List<string> paragraph = new();
        int index = 0;

        void FlushParagraph()
        {
            string value = Trim(string.Join('\n', paragraph));
            if (!string.IsNullOrEmpty(value))
            {
                blocks.Add(new MarkdownBlock("paragraph", 0, value, ""));
            }
            paragraph.Clear();
        }

        while (index < lines.Length)
        {
            string line = lines[index];
            if (string.IsNullOrEmpty(Trim(line)))
            {
                FlushParagraph();
                ++index;
            }
            else if (line.StartsWith("```", StringComparison.Ordinal))
            {
                FlushParagraph();
                string language = Trim(line[3..]);
                if (string.IsNullOrEmpty(language))
                {
                    language = "text";
                }
                ++index;
                List<string> code = new();
                while (index < lines.Length && !lines[index].StartsWith("```", StringComparison.Ordinal))
                {
                    code.Add(lines[index]);
                    ++index;
                }
                if (index < lines.Length)
                {
                    ++index;
                }
                blocks.Add(new MarkdownBlock("code", 0, string.Join('\n', code), language));
            }
            else if (line.Length > 0 && line[0] == '#')
            {
                FlushParagraph();
                int firstText = 0;
                while (firstText < line.Length && line[firstText] == '#')
                {
                    ++firstText;
                }
                blocks.Add(new MarkdownBlock("heading", firstText, Trim(firstText >= line.Length ? "" : line[firstText..]), ""));
                ++index;
            }
            else if (line.StartsWith("|", StringComparison.Ordinal) &&
                     index + 1 < lines.Length &&
                     IsTableSeparator(lines[index + 1]))
            {
                FlushParagraph();
                List<string> table = new() { line, lines[index + 1] };
                index += 2;
                while (index < lines.Length && lines[index].StartsWith("|", StringComparison.Ordinal))
                {
                    table.Add(lines[index]);
                    ++index;
                }
                blocks.Add(new MarkdownBlock("table", 0, string.Join('\n', table), ""));
            }
            else
            {
                paragraph.Add(line);
                ++index;
            }
        }
        FlushParagraph();
        return blocks;
    }

    private static string[] SplitLines(string text)
    {
        return text.Replace("\r\n", "\n").Split('\n');
    }

    private static string Trim(string value)
    {
        return value.Trim();
    }

    private static bool IsTableSeparator(string line)
    {
        string value = Trim(line);
        if (value.Length < 3 || value[0] != '|' || value[^1] != '|')
        {
            return false;
        }
        foreach (char ch in value)
        {
            if (ch != '|' && ch != ' ' && ch != '-' && ch != ':')
            {
                return false;
            }
        }
        return true;
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            MarkdownViewer app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
