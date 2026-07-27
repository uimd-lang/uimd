using System.Runtime.CompilerServices;
using Uimd;

public sealed class TextEditor : TextEditorUI
{
    private const string NotesDirEnv = "UI_TEXT_EDITOR_NOTES_DIR";
    private const string DefaultNoteEnv = "UI_TEXT_EDITOR_DEFAULT_NOTE";
    private const string BrowserRootDirEnv = "UI_TEXT_EDITOR_BROWSER_ROOT_DIR";
    private const string DefaultBrowserDirEnv = "UI_TEXT_EDITOR_BROWSER_DIR";
    private const string DefaultFileName = "welcome.txt";
    private const string UntitledName = "untitled.txt";

    private string? currentPath;
    private string loadedText = "";
    private Action? pendingAction;
    private Action? afterSaveAction;
    private FileBrowser? browser;
    private string overwritePath = "";
    private bool finished;

    public TextEditor()
    {
        LoadPath(Path.Combine(NotesDir(), DefaultNoteName()));
    }

    protected override void onNewBtnClick()
    {
        WithSavedChanges(NewFile);
    }

    protected override void onOpenBtnClick()
    {
        WithSavedChanges(OpenBrowser);
    }

    protected override void onSaveBtnClick()
    {
        SaveOrSaveAs();
    }

    protected override void onSaveAsBtnClick()
    {
        SaveAsBrowser();
    }

    protected override void onQuitBtnClick()
    {
        WithSavedChanges(Quit);
    }

    protected override void onEditorChange(string value)
    {
        MarkModified();
    }

    protected override bool shouldClose()
    {
        return finished;
    }

    public void NewFile()
    {
        currentPath = null;
        filename.SetText(UntitledName);
        SetEditorText("");
        loadedText = "";
        RefreshStatus("New file");
    }

    public void LoadPath(string path)
    {
        path = Path.GetFullPath(path);
        string text = ReadTextFile(path);
        currentPath = path;
        filename.SetText(DisplayFileName(path));
        SetEditorText(text);
        loadedText = text;
        RefreshStatus("Loaded");
    }

    public void SaveOrSaveAs()
    {
        if (string.IsNullOrEmpty(currentPath))
        {
            SaveAsBrowser();
            return;
        }
        SavePath(currentPath);
    }

    public void OpenBrowser()
    {
        browser = new FileBrowser(BrowserRootDir(), DefaultBrowserDir(), "open", OnOpenSelected);
        OpenWindow(browser, BrowserFrameOptions());
    }

    public void SaveAsBrowser()
    {
        string start = !string.IsNullOrEmpty(currentPath) ? Path.GetDirectoryName(currentPath) ?? NotesDir() : NotesDir();
        if (!PathStartsWith(start, BrowserRootDir()))
        {
            start = DefaultBrowserDir();
        }
        string initialFilename = !string.IsNullOrEmpty(currentPath) ? Path.GetFileName(currentPath) : UntitledName;
        browser = new FileBrowser(BrowserRootDir(), start, "save", OnSaveSelected, initialFilename);
        OpenWindow(browser, BrowserFrameOptions());
    }

    public void SavePath(string path)
    {
        path = Path.GetFullPath(path);
        WriteTextFile(path, editor.Value);
        currentPath = path;
        filename.SetText(DisplayFileName(path));
        loadedText = editor.Value;
        RefreshStatus("Saved");
    }

    public void MarkModified()
    {
        RefreshStatus("Modified");
    }

    public bool Modified()
    {
        return editor.Value != loadedText;
    }

    public void WithSavedChanges(Action action)
    {
        afterSaveAction = null;
        if (!Modified())
        {
            action();
            return;
        }
        pendingAction = action;
        MessageBoxYesNoCancel dialog = new("Unsaved Changes", "Save current file before continuing?");
        dialog.OnClose = OnUnsavedChoice;
        OpenWindow(dialog);
    }

    public void Quit()
    {
        finished = true;
    }

    private GeneratedWindowRuntimeOptions BrowserFrameOptions()
    {
        GeneratedWindowRuntimeOptions options = browser?.RuntimeOptions() ?? new GeneratedWindowRuntimeOptions();
        options.InitialFocusName = "entries";
        options.StartInEditMode = true;
        options.OnButton = name =>
        {
            if (name == "open_btn")
            {
                AcceptBrowserOpenButton();
            }
            else if (name == "close_btn")
            {
                CloseBrowser("");
            }
        };
        options.OnKey = key =>
        {
            if (key == "Escape")
            {
                CloseBrowser("");
                return true;
            }
            return false;
        };
        return options;
    }

    private void AcceptBrowserCurrent()
    {
        browser?.AcceptCurrent();
        CloseBrowserIfClosed();
    }

    private void AcceptBrowserOpenButton()
    {
        if (browser is null)
        {
            return;
        }
        if (browser.open_btn.Title == "Save")
        {
            string name = browser.filename.Value;
            if (string.IsNullOrEmpty(name))
            {
                return;
            }
            string path = Path.Combine(browser.CurrentDir, name);
            if (File.Exists(path))
            {
                overwritePath = path;
                MessageBoxYesNo dialog = new("Overwrite File", "File already exists:\n" + path + "\n\nOverwrite it?");
                dialog.OnClose = choice => CloseOverwrite(choice ? "yes" : "no");
                OpenWindow(dialog);
                return;
            }
            CloseBrowser(path);
            return;
        }
        string selectedName = browser.filename.Value;
        if (!string.IsNullOrEmpty(selectedName))
        {
            string path = Path.Combine(browser.CurrentDir, selectedName);
            if (File.Exists(path))
            {
                CloseBrowser(path);
                return;
            }
        }
        AcceptBrowserCurrent();
    }

    private void CloseBrowserIfClosed()
    {
        if (browser is not null && browser.Closed)
        {
            browser = null;
        }
    }

    private void CloseBrowser(string path)
    {
        if (browser is not null)
        {
            FileBrowser activeBrowser = browser;
            browser = null;
            activeBrowser.Close(path);
        }
    }

    private void CloseOverwrite(string choice)
    {
        if (choice == "yes")
        {
            CloseBrowser(overwritePath);
        }
        overwritePath = "";
    }

    private void OnUnsavedChoice(string choice)
    {
        Action? action = pendingAction;
        pendingAction = null;
        if (choice == "cancel" || action is null)
        {
            RefreshStatus("Cancelled");
            return;
        }
        if (choice == "yes")
        {
            if (string.IsNullOrEmpty(currentPath))
            {
                afterSaveAction = action;
                SaveAsBrowser();
                return;
            }
            SaveOrSaveAs();
            action();
            return;
        }
        if (choice == "no")
        {
            action();
        }
    }

    private void OnOpenSelected(string path)
    {
        if (!string.IsNullOrEmpty(path))
        {
            LoadPath(path);
        }
    }

    private void OnSaveSelected(string path)
    {
        Action? action = afterSaveAction;
        afterSaveAction = null;
        if (!string.IsNullOrEmpty(path))
        {
            SavePath(path);
            action?.Invoke();
        }
        else if (action is not null)
        {
            RefreshStatus("Cancelled");
        }
    }

    private void SetEditorText(string text)
    {
        editor.SetValue(text);
        editor.SetSelection(0, 0);
    }

    private void RefreshStatus(string prefix)
    {
        string text = editor.Value;
        string modifiedMarker = Modified() ? "*" : "";
        int lineCount = string.IsNullOrEmpty(text) ? 0 : text.Count(ch => ch == '\n') + 1;
        status.SetText(prefix + ": " + filename.Text + modifiedMarker + " - " + lineCount + " lines, " + text.Length + " chars");
    }

    private static string? EnvString(string name)
    {
        string? value = Environment.GetEnvironmentVariable(name);
        return string.IsNullOrEmpty(value) ? null : value;
    }

    private static string SourceDirectory([CallerFilePath] string sourceFile = "")
    {
        return Path.GetDirectoryName(sourceFile) ?? Directory.GetCurrentDirectory();
    }

    private static string NotesDir()
    {
        return EnvString(NotesDirEnv) ?? Path.Combine(SourceDirectory(), "notes");
    }

    private static string DefaultNoteName()
    {
        return EnvString(DefaultNoteEnv) ?? DefaultFileName;
    }

    private static string UserHomeDir()
    {
        return Environment.GetEnvironmentVariable("HOME") ?? Directory.GetCurrentDirectory();
    }

    private static string BrowserRootDir()
    {
        return EnvString(BrowserRootDirEnv) ?? UserHomeDir();
    }

    private static string DefaultBrowserDir()
    {
        string? value = EnvString(DefaultBrowserDirEnv);
        if (!string.IsNullOrEmpty(value))
        {
            return value;
        }
        string desktop = Path.Combine(UserHomeDir(), "Desktop");
        return Directory.Exists(desktop) ? desktop : UserHomeDir();
    }

    private static bool PathStartsWith(string path, string root)
    {
        string normalizedPath = Path.GetFullPath(path).TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar) + Path.DirectorySeparatorChar;
        string normalizedRoot = Path.GetFullPath(root).TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar) + Path.DirectorySeparatorChar;
        return normalizedPath.StartsWith(normalizedRoot, StringComparison.Ordinal);
    }

    private static string ReadTextFile(string path)
    {
        return File.ReadAllText(path);
    }

    private static void WriteTextFile(string path, string text)
    {
        Directory.CreateDirectory(Path.GetDirectoryName(path) ?? ".");
        File.WriteAllText(path, text);
    }

    private static string DisplayFileName(string path)
    {
        string name = Path.GetFileName(path);
        return string.IsNullOrEmpty(name) ? path : name;
    }
}

public static class Program
{
    public static int Main(string[] args)
    {
        return GeneratedWindowRuntime.RunGeneratedAppMain(() =>
        {
            TextEditor app = new();
            return GeneratedWindowRuntime.RunGeneratedWindow(app, app.RuntimeOptions(), args);
        });
    }
}
