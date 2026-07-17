// Auto-generated UI code for calculator - DO NOT EDIT MANUALLY.
package main

import uimd "uimd"

type CalculatorUI struct {
    *uimd.GeneratedWindowBase
    eventHandler any
    Display *uimd.Label
    N0 *uimd.Button
    N1 *uimd.Button
    N2 *uimd.Button
    N3 *uimd.Button
    N4 *uimd.Button
    N5 *uimd.Button
    N6 *uimd.Button
    N7 *uimd.Button
    N8 *uimd.Button
    N9 *uimd.Button
    Sb *uimd.Button
    Dt *uimd.Button
    Ac *uimd.Button
    Dl *uimd.Button
    Pr *uimd.Button
    Dv *uimd.Button
    Ml *uimd.Button
    Mn *uimd.Button
    Ad *uimd.Button
}

func NewCalculatorUI() *CalculatorUI {
    ui := &CalculatorUI{GeneratedWindowBase: uimd.NewGeneratedWindowBase("Calculator")}
    ui.SetGeneratedLayout([]uimd.LayoutItem{
        {
            Row: 1, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 18, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 11,
            Content: "display",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#32446d"),
        Color: uimd.NewColor("#c8e0ff"),
        TextAlign: "right",
    },
        },
        {
            Row: 3, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 17, MarginBottom: 9,
            Content: "ac",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 7,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 12, MarginBottom: 9,
            Content: "dl",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 12,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 7, MarginBottom: 9,
            Content: "pr",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 3, Col: 17,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 9,
            Content: "dv",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 5, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 17, MarginBottom: 7,
            Content: "n7",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 5, Col: 7,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 12, MarginBottom: 7,
            Content: "n8",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 5, Col: 12,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 7, MarginBottom: 7,
            Content: "n9",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 5, Col: 17,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 7,
            Content: "ml",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 7, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 17, MarginBottom: 5,
            Content: "n4",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 7, Col: 7,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 12, MarginBottom: 5,
            Content: "n5",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 7, Col: 12,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 7, MarginBottom: 5,
            Content: "n6",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 7, Col: 17,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 5,
            Content: "mn",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 9, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 17, MarginBottom: 3,
            Content: "n1",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 9, Col: 7,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 12, MarginBottom: 3,
            Content: "n2",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 9, Col: 12,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 7, MarginBottom: 3,
            Content: "n3",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 9, Col: 17,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 3,
            Content: "sb",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 11, Col: 2,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 8, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 8, CharsHeight: 1,
            MarginRight: 12, MarginBottom: 1,
            Content: "n0",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 11, Col: 12,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: 3, Height: 1,
            WidthMode: "auto", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 7, MarginBottom: 1,
            Content: "dt",
            CellStyle: uimd.NewStyle(),
            ElementStyle: uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    },
        },
        {
            Row: 11, Col: 17,
            CellRow: 0, CellCol: 0,
            CellCharsWidth: 22, CellCharsHeight: 13,
            CellName: "", CellWidth: 22, CellHeight: 13,
            CellWidthMode: "auto", CellHeightMode: "auto",
            Width: uimd.Expanded, Height: 1,
            WidthMode: "expanded", HeightMode: "auto",
            CharsWidth: 3, CharsHeight: 1,
            MarginRight: 2, MarginBottom: 1,
            Content: "ad",
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
    ui.SetMCPMetadata(true, "Calculator", "# Calculator\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\nstatus: stable\ndescription: \"Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.\"\ntags: [example]\n```\n\n## Members\n\n```yaml\ndisplay:\n  type: label\n  text: \"\"\n  description: \"Shows the current number, operator result, or calculation state.\"\n\nn0:\n  type: button\n  title: 0\n  description: \"Enter digit 0.\"\n\nn1:\n  type: button\n  title: 1\n  description: \"Enter digit 1.\"\n\nn2:\n  type: button\n  title: 2\n  description: \"Enter digit 2.\"\n\nn3:\n  type: button\n  title: 3\n  description: \"Enter digit 3.\"\n\nn4:\n  type: button\n  title: 4\n  description: \"Enter digit 4.\"\n\nn5:\n  type: button\n  title: 5\n  description: \"Enter digit 5.\"\n\nn6:\n  type: button\n  title: 6\n  description: \"Enter digit 6.\"\n\nn7:\n  type: button\n  title: 7\n  description: \"Enter digit 7.\"\n\nn8:\n  type: button\n  title: 8\n  description: \"Enter digit 8.\"\n\nn9:\n  type: button\n  title: 9\n  description: \"Enter digit 9.\"\n\nsb:\n  type: button\n  title: \"=\"\n  description: \"Evaluate the pending calculation.\"\n\ndt:\n  type: button\n  title: .\n  description: \"Insert a decimal point into the current number.\"\n\nac:\n  type: button\n  title: AC\n  description: \"Clear the current calculator state.\"\n\ndl:\n  type: button\n  title: DEL\n  description: \"Delete the last entered character.\"\n\npr:\n  type: button\n  title: \"%\"\n  description: \"Convert or apply percentage behavior for the current value.\"\n\ndv:\n  type: button\n  title: /\n  description: \"Choose division as the pending operator.\"\n\nml:\n  type: button\n  title: \"*\"\n  description: \"Choose multiplication as the pending operator.\"\n\nmn:\n  type: button\n  title: -\n  description: \"Choose subtraction as the pending operator.\"\n\nad:\n  type: button\n  title: \"+\"\n  description: \"Choose addition as the pending operator.\"\n```\n\n## Style\n\n```yaml\n@display:\n  background: #32446d\n  color: #c8e0ff\n  text-align: right\n```\n\n## User Interface\n\n```ui\n+----------------------+\n|                      |\n|  display...........  |\n|                      |\n|  ac.  dl.  pr.  dv.  |\n|                      |\n|  n7.  n8.  n9.  ml.  |\n|                      |\n|  n4.  n5.  n6.  mn.  |\n|                      |\n|  n1.  n2.  n3.  sb.  |\n|                      |\n|  n0......  dt.  ad.  |\n|                      |\n+----------------------+\n```\n", "Calculator for entering numbers, applying arithmetic operators, clearing input, deleting digits, and evaluating the current expression.")
    ui.Display = uimd.NewLabel("display", "")
    ui.AddElement(ui.Display)
    ui.Display.SetStyle(uimd.Style{
        Background: uimd.NewColor("#32446d"),
        Color: uimd.NewColor("#c8e0ff"),
        TextAlign: "right",
    })
    ui.Display.SetCursorStyle(uimd.Style{
        Background: uimd.NewColor("#b8d7ff"),
        Color: uimd.NewColor("#000000"),
    })
    ui.N0 = uimd.NewButton("n0", "0")
    ui.AddElement(ui.N0)
    ui.N0.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N0.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N0.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N1 = uimd.NewButton("n1", "1")
    ui.AddElement(ui.N1)
    ui.N1.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N1.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N1.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N2 = uimd.NewButton("n2", "2")
    ui.AddElement(ui.N2)
    ui.N2.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N2.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N2.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N3 = uimd.NewButton("n3", "3")
    ui.AddElement(ui.N3)
    ui.N3.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N3.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N3.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N4 = uimd.NewButton("n4", "4")
    ui.AddElement(ui.N4)
    ui.N4.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N4.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N4.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N5 = uimd.NewButton("n5", "5")
    ui.AddElement(ui.N5)
    ui.N5.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N5.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N5.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N6 = uimd.NewButton("n6", "6")
    ui.AddElement(ui.N6)
    ui.N6.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N6.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N6.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N7 = uimd.NewButton("n7", "7")
    ui.AddElement(ui.N7)
    ui.N7.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N7.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N7.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N8 = uimd.NewButton("n8", "8")
    ui.AddElement(ui.N8)
    ui.N8.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N8.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N8.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.N9 = uimd.NewButton("n9", "9")
    ui.AddElement(ui.N9)
    ui.N9.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.N9.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.N9.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Sb = uimd.NewButton("sb", "=")
    ui.AddElement(ui.Sb)
    ui.Sb.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Sb.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Sb.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Dt = uimd.NewButton("dt", ".")
    ui.AddElement(ui.Dt)
    ui.Dt.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Dt.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Dt.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Ac = uimd.NewButton("ac", "AC")
    ui.AddElement(ui.Ac)
    ui.Ac.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Ac.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Ac.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Dl = uimd.NewButton("dl", "DEL")
    ui.AddElement(ui.Dl)
    ui.Dl.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Dl.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Dl.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Pr = uimd.NewButton("pr", "%")
    ui.AddElement(ui.Pr)
    ui.Pr.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Pr.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Pr.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Dv = uimd.NewButton("dv", "/")
    ui.AddElement(ui.Dv)
    ui.Dv.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Dv.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Dv.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Ml = uimd.NewButton("ml", "*")
    ui.AddElement(ui.Ml)
    ui.Ml.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Ml.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Ml.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Mn = uimd.NewButton("mn", "-")
    ui.AddElement(ui.Mn)
    ui.Mn.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Mn.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Mn.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    ui.Ad = uimd.NewButton("ad", "+")
    ui.AddElement(ui.Ad)
    ui.Ad.SetStyle(uimd.Style{
        Background: uimd.NewColor("#334155"),
        Color: uimd.NewColor("#e2e8f0"),
    })
    ui.Ad.SetFocusStyle(uimd.Style{
        Background: uimd.NewColor("#2b5f9f"),
        Color: uimd.NewColor("#ffffff"),
    })
    ui.Ad.SetDisabledStyle(uimd.Style{
        Background: uimd.NewColor("#1f2937"),
        Color: uimd.NewColor("#64748b"),
    })
    return ui
}

func (ui *CalculatorUI) SetEventHandler(handler any) {
    ui.eventHandler = handler
}

func (ui *CalculatorUI) HandleGeneratedButton(name string) bool {
    if name == "n0" {
        if handler, ok := ui.eventHandler.(interface{ OnN0Click() }); ok {
            handler.OnN0Click()
            return true
        }
        return false
    }
    if name == "n1" {
        if handler, ok := ui.eventHandler.(interface{ OnN1Click() }); ok {
            handler.OnN1Click()
            return true
        }
        return false
    }
    if name == "n2" {
        if handler, ok := ui.eventHandler.(interface{ OnN2Click() }); ok {
            handler.OnN2Click()
            return true
        }
        return false
    }
    if name == "n3" {
        if handler, ok := ui.eventHandler.(interface{ OnN3Click() }); ok {
            handler.OnN3Click()
            return true
        }
        return false
    }
    if name == "n4" {
        if handler, ok := ui.eventHandler.(interface{ OnN4Click() }); ok {
            handler.OnN4Click()
            return true
        }
        return false
    }
    if name == "n5" {
        if handler, ok := ui.eventHandler.(interface{ OnN5Click() }); ok {
            handler.OnN5Click()
            return true
        }
        return false
    }
    if name == "n6" {
        if handler, ok := ui.eventHandler.(interface{ OnN6Click() }); ok {
            handler.OnN6Click()
            return true
        }
        return false
    }
    if name == "n7" {
        if handler, ok := ui.eventHandler.(interface{ OnN7Click() }); ok {
            handler.OnN7Click()
            return true
        }
        return false
    }
    if name == "n8" {
        if handler, ok := ui.eventHandler.(interface{ OnN8Click() }); ok {
            handler.OnN8Click()
            return true
        }
        return false
    }
    if name == "n9" {
        if handler, ok := ui.eventHandler.(interface{ OnN9Click() }); ok {
            handler.OnN9Click()
            return true
        }
        return false
    }
    if name == "sb" {
        if handler, ok := ui.eventHandler.(interface{ OnSbClick() }); ok {
            handler.OnSbClick()
            return true
        }
        return false
    }
    if name == "dt" {
        if handler, ok := ui.eventHandler.(interface{ OnDtClick() }); ok {
            handler.OnDtClick()
            return true
        }
        return false
    }
    if name == "ac" {
        if handler, ok := ui.eventHandler.(interface{ OnAcClick() }); ok {
            handler.OnAcClick()
            return true
        }
        return false
    }
    if name == "dl" {
        if handler, ok := ui.eventHandler.(interface{ OnDlClick() }); ok {
            handler.OnDlClick()
            return true
        }
        return false
    }
    if name == "pr" {
        if handler, ok := ui.eventHandler.(interface{ OnPrClick() }); ok {
            handler.OnPrClick()
            return true
        }
        return false
    }
    if name == "dv" {
        if handler, ok := ui.eventHandler.(interface{ OnDvClick() }); ok {
            handler.OnDvClick()
            return true
        }
        return false
    }
    if name == "ml" {
        if handler, ok := ui.eventHandler.(interface{ OnMlClick() }); ok {
            handler.OnMlClick()
            return true
        }
        return false
    }
    if name == "mn" {
        if handler, ok := ui.eventHandler.(interface{ OnMnClick() }); ok {
            handler.OnMnClick()
            return true
        }
        return false
    }
    if name == "ad" {
        if handler, ok := ui.eventHandler.(interface{ OnAdClick() }); ok {
            handler.OnAdClick()
            return true
        }
        return false
    }
    return false
}

func (ui *CalculatorUI) HandleGeneratedTextChanged(name string, value string) bool {
    return false
}

func (ui *CalculatorUI) HandleGeneratedTextConfirmed(name string, value string) bool {
    return false
}

func (ui *CalculatorUI) HandleGeneratedSelectionChanged(name string, value []string) bool {
    return false
}
