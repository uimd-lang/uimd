// Auto-generated UI code for formular - DO NOT EDIT MANUALLY.
using Uimd;

public class FormularUI : GeneratedWindowBase
{
    public Label title_label = null!;
    public Label name_label = null!;
    public TextInput name_input = null!;
    public Label email_label = null!;
    public TextInput email_input = null!;
    public Label age_label = null!;
    public NumberInput age_input = null!;
    public TextArea description_input = null!;
    public Label country_label = null!;
    public ComboBox country_combo = null!;
    public Label role_label = null!;
    public ListBox role_listbox = null!;
    public CheckBox notify_check = null!;
    public CheckBox terms_check = null!;
    public Button save_btn = null!;
    public Button cancel_btn = null!;

    public FormularUI() : base("FormApp")
    {
        SetGeneratedLayout(BuildLayout());
        SetGeneratedFocusable(false);
        SetGeneratedKind("window");
        SetMcpMetadata(true, "Formular", """
# FormApp

## Metadata

```yaml
format: uimd
format-version: 1
kind: window
status: stable
description: "User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance."
tags: [example]
```

## Members

```yaml
title_label:
  type: label
  text: "User profile"
  description: "Title of the form."
  expose: false

name_label:
  type: label
  text: Name
  description: "Label for the name field."

name_input:
  type: textinput
  value: ""
  description: "User name text field."

email_label:
  type: label
  text: Email
  description: "Label for the email field."

email_input:
  type: textinput
  value: ""
  description: "Email address text field."

age_label:
  type: label
  text: Age
  description: "Label for the age field."

age_input:
  type: numberinput
  description: "Numeric age input."

description_input:
  type: textarea
  description: "Multiline profile description field."

country_label:
  type: label
  text: Country
  description: "Label for the country selector."

country_combo:
  type: combobox
  options: [Slovakia, "Czech Republic", Poland, Hungary, Austria]
  selected_item: Slovakia
  description: "Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options."

role_label:
  type: label
  text: Role
  description: "Label for the role selector."

role_listbox:
  type: listbox
  options: [Developer, Designer, Manager, QA, DevOps, Tester, Hacker]
  selected_items: [Developer]
  multiple: "true"
  description: "Multi-select role listbox for choosing one or more user roles."

notify_check:
  type: checkbox
  title: "Email notifications"
  description: "Toggle email notifications."

terms_check:
  type: checkbox
  title: "I accept the terms and conditions"
  description: "Toggle acceptance of terms and conditions."

save_btn:
  type: button
  title: Save
  description: "Submit the form and serialize the entered data."

cancel_btn:
  type: button
  title: Cancel
  description: "Cancel the form action."
```

## Style

```yaml
@title_label:
  text-align: center
```

## User Interface

```ui
+----------------------------------------------------+
|                                                    |
|  title_label.....................................  |
|                                                    |
|  name_label       name_input.....................  |
|                                                    |
|  email_label      email_input....................  |
|                                                    |
|  age_label        age_input......................  |
|                                                    |
|  country_label    country_combo..................  |
|                                                    |
|  "Description"    description_input..............  |
|                  ..............................    |
|                  ..............................    |
|                  ..............................    |
|                                                    |
|  role_label       role_listbox...................  |
|                  ..............................    |
|                  ..............................    |
|                  ..............................    |
|                                                    |
|  notify_check....................................  |
|  terms_check.....................................  |
|                                                    |
|       save_btn......      cancel_btn.......        |
|                                                    |
+----------------------------------------------------+
```

""", "User profile form for entering contact details, demographic data, role preferences, notification consent, and terms acceptance.", new[]
        {
            new GeneratedElementMetadata { Name = "title_label", Description = "Title of the form.", Expose = false },
            new GeneratedElementMetadata { Name = "name_label", Description = "Label for the name field.", Expose = true },
            new GeneratedElementMetadata { Name = "name_input", Description = "User name text field.", Expose = true },
            new GeneratedElementMetadata { Name = "email_label", Description = "Label for the email field.", Expose = true },
            new GeneratedElementMetadata { Name = "email_input", Description = "Email address text field.", Expose = true },
            new GeneratedElementMetadata { Name = "age_label", Description = "Label for the age field.", Expose = true },
            new GeneratedElementMetadata { Name = "age_input", Description = "Numeric age input.", Expose = true },
            new GeneratedElementMetadata { Name = "description_input", Description = "Multiline profile description field.", Expose = true },
            new GeneratedElementMetadata { Name = "country_label", Description = "Label for the country selector.", Expose = true },
            new GeneratedElementMetadata { Name = "country_combo", Description = "Country combobox with Slovakia, Czech Republic, Poland, Hungary, and Austria options.", Expose = true },
            new GeneratedElementMetadata { Name = "role_label", Description = "Label for the role selector.", Expose = true },
            new GeneratedElementMetadata { Name = "role_listbox", Description = "Multi-select role listbox for choosing one or more user roles.", Expose = true },
            new GeneratedElementMetadata { Name = "notify_check", Description = "Toggle email notifications.", Expose = true },
            new GeneratedElementMetadata { Name = "terms_check", Description = "Toggle acceptance of terms and conditions.", Expose = true },
            new GeneratedElementMetadata { Name = "save_btn", Description = "Submit the form and serialize the entered data.", Expose = true },
            new GeneratedElementMetadata { Name = "cancel_btn", Description = "Cancel the form action.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
    {
        Background = new Color("#162033"),
        BorderColor = new Color("transparent"),
        BorderWidthHorizontal = 0,
        BorderWidthVertical = 0
    });
        title_label = AddElement(new Label("title_label", "User profile"));
        title_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextAlign = "center"
    });
        title_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        name_label = AddElement(new Label("name_label", "Name"));
        name_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        name_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        name_input = AddElement(new TextInput("name_input", "", 0));
        name_input.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        name_input.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        name_input.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        name_input.SetCursorStyle(new Style
    {
        Background = new Color("#ffffff"),
        Color = new Color("#0e1117")
    });
        name_input.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        name_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        email_label = AddElement(new Label("email_label", "Email"));
        email_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        email_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        email_input = AddElement(new TextInput("email_input", "", 0));
        email_input.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        email_input.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        email_input.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        email_input.SetCursorStyle(new Style
    {
        Background = new Color("#ffffff"),
        Color = new Color("#0e1117")
    });
        email_input.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        email_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        age_label = AddElement(new Label("age_label", "Age"));
        age_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        age_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        age_input = AddElement(new NumberInput("age_input", 0.0, 1.0));
        age_input.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        age_input.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        age_input.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        age_input.SetCursorStyle(new Style
    {
        Background = new Color("#ffffff"),
        Color = new Color("#0e1117")
    });
        age_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        description_input = AddElement(new TextArea("description_input", "", 0));
        description_input.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        description_input.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        description_input.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#ffffff")
    });
        description_input.SetCursorStyle(new Style
    {
        Background = new Color("#ffffff"),
        Color = new Color("#0e1117")
    });
        description_input.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        description_input.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        country_label = AddElement(new Label("country_label", "Country"));
        country_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        country_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        country_combo = AddElement(new ComboBox("country_combo", new[] {"Slovakia", "Czech Republic", "Poland", "Hungary", "Austria"}));
        country_combo.SetSelectedIndex(0);
        country_combo.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    });
        country_combo.SetFocusStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#ffffff")
    });
        country_combo.SetEditStyle(new Style
    {
        Background = new Color("#2d456d"),
        Color = new Color("#e5e7eb")
    });
        country_combo.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        country_combo.SetDisabledStyle(new Style
    {
        Background = new Color("#0b1220"),
        Color = new Color("#64748b")
    });
        role_label = AddElement(new Label("role_label", "Role"));
        role_label.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    });
        role_label.SetCursorStyle(new Style
    {
        Background = new Color("#b8d7ff"),
        Color = new Color("#000000")
    });
        role_listbox = AddElement(new ListBox("role_listbox", new[] {"Developer", "Designer", "Manager", "QA", "DevOps", "Tester", "Hacker"}));
        role_listbox.SetMultiple(true);
        role_listbox.SetSelectedIndex(0);
        role_listbox.SetStyle(new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#cbd5e1")
    });
        role_listbox.SetFocusStyle(new Style
    {
        Background = new Color("#1d2f4d"),
        Color = new Color("#ffffff")
    });
        role_listbox.SetEditStyle(new Style
    {
        Background = new Color("#243a5c"),
        Color = new Color("#cbd5e1")
    });
        role_listbox.SetSelectedStyle(new Style
    {
        Background = new Color("#2563eb"),
        Color = new Color("#ffffff")
    });
        role_listbox.SetActiveStyle(new Style
    {
        Background = new Color("#DDDDDD99"),
        Color = new Color("#FFFFFF")
    });
        role_listbox.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        notify_check = AddElement(new CheckBox("notify_check", "Email notifications", false));
        notify_check.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#b8c4d8")
    });
        notify_check.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#ffffff")
    });
        notify_check.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#44cc88")
    });
        notify_check.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent")
    });
        notify_check.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        terms_check = AddElement(new CheckBox("terms_check", "I accept the terms and conditions", false));
        terms_check.SetStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#b8c4d8")
    });
        terms_check.SetFocusStyle(new Style
    {
        Background = new Color("#ffffff44"),
        Color = new Color("#ffffff")
    });
        terms_check.SetCheckedStyle(new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#44cc88")
    });
        terms_check.SetUncheckedStyle(new Style
    {
        Background = new Color("transparent")
    });
        terms_check.SetDisabledStyle(new Style
    {
        Color = new Color("#64748b")
    });
        save_btn = AddElement(new Button("save_btn", "Save"));
        save_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        save_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        save_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
        cancel_btn = AddElement(new Button("cancel_btn", "Cancel"));
        cancel_btn.SetStyle(new Style
    {
        Background = new Color("#334155"),
        Color = new Color("#e2e8f0")
    });
        cancel_btn.SetFocusStyle(new Style
    {
        Background = new Color("#2b5f9f"),
        Color = new Color("#ffffff")
    });
        cancel_btn.SetDisabledStyle(new Style
    {
        Background = new Color("#1f2937"),
        Color = new Color("#64748b")
    });
    }

    private static List<GeneratedLayoutEntry> BuildLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            new GeneratedLayoutEntry
            {
                Name = "title_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(1, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(48, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 24,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1"),
        TextAlign = "center"
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "name_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(3, 2, 10, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Auto(10),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 40,
                MarginBottom = 22,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "name_input",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(3, 19, 536870911, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 22,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "email_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(5, 2, 11, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Auto(11),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(11, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 39,
                MarginBottom = 20,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "email_input",
                Type = "textinput",
                CellName = "",
                Relative = new Rect(5, 19, 536870911, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 20,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "age_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(7, 2, 9, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Auto(9),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(9, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 41,
                MarginBottom = 18,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "age_input",
                Type = "numberinput",
                CellName = "",
                Relative = new Rect(7, 19, 536870911, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 18,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "country_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(9, 2, 13, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Auto(13),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(13, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 37,
                MarginBottom = 16,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "country_combo",
                Type = "combobox",
                CellName = "",
                Relative = new Rect(9, 19, 536870911, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(31, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 16,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#e5e7eb")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "\"Description\"",
                Type = "label",
                CellName = "",
                Relative = new Rect(11, 2, 13, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Auto(13),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(13, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 37,
                MarginBottom = 14,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "description_input",
                Type = "textarea",
                CellName = "",
                Relative = new Rect(11, 19, 536870911, 4),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(4),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(31, 4),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 11,
                CellStyle = new Style(),
                ElementStyle = new Style(),
            },
            new GeneratedLayoutEntry
            {
                Name = "role_label",
                Type = "label",
                CellName = "",
                Relative = new Rect(16, 2, 10, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Auto(10),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(10, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 40,
                MarginBottom = 9,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "role_listbox",
                Type = "listbox",
                CellName = "",
                Relative = new Rect(16, 19, 536870911, 4),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(4),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(31, 4),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 6,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("#0d1524"),
        Color = new Color("#cbd5e1")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "notify_check",
                Type = "checkbox",
                CellName = "",
                Relative = new Rect(21, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(48, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 4,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#b8c4d8")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "terms_check",
                Type = "checkbox",
                CellName = "",
                Relative = new Rect(22, 2, 536870911, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(48, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 2,
                MarginBottom = 3,
                CellStyle = new Style(),
                ElementStyle = new Style
    {
        Background = new Color("transparent"),
        Color = new Color("#b8c4d8")
    },
            },
            new GeneratedLayoutEntry
            {
                Name = "save_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(24, 7, 14, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Auto(14),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(14, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 31,
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
                Name = "cancel_btn",
                Type = "button",
                CellName = "",
                Relative = new Rect(24, 27, 536870911, 1),
                SourceCell = new Rect(0, 0, 52, 26),
                Width = AxisDimension.Expanded(),
                Height = AxisDimension.Auto(1),
                CellWidth = AxisDimension.Auto(52),
                CellHeight = AxisDimension.Auto(26),
                CharsSize = new Size(17, 1),
                CellCharsSize = new Size(52, 26),
                MarginRight = 8,
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

    protected virtual void onNameInputChange(string value) { }
    protected virtual void onNameInputSubmit(string value) { }
    protected virtual void onEmailInputChange(string value) { }
    protected virtual void onEmailInputSubmit(string value) { }
    protected virtual void onAgeInputChange(string value) { }
    protected virtual void onAgeInputSubmit(string value) { }
    protected virtual void onDescriptionInputChange(string value) { }
    protected virtual void onDescriptionInputSubmit(string value) { }
    protected virtual void onCountryComboChange(string value) { }
    protected virtual void onRoleListboxSelectionChange(List<string> value) { }
    protected virtual void onNotifyCheckChange(string value) { }
    protected virtual void onTermsCheckChange(string value) { }
    protected virtual void onSaveBtnClick() { }
    protected virtual void onCancelBtnClick() { }
    protected override bool shouldClose() { return false; }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "save_btn")
        {
            onSaveBtnClick();
            return true;
        }
        else if (name == "cancel_btn")
        {
            onCancelBtnClick();
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextChanged(string name, string value)
    {
        if (name == "name_input")
        {
            onNameInputChange(value);
            return true;
        }
        else if (name == "email_input")
        {
            onEmailInputChange(value);
            return true;
        }
        else if (name == "age_input")
        {
            onAgeInputChange(value);
            return true;
        }
        else if (name == "description_input")
        {
            onDescriptionInputChange(value);
            return true;
        }
        else if (name == "notify_check")
        {
            onNotifyCheckChange(value);
            return true;
        }
        else if (name == "terms_check")
        {
            onTermsCheckChange(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedTextConfirmed(string name, string value)
    {
        if (name == "name_input")
        {
            onNameInputSubmit(value);
            return true;
        }
        else if (name == "email_input")
        {
            onEmailInputSubmit(value);
            return true;
        }
        else if (name == "age_input")
        {
            onAgeInputSubmit(value);
            return true;
        }
        else if (name == "description_input")
        {
            onDescriptionInputSubmit(value);
            return true;
        }
        return false;
    }

    public override bool HandleGeneratedSelectionChanged(string name, List<string> value)
    {
        if (name == "country_combo")
        {
            onCountryComboChange(value.Count == 0 ? "" : value[0]);
            return true;
        }
        else if (name == "role_listbox")
        {
            onRoleListboxSelectionChange(value);
            return true;
        }
        return false;
    }
}
