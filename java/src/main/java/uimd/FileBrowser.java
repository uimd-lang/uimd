package uimd;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;
import java.util.function.Consumer;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;
import java.util.stream.Stream;

public final class FileBrowser extends FileBrowserUI
{
    public static final String MODE_OPEN = "open";
    public static final String MODE_SAVE = "save";

    private static final String PARENT_ENTRY = "..";

    private final Path rootDir;
    private final String browserMode;
    private final Consumer<Path> onClose;
    private final String extensionFilter;
    private final String initialFilename;
    private Path currentDir;
    private Path result;
    private boolean closed;
    private GeneratedWindowStack activeStack;

    public FileBrowser(Path rootDir)
    {
        this(rootDir, null, MODE_OPEN, null, "", "");
    }

    public FileBrowser(
        Path rootDir,
        Path startPath,
        String mode,
        Consumer<Path> onClose,
        String initialFilename,
        String extensionFilter)
    {
        if (rootDir == null)
        {
            throw new IllegalArgumentException("rootDir must not be null");
        }
        this.rootDir = rootDir.toAbsolutePath().normalize();
        browserMode = MODE_SAVE.equals(mode) ? MODE_SAVE : MODE_OPEN;
        this.onClose = onClose;
        this.extensionFilter = extensionFilter == null ? "" : extensionFilter.trim();

        Path requestedStart = (startPath == null ? this.rootDir : startPath)
            .toAbsolutePath()
            .normalize();
        String requestedFilename = initialFilename == null ? "" : initialFilename;
        if (Files.isRegularFile(requestedStart))
        {
            currentDir = clampDir(requestedStart.getParent());
            if (requestedFilename.isEmpty() && requestedStart.getFileName() != null)
            {
                requestedFilename = requestedStart.getFileName().toString();
            }
        }
        else
        {
            currentDir = clampDir(requestedStart);
        }
        this.initialFilename = requestedFilename;
        dialog_header.setText(MODE_SAVE.equals(browserMode) ? "Save As" : "Open File");
        open_btn.setTitle(MODE_SAVE.equals(browserMode) ? "Save" : "Open");
        if (MODE_SAVE.equals(browserMode))
        {
            filename.setValue(this.initialFilename);
            moveFilenameCursorToEnd();
        }
        refreshEntries();
    }

    public Path currentDir()
    {
        return currentDir;
    }

    public Path result()
    {
        return result;
    }

    public boolean closed()
    {
        return closed;
    }

    public int run(String[] arguments)
    {
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowRuntimeOptions options = runtimeOptions(stack);
        return GeneratedWindowRuntime.runGeneratedWindow(this, options, arguments);
    }

    public GeneratedWindowRuntimeOptions runtimeOptions(GeneratedWindowStack stack)
    {
        activeStack = stack;
        GeneratedWindowRuntimeOptions options = runtimeOptions();
        options.setWindowStack(stack);
        return options;
    }

    @Override
    public GeneratedWindowRuntimeOptions runtimeOptions()
    {
        GeneratedWindowRuntimeOptions options = super.runtimeOptions();
        options.setInitialFocusName("entries");
        options.setStartInEditMode(true);
        options.setOnMousePressBeforeFocused(this::handleEntryMousePress);
        options.setOnEditStarted(name ->
        {
            if ("filename".equals(name))
            {
                moveFilenameCursorToEnd();
            }
        });
        options.setShouldClose(this::closed);
        return options;
    }

    @Override
    public boolean handleGeneratedListBoxItemActivate(
        String name,
        String elementId,
        int index,
        String value)
    {
        if (!"entries".equals(name))
        {
            return false;
        }
        entries.setSelectedIndex(index);
        entries.hideActiveItem();
        previewSelected();
        return selectedEntryIsDirectory() && acceptCurrent();
    }

    @Override
    public boolean onPreviewKey(KeyEvent event)
    {
        if (!"Escape".equals(event.key()) || event.editMode())
        {
            return false;
        }
        close(null);
        return true;
    }

    public void refreshEntries()
    {
        List<Path> directories = new ArrayList<>();
        List<Path> files = new ArrayList<>();
        try (Stream<Path> stream = Files.list(currentDir))
        {
            stream.forEach(path ->
            {
                if (Files.isDirectory(path))
                {
                    directories.add(path);
                }
                else if (Files.isRegularFile(path))
                {
                    files.add(path);
                }
            });
        }
        catch (IOException exception)
        {
            directories.clear();
            files.clear();
        }
        Comparator<Path> byName = Comparator.comparing(
            path -> path.getFileName().toString());
        directories.sort(byName);
        files.sort(byName);

        List<String> rows = new ArrayList<>();
        List<String> disabledRows = new ArrayList<>();
        rows.add(PARENT_ENTRY);
        for (Path directory : directories)
        {
            rows.add(directory.getFileName() + "/");
        }
        for (Path file : files)
        {
            String name = file.getFileName().toString();
            rows.add(name);
            if (!pathMatchesFilter(file))
            {
                disabledRows.add(name);
            }
        }

        path_label.setText(currentDir.toString());
        entries.setOptions(rows);
        entries.setDisabledValues(disabledRows);
        int selectedIndex = rows.indexOf(initialFilename);
        entries.setSelectedIndex(selectedIndex >= 0 ? selectedIndex : 0);
        previewSelected();
    }

    public boolean acceptCurrent()
    {
        String selected = selectedEntry();
        String name = filename.value().trim();
        if (!name.isEmpty())
        {
            Path path = currentDir.resolve(name).toAbsolutePath().normalize();
            if (!path.startsWith(rootDir))
            {
                return false;
            }
            if (Files.isDirectory(path))
            {
                currentDir = clampDir(path);
                refreshEntries();
                return true;
            }
            if (MODE_OPEN.equals(browserMode)
                && (!Files.isRegularFile(path) || !pathMatchesFilter(path)))
            {
                return false;
            }
            close(path);
            return true;
        }
        if (PARENT_ENTRY.equals(selected))
        {
            currentDir = clampDir(currentDir.getParent());
            refreshEntries();
            return true;
        }
        if (selected.endsWith("/"))
        {
            currentDir = clampDir(currentDir.resolve(selected.substring(0, selected.length() - 1)));
            refreshEntries();
            return true;
        }
        if (selected.trim().isEmpty())
        {
            return false;
        }
        Path path = currentDir.resolve(selected.trim()).toAbsolutePath().normalize();
        if (!path.startsWith(rootDir)
            || (MODE_OPEN.equals(browserMode)
                && (!Files.isRegularFile(path) || !pathMatchesFilter(path))))
        {
            return false;
        }
        close(path);
        return true;
    }

    public boolean acceptFilename()
    {
        String name = filename.value().trim();
        if (name.isEmpty())
        {
            return false;
        }
        Path path = currentDir.resolve(name).toAbsolutePath().normalize();
        if (!path.startsWith(rootDir) || Files.isDirectory(path))
        {
            return false;
        }
        if (Files.isRegularFile(path))
        {
            confirmOverwrite(path);
            return true;
        }
        close(path);
        return true;
    }

    public void selectEntry(int index)
    {
        entries.setSelectedIndex(index);
        previewSelected();
    }

    public boolean selectedEntryIsDirectory()
    {
        return entryIndexIsDirectory(entries.selectedIndex());
    }

    public boolean entryIndexIsDirectory(int index)
    {
        if (index < 0 || index >= entries.options().size())
        {
            return false;
        }
        String selected = entries.options().get(index);
        return PARENT_ENTRY.equals(selected) || selected.endsWith("/");
    }

    public boolean handleEntryMousePress(Point point)
    {
        Rect frame = entries.frame();
        if (!frame.contains(point))
        {
            return false;
        }
        int index = entries.scrollOffset() + point.row() - frame.row();
        if (index < 0 || index >= entries.options().size())
        {
            return false;
        }
        selectEntry(index);
        return entryIndexIsDirectory(index) && acceptCurrent();
    }

    public void moveFilenameCursorToEnd()
    {
        int end = filename.value().length();
        filename.selectRange(end, end);
    }

    public void close(Path path)
    {
        result = path;
        closed = true;
        if (onClose != null)
        {
            onClose.accept(result);
        }
        GeneratedWindowStack stack = runtimeWindowStack();
        if (stack != null)
        {
            stack.remove(this);
        }
    }

    @Override
    protected void onEntriesSelectionChange(List<String> value)
    {
        previewSelected();
    }

    @Override
    protected void onFilenameChange(String value)
    {
        updateOpenEnabled();
    }

    @Override
    protected void onFilenameSubmit(String value)
    {
        if (MODE_SAVE.equals(browserMode))
        {
            acceptFilename();
        }
        else
        {
            acceptCurrent();
        }
    }

    @Override
    protected void onOpenBtnClick()
    {
        if (MODE_SAVE.equals(browserMode))
        {
            acceptFilename();
        }
        else
        {
            acceptCurrent();
        }
    }

    @Override
    protected void onCloseBtnClick()
    {
        close(null);
    }

    private Path clampDir(Path path)
    {
        if (path == null)
        {
            return rootDir;
        }
        Path candidate = path.toAbsolutePath().normalize();
        return candidate.startsWith(rootDir) && Files.isDirectory(candidate)
            ? candidate
            : rootDir;
    }

    private void confirmOverwrite(Path path)
    {
        MessageBoxYesNo dialog = new MessageBoxYesNo(
            "Overwrite File",
            "File already exists:\n" + path + "\n\nOverwrite it?",
            value ->
            {
                if ("yes".equals(value))
                {
                    close(path);
                }
            });
        GeneratedWindowStack stack = activeStack == null ? runtimeWindowStack() : activeStack;
        if (stack == null)
        {
            dialog.run(new String[0]);
            return;
        }
        GeneratedWindowRuntimeOptions options = dialog.runtimeOptions();
        options.setWindowStack(stack);
        stack.push(dialog, options);
    }

    private void previewSelected()
    {
        String selected = selectedEntry();
        if (!selected.isEmpty() && !PARENT_ENTRY.equals(selected))
        {
            filename.setValue(selected.endsWith("/")
                ? selected.substring(0, selected.length() - 1)
                : selected);
        }
        else if (MODE_OPEN.equals(browserMode))
        {
            filename.setValue("");
        }
        moveFilenameCursorToEnd();
        updateOpenEnabled();
    }

    private String selectedEntry()
    {
        List<String> selected = entries.selectedValues();
        return selected.isEmpty() ? "" : selected.get(0);
    }

    private Path selectedPathForOpenState()
    {
        String name = filename.value().trim();
        if (!name.isEmpty())
        {
            return currentDir.resolve(name).toAbsolutePath().normalize();
        }
        String selected = selectedEntry();
        if (selected.isEmpty())
        {
            return null;
        }
        if (PARENT_ENTRY.equals(selected))
        {
            return clampDir(currentDir.getParent());
        }
        String pathName = selected.endsWith("/")
            ? selected.substring(0, selected.length() - 1)
            : selected;
        return currentDir.resolve(pathName).toAbsolutePath().normalize();
    }

    private boolean pathMatchesFilter(Path path)
    {
        if (extensionFilter.isEmpty())
        {
            return true;
        }
        String name = path.getFileName() == null ? "" : path.getFileName().toString();
        int separator = name.lastIndexOf('.');
        String extension = separator < 0 || separator + 1 >= name.length()
            ? ""
            : name.substring(separator + 1).toLowerCase(Locale.ROOT);
        if (extension.isEmpty())
        {
            return false;
        }
        try
        {
            return Pattern.compile(extensionFilter, Pattern.CASE_INSENSITIVE)
                .matcher(extension)
                .matches();
        }
        catch (PatternSyntaxException exception)
        {
            for (String part : extensionFilter.split("\\|"))
            {
                String allowed = part.trim().toLowerCase(Locale.ROOT);
                if (allowed.startsWith("."))
                {
                    allowed = allowed.substring(1);
                }
                if (extension.equals(allowed))
                {
                    return true;
                }
            }
            return false;
        }
    }

    private void updateOpenEnabled()
    {
        if (MODE_SAVE.equals(browserMode))
        {
            open_btn.setEnabled(!filename.value().trim().isEmpty());
            return;
        }
        Path path = selectedPathForOpenState();
        if (path == null || !path.startsWith(rootDir))
        {
            open_btn.setEnabled(false);
        }
        else if (Files.isDirectory(path))
        {
            open_btn.setEnabled(true);
        }
        else
        {
            open_btn.setEnabled(Files.isRegularFile(path) && pathMatchesFilter(path));
        }
    }
}
