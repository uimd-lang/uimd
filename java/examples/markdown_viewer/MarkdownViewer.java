import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Stream;

import uimd.GeneratedWindowBase;
import uimd.GeneratedWindowRuntime;
import uimd.GeneratedWindowRuntimeOptions;
import uimd.LabelSpan;
import uimd.Size;
import uimd.TerminalCell;

public final class MarkdownViewer extends MarkdownViewerUI
{
    private static final int EXPECTED_LAYOUT_ENTRIES = 8;
    private static final int SMOKE_RENDER_WIDTH = 80;
    private static final int SMOKE_RENDER_HEIGHT = 24;
    private static final String DOCS_RELATIVE_PATH = "python/examples/markdown_viewer/docs";
    private static final List<String> PREFERRED_DOCUMENTS = List.of(
        "overview.markdown",
        "styles.markdown",
        "tables.markdown",
        "code.markdown");

    private final Map<String, String> documents;
    private boolean finished;

    public MarkdownViewer()
    {
        viewer.setChild(new DocumentView());
        documents = loadDocuments();
        docs.setOptions(new ArrayList<>(documents.keySet()));
        docs.setSelectedIndex(0);
        showSelected();
    }

    @Override
    public GeneratedWindowRuntimeOptions runtimeOptions()
    {
        GeneratedWindowRuntimeOptions options = super.runtimeOptions();
        options.setInitialFocusName("docs");
        options.setStartInEditMode(true);
        options.setOnFocusChanged((name, focused) ->
        {
            if (!focused)
            {
                return;
            }
            if ("viewer".equals(name))
            {
                status.setText(
                    "viewer focus - Enter scroll mode, Esc navigation, "
                        + "arrows/PageUp/PageDown/Home/End scroll");
            }
            else if ("up_btn".equals(name) || "down_btn".equals(name))
            {
                status.setText("buttons scroll the viewer; Right focuses viewer");
            }
        });
        return options;
    }

    @Override
    protected void onUpBtnClick()
    {
        scroll(1);
    }

    @Override
    protected void onDownBtnClick()
    {
        scroll(-1);
    }

    @Override
    protected void onCloseBtnClick()
    {
        finished = true;
    }

    @Override
    protected void onDocsSelectionChange(List<String> value)
    {
        showSelected();
    }

    @Override
    protected boolean shouldClose()
    {
        return finished;
    }

    public void showSelected()
    {
        if (docs.options().isEmpty())
        {
            return;
        }
        int selectedIndex = Math.max(0, Math.min(docs.selectedIndex(), docs.options().size() - 1));
        String name = docs.options().get(selectedIndex);
        List<MarkdownBlock> blocks = parseMarkdown(documents.getOrDefault(name, ""));
        documentView().setBlocks(blocks);
        status.setText(name + " - " + blocks.size() + " blocks - arrows preview");
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() -> run(arguments));
        System.exit(exitCode);
    }

    private void scroll(int direction)
    {
        documentView().scrollView().handleKey(direction > 0 ? "Up" : "Down");
    }

    private DocumentView documentView()
    {
        if (viewer.child() instanceof DocumentView documentView)
        {
            return documentView;
        }
        throw new IllegalStateException("Document view child is not initialized.");
    }

    private static int run(String[] arguments)
    {
        if (arguments.length > 0 && "--smoke".equals(arguments[0]))
        {
            smoke();
            return 0;
        }
        if (arguments.length > 0 && "--logic-test".equals(arguments[0]))
        {
            logicTest();
            return 0;
        }
        MarkdownViewer application = new MarkdownViewer();
        return GeneratedWindowRuntime.runGeneratedWindow(
            application,
            application.runtimeOptions(),
            arguments);
    }

    private static void smoke()
    {
        MarkdownViewer window = new MarkdownViewer();
        require("Markdown Viewer".equals(window.title()), "unexpected title");
        require(
            window.generatedLayout().size() == EXPECTED_LAYOUT_ENTRIES,
            "unexpected layout size");
        require(window.header != null, "header missing");
        require(window.docs != null, "docs missing");
        require(window.viewer != null, "viewer missing");
        require(window.close_btn != null, "close_btn missing");
        require(window.docs.options().size() == PREFERRED_DOCUMENTS.size(), "unexpected docs");
        require(window.status.text().contains("blocks"), "status missing block count");
    }

    private static void logicTest()
    {
        MarkdownViewer window = new MarkdownViewer();
        DocumentView documentView = window.documentView();
        require(documentView.scrollView().contentHeight() > 0, "document content is empty");
        require(window.status.text().startsWith("overview.markdown"), "unexpected initial document");
        require(
            !GeneratedWindowRuntime.renderGeneratedWindowContent(
                window,
                new Size(SMOKE_RENDER_WIDTH, SMOKE_RENDER_HEIGHT),
                0,
                false).isEmpty(),
            "navigation render is empty");
        require(
            !GeneratedWindowRuntime.renderGeneratedWindowContent(
                window,
                new Size(SMOKE_RENDER_WIDTH, SMOKE_RENDER_HEIGHT),
                0,
                true).isEmpty(),
            "edit render is empty");
        window.docs.setSelectedIndex(2);
        window.showSelected();
        require(window.status.text().startsWith("tables.markdown"), "table document not selected");
        require(documentView.scrollView().contentHeight() > 0, "table document content is empty");
        require(
            parseMarkdown(
                "# H\n\ntext\n\n```python\nprint(1)\n```\n\n| A |\n| - |\n| B |").size() == 4,
            "markdown parser returned an unexpected block count");
    }

    private static Map<String, String> loadDocuments()
    {
        Map<String, String> result = new LinkedHashMap<>();
        Path docsDirectory = docsDirectory();
        for (String preferred : PREFERRED_DOCUMENTS)
        {
            Path path = docsDirectory.resolve(preferred);
            if (Files.isRegularFile(path))
            {
                result.put(preferred, readTextFile(path));
            }
        }
        try (Stream<Path> paths = Files.list(docsDirectory))
        {
            paths.filter(Files::isRegularFile)
                .filter(path -> path.getFileName().toString().endsWith(".markdown"))
                .sorted()
                .forEach(path -> result.putIfAbsent(
                    path.getFileName().toString(),
                    readTextFile(path)));
        }
        catch (IOException exception)
        {
            throw new IllegalStateException(
                "failed to list markdown documents: " + docsDirectory,
                exception);
        }
        return result;
    }

    private static Path docsDirectory()
    {
        Path current = Path.of("").toAbsolutePath().normalize();
        Path directory = findDocsDirectory(current);
        if (directory != null)
        {
            return directory;
        }
        try
        {
            Path location = Path.of(
                MarkdownViewer.class.getProtectionDomain().getCodeSource().getLocation().toURI());
            directory = findDocsDirectory(Files.isDirectory(location) ? location : location.getParent());
            if (directory != null)
            {
                return directory;
            }
        }
        catch (NullPointerException | SecurityException | URISyntaxException ignored)
        {
            // Report the missing domain resource below when code-source metadata is unavailable.
        }
        throw new IllegalStateException("markdown viewer docs directory was not found");
    }

    private static Path findDocsDirectory(Path start)
    {
        for (Path candidate = start; candidate != null; candidate = candidate.getParent())
        {
            Path local = candidate.resolve("docs");
            if (Files.isDirectory(local) && Files.isRegularFile(local.resolve("overview.markdown")))
            {
                return local;
            }
            Path source = candidate.resolve(DOCS_RELATIVE_PATH);
            if (Files.isDirectory(source))
            {
                return source;
            }
        }
        return null;
    }

    private static String readTextFile(Path path)
    {
        try
        {
            return Files.readString(path, StandardCharsets.UTF_8);
        }
        catch (IOException exception)
        {
            throw new IllegalStateException("failed to read markdown document: " + path, exception);
        }
    }

    private static List<MarkdownBlock> parseMarkdown(String text)
    {
        String[] lines = splitLines(text);
        List<MarkdownBlock> blocks = new ArrayList<>();
        List<String> paragraph = new ArrayList<>();
        int index = 0;

        while (index < lines.length)
        {
            String line = lines[index];
            if (line.strip().isEmpty())
            {
                flushParagraph(paragraph, blocks);
                ++index;
            }
            else if (line.startsWith("```"))
            {
                flushParagraph(paragraph, blocks);
                String language = line.substring(3).strip();
                if (language.isEmpty())
                {
                    language = "text";
                }
                ++index;
                List<String> code = new ArrayList<>();
                while (index < lines.length && !lines[index].startsWith("```"))
                {
                    code.add(lines[index]);
                    ++index;
                }
                if (index < lines.length)
                {
                    ++index;
                }
                blocks.add(new MarkdownBlock("code", 0, String.join("\n", code), language));
            }
            else if (line.startsWith("#"))
            {
                flushParagraph(paragraph, blocks);
                int firstText = 0;
                while (firstText < line.length() && line.charAt(firstText) == '#')
                {
                    ++firstText;
                }
                String value = firstText >= line.length() ? "" : line.substring(firstText).strip();
                blocks.add(new MarkdownBlock("heading", firstText, value, ""));
                ++index;
            }
            else if (line.startsWith("|")
                && index + 1 < lines.length
                && isTableSeparator(lines[index + 1]))
            {
                flushParagraph(paragraph, blocks);
                List<String> table = new ArrayList<>();
                table.add(line);
                table.add(lines[index + 1]);
                index += 2;
                while (index < lines.length && lines[index].startsWith("|"))
                {
                    table.add(lines[index]);
                    ++index;
                }
                blocks.add(new MarkdownBlock("table", 0, String.join("\n", table), ""));
            }
            else
            {
                paragraph.add(line);
                ++index;
            }
        }
        flushParagraph(paragraph, blocks);
        return blocks;
    }

    private static void flushParagraph(
        List<String> paragraph,
        List<MarkdownBlock> blocks)
    {
        String value = String.join("\n", paragraph).strip();
        if (!value.isEmpty())
        {
            blocks.add(new MarkdownBlock("paragraph", 0, value, ""));
        }
        paragraph.clear();
    }

    private static String[] splitLines(String text)
    {
        return (text == null ? "" : text)
            .replace("\r\n", "\n")
            .replace('\r', '\n')
            .split("\n", -1);
    }

    private static boolean isTableSeparator(String line)
    {
        String value = line.strip();
        if (value.length() < 3 || value.charAt(0) != '|' || value.charAt(value.length() - 1) != '|')
        {
            return false;
        }
        for (int index = 0; index < value.length(); ++index)
        {
            char character = value.charAt(index);
            if (character != '|' && character != ' ' && character != '-' && character != ':')
            {
                return false;
            }
        }
        return true;
    }

    private static void require(boolean condition, String message)
    {
        if (!condition)
        {
            throw new IllegalStateException(message);
        }
    }

    private record MarkdownBlock(String kind, int level, String text, String language)
    {
    }

    private static final class Paragraph extends ParagraphUI
    {
        void setTextValue(String value)
        {
            text.setText(value);
        }
    }

    private static final class Heading extends HeadingUI
    {
        void setHeading(int level, String value)
        {
            int clamped = Math.max(1, Math.min(3, level));
            String color = switch (clamped)
            {
                case 1 -> "#facc15";
                case 2 -> "#93c5fd";
                default -> "#a7f3d0";
            };
            heading_text.setSpans(List.of(
                new LabelSpan("#".repeat(clamped), "#64748b", ""),
                new LabelSpan(" "),
                new LabelSpan(value, color, "")));
        }
    }

    private static final class CodeBlock extends CodeBlockUI
    {
        private static final Set<String> PYTHON_KEYWORDS = Set.of(
            "False", "None", "True", "and", "as", "assert", "async", "await", "break",
            "class", "continue", "def", "del", "elif", "else", "except", "finally", "for",
            "from", "global", "if", "import", "in", "is", "lambda", "nonlocal", "not", "or",
            "pass", "raise", "return", "try", "while", "with", "yield");

        void setCode(String language, String source)
        {
            lang.setText(language == null || language.isEmpty() ? "text" : language);
            code.setSpans(highlight(source == null ? "" : source));
        }

        private static List<LabelSpan> highlight(String source)
        {
            List<LabelSpan> spans = new ArrayList<>();
            int cursor = 0;
            int index = 0;
            while (index < source.length())
            {
                if (!isIdentifierCharacter(source.charAt(index)))
                {
                    ++index;
                    continue;
                }
                int start = index;
                while (index < source.length() && isIdentifierCharacter(source.charAt(index)))
                {
                    ++index;
                }
                String word = source.substring(start, index);
                if (!PYTHON_KEYWORDS.contains(word))
                {
                    continue;
                }
                if (start > cursor)
                {
                    spans.add(new LabelSpan(source.substring(cursor, start)));
                }
                spans.add(new LabelSpan(word, "#c084fc", ""));
                cursor = index;
            }
            if (cursor < source.length())
            {
                spans.add(new LabelSpan(source.substring(cursor)));
            }
            return spans;
        }

        private static boolean isIdentifierCharacter(char character)
        {
            return Character.isLetterOrDigit(character) || character == '_';
        }
    }

    private static final class TableBlock extends TableBlockUI
    {
        void setTable(String value)
        {
            table.setText(value);
        }
    }

    private static final class DocumentView extends DocumentViewUI
    {
        private final List<MarkdownBlock> blocks = new ArrayList<>();

        DocumentView()
        {
            setAutoScroll(false);
            setDynamicChildrenRenderer(this::renderBlocks);
        }

        void setBlocks(List<MarkdownBlock> nextBlocks)
        {
            blocks.clear();
            blocks.addAll(nextBlocks);
            invalidateDynamicChildren();
            setAutoScroll(false);
            scrollToTop();
        }

        private List<List<List<TerminalCell>>> renderBlocks(int width)
        {
            int blockWidth = Math.max(1, width);
            List<List<List<TerminalCell>>> rendered = new ArrayList<>();
            for (MarkdownBlock block : blocks)
            {
                rendered.add(renderBlock(makeChild(block), blockWidth));
            }
            return rendered;
        }

        private static List<List<TerminalCell>> renderBlock(
            GeneratedWindowBase block,
            int width)
        {
            Size base = GeneratedWindowRuntime.generatedWindowContentSize(block);
            int renderWidth = Math.max(base.width(), width);
            Size resolved = GeneratedWindowRuntime.generatedWindowContentSizeForWidth(
                block,
                renderWidth);
            return GeneratedWindowRuntime.renderGeneratedWindowContent(
                block,
                new Size(renderWidth, Math.max(base.height(), resolved.height())),
                -1,
                false);
        }

        private static GeneratedWindowBase makeChild(MarkdownBlock block)
        {
            if ("heading".equals(block.kind()))
            {
                Heading child = new Heading();
                child.setHeading(block.level(), block.text());
                return child;
            }
            if ("code".equals(block.kind()))
            {
                CodeBlock child = new CodeBlock();
                child.setCode(block.language(), block.text());
                return child;
            }
            if ("table".equals(block.kind()))
            {
                TableBlock child = new TableBlock();
                child.setTable(block.text());
                return child;
            }
            Paragraph paragraph = new Paragraph();
            paragraph.setTextValue(block.text());
            return paragraph;
        }
    }
}
