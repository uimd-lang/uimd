// Auto-generated UI code for markdown_viewer - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type MarkdownViewerUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Header *uimd.Label
    DocsLabel *uimd.Label
    Docs *uimd.ListBox
    UpBtn *uimd.Button
    DownBtn *uimd.Button
    Viewer *uimd.ReusableElement
    CloseBtn *uimd.Button
    Status *uimd.Label
}

func NewMarkdownViewerUI() *MarkdownViewerUI {
    ui := &MarkdownViewerUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Markdown Viewer")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 0, Col: 0,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 67, CellCharsHeight: 1,
            CellName: "title", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 58, CharsHeight: 1,
            MarginRight: 9, MarginBottom: 0,
            Content: "header",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#223a5a"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    },
        },
        {
            Row: 0, Col: 0,
            CellRow: 2, CellCol: 27,
            CellCharsWidth: 40, CellCharsHeight: 11,
            CellName: "content", CellWidth: 0, CellHeight: 11,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: uimd.Expanded,
            WidthMode: "expanded", HeightMode: "expanded",
            CharsWidth: 40, CharsHeight: 11,
            MarginRight: 0, MarginBottom: 0,
            Content: "viewer",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#000000"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#000000"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(2),
        PaddingRight: uimd.Int(2),
        PaddingTop: uimd.Int(1),
    },
        },
        {
            Row: 0, Col: 1,
            CellRow: 14, CellCol: 0,
            CellCharsWidth: 67, CellCharsHeight: 1,
            CellName: "", CellWidth: 67, CellHeight: 1,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 65, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 0,
            Content: "status",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#111827"),
        Color: uimd.NewColor("#93c5fd"),
    },
        },
        {
            Row: 0, Col: 58,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 67, CellCharsHeight: 1,
            CellName: "title", CellWidth: 0, CellHeight: 1,
            CellWidthMode: "expanded", CellHeightMode: "auto",
            Width: 9, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 9, CharsHeight: 1,
            MarginRight: 0, MarginBottom: 0,
            Content: "close_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#223a5a"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#7f1d1d"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 1, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 26, CellCharsHeight: 11,
            CellName: "sidebar", CellWidth: 26, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 24, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 9,
            Content: "docs_label",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#101a2b"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 2, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 26, CellCharsHeight: 11,
            CellName: "sidebar", CellWidth: 26, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: uimd.FitContent,
            WidthMode: "expanded", HeightMode: "fit-content",
            CharsWidth: 24, CharsHeight: 4,
            MarginRight: 1, MarginBottom: 5,
            Content: "docs",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#101a2b"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#ffffff11"),
        Color: uimd.NewColor("#cbd5e1"),
    },
        },
        {
            Row: 7, Col: 1,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 26, CellCharsHeight: 11,
            CellName: "sidebar", CellWidth: 26, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: 10, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 10, CharsHeight: 1,
            MarginRight: 15, MarginBottom: 3,
            Content: "up_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#101a2b"),
    },
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 7, Col: 12,
            CellRow: 2, CellCol: 0,
            CellCharsWidth: 26, CellCharsHeight: 11,
            CellName: "sidebar", CellWidth: 26, CellHeight: 0,
            CellWidthMode: "auto", CellHeightMode: "expanded",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 13, CharsHeight: 1,
            MarginRight: 1, MarginBottom: 3,
            Content: "down_btn",
            CellStyle: uimd.Style{
        Background: uimd.NewColor("#101a2b"),
    },
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
    ui.SetMCPMetadata(true, "MarkdownViewer", "# Markdown Viewer\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.\"\ntags: [example]\n```\n\n## Definition\n\n```yaml\nuses:\n  - document_view\n```\n\n## Members\n\n```yaml\nheader:\n  type: label\n  text: \"Markdown Viewer\"\n  description: \"Application title.\"\n  expose: false\n\ndocs_label:\n  type: label\n  text: Documents\n  description: \"Label for the document list.\"\n\ndocs:\n  type: listbox\n  options: [overview.markdown, styles.markdown, tables.markdown, code.markdown]\n  selected_items: [overview.markdown]\n  multiple: \"false\"\n  description: \"Document listbox used to select the markdown document to render.\"\n\nup_btn:\n  type: button\n  title: up\n  description: \"Scroll the rendered document upward.\"\n\ndown_btn:\n  type: button\n  title: down\n  description: \"Scroll the rendered document downward.\"\n\nviewer:\n  type: document_view\n  cpp-class: \"markdown_viewer_example::DocumentView\"\n  cpp-header: document_view/document_view.hpp\n  description: \"Scrollable rendered markdown document area.\"\n\nclose_btn:\n  type: button\n  title: Quit\n  width: 9\n  description: \"Close the markdown viewer.\"\n\nstatus:\n  type: label\n  text: Ready\n  description: \"Shows the current document and scroll state.\"\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\n@header:\n  text-align: center\n  color: \"#ffffff\"\n@title:\n  background: \"#223a5a\"\n@sidebar:\n  background: \"#101a2b\"\n@content:\n  background: \"#000000\"\n@viewer:\n  background: \"#000000\"\n  padding: 1, 2, 1, 2\n@docs:\n  background: \"#ffffff11\"\n  color: \"#cbd5e1\"\n  focus-background: \"#ffffff22\"\n  edit-background: \"#00000000\"\n  selected-background: \"#2563eb\"\n  selected-color: \"#ffffff\"\nbutton:\n  background: \"#334155\"\n  color: \"#e2e8f0\"\n  focus-background: \"#2b5f9f\"\n@close_btn:\n  background: \"#7f1d1d\"\n  color: \"#e2e8f0\"\n  focus-background: \"#dc2626\"\n@status:\n  color: \"#93c5fd\"\n  background: \"#111827\"\n```\n\n## User Interface\n\n```ui\n+-title-**----------------------------------------------------------+\n|header..**................................................close_btn|\n+-sidebar------------------+-content-**-----------------------------+\n|                          |viewer..**..............................|\n| docs_label...**......... |........................................|\n| docs.........**......... |*.......................................|\n| ........................ |*.......................................|\n| #....................... |........................................|\n| #....................... |........................................|\n|                          |........................................|\n| up_btn.... down_btn..... |........................................|\n*                          |........................................|\n*                          |........................................|\n|                          |........................................|\n+--------------------------+----------------------------------------+\n| status..**....................................................... |\n+-------------------------------------------------------------------+\n```\n", "Markdown viewer for selecting example documents, scrolling rendered content, and switching between overview, styles, tables, and code documents.")
    ui.Header = uimd.NewLabel("header", "Markdown Viewer")
    ui.AddElement(ui.Header)
    ui.Header.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#ffffff"),
        TextAlign: "center",
    })
    ui.Header.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.DocsLabel = uimd.NewLabel("docs_label", "Documents")
    ui.AddElement(ui.DocsLabel)
    ui.DocsLabel.SetStyle(uimd.Style{
        Background: uimd.NewColor("transparent"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.DocsLabel.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.Docs = uimd.NewListBox("docs", []string{"overview.markdown", "styles.markdown", "tables.markdown", "code.markdown"}, false)
    ui.AddElement(ui.Docs)
    ui.Docs.SetSelectedIndex(0)
    ui.Docs.SetStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff11"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Docs.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff22"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Docs.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#00000000"),
        Color: uimd.NewColor("#cbd5e1"),
    })
    ui.Docs.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#2563eb"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Docs.SetActiveStyle(uimd.Style{
        Background: uimd.NewColor("#DDDDDD99"),
        Color: uimd.NewColor("#FFFFFF"),
    })
    ui.Docs.SetDisabledStyle(uimd.Style{
        Color: uimd.NewColor("#64748b"),
    })
    ui.UpBtn = uimd.NewButton("up_btn", "up")
    ui.AddElement(ui.UpBtn)
    ui.UpBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.UpBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.UpBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.DownBtn = uimd.NewButton("down_btn", "down")
    ui.AddElement(ui.DownBtn)
    ui.DownBtn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.DownBtn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.DownBtn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Viewer = uimd.NewReusableElement("viewer", "document_view")
    ui.AddElement(ui.Viewer)
    ui.Viewer.SetStyle(uimd.Style{
        Background: uimd.NewColor("#000000"),
        PaddingBottom: uimd.Int(1),
        PaddingLeft: uimd.Int(2),
        PaddingRight: uimd.Int(2),
        PaddingTop: uimd.Int(1),
    })
    ui.Viewer.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff14"),
    })
    ui.Viewer.SetEditStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff18"),
    })
    ui.Viewer.SetSelectedStyle(uimd.Style{
        Background: uimd.NewColor("#ffffff26"),
        Color: uimd.NewColor("#ffffff"),
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

func (ui *MarkdownViewerUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *MarkdownViewerUI) HandleGeneratedButton(name string) bool {
    if name == "up_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnUpBtnClick() }); ok {
            handler.OnUpBtnClick()
            return true
        }
        return false
    }
    if name == "down_btn" {
        if handler, ok := ui.eventHandler.(interface{ OnDownBtnClick() }); ok {
            handler.OnDownBtnClick()
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

func (ui *MarkdownViewerUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *MarkdownViewerUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *MarkdownViewerUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    if name == "docs" {
        if handler, ok := ui.eventHandler.(interface{ OnDocsSelectionChange([]string) }); ok {
            handler.OnDocsSelectionChange(value)
            return true
        }
        return false
    }
    return false
}
