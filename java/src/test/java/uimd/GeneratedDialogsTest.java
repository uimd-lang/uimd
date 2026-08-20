package uimd;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertInstanceOf;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;


final class GeneratedDialogsTest
{
    @TempDir
    Path temporaryDirectory;

    @Test
    void messageBoxUsesDefaultFocusAndClosedModalIsRemovedFromStack()
    {
        AtomicReference<String> result = new AtomicReference<>();
        MessageBoxYesNo dialog = new MessageBoxYesNo("Question", "Continue?", result::set);
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(dialog, dialog.runtimeOptions());
        List<Element> focusable = GeneratedWindowRuntime.focusableElements(dialog);

        assertSame(dialog.no_btn, focusable.get(frame.focusedIndex()));

        assertTrue(GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.key("Enter")));

        assertTrue(stack.empty());
        assertTrue(dialog.closed());
        assertEquals("no", dialog.result());
        assertEquals("no", result.get());
    }

    @Test
    void messageBoxEscapeReturnsItsDocumentedDefaultResult()
    {
        AtomicReference<String> result = new AtomicReference<>();
        MessageBoxOkCancel dialog = new MessageBoxOkCancel("Save", "Save changes?", result::set);
        GeneratedWindowStack stack = new GeneratedWindowStack();
        stack.push(dialog, dialog.runtimeOptions());

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Escape"));

        assertTrue(stack.empty());
        assertEquals("cancel", dialog.result());
        assertEquals("cancel", result.get());
    }

    @Test
    void messageBoxEscapeFocusesSemanticNegativeActionBeforeClosing()
    {
        MessageBoxYesNo dialog = new MessageBoxYesNo("Question", "Continue?");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        GeneratedWindowStackFrame frame = stack.push(dialog, dialog.runtimeOptions());
        GeneratedWindowRuntime.focusFrameElement(frame, dialog.yes_btn);
        AtomicReference<Element> focusedDuringFlash = new AtomicReference<>();

        GeneratedWindowRuntime.dispatchWindowStackEvent(
            stack,
            Event.key("Escape"),
            () -> focusedDuringFlash.set(
                GeneratedWindowRuntime.focusableElements(dialog).get(frame.focusedIndex())));

        assertSame(dialog.no_btn, focusedDuringFlash.get());
        assertTrue(stack.empty());
        assertEquals("no", dialog.result());
    }

    @Test
    void fileBrowserListsDirectoriesThenFilesAndHonorsExtensionFilter() throws IOException
    {
        Path root = Files.createDirectory(temporaryDirectory.resolve("root"));
        Files.createDirectory(root.resolve("folder"));
        Path textFile = Files.writeString(root.resolve("alpha.txt"), "alpha");
        Files.writeString(root.resolve("beta.png"), "png");
        AtomicReference<Path> result = new AtomicReference<>();
        FileBrowser browser = new FileBrowser(
            root,
            root,
            FileBrowser.MODE_OPEN,
            result::set,
            "",
            "txt");

        assertEquals(List.of("..", "folder/", "alpha.txt", "beta.png"), browser.entries.options());
        assertEquals(List.of("beta.png"), browser.entries.disabledValues());
        assertTrue(browser.open_btn.enabled());

        browser.selectEntry(browser.entries.options().indexOf("alpha.txt"));

        assertTrue(browser.open_btn.enabled());
        assertTrue(browser.acceptCurrent());
        assertTrue(browser.closed());
        assertEquals(textFile.toAbsolutePath().normalize(), browser.result());
        assertEquals(browser.result(), result.get());
    }

    @Test
    void fileBrowserClampsOutsideStartAndUsesModalForOverwrite() throws IOException
    {
        Path root = Files.createDirectory(temporaryDirectory.resolve("root"));
        Path outside = Files.createDirectory(temporaryDirectory.resolve("outside"));
        Path existing = Files.writeString(root.resolve("existing.txt"), "existing");
        AtomicReference<Path> result = new AtomicReference<>();
        FileBrowser browser = new FileBrowser(
            root,
            outside,
            FileBrowser.MODE_SAVE,
            result::set,
            existing.getFileName().toString(),
            "");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        stack.push(browser, browser.runtimeOptions(stack));

        assertEquals(root.toAbsolutePath().normalize(), browser.currentDir());
        assertTrue(browser.acceptFilename());
        assertEquals(2, stack.size());
        assertInstanceOf(MessageBoxYesNo.class, stack.topWindow());

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Escape"));

        assertEquals(1, stack.size());
        assertSame(browser, stack.topWindow());
        assertFalse(browser.closed());
        assertNull(browser.result());
        assertNull(result.get());
    }

    @Test
    void acceptedOverwriteRemovesBothNestedDialogAndClosedBrowser() throws IOException
    {
        Path root = Files.createDirectory(temporaryDirectory.resolve("root"));
        Path existing = Files.writeString(root.resolve("existing.txt"), "existing");
        AtomicReference<Path> result = new AtomicReference<>();
        FileBrowser browser = new FileBrowser(
            root,
            root,
            FileBrowser.MODE_SAVE,
            result::set,
            existing.getFileName().toString(),
            "");
        GeneratedWindowStack stack = new GeneratedWindowStack();
        stack.push(browser, browser.runtimeOptions(stack));

        assertTrue(browser.acceptFilename());
        MessageBoxYesNo confirmation = assertInstanceOf(
            MessageBoxYesNo.class,
            stack.topWindow());
        confirmation.setFocus(confirmation.yes_btn);

        GeneratedWindowRuntime.dispatchWindowStackEvent(stack, Event.key("Enter"));

        assertTrue(stack.empty());
        assertTrue(browser.closed());
        assertEquals(existing.toAbsolutePath().normalize(), browser.result());
        assertEquals(browser.result(), result.get());
    }
}
