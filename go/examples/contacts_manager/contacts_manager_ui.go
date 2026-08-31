// Auto-generated UI code for contacts_manager - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type ContactsManagerUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Header *uimd.Label
    ContactsLabel *uimd.Label
    Contacts *uimd.ListBox
    Detail *uimd.Label
    AddBtn *uimd.Button
    EditBtn *uimd.Button
    DeleteBtn *uimd.Button
    SaveBtn *uimd.Button
    ReloadBtn *uimd.Button
    CloseBtn *uimd.Button
    Status *uimd.Label
}

func NewContactsManagerUI() *ContactsManagerUI {
    ui := &ContactsManagerUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Contacts Manager")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 88, CellCharsHeight: 1,
            CellName: "title", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 77, CharsHeight: 1,
            MarginRight: 11, MarginBottom: 0,
            Content: "header",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 6, CellCol: 0,
            CellCharsWidth: 35, CellCharsHeight: 10,
            CellName: "list", CellWidth: 32, CellHeight: 0,
            CellWidthMode: "fixed", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 33, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 9,
            Content: "contacts_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#101a2b"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 6, CellCol: 36,
            CellCharsWidth: 52, CellCharsHeight: 10,
            CellName: "detail_cell", CellWidth: 0, CellHeight: 0,
            CellWidthMode: "expanded", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 50, CharsHeight: 10,
            MarginRight: 1, MarginBottom: 0,
            Content: "detail",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#0b1120"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(1),
        PaddingRight: uimd.Int(1),
        PaddingTop: uimd.Int(1),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
        UserSelect: "text",
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 17, CellCol: 0,
            CellCharsWidth: 88, CellCharsHeight: 1,
            CellName: "", CellWidth: 88, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 86, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "status",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 0, Col: 77,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 88, CellCharsHeight: 1,
            CellName: "title", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 11, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 11, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "close_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 88, CellCharsHeight: 3,
            CellName: "actions", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 14, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 14, CharsHeight: 1,
            MarginRight: 73, MarginBottom: 1,
            Content: "add_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 6, CellCol: 0,
            CellCharsWidth: 35, CellCharsHeight: 10,
            CellName: "list", CellWidth: 32, CellHeight: 0,
            CellWidthMode: "fixed", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 33, CharsHeight: 8,
            MarginRight: 1, MarginBottom: 1,
            Content: "contacts",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#101a2b"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#ffffff11"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 1, Col: 16,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 88, CellCharsHeight: 3,
            CellName: "actions", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 15, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 15, CharsHeight: 1,
            MarginRight: 57, MarginBottom: 1,
            Content: "edit_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 32,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 88, CellCharsHeight: 3,
            CellName: "actions", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 17, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 17, CharsHeight: 1,
            MarginRight: 39, MarginBottom: 1,
            Content: "delete_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 50,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 88, CellCharsHeight: 3,
            CellName: "actions", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 15, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 15, CharsHeight: 1,
            MarginRight: 23, MarginBottom: 1,
            Content: "save_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 66,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 88, CellCharsHeight: 3,
            CellName: "actions", CellWidth: 0, CellHeight: 3,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 17, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 17, CharsHeight: 1,
            MarginRight: 5, MarginBottom: 1,
            Content: "reload_btn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
    })
    ui.SetGeneratedKind("window")
    ui.SetGeneratedFocusable(false)
    ui.SetGeneratedWindowStyle(uimd.Style{
        Background: uimd.NewColor("#162033"),
        BorderColor: uimd.NewColor("transparent"),
        BorderWidthHorizontal: uimd.Int(0),
        BorderWidthVertical: uimd.Int(0),
    })
    ui.SetMCPMetadata(true, "ContactsManager", "# Contacts Manager\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - contact_form\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Contacts Manager\"\n  description: \"Application title.\"\n  expose: false\n\ncontacts_label:\n  type: label\n  text: Contacts\n  description: \"Label for the contacts list.\"\n\ncontacts:\n  type: listbox\n  options: []\n  selected_items: []\n  multiple: \"false\"\n  description: \"List of contact records; selecting a row updates the detail panel.\"\n\ndetail:\n  type: label\n  text: \"Select a contact.\"\n  description: \"Read-only details for the selected contact.\"\n\nadd_btn:\n  type: button\n  title: Add\n  description: \"Open the contact form to create a new contact.\"\n\nedit_btn:\n  type: button\n  title: Edit\n  description: \"Open the selected contact for editing.\"\n\ndelete_btn:\n  type: button\n  title: Delete\n  description: \"Delete the selected contact.\"\n\nsave_btn:\n  type: button\n  title: \"Save CSV\"\n  description: \"Save contacts to the configured CSV file.\"\n\nreload_btn:\n  type: button\n  title: Reload\n  description: \"Reload contacts from the configured CSV file.\"\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: \"Close the contacts manager.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows save, load, and validation status messages.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  background: \"#1f2937\"\n  color: \"#ffffff\"\n@list:\n  background: \"#101a2b\"\n@contacts:\n  background: \"#ffffff11\"\n  color: \"#cbd5e1\"\n  focus-background: \"#ffffff22\"\n  edit-background: \"#00000000\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\n@detail_cell:\n  background: \"#0b1120\"\n  padding: 1, 1, 1, 1\n@detail:\n  color: \"#d1d5db\"\n  user-select: text\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2563eb\"\n@delete_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@close_btn:\n  background: \"#7f1d1d\"\n  focus-background: \"#dc2626\"\n@status:\n  background: \"#111827\"\n  color: \"#93c5fd\"\n```\n\n## User Interface\n\n```ui\n+-title-**-------------------------------------------------------------------------------+\n|header..**...................................................................close_btn..|\n+-actions-**-----------------------------------------------------------------------------+\n|                                                                                        |\n| add_btn....... edit_btn....... delete_btn....... save_btn....... reload_btn.......     |\n|                                                                                        |\n+-list-32---------------------------+-detail_cell-**-------------------------------------+\n| contacts_label.**................ | detail..**........................................ |\n| contacts.......**................ | .................................................. |\n| #................................ | #................................................. |\n| #................................ | #................................................. |\n| ................................. | .................................................. |\n* ................................. | .................................................. |\n* ................................. * .................................................. |\n| ................................. * .................................................. |\n| ................................. | .................................................. |\n|                                   | .................................................. |\n+-----------------------------------+----------------------------------------------------+\n| status..**............................................................................ |\n+----------------------------------------------------------------------------------------+\n```\n", "Contacts management application for browsing contacts, viewing details, adding, editing, deleting, saving CSV data, and reloading from storage.")
    ui.Header = uimd.NewLabel("header", "Contacts Manager")
    ui.AddElement(ui.Header)
    ui.Header.SetStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.Header.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.ContactsLabel = uimd.NewLabel("contacts_label", "Contacts")
    ui.AddElement(ui.ContactsLabel)
    ui.ContactsLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.ContactsLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Contacts = uimd.NewListBox("contacts", []string{}, false)
    ui.AddElement(ui.Contacts)
    ui.Contacts.SetStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff11"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Contacts.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff22"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Contacts.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#00000000"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Contacts.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Contacts.SetActiveStyle(uimd.Style{
        Background: uimd.NewColor("#DDDDDD99"),
        Color: uimd.NewColor("#FFFFFF"),
    })
    ui.Contacts.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.Detail = uimd.NewLabel("detail", "Select a contact.")
    ui.AddElement(ui.Detail)
    ui.Detail.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#d1d5db"),
        UserSelect: "text",
    })
    ui.Detail.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.AddBtn = uimd.NewButton("add_btn", "Add")
    ui.AddElement(ui.AddBtn)
    ui.AddBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.AddBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.AddBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.EditBtn = uimd.NewButton("edit_btn", "Edit")
    ui.AddElement(ui.EditBtn)
    ui.EditBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.EditBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.EditBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.DeleteBtn = uimd.NewButton("delete_btn", "Delete")
    ui.AddElement(ui.DeleteBtn)
    ui.DeleteBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.DeleteBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DeleteBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.SaveBtn = uimd.NewButton("save_btn", "Save CSV")
    ui.AddElement(ui.SaveBtn)
    ui.SaveBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.SaveBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.SaveBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.ReloadBtn = uimd.NewButton("reload_btn", "Reload")
    ui.AddElement(ui.ReloadBtn)
    ui.ReloadBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.ReloadBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.ReloadBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.CloseBtn = uimd.NewButton("close_btn", "Quit")
    ui.AddElement(ui.CloseBtn)
    ui.CloseBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.CloseBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#dc2626"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.CloseBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Status = uimd.NewLabel("status", "Ready")
    ui.AddElement(ui.Status)
    ui.Status.SetStyle(uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#93c5fd"),
    })
    ui.Status.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    return ui
}

func (ui *ContactsManagerUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *ContactsManagerUI) HandleGeneratedButton(name string) bool {
    if name == "add_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnAddBtnClick() }); ok {
            handler.OnAddBtnClick()
            return true
        }
        return false
    }
    if name == "edit_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnEditBtnClick() }); ok {
            handler.OnEditBtnClick()
            return true
        }
        return false
    }
    if name == "delete_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnDeleteBtnClick() }); ok {
            handler.OnDeleteBtnClick()
            return true
        }
        return false
    }
    if name == "save_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnSaveBtnClick() }); ok {
            handler.OnSaveBtnClick()
            return true
        }
        return false
    }
    if name == "reload_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnReloadBtnClick() }); ok {
            handler.OnReloadBtnClick()
            return true
        }
        return false
    }
    if name == "close_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnCloseBtnClick() }); ok {
            handler.OnCloseBtnClick()
            return true
        }
        return false
    }
    return false
}

func (ui *ContactsManagerUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *ContactsManagerUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *ContactsManagerUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "contacts" {
        if handler, ok := ui.eventHandler.(interface{ OnContactsSelectionChange([]string) }); ok {
            handler.OnContactsSelectionChange(value)
            return true
        }
        return false
    }
    return false
}

func (ui *ContactsManagerUI) HandleGeneratedListBoxItemActivate(name string, elementID string, index int, value string) bool {
    _ = elementID
    if name == "contacts" {
        if handler, ok := ui.eventHandler.(interface{ OnContactsItemActivate(int, string) bool }); ok {
            return handler.OnContactsItemActivate(index, value)
        }
        return false
    }
    return false
}

func (ui *ContactsManagerUI) HandleGeneratedPreviewKey(event uimd.KeyEvent) bool {
    if handler, ok := ui.eventHandler.(interface{ OnPreviewKey(uimd.KeyEvent) bool }); ok {
        return handler.OnPreviewKey(event)
    }
    return false
}

func (ui *ContactsManagerUI) HandleGeneratedKey(key string) bool {
    if handler, ok := ui.eventHandler.(interface{ OnKey(string) bool }); ok {
        return handler.OnKey(key)
    }
    return false
}
