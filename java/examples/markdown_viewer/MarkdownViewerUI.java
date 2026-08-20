// Auto-generated UI code for markdown_viewer - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.ListBox;
import uimd.Rect;
import uimd.ReusableElement;
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;

public class MarkdownViewerUI extends GeneratedWindowBase
{
    public Label header;
    public Label docs_label;
    public ListBox docs;
    public Button up_btn;
    public Button down_btn;
    public ReusableElement viewer;
    public Button close_btn;
    public Label status;

    public MarkdownViewerUI()
    {
        super("Markdown Viewer");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "MarkdownViewer",
            "# Markdown Viewer\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - document_view\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Markdown Viewer\"\n  description: \"Application title.\"\n  expose: false\n\ndocs_label:\n  type: label\n  text: Documents\n  description: \"Label for the document list.\"\n\ndocs:\n  type: listbox\n  options: [overview.markdown, styles.markdown, tables.markdown, code.markdown]\n  selected_items: [overview.markdown]\n  multiple: \"false\"\n  description: \"Document listbox used to select the markdown document to render.\"\n\nup_btn:\n  type: button\n  title: up\n  description: \"Scroll the rendered document upward.\"\n\ndown_btn:\n  type: button\n  title: down\n  description: \"Scroll the rendered document downward.\"\n\nviewer:\n  type: document_view\n  cpp-class: \"markdown_viewer_example::DocumentView\"\n  cpp-header: document_view/document_view.hpp\n  description: \"Scrollable rendered markdown document area.\"\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: \"Close the markdown viewer.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows the current document and scroll state.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  color: \"#ffffff\"\n@title:\n  background: \"#223a5a\"\n@sidebar:\n  background: \"#101a2b\"\n@content:\n  background: \"#000000\"\n@viewer:\n  background: \"#000000\"\n  padding: 1, 2, 1, 2\n@docs:\n  background: \"#ffffff11\"\n  color: \"#cbd5e1\"\n  focus-background: \"#ffffff22\"\n  edit-background: \"#00000000\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2b5f9f\"\n@close_btn:\n  background: \"#7f1d1d\"\n  color: \"#e2e8f0\"\n  focus-background: \"#dc2626\"\n@status:\n  color: \"#93c5fd\"\n  background: \"#111827\"\n```\n\n## User Interface\n\n```ui\n+-title-**----------------------------------------------------------+\n|header..**................................................close_btn|\n+-sidebar------------------+-content-**-----------------------------+\n|                          |viewer..**..............................|\n| docs_label...**......... |........................................|\n| docs.........**......... |*.......................................|\n| ........................ |*.......................................|\n| #....................... |........................................|\n| #....................... |........................................|\n|                          |........................................|\n| up_btn.... down_btn..... |........................................|\n*                          |........................................|\n*                          |........................................|\n|                          |........................................|\n+--------------------------+----------------------------------------+\n| status..**....................................................... |\n+-------------------------------------------------------------------+\n```\n",
            "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.",
            List.of(
            new GeneratedElementMetadata("header", "Application title.", false),
            new GeneratedElementMetadata("docs_label", "Label for the document list.", true),
            new GeneratedElementMetadata("docs", "Document listbox used to select the markdown document to render.", true),
            new GeneratedElementMetadata("up_btn", "Scroll the rendered document upward.", true),
            new GeneratedElementMetadata("down_btn", "Scroll the rendered document downward.", true),
            new GeneratedElementMetadata("viewer", "Scrollable rendered markdown document area.", true),
            new GeneratedElementMetadata("close_btn", "Close the markdown viewer.", true),
            new GeneratedElementMetadata("status", "Shows the current document and scroll state.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header = addElement(new Label("header", "Markdown Viewer"));
        header.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center"));
        header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        docs_label = addElement(new Label("docs_label", "Documents"));
        docs_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        docs_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        docs = addElement(new ListBox("docs", List.of("overview.markdown", "styles.markdown", "tables.markdown", "code.markdown")));
        docs.setSelectedIndex(0);
        docs.setStyle(Style.fromProperties("background", "#ffffff11", "color", "#cbd5e1"));
        docs.setFocusStyle(Style.fromProperties("background", "#ffffff22", "color", "#ffffff"));
        docs.setEditStyle(Style.fromProperties("background", "#00000000", "color", "#cbd5e1"));
        docs.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        docs.setActiveStyle(Style.fromProperties("background", "#DDDDDD99", "color", "#FFFFFF"));
        docs.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        up_btn = addElement(new Button("up_btn", "up"));
        up_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        up_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        up_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        down_btn = addElement(new Button("down_btn", "down"));
        down_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        down_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        down_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        viewer = addElement(new ReusableElement("viewer", "document_view"));
        viewer.setChild(new DocumentViewUI());
        viewer.setStyle(Style.fromProperties("background", "#000000", "padding-bottom", "1", "padding-left", "2", "padding-right", "2", "padding-top", "1"));
        viewer.setFocusStyle(Style.fromProperties("background", "#ffffff14"));
        viewer.setEditStyle(Style.fromProperties("background", "#ffffff18"));
        viewer.setSelectedStyle(Style.fromProperties("background", "#ffffff26", "color", "#ffffff"));
        close_btn = addElement(new Button("close_btn", "Quit"));
        close_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        close_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        close_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        status = addElement(new Label("status", "Ready"));
        status.setStyle(Style.fromProperties("background", "#111827", "color", "#93c5fd"));
        status.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
    }

    private static TextGradient makeTextGradient(
        int intervalMs,
        int step,
        int segmentSize,
        String... colors)
    {
        TextGradient gradient = new TextGradient();
        gradient.setIntervalMs(intervalMs);
        gradient.setStep(step);
        gradient.setSegmentSize(segmentSize);
        for (String color : colors)
        {
            gradient.colors().add(new Color(color));
        }
        return gradient;
    }

    private static Style styleWithGradients(
        Style style,
        TextGradient colorGradient,
        TextGradient backgroundGradient)
    {
        style.setTextColorGradient(colorGradient);
        style.setTextBackgroundGradient(backgroundGradient);
        return style;
    }

    private static List<GeneratedLayoutEntry> buildLayout()
    {
        return List.of(
            new GeneratedLayoutEntry()
                .setName("header")
                .setType("label")
                .setCellName("title")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 67, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(58, 1))
                .setCellCharsSize(new Size(67, 1))
                .setMarginRight(9)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#223a5a"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("viewer")
                .setType("document_view")
                .setCellName("content")
                .setRelative(new Rect(0, 0, 536870911, 536870911))
                .setSourceCell(new Rect(2, 27, 40, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(11))
                .setCharsSize(new Size(40, 11))
                .setCellCharsSize(new Size(40, 11))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#000000"))
                .setElementStyle(Style.fromProperties("background", "#000000", "padding-bottom", "1", "padding-left", "2", "padding-right", "2", "padding-top", "1")),
            new GeneratedLayoutEntry()
                .setName("status")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(14, 0, 67, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(67))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(65, 1))
                .setCellCharsSize(new Size(67, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#111827", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("close_btn")
                .setType("button")
                .setCellName("title")
                .setRelative(new Rect(0, 58, 9, 1))
                .setSourceCell(new Rect(0, 0, 67, 1))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(67, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#223a5a"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("docs_label")
                .setType("label")
                .setCellName("sidebar")
                .setRelative(new Rect(1, 1, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 26, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(24, 1))
                .setCellCharsSize(new Size(26, 11))
                .setMarginRight(1)
                .setMarginBottom(9)
                .setCellStyle(Style.fromProperties("background", "#101a2b"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("docs")
                .setType("listbox")
                .setCellName("sidebar")
                .setRelative(new Rect(2, 1, 536870911, 536870910))
                .setSourceCell(new Rect(2, 0, 26, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(24, 4))
                .setCellCharsSize(new Size(26, 11))
                .setMarginRight(1)
                .setMarginBottom(5)
                .setCellStyle(Style.fromProperties("background", "#101a2b"))
                .setElementStyle(Style.fromProperties("background", "#ffffff11", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("up_btn")
                .setType("button")
                .setCellName("sidebar")
                .setRelative(new Rect(7, 1, 10, 1))
                .setSourceCell(new Rect(2, 0, 26, 11))
                .setWidth(AxisDimension.auto(10))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(10, 1))
                .setCellCharsSize(new Size(26, 11))
                .setMarginRight(15)
                .setMarginBottom(3)
                .setCellStyle(Style.fromProperties("background", "#101a2b"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("down_btn")
                .setType("button")
                .setCellName("sidebar")
                .setRelative(new Rect(7, 12, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 26, 11))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(26))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(13, 1))
                .setCellCharsSize(new Size(26, 11))
                .setMarginRight(1)
                .setMarginBottom(3)
                .setCellStyle(Style.fromProperties("background", "#101a2b"))
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onDocsSelectionChange(List<String> value)
    {
    }

    protected void onUpBtnClick()
    {
    }

    protected void onDownBtnClick()
    {
    }

    protected void onCloseBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("up_btn".equals(name))
        {
            onUpBtnClick();
            return true;
        }
        if ("down_btn".equals(name))
        {
            onDownBtnClick();
            return true;
        }
        if ("close_btn".equals(name))
        {
            onCloseBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("docs".equals(name))
        {
            onDocsSelectionChange(value);
            return true;
        }
        return false;
    }
}
