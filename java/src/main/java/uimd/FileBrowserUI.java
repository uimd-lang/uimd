// Auto-generated UI code for file_browser - DO NOT EDIT MANUALLY.
package uimd;

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
import uimd.Size;
import uimd.Style;
import uimd.TextGradient;
import uimd.TextInput;

public class FileBrowserUI extends GeneratedWindowBase
{
    public Label dialog_header;
    public Label path_label;
    public ListBox entries;
    public TextInput filename;
    public Button open_btn;
    public Button close_btn;

    public FileBrowserUI()
    {
        super("File Browser");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("dialog");
        setMcpMetadata(
            true,
            "FileBrowser",
            "# File Browser\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: dialog\nstatus: stable\ndescription: \"File Browser UI source.\"\ntags: [dialog]\n```\n\n## Members\n\n```yaml\ndialog_header:\n  type: label\n  text: \"Open File\"\n\npath_label:\n  type: label\n  text: .\n\nentries:\n  type: listbox\n  options: []\n  selected_items: []\n  multiple: \"false\"\n  commit-mode: leave\n\nfilename:\n  type: textinput\n  value: \"\"\n  maxlength: 120\n\nopen_btn:\n  type: button\n  title: Open\n\nclose_btn:\n  type: button\n  title: Close\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@entries:\n  background: \"#0d1524\"\n  color: \"#cbd5e1\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n```\n\n## User Interface\n\n```ui\n+------------------------------------------------+\n|dialog_header...................................|\n+------------------------------------------------+\n|                                                |\n|   path_label................................   |\n|                                                |\n|   entries...................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|   ..........................................   |\n|                                                |\n|   \"File:\"..  filename.......................   |\n|                                                |\n|   open_btn............  close_btn...........   |\n|                                                |\n+------------------------------------------------+\n```\n",
            "File Browser UI source.",
            List.of(
            new GeneratedElementMetadata("dialog_header", "Open File", true),
            new GeneratedElementMetadata("path_label", ".", true),
            new GeneratedElementMetadata("entries", "entries", true),
            new GeneratedElementMetadata("filename", "filename", true),
            new GeneratedElementMetadata("open_btn", "Open", true),
            new GeneratedElementMetadata("close_btn", "Close", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        dialog_header = addElement(new Label("dialog_header", "Open File"));
        dialog_header.setStyle(Style.fromProperties("background", "#1d314c", "color", "#cbd5e1", "text-align", "center"));
        dialog_header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        path_label = addElement(new Label("path_label", "."));
        path_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        path_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        entries = addElement(new ListBox("entries", List.of()));
        entries.setCommitMode("leave");
        entries.setStyle(Style.fromProperties("background", "#0d1524", "color", "#cbd5e1"));
        entries.setFocusStyle(Style.fromProperties("background", "#1d2f4d", "color", "#ffffff"));
        entries.setEditStyle(Style.fromProperties("background", "#243a5c", "color", "#cbd5e1"));
        entries.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        entries.setActiveStyle(Style.fromProperties("background", "#DDDDDD99", "color", "#FFFFFF"));
        entries.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        filename = addElement(new TextInput("filename", "", 120));
        filename.setStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb"));
        filename.setFocusStyle(Style.fromProperties("background", "#243a5c", "color", "#ffffff"));
        filename.setEditStyle(Style.fromProperties("background", "#2d456d", "color", "#ffffff"));
        filename.setCursorStyle(Style.fromProperties("background", "#ffffff", "color", "#0e1117"));
        filename.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        filename.setDisabledStyle(Style.fromProperties("background", "#0b1220", "color", "#64748b"));
        open_btn = addElement(new Button("open_btn", "Open"));
        open_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        open_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        open_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        close_btn = addElement(new Button("close_btn", "Close"));
        close_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        close_btn.setFocusStyle(Style.fromProperties("background", "#2b5f9f", "color", "#ffffff"));
        close_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setName("dialog_header")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 0, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 48, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(48))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(48, 1))
                .setCellCharsSize(new Size(48, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#1d314c", "color", "#cbd5e1", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("path_label")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(1, 3, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 48, 14))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(48))
                .setCellHeight(AxisDimension.auto(14))
                .setCharsSize(new Size(42, 1))
                .setCellCharsSize(new Size(48, 14))
                .setMarginRight(3)
                .setMarginBottom(12)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("entries")
                .setType("listbox")
                .setCellName("")
                .setRelative(new Rect(3, 3, 536870911, 6))
                .setSourceCell(new Rect(2, 0, 48, 14))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(6))
                .setCellWidth(AxisDimension.auto(48))
                .setCellHeight(AxisDimension.auto(14))
                .setCharsSize(new Size(42, 6))
                .setCellCharsSize(new Size(48, 14))
                .setMarginRight(3)
                .setMarginBottom(5)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("\"File:\"")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(10, 3, 9, 1))
                .setSourceCell(new Rect(2, 0, 48, 14))
                .setWidth(AxisDimension.auto(9))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(48))
                .setCellHeight(AxisDimension.auto(14))
                .setCharsSize(new Size(9, 1))
                .setCellCharsSize(new Size(48, 14))
                .setMarginRight(36)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("filename")
                .setType("textinput")
                .setCellName("")
                .setRelative(new Rect(10, 14, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 48, 14))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(48))
                .setCellHeight(AxisDimension.auto(14))
                .setCharsSize(new Size(31, 1))
                .setCellCharsSize(new Size(48, 14))
                .setMarginRight(3)
                .setMarginBottom(3)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#0d1524", "color", "#e5e7eb")),
            new GeneratedLayoutEntry()
                .setName("open_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(12, 3, 20, 1))
                .setSourceCell(new Rect(2, 0, 48, 14))
                .setWidth(AxisDimension.auto(20))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(48))
                .setCellHeight(AxisDimension.auto(14))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(48, 14))
                .setMarginRight(25)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("close_btn")
                .setType("button")
                .setCellName("")
                .setRelative(new Rect(12, 25, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 48, 14))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(48))
                .setCellHeight(AxisDimension.auto(14))
                .setCharsSize(new Size(20, 1))
                .setCellCharsSize(new Size(48, 14))
                .setMarginRight(3)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onEntriesSelectionChange(List<String> value)
    {
    }

    protected void onFilenameChange(String value)
    {
    }

    protected void onFilenameSubmit(String value)
    {
    }

    protected void onOpenBtnClick()
    {
    }

    protected void onCloseBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("open_btn".equals(name))
        {
            onOpenBtnClick();
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
        if ("filename".equals(name))
        {
            onFilenameChange(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedTextConfirmed(String name, String value)
    {
        if ("filename".equals(name))
        {
            onFilenameSubmit(value);
            return true;
        }
        return false;
    }

    @Override
    public boolean handleGeneratedSelectionChanged(String name, List<String> value)
    {
        if ("entries".equals(name))
        {
            onEntriesSelectionChange(value);
            return true;
        }
        return false;
    }
}
