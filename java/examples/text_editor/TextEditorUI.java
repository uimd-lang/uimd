// Auto-generated UI code for text_editor - DO NOT EDIT MANUALLY.
import java.util.List;

import uimd.AxisDimension;
import uimd.Button;
import uimd.Color;
import uimd.GeneratedAppToolMetadata;
import uimd.GeneratedElementMetadata;
import uimd.GeneratedLayoutEntry;
import uimd.GeneratedWindowBase;
import uimd.Label;
import uimd.Rect;
import uimd.Size;
import uimd.Style;
import uimd.TextArea;
import uimd.TextGradient;

public class TextEditorUI extends GeneratedWindowBase
{
    public Label window_header;
    public Label filename;
    public TextArea editor;
    public Button new_btn;
    public Button open_btn;
    public Button save_btn;
    public Button save_as_btn;
    public Button quit_btn;
    public Label status;

    public TextEditorUI()
    {
        super("Text Editor");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "TextEditor",
            "# Text Editor\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - ../../dialogs/file_browser\n```\n\n## Members\n\n```yaml\nwindow_header:\n  type: label\n  text: \"Text Editor\"\n  description: \"Application title.\"\n  expose: false\n\nfilename:\n  type: label\n  text: welcome.txt\n  description: \"Current file path or file name.\"\n\neditor:\n  type: textarea\n  value: \"\"\n  description: \"Main multiline text editing area.\"\n\nnew_btn:\n  type: button\n  title: New\n  description: \"Create a new empty document, asking about unsaved changes when needed.\"\n\nopen_btn:\n  type: button\n  title: Open\n  description: \"Open the file browser to load a text file, asking about unsaved changes when needed.\"\n\nsave_btn:\n  type: button\n  title: Save\n  description: \"Save the current document to its current path.\"\n\nsave_as_btn:\n  type: button\n  title: \"Save As\"\n  description: \"Open the file browser in save mode and save the document to a chosen path.\"\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the editor, asking about unsaved changes when needed.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows load, save, cancel, and error messages.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@filename:\n  color: \"#bfdbfe\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@status:\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-title-**------------------------------------------------------------------------------+\n|window_header..**......................................................................|\n+-filebar-------------------------------------------------------------------------------+\n|                                                                                       |\n| \"file\" filename..**.................................................................. |\n|                                                                                       |\n| new_btn....... open_btn....... save_btn....... save_as_btn....... quit_btn.......     |\n+-main----------------------------------------------------------------------------------+\n|                                                                                       |\n| editor..**............................................................................|\n| ..................................................................................... |\n* *.................................................................................... |\n* *.................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n| ..................................................................................... |\n+---------------------------------------------------------------------------------------+\n| status............................................................................... |\n+---------------------------------------------------------------------------------------+\n```\n",
            "Text editor for creating, opening, editing, saving, saving as, and closing plain text files with confirmation dialogs for unsaved changes.",
            List.of(
            new GeneratedElementMetadata("window_header", "Application title.", false),
            new GeneratedElementMetadata("filename", "Current file path or file name.", true),
            new GeneratedElementMetadata("editor", "Main multiline text editing area.", true),
            new GeneratedElementMetadata("new_btn", "Create a new empty document, asking about unsaved changes when needed.", true),
            new GeneratedElementMetadata("open_btn", "Open the file browser to load a text file, asking about unsaved changes when needed.", true),
            new GeneratedElementMetadata("save_btn", "Save the current document to its current path.", true),
            new GeneratedElementMetadata("save_as_btn", "Open the file browser in save mode and save the document to a chosen path.", true),
            new GeneratedElementMetadata("quit_btn", "Close the editor, asking about unsaved changes when needed.", true),
            new GeneratedElementMetadata("status", "Shows load, save, cancel, and error messages.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        window_header = addElement(new Label("window_header", "Text Editor"));
        window_header.setStyle(Style.fromProperties("background", "#1d314c", "color", "#cbd5e1", "text-align", "center"));
        window_header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        filename = addElement(new Label("filename", "welcome.txt"));
        filename.setStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe"));
        filename.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        editor = addElement(new TextArea("editor", "", 0));
        editor.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        editor.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        editor.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        editor.setCursorStyle(Style.fromProperties("background", "#ffffff", "color", "#0e1117"));
        editor.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        editor.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        new_btn = addElement(new Button("new_btn", "New"));
        new_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        new_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        new_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        open_btn = addElement(new Button("open_btn", "Open"));
        open_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        open_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        open_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        save_btn = addElement(new Button("save_btn", "Save"));
        save_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        save_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        save_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        save_as_btn = addElement(new Button("save_as_btn", "Save As"));
        save_as_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        save_as_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        save_as_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        quit_btn = addElement(new Button("quit_btn", "Quit"));
        quit_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        quit_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        quit_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        status = addElement(new Label("status", "Ready"));
        status.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
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
                .setName("window_header")
                .setType("label")
                .setCellName("title")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 87, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(87, 1))
                .setCellCharsSize(new Size(87, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#1d314c", "color", "#cbd5e1", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("status")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 1, 85, 1))
                .setSourceCell(new Rect(18, 0, 87, 1))
                .setWidth(AxisDimension.auto(85))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(85, 1))
                .setCellCharsSize(new Size(87, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("\"file\"")
                .setType("label")
                .setCellName("filebar")
                .setRelative(new Rect(1, 1, 6, 1))
                .setSourceCell(new Rect(2, 0, 87, 4))
                .setWidth(AxisDimension.auto(6))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(6, 1))
                .setCellCharsSize(new Size(87, 4))
                .setMarginRight(80)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("editor")
                .setType("textarea")
                .setCellName("main")
                .setRelative(new Rect(1, 1, 536870911, 536870911))
                .setSourceCell(new Rect(7, 0, 87, 10))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(86, 9))
                .setCellCharsSize(new Size(87, 10))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(new Style()),
            new GeneratedLayoutEntry()
                .setName("filename")
                .setType("label")
                .setCellName("filebar")
                .setRelative(new Rect(1, 8, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 87, 4))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(78, 1))
                .setCellCharsSize(new Size(87, 4))
                .setMarginRight(1)
                .setMarginBottom(2)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#bfdbfe")),
            new GeneratedLayoutEntry()
                .setName("new_btn")
                .setType("button")
                .setCellName("filebar")
                .setRelative(new Rect(3, 1, 14, 1))
                .setSourceCell(new Rect(2, 0, 87, 4))
                .setWidth(AxisDimension.auto(14))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(87, 4))
                .setMarginRight(72)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("open_btn")
                .setType("button")
                .setCellName("filebar")
                .setRelative(new Rect(3, 16, 15, 1))
                .setSourceCell(new Rect(2, 0, 87, 4))
                .setWidth(AxisDimension.auto(15))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(15, 1))
                .setCellCharsSize(new Size(87, 4))
                .setMarginRight(56)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("save_btn")
                .setType("button")
                .setCellName("filebar")
                .setRelative(new Rect(3, 32, 15, 1))
                .setSourceCell(new Rect(2, 0, 87, 4))
                .setWidth(AxisDimension.auto(15))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(15, 1))
                .setCellCharsSize(new Size(87, 4))
                .setMarginRight(40)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("save_as_btn")
                .setType("button")
                .setCellName("filebar")
                .setRelative(new Rect(3, 48, 18, 1))
                .setSourceCell(new Rect(2, 0, 87, 4))
                .setWidth(AxisDimension.auto(18))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(18, 1))
                .setCellCharsSize(new Size(87, 4))
                .setMarginRight(21)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("quit_btn")
                .setType("button")
                .setCellName("filebar")
                .setRelative(new Rect(3, 67, 15, 1))
                .setSourceCell(new Rect(2, 0, 87, 4))
                .setWidth(AxisDimension.auto(15))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(87))
                .setCellHeight(AxisDimension.auto(4))
                .setCharsSize(new Size(15, 1))
                .setCellCharsSize(new Size(87, 4))
                .setMarginRight(5)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"))
        );
    }

    protected void onEditorChange(String value)
    {
    }

    protected void onEditorSubmit(String value)
    {
    }

    protected void onNewBtnClick()
    {
    }

    protected void onOpenBtnClick()
    {
    }

    protected void onSaveBtnClick()
    {
    }

    protected void onSaveAsBtnClick()
    {
    }

    protected void onQuitBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("new_btn".equals(name))
        {
            onNewBtnClick();
            return true;
        }
        if ("open_btn".equals(name))
        {
            onOpenBtnClick();
            return true;
        }
        if ("save_btn".equals(name))
        {
            onSaveBtnClick();
            return true;
        }
        if ("save_as_btn".equals(name))
        {
            onSaveAsBtnClick();
            return true;
        }
        if ("quit_btn".equals(name))
        {
            onQuitBtnClick();
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextChanged(String name, String value)
    {
        if ("editor".equals(name))
        {
            onEditorChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("editor".equals(name))
        {
            onEditorSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        return false;
    }
}
