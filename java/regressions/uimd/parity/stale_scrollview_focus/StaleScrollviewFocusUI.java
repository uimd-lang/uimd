// Auto-generated UI code for stale_scrollview_focus - DO NOT EDIT MANUALLY.
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
import uimd.TextGradient;
import uimd.ViewHost;

public class StaleScrollviewFocusUI extends GeneratedWindowBase
{
    public Label title;
    public Button quit_btn;
    public Button tab_list;
    public Button tab_other;
    public Label hint;
    public ViewHost page;

    public StaleScrollviewFocusUI()
    {
        super("Stale ScrollView Focus");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "StaleScrollviewFocus",
            "# Stale ScrollView Focus\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: regression\ndescription: \"Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button.\"\ntags: [regression, issue-5, viewhost, scrollview]\n```\n\n## Definition\n\n```yaml\nuses:\n  - list_page\n  - edit_page\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: \"Scroll Focus Repro\"\n  description: \"Regression window title.\"\n  expose: false\n\nquit_btn:\n  type: button\n  title: Quit\n  description: \"Close the regression app.\"\n\ntab_list:\n  type: button\n  title: List\n  description: \"Show the list page.\"\n\ntab_other:\n  type: button\n  title: Other\n  description: \"Inactive top-level navigation button used to expose stale focus.\"\n\nhint:\n  type: label\n  text: \"Focus list, press Enter, choose Open.\"\n  description: \"Manual reproduction hint.\"\n  expose: false\n\npage:\n  type: viewhost\n  description: \"Dynamic page host.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  background: \"#1f2937\"\n@title:\n  color: \"#ffffff\"\n  text-align: center\n@nav:\n  background: \"#172033\"\n@content:\n  background: \"#030712\"\n@hint:\n  color: \"#93c5fd\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@tab_list:\n  background: \"#2f80ed\"\n  focus-background: \"#22c55e\"\n@tab_other:\n  background: \"#343b4b\"\n  focus-background: \"#9333ea\"\n@quit_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\nviewhost:\n  background: \"#030712\"\n```\n\n## User Interface\n\n```ui\n+-header-**--------------------------------------------------------------------------------+\n| title..**.................................................................. quit_btn.... |\n+-nav-**-----------------------------------------------------------------------------------+\n| tab_list........ tab_other........ hint..**............................................. |\n+-content-**-------------------------------------------------------------------------------+\n| page..**................................................................................ |\n* *....................................................................................... |\n* *....................................................................................... |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n| ........................................................................................ |\n+------------------------------------------------------------------------------------------+\n```\n",
            "Manual and MCP regression for replacing a ViewHost page from a focused ScrollView row button.",
            List.of(
            new GeneratedElementMetadata("title", "Regression window title.", false),
            new GeneratedElementMetadata("quit_btn", "Close the regression app.", true),
            new GeneratedElementMetadata("tab_list", "Show the list page.", true),
            new GeneratedElementMetadata("tab_other", "Inactive top-level navigation button used to expose stale focus.", true),
            new GeneratedElementMetadata("hint", "Manual reproduction hint.", false),
            new GeneratedElementMetadata("page", "Dynamic page host.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        title = addElement(new Label("title", "Scroll Focus Repro"));
        title.setStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center"));
        title.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        quit_btn = addElement(new Button("quit_btn", "Quit"));
        quit_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        quit_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        quit_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        tab_list = addElement(new Button("tab_list", "List"));
        tab_list.setStyle(Style.fromProperties("background", "#2f80ed", "color", "#e2e8f0"));
        tab_list.setFocusStyle(Style.fromProperties("background", "#22c55e", "color", "#ffffff"));
        tab_list.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        tab_other = addElement(new Button("tab_other", "Other"));
        tab_other.setStyle(Style.fromProperties("background", "#343b4b", "color", "#e2e8f0"));
        tab_other.setFocusStyle(Style.fromProperties("background", "#9333ea", "color", "#ffffff"));
        tab_other.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        hint = addElement(new Label("hint", "Focus list, press Enter, choose Open."));
        hint.setStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd"));
        hint.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        page = addElement(new ViewHost("page"));
        page.setStyle(Style.fromProperties("background", "#030712"));
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
                .setName("page")
                .setType("viewhost")
                .setCellName("content")
                .setRelative(new Rect(0, 1, 536870911, 536870911))
                .setSourceCell(new Rect(4, 0, 90, 29))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.expanded())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(88, 29))
                .setCellCharsSize(new Size(90, 29))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#030712"))
                .setElementStyle(Style.fromProperties("background", "#030712")),
            new GeneratedLayoutEntry()
                .setName("tab_list")
                .setType("button")
                .setCellName("nav")
                .setRelative(new Rect(0, 1, 16, 1))
                .setSourceCell(new Rect(2, 0, 90, 1))
                .setWidth(AxisDimension.auto(16))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(16, 1))
                .setCellCharsSize(new Size(90, 1))
                .setMarginRight(73)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033"))
                .setElementStyle(Style.fromProperties("background", "#2f80ed", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("title")
                .setType("label")
                .setCellName("header")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(0, 0, 90, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(75, 1))
                .setCellCharsSize(new Size(90, 1))
                .setMarginRight(14)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("tab_other")
                .setType("button")
                .setCellName("nav")
                .setRelative(new Rect(0, 18, 17, 1))
                .setSourceCell(new Rect(2, 0, 90, 1))
                .setWidth(AxisDimension.auto(17))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(17, 1))
                .setCellCharsSize(new Size(90, 1))
                .setMarginRight(55)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033"))
                .setElementStyle(Style.fromProperties("background", "#343b4b", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("hint")
                .setType("label")
                .setCellName("nav")
                .setRelative(new Rect(0, 36, 536870911, 1))
                .setSourceCell(new Rect(2, 0, 90, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(53, 1))
                .setCellCharsSize(new Size(90, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#172033"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("quit_btn")
                .setType("button")
                .setCellName("header")
                .setRelative(new Rect(0, 77, 12, 1))
                .setSourceCell(new Rect(0, 0, 90, 1))
                .setWidth(AxisDimension.auto(12))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(12, 1))
                .setCellCharsSize(new Size(90, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#1f2937"))
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"))
        );
    }

    protected void onQuitBtnClick()
    {
    }

    protected void onTabListClick()
    {
    }

    protected void onTabOtherClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("quit_btn".equals(name))
        {
            onQuitBtnClick();
            return true;
        }
        if ("tab_list".equals(name))
        {
            onTabListClick();
            return true;
        }
        if ("tab_other".equals(name))
        {
            onTabOtherClick();
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
        return false;
    }
}
