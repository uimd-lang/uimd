import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

import uimd.FileBrowser;
import uimd.GeneratedWindowRuntime;
import uimd.MessageBoxYesNoCancel;

public final class TextEditor extends TextEditorUI
{
    private static final String NOTES_DIR_ENV = "UI_TEXT_EDITOR_NOTES_DIR";
    private static final String DEFAULT_NOTE_ENV = "UI_TEXT_EDITOR_DEFAULT_NOTE";
    private static final String BROWSER_ROOT_DIR_ENV = "UI_TEXT_EDITOR_BROWSER_ROOT_DIR";
    private static final String DEFAULT_BROWSER_DIR_ENV = "UI_TEXT_EDITOR_BROWSER_DIR";
    private static final String DEFAULT_FILE_NAME = "welcome.txt";
    private static final String UNTITLED_NAME = "untitled.txt";
    private static final int EXPECTED_LAYOUT_ENTRIES = 10;

    private Path currentPath;
    private String loadedText = "";
    private Runnable pendingAction;
    private Runnable afterSaveAction;
    private boolean closeRequested;

    public TextEditor()
    {
        loadPath(notesDir().resolve(defaultNoteName()));
    }

    @Override
    protected void onNewBtnClick()
    {
        withSavedChanges(this::newFile);
    }

    @Override
    protected void onOpenBtnClick()
    {
        withSavedChanges(this::openBrowser);
    }

    @Override
    protected void onSaveBtnClick()
    {
        saveOrSaveAs();
    }

    @Override
    protected void onSaveAsBtnClick()
    {
        saveAsBrowser();
    }

    @Override
    protected void onQuitBtnClick()
    {
        withSavedChanges(this::quit);
    }

    @Override
    protected void onEditorChange(String value)
    {
        markModified();
    }

    @Override
    protected boolean shouldClose()
    {
        return closeRequested;
    }

    public void newFile()
    {
        currentPath = null;
        filename.setText(UNTITLED_NAME);
        setEditorText("");
        loadedText = "";
        refreshStatus("New file");
    }

    public void loadPath(Path path)
    {
        Path absolutePath = path.toAbsolutePath().normalize();
        String text = readTextFile(absolutePath);
        currentPath = absolutePath;
        filename.setText(displayFileName(absolutePath));
        setEditorText(text);
        loadedText = text;
        refreshStatus("Loaded");
    }

    public void saveOrSaveAs()
    {
        if (currentPath == null)
        {
            saveAsBrowser();
            return;
        }
        savePath(currentPath);
    }

    public void openBrowser()
    {
        FileBrowser browser = new FileBrowser(
            browserRootDir(),
            defaultBrowserDir(),
            FileBrowser.MODE_OPEN,
            this::onOpenSelected,
            "",
            "");
        openWindow(browser, browser.runtimeOptions());
    }

    public void saveAsBrowser()
    {
        Path start = currentPath == null || currentPath.getParent() == null
            ? notesDir()
            : currentPath.getParent();
        if (!pathStartsWith(start, browserRootDir()))
        {
            start = defaultBrowserDir();
        }
        String initialFilename = currentPath == null
            ? UNTITLED_NAME
            : displayFileName(currentPath);
        FileBrowser browser = new FileBrowser(
            browserRootDir(),
            start,
            FileBrowser.MODE_SAVE,
            this::onSaveSelected,
            initialFilename,
            "");
        openWindow(browser, browser.runtimeOptions());
    }

    public void savePath(Path path)
    {
        Path absolutePath = path.toAbsolutePath().normalize();
        writeTextFile(absolutePath, editor.value());
        currentPath = absolutePath;
        filename.setText(displayFileName(absolutePath));
        loadedText = editor.value();
        refreshStatus("Saved");
    }

    public void markModified()
    {
        refreshStatus("Modified");
    }

    public boolean modified()
    {
        return !editor.value().equals(loadedText);
    }

    public void withSavedChanges(Runnable action)
    {
        withSavedChanges(action, "");
    }

    public void withSavedChanges(Runnable action, String simulatedChoice)
    {
        afterSaveAction = null;
        if (!modified())
        {
            action.run();
            return;
        }
        pendingAction = action;
        if (simulatedChoice != null && !simulatedChoice.isEmpty())
        {
            onUnsavedChoice(simulatedChoice);
            return;
        }
        MessageBoxYesNoCancel dialog = new MessageBoxYesNoCancel(
            "Unsaved Changes",
            "Save current file before continuing?",
            this::onUnsavedChoice);
        openWindow(dialog, dialog.runtimeOptions());
    }

    public void quit()
    {
        closeRequested = true;
    }

    public void onOpenSelected(Path path)
    {
        if (path != null)
        {
            loadPath(path);
        }
    }

    public void onSaveSelected(Path path)
    {
        Runnable action = afterSaveAction;
        afterSaveAction = null;
        if (path != null)
        {
            savePath(path);
            if (action != null)
            {
                action.run();
            }
        }
        else if (action != null)
        {
            refreshStatus("Cancelled");
        }
    }

    private void onUnsavedChoice(String choice)
    {
        Runnable action = pendingAction;
        pendingAction = null;
        if ("cancel".equals(choice) || action == null)
        {
            refreshStatus("Cancelled");
            return;
        }
        if ("yes".equals(choice))
        {
            if (currentPath == null)
            {
                afterSaveAction = action;
                saveAsBrowser();
                return;
            }
            saveOrSaveAs();
            action.run();
            return;
        }
        if ("no".equals(choice))
        {
            action.run();
        }
    }

    private void setEditorText(String text)
    {
        editor.setValue(text);
        editor.selectRange(0, 0);
    }

    private void refreshStatus(String prefix)
    {
        String text = editor.value();
        String modifiedMarker = modified() ? "*" : "";
        int lineCount = text.isEmpty() ? 0 : countNewlines(text) + 1;
        status.setText(prefix + ": " + filename.text() + modifiedMarker
            + " - " + lineCount + " lines, " + text.length() + " chars");
    }

    private static int countNewlines(String text)
    {
        int count = 0;
        for (int index = 0; index < text.length(); ++index)
        {
            if (text.charAt(index) == '\n')
            {
                ++count;
            }
        }
        return count;
    }

    private static String environmentValue(String name)
    {
        String value = System.getenv(name);
        return value == null || value.isEmpty() ? null : value;
    }

    private static Path environmentPath(String name)
    {
        String value = environmentValue(name);
        return value == null ? null : Path.of(value).toAbsolutePath().normalize();
    }

    private static Path notesDir()
    {
        Path configured = environmentPath(NOTES_DIR_ENV);
        if (configured != null)
        {
            return configured;
        }
        Path location = classLocation();
        while (location != null)
        {
            Path candidate = location.resolve("notes");
            if (Files.isDirectory(candidate))
            {
                return candidate;
            }
            location = location.getParent();
        }
        return Path.of("notes").toAbsolutePath().normalize();
    }

    private static Path classLocation()
    {
        try
        {
            URI location = TextEditor.class.getProtectionDomain()
                .getCodeSource()
                .getLocation()
                .toURI();
            Path path = Path.of(location).toAbsolutePath().normalize();
            return Files.isDirectory(path) ? path : path.getParent();
        }
        catch (NullPointerException | SecurityException | URISyntaxException exception)
        {
            return Path.of("").toAbsolutePath().normalize();
        }
    }

    private static String defaultNoteName()
    {
        String configured = environmentValue(DEFAULT_NOTE_ENV);
        return configured == null ? DEFAULT_FILE_NAME : configured;
    }

    private static Path userHomeDir()
    {
        String home = environmentValue("HOME");
        if (home == null)
        {
            home = System.getProperty("user.home", System.getProperty("user.dir", "."));
        }
        return Path.of(home).toAbsolutePath().normalize();
    }

    private static Path browserRootDir()
    {
        Path configured = environmentPath(BROWSER_ROOT_DIR_ENV);
        return configured == null ? userHomeDir() : configured;
    }

    private static Path defaultBrowserDir()
    {
        Path configured = environmentPath(DEFAULT_BROWSER_DIR_ENV);
        if (configured != null)
        {
            return configured;
        }
        Path desktop = userHomeDir().resolve("Desktop");
        return Files.isDirectory(desktop) ? desktop : userHomeDir();
    }

    private static boolean pathStartsWith(Path path, Path root)
    {
        return path.toAbsolutePath().normalize().startsWith(root.toAbsolutePath().normalize());
    }

    private static String readTextFile(Path path)
    {
        try
        {
            return Files.readString(path, StandardCharsets.UTF_8);
        }
        catch (IOException exception)
        {
            return "";
        }
    }

    private static void writeTextFile(Path path, String text)
    {
        try
        {
            Files.writeString(path, text, StandardCharsets.UTF_8);
        }
        catch (IOException exception)
        {
            throw new IllegalStateException("failed to save text file: " + path, exception);
        }
    }

    private static String displayFileName(Path path)
    {
        Path name = path.getFileName();
        return name == null ? path.toString() : name.toString();
    }

    public static void main(String[] arguments)
    {
        int exitCode = GeneratedWindowRuntime.runGeneratedAppMain(() -> run(arguments));
        System.exit(exitCode);
    }

    private static int run(String[] arguments)
    {
        if (arguments.length > 0 && "--smoke".equals(arguments[0]))
        {
            smoke();
            return 0;
        }
        if (arguments.length > 0 && "--logic-test".equals(arguments[0]))
        {
            logicTest();
            return 0;
        }
        TextEditor application = new TextEditor();
        return GeneratedWindowRuntime.runGeneratedWindow(
            application,
            application.runtimeOptions(),
            arguments);
    }

    private static void smoke()
    {
        TextEditor window = new TextEditor();
        require("Text Editor".equals(window.title()), "unexpected title");
        require(window.generatedLayout().size() == EXPECTED_LAYOUT_ENTRIES,
            "unexpected layout size");
        require(DEFAULT_FILE_NAME.equals(window.filename.text()), "unexpected default filename");
    }

    private static void logicTest()
    {
        TextEditor window = new TextEditor();
        window.newFile();
        require(UNTITLED_NAME.equals(window.filename.text()), "new filename mismatch");
        require(window.editor.value().isEmpty(), "new editor should be empty");
        window.editor.setValue("draft");
        window.markModified();
        require(window.modified(), "draft should be modified");
        require("Modified: untitled.txt* - 1 lines, 5 chars".equals(window.status.text()),
            "modified status mismatch");
        boolean[] actionCalled = {false};
        window.withSavedChanges(() -> actionCalled[0] = true, "no");
        require(actionCalled[0], "discard action was not called");

        Path savedPath = null;
        try
        {
            savedPath = Files.createTempFile("uimd-java-text-editor-", ".txt");
            window.editor.setValue("saved draft");
            window.onSaveSelected(savedPath);
            require("saved draft".equals(readTextFile(savedPath)), "saved content mismatch");
            require(!window.modified(), "saved document should not be modified");
        }
        catch (IOException exception)
        {
            throw new IllegalStateException("failed to create text editor test file", exception);
        }
        finally
        {
            if (savedPath != null)
            {
                try
                {
                    Files.deleteIfExists(savedPath);
                }
                catch (IOException exception)
                {
                    throw new IllegalStateException("failed to remove text editor test file", exception);
                }
            }
        }
    }

    private static void require(boolean condition, String message)
    {
        if (!condition)
        {
            throw new IllegalStateException(message);
        }
    }
}
