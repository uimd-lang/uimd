// Auto-generated UI code for contacts_manager - DO NOT EDIT MANUALLY.
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

public class ContactsManagerUI extends GeneratedWindowBase
{
    public Label header;
    public Label contacts_label;
    public ListBox contacts;
    public Label detail;
    public Button add_btn;
    public Button edit_btn;
    public Button delete_btn;
    public Button save_btn;
    public Button reload_btn;
    public Button close_btn;
    public Label status;

    public ContactsManagerUI()
    {
        super("Contacts Manager");
        setGeneratedLayout(buildLayout());
        setGeneratedFocusable(false);
        setGeneratedKind("window");
        setMcpMetadata(
            true,
            "ContactsManager",
            "# Contacts Manager\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - contact_form\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Contacts Manager\"\n  description: \"Application title.\"\n  expose: false\n\ncontacts_label:\n  type: label\n  text: Contacts\n  description: \"Label for the contacts list.\"\n\ncontacts:\n  type: listbox\n  options: []\n  selected_items: []\n  multiple: \"false\"\n  description: \"List of contact records; selecting a row updates the detail panel.\"\n\ndetail:\n  type: label\n  text: \"Select a contact.\"\n  description: \"Read-only details for the selected contact.\"\n\nadd_btn:\n  type: button\n  title: Add\n  description: \"Open the contact form to create a new contact.\"\n\nedit_btn:\n  type: button\n  title: Edit\n  description: \"Open the selected contact for editing.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Delete the selected contact.\"\n\nsave_btn:\n  type: button\n  title: \"Save CSV\"\n  description: \"Save contacts to the configured CSV file.\"\n\nreload_btn:\n  type: button\n  title: Reload\n  description: \"Reload contacts from the configured CSV file.\"\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: \"Close the contacts manager.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows save, load, and validation status messages.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\n@list:\n  background: \"#101a2b\"\n@contacts:\n  background: \"#ffffff11\"\n  color: \"#cbd5e1\"\n  focus-background: \"#ffffff22\"\n  edit-background: \"#00000000\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n@detail_cell:\n  background: \"#0b1120\"\n  padding: 1, 1, 1, 1\n@detail:\n  color: \"#d1d5db\"\n  user-select: text\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@status:\n  background: \"#111827\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-title-**-------------------------------------------------------------------------------+\n|header..**...................................................................close_btn..|\n+-actions-**-----------------------------------------------------------------------------+\n|                                                                                        |\n| add_btn....... edit_btn....... delete_btn....... save_btn....... reload_btn.......     |\n|                                                                                        |\n+-list-32---------------------------+-detail_cell-**-------------------------------------+\n| contacts_label.**................ | detail..**........................................ |\n| contacts.......**................ | .................................................. |\n| #................................ | #................................................. |\n| #................................ | #................................................. |\n| ................................. | .................................................. |\n* ................................. | .................................................. |\n* ................................. * .................................................. |\n| ................................. * .................................................. |\n| ................................. | .................................................. |\n|                                   | .................................................. |\n+-----------------------------------+----------------------------------------------------+\n| status..**............................................................................ |\n+----------------------------------------------------------------------------------------+\n```\n",
            "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.",
            List.of(
            new GeneratedElementMetadata("header", "Application title.", false),
            new GeneratedElementMetadata("contacts_label", "Label for the contacts list.", true),
            new GeneratedElementMetadata("contacts", "List of contact records; selecting a row updates the detail panel.", true),
            new GeneratedElementMetadata("detail", "Read-only details for the selected contact.", true),
            new GeneratedElementMetadata("add_btn", "Open the contact form to create a new contact.", true),
            new GeneratedElementMetadata("edit_btn", "Open the selected contact for editing.", true),
            new GeneratedElementMetadata("delete_btn", "Delete the selected contact.", true),
            new GeneratedElementMetadata("save_btn", "Save contacts to the configured CSV file.", true),
            new GeneratedElementMetadata("reload_btn", "Reload contacts from the configured CSV file.", true),
            new GeneratedElementMetadata("close_btn", "Close the contacts manager.", true),
            new GeneratedElementMetadata("status", "Shows save, load, and validation status messages.", true)));
        setMcpAppTools(List.of());
        setGeneratedWindowStyle(Style.fromProperties("background", "#162033", "border-color", "transparent", "border-width-horizontal", "0", "border-width-vertical", "0"));
        header = addElement(new Label("header", "Contacts Manager"));
        header.setStyle(Style.fromProperties("background", "#1f2937", "color", "#ffffff", "text-align", "center"));
        header.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        contacts_label = addElement(new Label("contacts_label", "Contacts"));
        contacts_label.setStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1"));
        contacts_label.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        contacts = addElement(new ListBox("contacts", List.of()));
        contacts.setStyle(Style.fromProperties("background", "#ffffff11", "color", "#cbd5e1"));
        contacts.setFocusStyle(Style.fromProperties("background", "#ffffff22", "color", "#ffffff"));
        contacts.setEditStyle(Style.fromProperties("background", "#00000000", "color", "#cbd5e1"));
        contacts.setSelectedStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        contacts.setActiveStyle(Style.fromProperties("background", "#DDDDDD99", "color", "#FFFFFF"));
        contacts.setDisabledStyle(Style.fromProperties("color", "#64748b"));
        detail = addElement(new Label("detail", "Select a contact."));
        detail.setStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db", "user-select", "text"));
        detail.setCursorStyle(Style.fromProperties("background", "#b8d7ff", "color", "#000000"));
        add_btn = addElement(new Button("add_btn", "Add"));
        add_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        add_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        add_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        edit_btn = addElement(new Button("edit_btn", "Edit"));
        edit_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        edit_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        edit_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        delete_btn = addElement(new Button("delete_btn", "Delete"));
        delete_btn.setStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0"));
        delete_btn.setFocusStyle(Style.fromProperties("background", "#dc2626", "color", "#ffffff"));
        delete_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        save_btn = addElement(new Button("save_btn", "Save CSV"));
        save_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        save_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        save_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
        reload_btn = addElement(new Button("reload_btn", "Reload"));
        reload_btn.setStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"));
        reload_btn.setFocusStyle(Style.fromProperties("background", "#2563eb", "color", "#ffffff"));
        reload_btn.setDisabledStyle(Style.fromProperties("background", "#1f2937", "color", "#64748b"));
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
                .setSourceCell(new Rect(0, 0, 88, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(77, 1))
                .setCellCharsSize(new Size(88, 1))
                .setMarginRight(11)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#1f2937", "color", "#ffffff", "text-align", "center")),
            new GeneratedLayoutEntry()
                .setName("contacts_label")
                .setType("label")
                .setCellName("list")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(6, 0, 35, 10))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.fixed(32))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(33, 1))
                .setCellCharsSize(new Size(35, 10))
                .setMarginRight(1)
                .setMarginBottom(9)
                .setCellStyle(Style.fromProperties("background", "#101a2b"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("detail")
                .setType("label")
                .setCellName("detail_cell")
                .setRelative(new Rect(0, 1, 536870911, 536870910))
                .setSourceCell(new Rect(6, 36, 52, 10))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(50, 10))
                .setCellCharsSize(new Size(52, 10))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(Style.fromProperties("background", "#0b1120", "padding-bottom", "1", "padding-left", "1", "padding-right", "1", "padding-top", "1"))
                .setElementStyle(Style.fromProperties("background", "transparent", "color", "#d1d5db", "user-select", "text")),
            new GeneratedLayoutEntry()
                .setName("status")
                .setType("label")
                .setCellName("")
                .setRelative(new Rect(0, 1, 536870911, 1))
                .setSourceCell(new Rect(17, 0, 88, 1))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.auto(88))
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(86, 1))
                .setCellCharsSize(new Size(88, 1))
                .setMarginRight(1)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#111827", "color", "#93c5fd")),
            new GeneratedLayoutEntry()
                .setName("close_btn")
                .setType("button")
                .setCellName("title")
                .setRelative(new Rect(0, 77, 11, 1))
                .setSourceCell(new Rect(0, 0, 88, 1))
                .setWidth(AxisDimension.auto(11))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(1))
                .setCharsSize(new Size(11, 1))
                .setCellCharsSize(new Size(88, 1))
                .setMarginRight(0)
                .setMarginBottom(0)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("add_btn")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(1, 1, 14, 1))
                .setSourceCell(new Rect(2, 0, 88, 3))
                .setWidth(AxisDimension.auto(14))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(14, 1))
                .setCellCharsSize(new Size(88, 3))
                .setMarginRight(73)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("contacts")
                .setType("listbox")
                .setCellName("list")
                .setRelative(new Rect(1, 1, 536870911, 536870910))
                .setSourceCell(new Rect(6, 0, 35, 10))
                .setWidth(AxisDimension.expanded())
                .setHeight(AxisDimension.fitContent())
                .setCellWidth(AxisDimension.fixed(32))
                .setCellHeight(AxisDimension.expanded())
                .setCharsSize(new Size(33, 8))
                .setCellCharsSize(new Size(35, 10))
                .setMarginRight(1)
                .setMarginBottom(1)
                .setCellStyle(Style.fromProperties("background", "#101a2b"))
                .setElementStyle(Style.fromProperties("background", "#ffffff11", "color", "#cbd5e1")),
            new GeneratedLayoutEntry()
                .setName("edit_btn")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(1, 16, 15, 1))
                .setSourceCell(new Rect(2, 0, 88, 3))
                .setWidth(AxisDimension.auto(15))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(15, 1))
                .setCellCharsSize(new Size(88, 3))
                .setMarginRight(57)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("delete_btn")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(1, 32, 17, 1))
                .setSourceCell(new Rect(2, 0, 88, 3))
                .setWidth(AxisDimension.auto(17))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(17, 1))
                .setCellCharsSize(new Size(88, 3))
                .setMarginRight(39)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#7f1d1d", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("save_btn")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(1, 50, 15, 1))
                .setSourceCell(new Rect(2, 0, 88, 3))
                .setWidth(AxisDimension.auto(15))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(15, 1))
                .setCellCharsSize(new Size(88, 3))
                .setMarginRight(23)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0")),
            new GeneratedLayoutEntry()
                .setName("reload_btn")
                .setType("button")
                .setCellName("actions")
                .setRelative(new Rect(1, 66, 17, 1))
                .setSourceCell(new Rect(2, 0, 88, 3))
                .setWidth(AxisDimension.auto(17))
                .setHeight(AxisDimension.auto(1))
                .setCellWidth(AxisDimension.expanded())
                .setCellHeight(AxisDimension.auto(3))
                .setCharsSize(new Size(17, 1))
                .setCellCharsSize(new Size(88, 3))
                .setMarginRight(5)
                .setMarginBottom(1)
                .setCellStyle(new Style())
                .setElementStyle(Style.fromProperties("background", "#334155", "color", "#e2e8f0"))
        );
    }

    protected void onContactsSelectionChange(List<String> value)
    {
    }

    protected void onAddBtnClick()
    {
    }

    protected void onEditBtnClick()
    {
    }

    protected void onDeleteBtnClick()
    {
    }

    protected void onSaveBtnClick()
    {
    }

    protected void onReloadBtnClick()
    {
    }

    protected void onCloseBtnClick()
    {
    }

    @Override
    public boolean handleGeneratedButton(String name)
    {
        if ("add_btn".equals(name))
        {
            onAddBtnClick();
            return true;
        }
        if ("edit_btn".equals(name))
        {
            onEditBtnClick();
            return true;
        }
        if ("delete_btn".equals(name))
        {
            onDeleteBtnClick();
            return true;
        }
        if ("save_btn".equals(name))
        {
            onSaveBtnClick();
            return true;
        }
        if ("reload_btn".equals(name))
        {
            onReloadBtnClick();
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
        if ("contacts".equals(name))
        {
            onContactsSelectionChange(value);
            return true;
        }
        return false;
    }
}
