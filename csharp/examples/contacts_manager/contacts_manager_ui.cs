// Auto-generated UI code for contacts_manager - DO NOT EDIT MANUALLY.
using Uimd;

public class ContactsManagerUI : GeneratedWindowBase
{
    public Label header = null!;
    public Label contacts_label = null!;
    public ListBox contacts = null!;
    public Label detail = null!;
    public Button add_btn = null!;
    public Button edit_btn = null!;
    public Button delete_btn = null!;
    public Button save_btn = null!;
    public Button reload_btn = null!;
    public Button close_btn = null!;
    public Label status = null!;

    public ContactsManagerUI() : base("Contacts Manager")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "ContactsManager", """
# Contacts Manager

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage."
tags: [example]
```

## Definition

```yaml
uses:
  - contact_form
```

## Members

```yaml
header:
  type: label
  text: "Contacts Manager"
  description: "Application title."
  expose: false

contacts_label:
  type: label
  text: Contacts
  description: "Label for the contacts list."

contacts:
  type: listbox
  options: []
  selected_items: []
  multiple: "false"
  description: "List of contact records; selecting a row updates the detail panel."

detail:
  type: label
  text: "Select a contact."
  description: "Read-only details for the selected contact."

add_btn:
  type: button
  title: Add
  description: "Open the contact form to create a new contact."

edit_btn:
  type: button
  title: Edit
  description: "Open the selected contact for editing."

delete_btn:
  type: button
  title: Delete
  description: "Delete the selected contact."

save_btn:
  type: button
  title: "Save CSV"
  description: "Save contacts to the configured CSV file."

reload_btn:
  type: button
  title: Reload
  description: "Reload contacts from the configured CSV file."

close_btn:
  type: button
  title: Quit
  width: 9
  description: "Close the contacts manager."

status:
  type: label
  text: Ready
  description: "Shows save, load, and validation status messages."
```

## Style

```yaml
include: dark
this:
  border-width: 0
@header:
  text-align: center
  background: "#1f2937"
  color: "#ffffff"
@list:
  background: "#101a2b"
@contacts:
  background: "#ffffff11"
  color: "#cbd5e1"
  focus-background: "#ffffff22"
  edit-background: "#00000000"
  selected-background: "#2563eb"
  selected-color: "#ffffff"
@detail_cell:
  background: "#0b1120"
  padding: 1, 1, 1, 1
@detail:
  color: "#d1d5db"
  user-select: text
button:
  background: "#334155"
  color: "#e2e8f0"
  focus-background: "#2563eb"
@delete_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@close_btn:
  background: "#7f1d1d"
  focus-background: "#dc2626"
@status:
  background: "#111827"
  color: "#93c5fd"
```

## User Interface

```ui
+-title-**-------------------------------------------------------------------------------+
|header..**...................................................................close_btn..|
+-actions-**-----------------------------------------------------------------------------+
|                                                                                        |
| add_btn....... edit_btn....... delete_btn....... save_btn....... reload_btn.......     |
|                                                                                        |
+-list-32---------------------------+-detail_cell-**-------------------------------------+
| contacts_label.**................ | detail..**........................................ |
| contacts.......**................ | .................................................. |
| #................................ | #................................................. |
| #................................ | #................................................. |
| ................................. | .................................................. |
* ................................. | .................................................. |
* ................................. * .................................................. |
| ................................. * .................................................. |
| ................................. | .................................................. |
|                                   | .................................................. |
+-----------------------------------+----------------------------------------------------+
| status..**............................................................................ |
+----------------------------------------------------------------------------------------+
```

""", "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.", new[]
        {
            new GeneratedElementMetadata { Name = "header", Description = "Application title.", Expose = false },
            new GeneratedElementMetadata { Name = "contacts_label", Description = "Label for the contacts list.", Expose = true },
            new GeneratedElementMetadata { Name = "contacts", Description = "List of contact records; selecting a row updates the detail panel.", Expose = true },
            new GeneratedElementMetadata { Name = "detail", Description = "Read-only details for the selected contact.", Expose = true },
            new GeneratedElementMetadata { Name = "add_btn", Description = "Open the contact form to create a new contact.", Expose = true },
            new GeneratedElementMetadata { Name = "edit_btn", Description = "Open the selected contact for editing.", Expose = true },
            new GeneratedElementMetadata { Name = "delete_btn", Description = "Delete the selected contact.", Expose = true },
            new GeneratedElementMetadata { Name = "save_btn", Description = "Save contacts to the configured CSV file.", Expose = true },
            new GeneratedElementMetadata { Name = "reload_btn", Description = "Reload contacts from the configured CSV file.", Expose = true },
            new GeneratedElementMetadata { Name = "close_btn", Description = "Close the contacts manager.", Expose = true },
            new GeneratedElementMetadata { Name = "status", Description = "Shows save, load, and validation status messages.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        header = AddElement(new Label("header", "Contacts Manager"));
        header.SetStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    });
        header.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        contacts_label = AddElement(new Label("contacts_label", "Contacts"));
        contacts_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        contacts_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        contacts = AddElement(new ListBox("contacts", new string[] {}));
        contacts.SetStyle(new Style
    {
        Background = new Color("#ffffff11"),
        Color = new Color("#cbd5e1")
    });
        contacts.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff22"),
        Color = new Color("#ffffff")
    });
        contacts.SetEditStyle(new Style
    {
        Background = new Color("#00000000"),
        Color = new Color("#cbd5e1")
    });
        contacts.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        contacts.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        detail = AddElement(new Label("detail", "Select a contact."));
        detail.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db"),
        UserSelect = "text"
    });
        detail.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        add_btn = AddElement(new Button("add_btn", "Add"));
        add_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        add_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        add_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        edit_btn = AddElement(new Button("edit_btn", "Edit"));
        edit_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        edit_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        edit_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        delete_btn = AddElement(new Button("delete_btn", "Delete"));
        delete_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    });
        delete_btn.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        delete_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        save_btn = AddElement(new Button("save_btn", "Save CSV"));
        save_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        save_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        save_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        reload_btn = AddElement(new Button("reload_btn", "Reload"));
        reload_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        reload_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        reload_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        close_btn = AddElement(new Button("close_btn", "Quit"));
        close_btn.SetStyle(new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    });
        close_btn.SetFocusStyle(new Style
    {
        Background = new Color("#dc2626"),
        Color = new Color("#ffffff")
    });
        close_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        status = AddElement(new Label("status", "Ready"));
        status.SetStyle(new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#93c5fd")
    });
        status.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "header",
                Type = "label",
                CellName = "title",
                Relative = new Rect(0, 0, 536870911, 1),
                SourceCell = new Rect(0, 0, 88, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(77, 1),
                CellCharsSize = new Size(88, 1),
                MarginRight = 11,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#ffffff"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "contacts_label",
                Type = "label",
                CellName = "list",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(6, 0, 35, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Fixed(32),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(33, 1),
                CellCharsSize = new Size(35, 10),
                MarginRight = 1,
                MarginBottom = 9,
                CellStyle = new Style
    {
        Background = new Color("#101a2b")
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "detail",
                Type = "label",
                CellName = "detail_cell",
                Relative = new Rect(0, 1, 536870911, 536870910),
                SourceCell = new Rect(6, 36, 52, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.FitContent(),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(50, 10),
                CellCharsSize = new Size(52, 10),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style
    {
        Background = new Color("#0b1120"),
        PaddingBottom = 1,
        PaddingLeft = 1,
        PaddingRight = 1,
        PaddingTop = 1
    },
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#d1d5db"),
        UserSelect = "text"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "status",
                Type = "label",
                CellName = "",
                Relative = new Rect(0, 1, 536870911, 1),
                SourceCell = new Rect(17, 0, 88, 1),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(88),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(86, 1),
                CellCharsSize = new Size(88, 1),
                MarginRight = 1,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#111827"),
        Color = new Color("#93c5fd")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "close_btn",
                Type = "button",
                CellName = "title",
                Relative = new Rect(0, 77, 11, 1),
                SourceCell = new Rect(0, 0, 88, 1),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(1),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(88, 1),
                MarginRight = 0,
                MarginBottom = 0,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "add_btn",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(1, 1, 14, 1),
                SourceCell = new Rect(2, 0, 88, 3),
                Width = AxisDimension.Auto(14),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(14, 1),
                CellCharsSize = new Size(88, 3),
                MarginRight = 73,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "contacts",
                Type = "listbox",
                CellName = "list",
                Relative = new Rect(1, 1, 536870911, 536870910),
                SourceCell = new Rect(6, 0, 35, 10),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.FitContent(),
                CellWidth = AxisDimension.Fixed(32),
                CellHeight = AxisDimension.Expanded(),
                CharsSize = new Size(33, 8),
                CellCharsSize = new Size(35, 10),
                MarginRight = 1,
                MarginBottom = 1,
                CellStyle = new Style
    {
        Background = new Color("#101a2b")
    },
                ElementStyle = new Style
    {
        Background = new Color("#ffffff11"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "edit_btn",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(1, 16, 15, 1),
                SourceCell = new Rect(2, 0, 88, 3),
                Width = AxisDimension.Auto(15),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(15, 1),
                CellCharsSize = new Size(88, 3),
                MarginRight = 57,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "delete_btn",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(1, 32, 17, 1),
                SourceCell = new Rect(2, 0, 88, 3),
                Width = AxisDimension.Auto(17),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(17, 1),
                CellCharsSize = new Size(88, 3),
                MarginRight = 39,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#7f1d1d"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "save_btn",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(1, 50, 15, 1),
                SourceCell = new Rect(2, 0, 88, 3),
                Width = AxisDimension.Auto(15),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(15, 1),
                CellCharsSize = new Size(88, 3),
                MarginRight = 23,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "reload_btn",
                Type = "button",
                CellName = "actions",
                Relative = new Rect(1, 66, 17, 1),
                SourceCell = new Rect(2, 0, 88, 3),
                Width = AxisDimension.Auto(17),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Expanded(),
                CellHeight = AxisDimension.Auto(3),
                CharsSize = new Size(17, 1),
                CellCharsSize = new Size(88, 3),
                MarginRight = 5,
                MarginBottom = 1,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    },
            },
        };
    }

    protected virtual void onContactsSelectionChange(List<string> value) { }
    protected virtual void onAddBtnClick() { }
    protected virtual void onEditBtnClick() { }
    protected virtual void onDeleteBtnClick() { }
    protected virtual void onSaveBtnClick() { }
    protected virtual void onReloadBtnClick() { }
    protected virtual void onCloseBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "add_btn")
        {
            onAddBtnClick();
            return true;
        }
        else if (name == "edit_btn")
        {
            onEditBtnClick();
            return true;
        }
        else if (name == "delete_btn")
        {
            onDeleteBtnClick();
            return true;
        }
        else if (name == "save_btn")
        {
            onSaveBtnClick();
            return true;
        }
        else if (name == "reload_btn")
        {
            onReloadBtnClick();
            return true;
        }
        else if (name == "close_btn")
        {
            onCloseBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "contacts")
        {
            onContactsSelectionChange(value);
            return true;
        }
        return false;
    }
}
