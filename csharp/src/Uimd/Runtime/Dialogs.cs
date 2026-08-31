using System.Text.RegularExpressions;

namespace Uimd;

public sealed class MessageBoxYesNo : GeneratedWindowBase
{
    public Label dialog_header = null!;
    public Label message = null!;
    public Button yes_btn = null!;
    public Button no_btn = null!;
    private bool closeRequested;

    public Action<bool>? OnClose { get; set; }

    public MessageBoxYesNo(string header = "", string text = "") : base("Message Box")
    {
        SetGeneratedKind("dialog");
        SetMcpMetadata(true, "MessageBoxYesNo", "", "Yes/No message box.", new[]
        {
            new GeneratedElementMetadata { Name = "dialog_header", Description = "Dialog header.", Expose = true },
            new GeneratedElementMetadata { Name = "message", Description = "Dialog message.", Expose = true },
            new GeneratedElementMetadata { Name = "yes_btn", Description = "Confirm yes.", Expose = true },
            new GeneratedElementMetadata { Name = "no_btn", Description = "Confirm no.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
        {
            Background = new Color("#162033"),
            BorderColor = new Color("transparent"),
            BorderWidthHorizontal = 0,
            BorderWidthVertical = 0,
        });
        SetGeneratedLayout(BasicLayout());
        dialog_header = AddElement(new Label("dialog_header", header));
        dialog_header.SetStyle(new Style
        {
            Background = new Color("#1d314c"),
            Color = new Color("#cbd5e1"),
            TextAlign = "center",
        });
        message = AddElement(new Label("message", text));
        message.SetStyle(new Style
        {
            Background = new Color("transparent"),
            Color = new Color("#cbd5e1"),
        });
        yes_btn = AddElement(new Button("yes_btn", "Yes"));
        ApplyDialogButtonStyle(yes_btn);
        no_btn = AddElement(new Button("no_btn", "No"));
        ApplyDialogButtonStyle(no_btn);
        SetFocus(no_btn);
    }

    private void onYesBtnClick()
    {
        closeRequested = true;
        CloseWindow();
        OnClose?.Invoke(true);
    }

    private void onNoBtnClick()
    {
        closeRequested = true;
        CloseWindow();
        OnClose?.Invoke(false);
    }

    protected override bool shouldClose()
    {
        return closeRequested;
    }

    public override bool HandleGeneratedButton(string name)
    {
        if (name == "yes_btn")
        {
            onYesBtnClick();
            return true;
        }
        if (name == "no_btn")
        {
            onNoBtnClick();
            return true;
        }
        return false;
    }

    private static List<GeneratedLayoutEntry> BasicLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            DialogEntry(
                "dialog_header",
                "label",
                new Rect(0, 0, 44, 1),
                new Rect(0, 0, 44, 1),
                AxisDimension.Expanded(),
                AxisDimension.Auto(1),
                0,
                0),
            DialogEntry(
                "message",
                "label",
                new Rect(1, 2, 40, 2),
                new Rect(2, 0, 44, 6),
                AxisDimension.Expanded(),
                AxisDimension.Auto(2),
                2,
                3),
            DialogEntry(
                "yes_btn",
                "button",
                new Rect(4, 3, 18, 1),
                new Rect(2, 0, 44, 6),
                AxisDimension.Auto(18),
                AxisDimension.Auto(1),
                23,
                1),
            DialogEntry(
                "no_btn",
                "button",
                new Rect(4, 23, 18, 1),
                new Rect(2, 0, 44, 6),
                AxisDimension.Expanded(),
                AxisDimension.Auto(1),
                3,
                1),
        };
    }

    internal static void ApplyDialogButtonStyle(Button button)
    {
        button.SetStyle(new Style
        {
            Background = new Color("#334155"),
            Color = new Color("#e2e8f0"),
        });
        button.SetFocusStyle(new Style
        {
            Background = new Color("#2b5f9f"),
            Color = new Color("#ffffff"),
        });
        button.SetDisabledStyle(new Style
        {
            Background = new Color("#1f2937"),
            Color = new Color("#64748b"),
        });
    }

    private static GeneratedLayoutEntry DialogEntry(
        string name,
        string type,
        Rect relative,
        Rect sourceCell,
        AxisDimension width,
        AxisDimension height,
        int marginRight,
        int marginBottom)
    {
        return new GeneratedLayoutEntry
        {
            Name = name,
            Type = type,
            Relative = relative,
            SourceCell = sourceCell,
            Width = width,
            Height = height,
            CellWidth = AxisDimension.Auto(sourceCell.Width),
            CellHeight = AxisDimension.Auto(sourceCell.Height),
            CharsSize = new Size(relative.Width, relative.Height),
            CellCharsSize = new Size(sourceCell.Width, sourceCell.Height),
            MarginRight = marginRight,
            MarginBottom = marginBottom,
        };
    }

}

public sealed class MessageBoxYesNoCancel : GeneratedWindowBase
{
    public Label dialog_header = null!;
    public Label message = null!;
    public Button yes_btn = null!;
    public Button no_btn = null!;
    public Button cancel_btn = null!;
    private bool closeRequested;

    public Action<string>? OnClose { get; set; }

    public MessageBoxYesNoCancel(string header = "", string text = "") : base("Message Box")
    {
        SetGeneratedKind("dialog");
        SetMcpMetadata(true, "MessageBoxYesNoCancel", "", "Yes/No/Cancel message box.", new[]
        {
            new GeneratedElementMetadata { Name = "dialog_header", Description = "Dialog header.", Expose = true },
            new GeneratedElementMetadata { Name = "message", Description = "Dialog message.", Expose = true },
            new GeneratedElementMetadata { Name = "yes_btn", Description = "Confirm yes.", Expose = true },
            new GeneratedElementMetadata { Name = "no_btn", Description = "Confirm no.", Expose = true },
            new GeneratedElementMetadata { Name = "cancel_btn", Description = "Cancel.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedWindowStyle(new Style
        {
            Background = new Color("#162033"),
            BorderColor = new Color("transparent"),
            BorderWidthHorizontal = 0,
            BorderWidthVertical = 0,
        });
        SetGeneratedLayout(BasicLayout());
        dialog_header = AddElement(new Label("dialog_header", header));
        dialog_header.SetStyle(new Style
        {
            Background = new Color("#1d314c"),
            Color = new Color("#cbd5e1"),
            TextAlign = "center",
        });
        message = AddElement(new Label("message", text));
        message.SetStyle(new Style
        {
            Background = new Color("transparent"),
            Color = new Color("#cbd5e1"),
        });
        yes_btn = AddElement(new Button("yes_btn", "Yes"));
        MessageBoxYesNo.ApplyDialogButtonStyle(yes_btn);
        no_btn = AddElement(new Button("no_btn", "No"));
        MessageBoxYesNo.ApplyDialogButtonStyle(no_btn);
        cancel_btn = AddElement(new Button("cancel_btn", "Cancel"));
        MessageBoxYesNo.ApplyDialogButtonStyle(cancel_btn);
        SetFocus(cancel_btn);
    }

    public override bool HandleGeneratedButton(string name)
    {
        if (name is "yes_btn" or "no_btn" or "cancel_btn")
        {
            string result = name == "yes_btn" ? "yes" : name == "no_btn" ? "no" : "cancel";
            closeRequested = true;
            CloseWindow();
            OnClose?.Invoke(result);
            return true;
        }
        return false;
    }

    protected override bool shouldClose()
    {
        return closeRequested;
    }

    private static List<GeneratedLayoutEntry> BasicLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            DialogEntry(
                "dialog_header",
                "label",
                new Rect(0, 0, 46, 1),
                new Rect(0, 0, 46, 1),
                AxisDimension.Expanded(),
                AxisDimension.Auto(1),
                0,
                0),
            DialogEntry(
                "message",
                "label",
                new Rect(1, 2, 42, 2),
                new Rect(2, 0, 46, 6),
                AxisDimension.Expanded(),
                AxisDimension.Auto(2),
                2,
                3),
            DialogEntry(
                "yes_btn",
                "button",
                new Rect(4, 2, 12, 1),
                new Rect(2, 0, 46, 6),
                AxisDimension.Auto(12),
                AxisDimension.Auto(1),
                32,
                1),
            DialogEntry(
                "no_btn",
                "button",
                new Rect(4, 16, 13, 1),
                new Rect(2, 0, 46, 6),
                AxisDimension.Auto(13),
                AxisDimension.Auto(1),
                17,
                1),
            DialogEntry(
                "cancel_btn",
                "button",
                new Rect(4, 31, 13, 1),
                new Rect(2, 0, 46, 6),
                AxisDimension.Expanded(),
                AxisDimension.Auto(1),
                2,
                1),
        };
    }

    private static GeneratedLayoutEntry DialogEntry(
        string name,
        string type,
        Rect relative,
        Rect sourceCell,
        AxisDimension width,
        AxisDimension height,
        int marginRight,
        int marginBottom)
    {
        return new GeneratedLayoutEntry
        {
            Name = name,
            Type = type,
            Relative = relative,
            SourceCell = sourceCell,
            Width = width,
            Height = height,
            CellWidth = AxisDimension.Auto(sourceCell.Width),
            CellHeight = AxisDimension.Auto(sourceCell.Height),
            CharsSize = new Size(relative.Width, relative.Height),
            CellCharsSize = new Size(sourceCell.Width, sourceCell.Height),
            MarginRight = marginRight,
            MarginBottom = marginBottom,
        };
    }
}

public sealed class FileBrowser : GeneratedWindowBase
{
    private const string ParentEntry = "..";

    public Label dialog_header = null!;
    public Label path_label = null!;
    public ListBox entries = null!;
    public TextInput filename = null!;
    public Button open_btn = null!;
    public Button close_btn = null!;
    private Label _label_3 = null!;

    private readonly string rootDir;
    private readonly string mode;
    private readonly Action<string>? onClose;
    private readonly string extensionFilter;
    private string currentDir;
    private string initialFilename;
    private string result = "";
    private bool closed;

    public FileBrowser(
        string rootDir,
        string? startPath = null,
        string mode = "open",
        Action<string>? onClose = null,
        string initialFilename = "",
        string extensionFilter = "") : base("File Browser")
    {
        this.rootDir = Path.GetFullPath(rootDir);
        this.mode = mode;
        this.onClose = onClose;
        this.extensionFilter = extensionFilter.Trim();
        string requestedStart = Path.GetFullPath(string.IsNullOrEmpty(startPath) ? this.rootDir : startPath);
        if (File.Exists(requestedStart))
        {
            currentDir = ClampDir(Path.GetDirectoryName(requestedStart) ?? this.rootDir);
            this.initialFilename = string.IsNullOrEmpty(initialFilename) ? Path.GetFileName(requestedStart) : initialFilename;
        }
        else
        {
            currentDir = ClampDir(requestedStart);
            this.initialFilename = initialFilename;
        }

        SetGeneratedKind("dialog");
        SetMcpMetadata(true, "FileBrowser", "", "File browser dialog.", new[]
        {
            new GeneratedElementMetadata { Name = "dialog_header", Description = "Dialog header.", Expose = true },
            new GeneratedElementMetadata { Name = "path_label", Description = "Current directory.", Expose = true },
            new GeneratedElementMetadata { Name = "entries", Description = "Directory entries.", Expose = true },
            new GeneratedElementMetadata { Name = "filename", Description = "Selected file name.", Expose = true },
            new GeneratedElementMetadata { Name = "open_btn", Description = "Accept selected file.", Expose = true },
            new GeneratedElementMetadata { Name = "close_btn", Description = "Close dialog.", Expose = true },
        });
        SetMcpAppTools(Array.Empty<GeneratedAppToolMetadata>());
        SetGeneratedLayout(BasicLayout());
        SetGeneratedWindowStyle(new Style { Background = new Color("#162033"), BorderColor = new Color("transparent"), BorderWidthHorizontal = 0, BorderWidthVertical = 0 });

        dialog_header = AddElement(new Label("dialog_header", mode == "save" ? "Save As" : "Open File"));
        dialog_header.SetStyle(new Style
        {
            Background = new Color("#1d314c"),
            Color = new Color("#cbd5e1"),
            TextAlign = "center",
        });
        path_label = AddElement(new Label("path_label", currentDir));
        ApplyDialogLabelStyle(path_label);
        entries = AddElement(new ListBox("entries", Array.Empty<string>()));
        entries.SetCommitMode(Element.CommitModeLeave);
        entries.SetStyle(new Style { Background = new Color("#0d1524"), Color = new Color("#cbd5e1") });
        entries.SetFocusStyle(new Style { Background = new Color("#1d2f4d"), Color = new Color("#ffffff") });
        entries.SetEditStyle(new Style { Background = new Color("#243a5c"), Color = new Color("#cbd5e1") });
        entries.SetSelectedStyle(new Style { Background = new Color("#2563eb"), Color = new Color("#ffffff") });
        entries.SetActiveStyle(new Style { Background = new Color("#DDDDDD99"), Color = new Color("#FFFFFF") });
        entries.SetDisabledStyle(new Style { Color = new Color("#64748b") });
        _label_3 = AddElement(new Label("_label_3", "File:"));
        ApplyDialogLabelStyle(_label_3);
        filename = AddElement(new TextInput("filename", "", 120));
        ApplyDialogTextInputStyle(filename);
        open_btn = AddElement(new Button("open_btn", mode == "save" ? "Save" : "Open"));
        MessageBoxYesNo.ApplyDialogButtonStyle(open_btn);
        close_btn = AddElement(new Button("close_btn", "Close"));
        MessageBoxYesNo.ApplyDialogButtonStyle(close_btn);

        if (mode == "save")
        {
            filename.SetValue(this.initialFilename);
        }
        RefreshEntries();
    }

    public string CurrentDir => currentDir;
    public string Result => result;
    public bool Closed => closed;

    public override GeneratedWindowRuntimeOptions RuntimeOptions()
    {
        GeneratedWindowRuntimeOptions options = base.RuntimeOptions();
        options.InitialFocusName = "entries";
        options.StartInEditMode = true;
        options.OnButton = name =>
        {
            if (name == "open_btn")
            {
                if (mode == "save")
                {
                    AcceptFilename();
                }
                else
                {
                    AcceptCurrent();
                }
            }
            else if (name == "close_btn")
            {
                Close("");
            }
        };
        options.OnTextChanged = (name, _) =>
        {
            if (name == "entries")
            {
                PreviewSelected();
            }
            else if (name == "filename")
            {
                UpdateOpenEnabled();
            }
        };
        options.OnSelectionChanged = (name, _) =>
        {
            if (name == "entries")
            {
                PreviewSelected();
            }
        };
        options.OnTextConfirmed = (name, _) =>
        {
            if (name == "entries")
            {
                PreviewSelected();
            }
        };
        options.OnEditStarted = name =>
        {
            if (name == "filename")
            {
                MoveFilenameCursorToEnd();
            }
        };
        options.OnMousePressBeforeFocused = HandleEntryMousePress;
        options.ShouldClose = () => closed;
        return options;
    }

    public override bool HandleGeneratedListBoxItemActivate(
        string name,
        string elementId,
        int index,
        string value)
    {
        if (name != "entries" || !EntryIndexIsDirectory(index))
        {
            return false;
        }
        SelectEntry(index);
        entries.HideActiveItem();
        PreviewSelected();
        AcceptCurrent();
        return true;
    }

    public override bool OnPreviewKey(KeyEvent keyEvent)
    {
        if (keyEvent.Key == "Escape" && !keyEvent.EditMode)
        {
            Close("");
            return true;
        }
        return false;
    }

    public void RefreshEntries()
    {
        List<string> rows = new() { ParentEntry };
        List<string> disabledRows = new();
        IEnumerable<string> dirs = Enumerable.Empty<string>();
        IEnumerable<string> files = Enumerable.Empty<string>();
        if (Directory.Exists(currentDir))
        {
            dirs = Directory.EnumerateDirectories(currentDir)
                .Select(Path.GetFileName)
                .Where(name => !string.IsNullOrEmpty(name))
                .Select(name => name! + "/")
                .OrderBy(name => name, StringComparer.Ordinal);
            files = Directory.EnumerateFiles(currentDir)
                .Select(Path.GetFileName)
                .Where(name => !string.IsNullOrEmpty(name))
                .Select(name => name!)
                .OrderBy(name => name, StringComparer.Ordinal);
        }
        rows.AddRange(dirs);
        foreach (string file in files)
        {
            rows.Add(file);
            if (!PathMatchesFilter(Path.Combine(currentDir, file)))
            {
                disabledRows.Add(file);
            }
        }
        path_label.SetText(currentDir);
        entries.SetOptions(rows);
        entries.SetDisabledValues(disabledRows);
        int selectedIndex = rows.IndexOf(initialFilename);
        entries.SetSelectedIndex(selectedIndex >= 0 ? selectedIndex : 0);
        PreviewSelected();
    }

    private bool HandleEntryMousePress(Point point)
    {
        Rect frame = entries.Frame;
        if (!frame.Contains(point))
        {
            return false;
        }
        int index = entries.ScrollOffset + point.Row - frame.Row;
        if (index < 0 || index >= entries.Options.Count)
        {
            return false;
        }
        entries.SetSelectedIndex(index);
        PreviewSelected();
        if (SelectedEntryIsDirectory())
        {
            return AcceptCurrent();
        }
        return false;
    }

    public bool AcceptCurrent()
    {
        string selected = entries.SelectedValues.FirstOrDefault() ?? "";
        string name = filename.Value.Trim();
        if (!string.IsNullOrEmpty(name))
        {
            string path = Path.GetFullPath(Path.Combine(currentDir, name));
            if (!PathStartsWith(path, rootDir))
            {
                return false;
            }
            if (Directory.Exists(path))
            {
                currentDir = ClampDir(path);
                RefreshEntries();
                return true;
            }
            if (mode == "open" && !File.Exists(path))
            {
                return false;
            }
            if (mode == "open" && !PathMatchesFilter(path))
            {
                return false;
            }
            Close(path);
            return true;
        }

        if (selected == ParentEntry)
        {
            currentDir = ClampDir(Directory.GetParent(currentDir)?.FullName ?? currentDir);
            RefreshEntries();
            return true;
        }
        if (selected.EndsWith("/", StringComparison.Ordinal))
        {
            currentDir = ClampDir(Path.Combine(currentDir, selected[..^1]));
            RefreshEntries();
            return true;
        }

        name = selected.Trim();
        if (string.IsNullOrEmpty(name))
        {
            return false;
        }
        string selectedPath = Path.GetFullPath(Path.Combine(currentDir, name));
        if (!PathStartsWith(selectedPath, rootDir))
        {
            return false;
        }
        if (mode == "open" && !File.Exists(selectedPath))
        {
            return false;
        }
        if (mode == "open" && !PathMatchesFilter(selectedPath))
        {
            return false;
        }
        Close(selectedPath);
        return true;
    }

    public bool AcceptFilename()
    {
        string name = filename.Value.Trim();
        if (string.IsNullOrEmpty(name))
        {
            return false;
        }
        string path = Path.GetFullPath(Path.Combine(currentDir, name));
        if (!PathStartsWith(path, rootDir) || Directory.Exists(path))
        {
            return false;
        }
        Close(path);
        return true;
    }

    public void SelectEntry(int index)
    {
        entries.SetSelectedIndex(index);
        PreviewSelected();
    }

    public bool SelectedEntryIsDirectory()
    {
        return EntryIndexIsDirectory(entries.SelectedIndex);
    }

    public bool EntryIndexIsDirectory(int index)
    {
        if (index < 0 || index >= entries.Options.Count)
        {
            return false;
        }
        string selected = entries.Options[index];
        return selected == ParentEntry || selected.EndsWith("/", StringComparison.Ordinal);
    }

    public void MoveFilenameCursorToEnd()
    {
        int end = filename.Value.Length;
        filename.SetSelection(end, end);
    }

    public void Close(string path)
    {
        result = path;
        closed = true;
        onClose?.Invoke(result);
        CloseWindow();
    }

    private void PreviewSelected()
    {
        string selected = entries.SelectedValues.FirstOrDefault() ?? "";
        if (!string.IsNullOrEmpty(selected) && selected != ParentEntry)
        {
            filename.SetValue(selected.EndsWith("/", StringComparison.Ordinal) ? selected[..^1] : selected);
        }
        else if (mode == "open")
        {
            filename.SetValue("");
        }
        MoveFilenameCursorToEnd();
        UpdateOpenEnabled();
    }

    private void UpdateOpenEnabled()
    {
        if (mode == "save")
        {
            open_btn.Enabled = !string.IsNullOrEmpty(filename.Value.Trim());
            return;
        }
        string? path = SelectedPathForOpenState();
        if (string.IsNullOrEmpty(path))
        {
            open_btn.Enabled = false;
            return;
        }
        if (Directory.Exists(path))
        {
            open_btn.Enabled = true;
            return;
        }
        open_btn.Enabled = File.Exists(path) && PathMatchesFilter(path);
    }

    private string? SelectedPathForOpenState()
    {
        string name = filename.Value.Trim();
        if (!string.IsNullOrEmpty(name))
        {
            return Path.GetFullPath(Path.Combine(currentDir, name));
        }
        string selected = entries.SelectedValues.FirstOrDefault() ?? "";
        if (string.IsNullOrEmpty(selected))
        {
            return null;
        }
        if (selected == ParentEntry)
        {
            return ClampDir(Directory.GetParent(currentDir)?.FullName ?? currentDir);
        }
        return Path.GetFullPath(Path.Combine(currentDir, selected.TrimEnd('/')));
    }

    private string ClampDir(string path)
    {
        string full = Path.GetFullPath(path);
        if (!PathStartsWith(full, rootDir) || !Directory.Exists(full))
        {
            return rootDir;
        }
        return full;
    }

    private bool PathMatchesFilter(string path)
    {
        if (string.IsNullOrEmpty(extensionFilter))
        {
            return true;
        }
        string ext = Path.GetExtension(path).TrimStart('.').ToLowerInvariant();
        if (string.IsNullOrEmpty(ext))
        {
            return false;
        }
        try
        {
            return Regex.IsMatch(ext, "^(" + extensionFilter + ")$", RegexOptions.IgnoreCase);
        }
        catch (ArgumentException)
        {
            return extensionFilter
                .Split('|', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries)
                .Select(item => item.TrimStart('.').ToLowerInvariant())
                .Contains(ext);
        }
    }

    private static bool PathStartsWith(string path, string root)
    {
        string normalizedPath = Path.GetFullPath(path).TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar) + Path.DirectorySeparatorChar;
        string normalizedRoot = Path.GetFullPath(root).TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar) + Path.DirectorySeparatorChar;
        return normalizedPath.StartsWith(normalizedRoot, StringComparison.Ordinal);
    }

    private static List<GeneratedLayoutEntry> BasicLayout()
    {
        return new List<GeneratedLayoutEntry>
        {
            Entry(
                "dialog_header",
                "label",
                new Rect(0, 0, 48, 1),
                new Rect(0, 0, 48, 1),
                AxisDimension.Expanded(),
                AxisDimension.Auto(1),
                0,
                0),
            Entry(
                "path_label",
                "label",
                new Rect(1, 3, 42, 1),
                new Rect(2, 0, 48, 14),
                AxisDimension.Expanded(),
                AxisDimension.Auto(1),
                3,
                12),
            Entry(
                "entries",
                "listbox",
                new Rect(3, 3, 42, 6),
                new Rect(2, 0, 48, 14),
                AxisDimension.Expanded(),
                AxisDimension.Auto(6),
                3,
                5),
            Entry(
                "_label_3",
                "label",
                new Rect(10, 3, 9, 1),
                new Rect(2, 0, 48, 14),
                AxisDimension.Auto(9),
                AxisDimension.Auto(1),
                36,
                3),
            Entry(
                "filename",
                "textinput",
                new Rect(10, 14, 31, 1),
                new Rect(2, 0, 48, 14),
                AxisDimension.Expanded(),
                AxisDimension.Auto(1),
                3,
                3),
            Entry(
                "open_btn",
                "button",
                new Rect(12, 3, 20, 1),
                new Rect(2, 0, 48, 14),
                AxisDimension.Auto(20),
                AxisDimension.Auto(1),
                25,
                1),
            Entry(
                "close_btn",
                "button",
                new Rect(12, 25, 20, 1),
                new Rect(2, 0, 48, 14),
                AxisDimension.Expanded(),
                AxisDimension.Auto(1),
                3,
                1),
        };
    }

    private static GeneratedLayoutEntry Entry(
        string name,
        string type,
        Rect relative,
        Rect sourceCell,
        AxisDimension width,
        AxisDimension height,
        int marginRight,
        int marginBottom)
    {
        return new GeneratedLayoutEntry
        {
            Name = name,
            Type = type,
            Relative = relative,
            SourceCell = sourceCell,
            Width = width,
            Height = height,
            CellWidth = AxisDimension.Auto(sourceCell.Width),
            CellHeight = AxisDimension.Auto(sourceCell.Height),
            CharsSize = new Size(relative.Width, relative.Height),
            CellCharsSize = new Size(sourceCell.Width, sourceCell.Height),
            MarginRight = marginRight,
            MarginBottom = marginBottom,
        };
    }

    private static void ApplyDialogLabelStyle(Label label)
    {
        label.SetStyle(new Style
        {
            Background = new Color("transparent"),
            Color = new Color("#cbd5e1"),
        });
        label.SetCursorStyle(new Style
        {
            Background = new Color("#b8d7ff"),
            Color = new Color("#000000"),
        });
    }

    private static void ApplyDialogTextInputStyle(TextInput input)
    {
        input.SetStyle(new Style
        {
            Background = new Color("#0d1524"),
            Color = new Color("#e5e7eb"),
        });
        input.SetDisabledStyle(new Style
        {
            Background = new Color("#0b1220"),
            Color = new Color("#64748b"),
        });
        input.SetFocusStyle(new Style
        {
            Background = new Color("#243a5c"),
            Color = new Color("#ffffff"),
        });
        input.SetEditStyle(new Style
        {
            Background = new Color("#2d456d"),
            Color = new Color("#ffffff"),
        });
        input.SetCursorStyle(new Style
        {
            Background = new Color("#ffffff"),
            Color = new Color("#0e1117"),
        });
        input.SetSelectedStyle(new Style
        {
            Background = new Color("#2563eb"),
            Color = new Color("#ffffff"),
        });
    }
}
